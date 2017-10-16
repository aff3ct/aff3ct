#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"

#include "Decoder_turbo_DB.hpp"

const std::string aff3ct::factory::Decoder_turbo_DB::name   = "Decoder Turbo DB";
const std::string aff3ct::factory::Decoder_turbo_DB::prefix = "dec";

using namespace aff3ct;
using namespace aff3ct::factory;

Decoder_turbo_DB::parameters
::parameters(const std::string prefix)
: Decoder::parameters(Decoder_turbo_DB::name, prefix),
  sub(new Decoder_RSC_DB::parameters(prefix+"-sub")),
  itl(new Interleaver::parameters("itl")),
  sf(new Scaling_factor::parameters(prefix+"-sf")),
  fnc(new Flip_and_check_DB::parameters(prefix+"-fnc"))
{
	this->type   = "TURBO_DB";
	this->implem = "STD";
}

Decoder_turbo_DB::parameters* Decoder_turbo_DB::parameters
::clone() const
{
	auto clone = new Decoder_turbo_DB::parameters(*this);

	if (sub != nullptr) { clone->sub = sub->clone(); }
	if (itl != nullptr) { clone->itl = itl->clone(); }
	if (sf  != nullptr) { clone->sf  = sf ->clone(); }
	if (fnc != nullptr) { clone->fnc = fnc->clone(); }

	return clone;
}

std::vector<std::string> Decoder_turbo_DB::parameters
::get_names() const
{
	auto n = Decoder::parameters::get_names();
	if (sub != nullptr) { auto nn = sub->get_names(); for (auto &x : nn) n.push_back(x); }
	if (sf  != nullptr) { auto nn = sf ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (fnc != nullptr) { auto nn = fnc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> Decoder_turbo_DB::parameters
::get_short_names() const
{
	auto sn = Decoder::parameters::get_short_names();
	if (sub != nullptr) { auto nn = sub->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (sf  != nullptr) { auto nn = sf ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (fnc != nullptr) { auto nn = fnc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Decoder_turbo_DB::parameters
::get_prefixes() const
{
	auto p = Decoder::parameters::get_prefixes();
	if (sub != nullptr) { auto nn = sub->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (sf  != nullptr) { auto nn = sf ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (fnc != nullptr) { auto nn = fnc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

Decoder_turbo_DB::parameters
::~parameters()
{
	if (sub != nullptr) { delete sub; sub = nullptr; }
	if (itl != nullptr) { delete itl; itl = nullptr; }
	if (sf  != nullptr) { delete sf ; sf  = nullptr; }
	if (fnc != nullptr) { delete fnc; fnc = nullptr; }
}

void Decoder_turbo_DB::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Decoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.erase({p+"-cw-size", "N"});

	itl->get_description(req_args, opt_args);

	auto pi = this->itl->get_prefix();

	req_args.erase({pi+"-size"    });
	opt_args.erase({pi+"-fra", "F"});

	opt_args[{p+"-type", "D"}].push_back("TURBO_DB");

	opt_args[{p+"-implem"}].push_back("STD");

	opt_args[{p+"-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo."};

	sf->get_description(req_args, opt_args);

	auto psf = sf->get_prefix();

	opt_args.erase({psf+"-ite"});

	fnc->get_description(req_args, opt_args);

	auto pfnc = fnc->get_prefix();

	req_args.erase({pfnc+"-size", "K"});
	opt_args.erase({pfnc+"-fra",  "F"});
	opt_args.erase({pfnc+"-ite",  "i"});

	sub->get_description(req_args, opt_args);

	auto ps = sub->get_prefix();

	req_args.erase({ps+"-info-bits", "K"});
	req_args.erase({ps+"-cw-size",   "N"});
	opt_args.erase({ps+"-fra",       "F"});
}

void Decoder_turbo_DB::parameters
::store(const arg_val_map &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-ite", "i"})) this->n_ite = std::stoi(vals.at({p+"-ite", "i"}));

	this->sub->K        = this->K;
	this->sub->n_frames = this->n_frames;

	sub->store(vals);

	this->N_cw = 2 * this->sub->N_cw - this->K;
	this->R    = (float)this->K / (float)this->N_cw;

	this->itl->core->size     = this->K >> 1;
	this->itl->core->n_frames = this->n_frames;

	itl->store(vals);

	if (this->sub->implem == "DVB-RCS1" && !exist(vals, {"itl-type"}))
		this->itl->core->type = "DVB-RCS1";

	if (this->sub->implem == "DVB-RCS2" && !exist(vals, {"itl-type"}))
		this->itl->core->type = "DVB-RCS2";

	this->sf->n_ite = this->n_ite;

	sf->store(vals);

	this->fnc->size     = this->K;
	this->fnc->n_frames = this->n_frames;
	this->fnc->n_ite    = this->n_ite;

	fnc->store(vals);
}

void Decoder_turbo_DB::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	auto p = this->get_prefix();

	itl->get_headers(headers, full);

	headers[p].push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->n_ite)));
	if (this->tail_length && full)
		headers[p].push_back(std::make_pair("Tail length", std::to_string(this->tail_length)));

	sf ->get_headers(headers, full);
	fnc->get_headers(headers, full);
	sub->get_headers(headers, full);
}

template <typename B, typename Q>
module::Decoder_turbo_DB<B,Q>* Decoder_turbo_DB::parameters
::build(const module::Interleaver<Q>           &itl,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_n,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_i) const
{
	if (this->type == "TURBO_DB")
	{
		if (this->implem == "STD") return new module::Decoder_turbo_DB<B,Q>(this->K, this->N_cw, this->n_ite, itl, siso_n, siso_i);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_turbo_DB<B,Q>* Decoder_turbo_DB
::build(const parameters                       &params,
        const module::Interleaver<Q>           &itl,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_n,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_i)
{
	return params.template build<B,Q>(itl, siso_n, siso_i);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_turbo_DB<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_8 ,Q_8 >(const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&) const;
template aff3ct::module::Decoder_turbo_DB<B_16,Q_16>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_16,Q_16>(const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&) const;
template aff3ct::module::Decoder_turbo_DB<B_32,Q_32>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_32,Q_32>(const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&) const;
template aff3ct::module::Decoder_turbo_DB<B_64,Q_64>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_64,Q_64>(const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&) const;
template aff3ct::module::Decoder_turbo_DB<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&);
template aff3ct::module::Decoder_turbo_DB<B_16,Q_16>* aff3ct::factory::Decoder_turbo_DB::build<B_16,Q_16>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&);
template aff3ct::module::Decoder_turbo_DB<B_32,Q_32>* aff3ct::factory::Decoder_turbo_DB::build<B_32,Q_32>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&);
template aff3ct::module::Decoder_turbo_DB<B_64,Q_64>* aff3ct::factory::Decoder_turbo_DB::build<B_64,Q_64>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&);
#else
template aff3ct::module::Decoder_turbo_DB<B,Q>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B,Q>(const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&) const;
template aff3ct::module::Decoder_turbo_DB<B,Q>* aff3ct::factory::Decoder_turbo_DB::build<B,Q>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&);
#endif
// ==================================================================================== explicit template instantiation
