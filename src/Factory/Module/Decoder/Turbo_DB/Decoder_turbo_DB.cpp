#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"

#include "Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"

#include "Decoder_turbo_DB.hpp"

const std::string aff3ct::factory::Decoder_turbo_DB_name   = "Decoder Turbo DB";
const std::string aff3ct::factory::Decoder_turbo_DB_prefix = "dec";

using namespace aff3ct;
using namespace aff3ct::factory;

Decoder_turbo_DB::parameters
::parameters(const std::string &prefix)
: Decoder::parameters(Decoder_turbo_DB_name, prefix),
  sub(new Decoder_RSC_DB::parameters(prefix+"-sub")),
  itl(new Interleaver::parameters("itl")),
  sf (new Scaling_factor::parameters(prefix+"-sf")),
  fnc(new Flip_and_check_DB::parameters(prefix+"-fnc"))
{
	this->type   = "TURBO_DB";
	this->implem = "STD";
}

Decoder_turbo_DB::parameters* Decoder_turbo_DB::parameters
::clone() const
{
	return new Decoder_turbo_DB::parameters(*this);
}

std::vector<std::string> Decoder_turbo_DB::parameters
::get_names() const
{
	auto n = Decoder::parameters::get_names();
	if (sf  != nullptr) { auto nn = sf ->get_names(); for (auto &x : nn) n.push_back(x); }
	if (fnc != nullptr) { auto nn = fnc->get_names(); for (auto &x : nn) n.push_back(x); }
	if (sub != nullptr) { auto nn = sub->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> Decoder_turbo_DB::parameters
::get_short_names() const
{
	auto sn = Decoder::parameters::get_short_names();
	if (sf  != nullptr) { auto nn = sf ->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (fnc != nullptr) { auto nn = fnc->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (sub != nullptr) { auto nn = sub->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Decoder_turbo_DB::parameters
::get_prefixes() const
{
	auto p = Decoder::parameters::get_prefixes();
	if (sf  != nullptr) { auto nn = sf ->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (fnc != nullptr) { auto nn = fnc->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (sub != nullptr) { auto nn = sub->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void Decoder_turbo_DB::parameters
::get_description(tools::Argument_map_info &args) const
{
	Decoder::parameters::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Decoder_turbo_DB::parameters::";

	args.erase({p+"-cw-size", "N"});

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = this->itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}

	tools::add_options(args.at({p+"-type", "D"}), 0, "TURBO_DB");
	tools::add_options(args.at({p+"-implem"   }), 0, "STD");

	tools::add_arg(args, p, class_name+"p+ite,i",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+crc-start",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	sf->get_description(args);

	auto psf = sf->get_prefix();

	args.erase({psf+"-ite"});

	fnc->get_description(args);

	auto pfnc = fnc->get_prefix();

	args.erase({pfnc+"-size"     });
	args.erase({pfnc+"-fra",  "F"});
	args.erase({pfnc+"-ite",  "i"});
	args.erase({pfnc+"-crc-ite"  });

	sub->get_description(args);

	auto ps = sub->get_prefix();

	args.erase({ps+"-info-bits", "K"});
	args.erase({ps+"-cw-size",   "N"});
	args.erase({ps+"-fra",       "F"});
}

void Decoder_turbo_DB::parameters
::store(const tools::Argument_map_value &vals)
{
	Decoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-ite", "i"}))  this->n_ite         = vals.to_int({p+"-ite", "i"});
	if(vals.exist({p+"-crc-start"})) this->crc_start_ite = vals.to_int({p+"-crc-start"});

	this->sub->K        = this->K;
	this->sub->n_frames = this->n_frames;

	sub->store(vals);

	this->N_cw = 2 * this->sub->N_cw - this->K;
	this->R    = (float)this->K / (float)this->N_cw;

	if (itl != nullptr)
	{
		this->itl->core->size     = this->K >> 1;
		this->itl->core->n_frames = this->n_frames;

		itl->store(vals);

		if (this->sub->implem == "DVB-RCS1" && !vals.exist({"itl-type"}))
			this->itl->core->type = "DVB-RCS1";

		if (this->sub->implem == "DVB-RCS2" && !vals.exist({"itl-type"}))
			this->itl->core->type = "DVB-RCS2";
	}

	this->sf->n_ite = this->n_ite;

	sf->store(vals);

	this->fnc->size          = this->K;
	this->fnc->n_frames      = this->n_frames;
	this->fnc->n_ite         = this->n_ite;
	this->fnc->crc_start_ite = this->crc_start_ite;

	fnc->store(vals);
}

void Decoder_turbo_DB::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Decoder::parameters::get_headers(headers, full);

	if (this->type != "ML" && this->type != "CHASE")
	{
		auto p = this->get_prefix();

		if (itl != nullptr)
			itl->get_headers(headers, full);

		headers[p].push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->n_ite)));
		headers[p].push_back(std::make_pair("CRC start iteration", std::to_string(this->crc_start_ite)));
		if (this->tail_length && full)
			headers[p].push_back(std::make_pair("Tail length", std::to_string(this->tail_length)));

		sf ->get_headers(headers, full);
		fnc->get_headers(headers, full);
		sub->get_headers(headers, full);
	}
}

template <typename B, typename Q>
module::Decoder_turbo_DB<B,Q>* Decoder_turbo_DB::parameters
::build(const module::Interleaver<Q>           &itl,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_n,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_i,
              const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	if (this->type == "TURBO_DB")
	{
		if (this->implem == "STD") return new module::Decoder_turbo_DB<B,Q>(this->K, this->N_cw, this->n_ite, itl, siso_n, siso_i);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_turbo_DB::parameters
::build(const std::unique_ptr<module::Encoder<B>>& encoder) const
{
	return Decoder::parameters::build<B,Q>(encoder);
}

template <typename B, typename Q>
module::Decoder_turbo_DB<B,Q>* Decoder_turbo_DB
::build(const parameters                       &params,
        const module::Interleaver<Q>           &itl,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_n,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_i,
              const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build<B,Q>(itl, siso_n, siso_i, encoder);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_turbo_DB
::build(const parameters &params, const std::unique_ptr<module::Encoder<B>>& encoder)
{
	return params.template build<B,Q>(encoder);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_turbo_DB<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_8 ,Q_8 >(const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&, const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_turbo_DB<B_16,Q_16>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_16,Q_16>(const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&, const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_turbo_DB<B_32,Q_32>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_32,Q_32>(const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&, const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_turbo_DB<B_64,Q_64>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_64,Q_64>(const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&, const std::unique_ptr<module::Encoder<B_64>>&) const;
template aff3ct::module::Decoder_turbo_DB<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q_8 >&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&, const std::unique_ptr<module::Encoder<B_8 >>&);
template aff3ct::module::Decoder_turbo_DB<B_16,Q_16>* aff3ct::factory::Decoder_turbo_DB::build<B_16,Q_16>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q_16>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&, const std::unique_ptr<module::Encoder<B_16>>&);
template aff3ct::module::Decoder_turbo_DB<B_32,Q_32>* aff3ct::factory::Decoder_turbo_DB::build<B_32,Q_32>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q_32>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&, const std::unique_ptr<module::Encoder<B_32>>&);
template aff3ct::module::Decoder_turbo_DB<B_64,Q_64>* aff3ct::factory::Decoder_turbo_DB::build<B_64,Q_64>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q_64>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&, const std::unique_ptr<module::Encoder<B_64>>&);
#else
template aff3ct::module::Decoder_turbo_DB<B,Q>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B,Q>(const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&, const std::unique_ptr<module::Encoder<B>>& ) const;
template aff3ct::module::Decoder_turbo_DB<B,Q>* aff3ct::factory::Decoder_turbo_DB::build<B,Q>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<Q>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&, const std::unique_ptr<module::Encoder<B>>& );
#endif

#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_8 ,Q_8 >(const std::unique_ptr<module::Encoder<B_8 >>&) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_16,Q_16>(const std::unique_ptr<module::Encoder<B_16>>&) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_32,Q_32>(const std::unique_ptr<module::Encoder<B_32>>&) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B_64,Q_64>(const std::unique_ptr<module::Encoder<B_64>>&) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_turbo_DB::parameters&, const std::unique_ptr<module::Encoder<B_8 >>&);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_turbo_DB::build<B_16,Q_16>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const std::unique_ptr<module::Encoder<B_16>>&);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_turbo_DB::build<B_32,Q_32>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const std::unique_ptr<module::Encoder<B_32>>&);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_turbo_DB::build<B_64,Q_64>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const std::unique_ptr<module::Encoder<B_64>>&);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_DB::parameters::build<B,Q>(const std::unique_ptr<module::Encoder<B>>& ) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_turbo_DB::build<B,Q>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const std::unique_ptr<module::Encoder<B>>& );
#endif
// ==================================================================================== explicit template instantiation
