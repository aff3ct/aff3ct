#include "Module/Puncturer/NO/Puncturer_NO.hpp"

#include "Puncturer.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer::name   = "Puncturer";
const std::string aff3ct::factory::Puncturer::prefix = "pct";

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer
::build(const parameters &params)
{
	if (params.type == "NO") return new module::Puncturer_NO<B,Q>(params.K, params.N, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Puncturer
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{p+"-fra-size", "N"}] =
		{"positive_int",
		 "total number of bit transmitted (frame size)."};

	opt_args[{p+"-type"}] =
		{"string",
		 "code puncturer type.",
		 "NO"};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};
}

void Puncturer
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p+"-info-bits", "K"})) params.K        = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-fra-size",  "N"})) params.N        = std::stoi(vals.at({p+"-fra-size",  "N"}));
	if(exist(vals, {p+"-fra",       "F"})) params.n_frames = std::stoi(vals.at({p+"-fra",       "F"}));
	if(exist(vals, {p+"-type"          })) params.type     =           vals.at({p+"-type"          });

	params.N_cw = params.N;
	params.R = (float)params.K / (float)params.N;
}

void Puncturer
::header(params_list& head_pct, const parameters& params)
{
	head_pct.push_back(std::make_pair("Type", params.type));
	head_pct.push_back(std::make_pair("Info. bits (K)", std::to_string(params.K)));
	head_pct.push_back(std::make_pair("Frame size (N)", std::to_string(params.N)));
	head_pct.push_back(std::make_pair("Codeword size", std::to_string(params.N_cw)));
	head_pct.push_back(std::make_pair("Code rate (R)", std::to_string(params.R)));
	head_pct.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer::parameters&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer::build<B_16,Q_16>(const aff3ct::factory::Puncturer::parameters&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer::build<B_32,Q_32>(const aff3ct::factory::Puncturer::parameters&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer::build<B_64,Q_64>(const aff3ct::factory::Puncturer::parameters&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer::build<B,Q>(const aff3ct::factory::Puncturer::parameters&);
#endif
// ==================================================================================== explicit template instantiation
