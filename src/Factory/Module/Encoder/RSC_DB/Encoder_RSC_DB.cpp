#include <algorithm>

#include "Tools/Exception/exception.hpp"

#include "Encoder_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_RSC_DB::name   = "Encoder RSC DB";
const std::string aff3ct::factory::Encoder_RSC_DB::prefix = "enc";

template <typename B>
module::Encoder_RSC_DB<B>* Encoder_RSC_DB::parameters
::build() const
{
	if (this->type == "RSC_DB") return new module::Encoder_RSC_DB<B>(this->K, this->N_cw, this->standard, this->buffered, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_RSC_DB<B>* Encoder_RSC_DB
::build(const parameters &params)
{
	return params.template build<B>();
}

void Encoder_RSC_DB
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args, p);
	req_args.erase({p+"-cw-size", "N"});

	opt_args[{p+"-type"}][2] += ", RSC_DB";

	opt_args[{p+"-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	opt_args[{p+"-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "DVB-RCS1, DVB-RCS2"};
}

void Encoder_RSC_DB
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "RSC_DB";

	Encoder::store_args(vals, params, p);

	if(exist(vals, {p+"-no-buff"})) params.buffered = false;

	if(exist(vals, {p+"-std"})) params.standard = vals.at({p+"-std"});

	params.N_cw = 2 * params.K;
	params.R    = (float)params.K / (float)params.N_cw;
}

void Encoder_RSC_DB
::make_header(params_list& head_enc, const parameters& params, const bool full)
{
	Encoder::make_header(head_enc, params, full);

	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));

	if (!params.standard.empty())
		head_enc.push_back(std::make_pair("Standard", params.standard));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_RSC_DB<B_8 >* aff3ct::factory::Encoder_RSC_DB::parameters::build<B_8 >() const;
template aff3ct::module::Encoder_RSC_DB<B_16>* aff3ct::factory::Encoder_RSC_DB::parameters::build<B_16>() const;
template aff3ct::module::Encoder_RSC_DB<B_32>* aff3ct::factory::Encoder_RSC_DB::parameters::build<B_32>() const;
template aff3ct::module::Encoder_RSC_DB<B_64>* aff3ct::factory::Encoder_RSC_DB::parameters::build<B_64>() const;
template aff3ct::module::Encoder_RSC_DB<B_8 >* aff3ct::factory::Encoder_RSC_DB::build<B_8 >(const aff3ct::factory::Encoder_RSC_DB::parameters&);
template aff3ct::module::Encoder_RSC_DB<B_16>* aff3ct::factory::Encoder_RSC_DB::build<B_16>(const aff3ct::factory::Encoder_RSC_DB::parameters&);
template aff3ct::module::Encoder_RSC_DB<B_32>* aff3ct::factory::Encoder_RSC_DB::build<B_32>(const aff3ct::factory::Encoder_RSC_DB::parameters&);
template aff3ct::module::Encoder_RSC_DB<B_64>* aff3ct::factory::Encoder_RSC_DB::build<B_64>(const aff3ct::factory::Encoder_RSC_DB::parameters&);
#else
template aff3ct::module::Encoder_RSC_DB<B>* aff3ct::factory::Encoder_RSC_DB::parameters::build<B>() const;
template aff3ct::module::Encoder_RSC_DB<B>* aff3ct::factory::Encoder_RSC_DB::build<B>(const aff3ct::factory::Encoder_RSC_DB::parameters&);
#endif
// ==================================================================================== explicit template instantiation
