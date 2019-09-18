#include <utility>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"

const std::string aff3ct::factory::Encoder_turbo_DB_name   = "Encoder Turbo DB";
const std::string aff3ct::factory::Encoder_turbo_DB_prefix = "enc";

using namespace aff3ct;
using namespace aff3ct::factory;

Encoder_turbo_DB::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_turbo_DB_name, prefix),
  itl(new Interleaver::parameters("itl")),
  sub(new Encoder_RSC_DB::parameters(prefix+"-sub"))
{
	this->type = "TURBO_DB";
}

Encoder_turbo_DB::parameters* Encoder_turbo_DB::parameters
::clone() const
{
	return new Encoder_turbo_DB::parameters(*this);
}

std::vector<std::string> Encoder_turbo_DB::parameters
::get_names() const
{
	auto n = Encoder::parameters::get_names();
	if (sub != nullptr) { auto nn = sub->get_names(); for (auto &x : nn) n.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}
std::vector<std::string> Encoder_turbo_DB::parameters
::get_short_names() const
{
	auto sn = Encoder::parameters::get_short_names();
	if (sub != nullptr) { auto nn = sub->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Encoder_turbo_DB::parameters
::get_prefixes() const
{
	auto p = Encoder::parameters::get_prefixes();
	if (sub != nullptr) { auto nn = sub->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (itl != nullptr) { auto nn = itl->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void Encoder_turbo_DB::parameters
::get_description(tools::Argument_map_info &args) const
{
	Encoder::parameters::get_description(args);

	auto p = this->get_prefix();

	args.erase({p+"-cw-size", "N"});

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}

	tools::add_options(args.at({p+"-type"}), 0, "TURBO_DB");

	sub->get_description(args);

	auto ps = sub->get_prefix();

	args.erase({ps+"-info-bits", "K"});
	args.erase({ps+"-cw-size",   "N"});
	args.erase({ps+"-fra",       "F"});
	args.erase({ps+"-seed",      "S"});
	args.erase({ps+"-path"          });
	args.erase({ps+"-no-buff"       });
}

void Encoder_turbo_DB::parameters
::store(const tools::Argument_map_value &vals)
{
	Encoder::parameters::store(vals);

	this->sub->K        = this->K;
	this->sub->n_frames = this->n_frames;
	this->sub->seed     = this->seed;

	sub->store(vals);

	this->N_cw = 2 * this->sub->N_cw - this->K;
	this->R    = (float)this->K / (float)this->N_cw;

	if (itl != nullptr)
	{
		this->itl->core->size     = this->K >> 1;
		this->itl->core->n_frames = this->n_frames;

		itl->store(vals);

		if (this->sub->standard == "DVB-RCS1" && !vals.exist({"itl-type"}))
			this->itl->core->type = "DVB-RCS1";

		if (this->sub->standard == "DVB-RCS2" && !vals.exist({"itl-type"}))
			this->itl->core->type = "DVB-RCS2";
	}
}

void Encoder_turbo_DB::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	if (itl != nullptr)
		itl->get_headers(headers, full);

	auto p = this->get_prefix();

	if (this->tail_length)
		headers[p].push_back(std::make_pair("Tail length", std::to_string(this->tail_length)));

	sub->get_headers(headers, full);
}

template <typename B>
module::Encoder_turbo_DB<B>* Encoder_turbo_DB::parameters
::build(const module::Interleaver<B> &itl, module::Encoder_RSC_DB<B> &sub_enc) const
{
	if (this->type == "TURBO_DB") return new module::Encoder_turbo_DB<B>(this->K, this->N_cw, itl, sub_enc, sub_enc);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_turbo_DB<B>* Encoder_turbo_DB
::build(const parameters                &params,
        const module::Interleaver<B>    &itl,
              module::Encoder_RSC_DB<B> &sub_enc)
{
	return params.template build<B>(itl, sub_enc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_turbo_DB<B_8 >* aff3ct::factory::Encoder_turbo_DB::parameters::build<B_8 >(const aff3ct::module::Interleaver<B_8 >&, aff3ct::module::Encoder_RSC_DB<B_8 >&) const;
template aff3ct::module::Encoder_turbo_DB<B_16>* aff3ct::factory::Encoder_turbo_DB::parameters::build<B_16>(const aff3ct::module::Interleaver<B_16>&, aff3ct::module::Encoder_RSC_DB<B_16>&) const;
template aff3ct::module::Encoder_turbo_DB<B_32>* aff3ct::factory::Encoder_turbo_DB::parameters::build<B_32>(const aff3ct::module::Interleaver<B_32>&, aff3ct::module::Encoder_RSC_DB<B_32>&) const;
template aff3ct::module::Encoder_turbo_DB<B_64>* aff3ct::factory::Encoder_turbo_DB::parameters::build<B_64>(const aff3ct::module::Interleaver<B_64>&, aff3ct::module::Encoder_RSC_DB<B_64>&) const;
template aff3ct::module::Encoder_turbo_DB<B_8 >* aff3ct::factory::Encoder_turbo_DB::build<B_8 >(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<B_8 >&, aff3ct::module::Encoder_RSC_DB<B_8 >&);
template aff3ct::module::Encoder_turbo_DB<B_16>* aff3ct::factory::Encoder_turbo_DB::build<B_16>(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<B_16>&, aff3ct::module::Encoder_RSC_DB<B_16>&);
template aff3ct::module::Encoder_turbo_DB<B_32>* aff3ct::factory::Encoder_turbo_DB::build<B_32>(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<B_32>&, aff3ct::module::Encoder_RSC_DB<B_32>&);
template aff3ct::module::Encoder_turbo_DB<B_64>* aff3ct::factory::Encoder_turbo_DB::build<B_64>(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<B_64>&, aff3ct::module::Encoder_RSC_DB<B_64>&);
#else
template aff3ct::module::Encoder_turbo_DB<B>* aff3ct::factory::Encoder_turbo_DB::parameters::build<B>(const aff3ct::module::Interleaver<B>&, aff3ct::module::Encoder_RSC_DB<B>&) const;
template aff3ct::module::Encoder_turbo_DB<B>* aff3ct::factory::Encoder_turbo_DB::build<B>(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<B>&, aff3ct::module::Encoder_RSC_DB<B>&);
#endif
// ==================================================================================== explicit template instantiation
