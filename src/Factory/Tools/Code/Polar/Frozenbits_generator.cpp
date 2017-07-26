#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"

#include "Frozenbits_generator.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Frozenbits_generator::name   = "Frozen bits generator";
const std::string aff3ct::factory::Frozenbits_generator::prefix = "fbg";

template <typename B>
tools::Frozenbits_generator<B>* Frozenbits_generator
::build(const parameters &params)
{
	     if (params.type == "GA"  ) return new tools::Frozenbits_generator_GA  <B>(params.K, params.N_cw,                                 params.sigma);
	else if (params.type == "TV"  ) return new tools::Frozenbits_generator_TV  <B>(params.K, params.N_cw, params.path_fb, params.path_pb, params.sigma);
	else if (params.type == "FILE") return new tools::Frozenbits_generator_file<B>(params.K, params.N_cw, params.path_fb                              );

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Frozenbits_generator
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{p+"-cw-size", "N"}] =
		{"positive_int",
		 "the codeword size."};

	opt_args[{p+"-sigma"}] =
		{"positive_float",
		 "sigma value for the polar codes generation (adaptative frozen bits if sigma is not set)."};

	opt_args[{p+"-gen-method"}] =
		{"string",
		 "select the frozen bits generation method.",
		 "GA, FILE, TV"};

	opt_args[{p+"-awgn-path"}] =
		{"string",
		 "path to a file or a directory containing the best channels to use for information bits."};

#ifdef ENABLE_POLAR_BOUNDS
	opt_args[{p+"-pb-path"}] =
		{"string",
		 "path of the polar bounds code generator (generates best channels to use)."};
#endif
}

void Frozenbits_generator
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p+"-info-bits", "K"})) params.K       = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-cw-size",   "N"})) params.N_cw    = std::stoi(vals.at({p+"-cw-size",   "N"}));
	if(exist(vals, {p+"-sigma"         })) params.sigma   = std::stof(vals.at({p+"-sigma"         }));
	if(exist(vals, {p+"-awgn-path"     })) params.path_fb =           vals.at({p+"-awgn-path"     });
	if(exist(vals, {p+"-gen-method"    })) params.type    =           vals.at({p+"-gen-method"    });

#ifdef ENABLE_POLAR_BOUNDS
	if(exist(vals, {p+"-pb-path"})) params.path_pb = vals.at({p+"-pb-path"});
#endif
}

void Frozenbits_generator
::make_header(params_list& head_fb, const parameters& params, const bool full)
{
	head_fb.push_back(std::make_pair("Fb gen. method", params.type));
	if (full) head_fb.push_back(std::make_pair("Fb info. bits (K)", std::to_string(params.K)));
	if (full) head_fb.push_back(std::make_pair("Fb codeword size (N)", std::to_string(params.N_cw)));
	head_fb.push_back(std::make_pair("Fb sigma", params.sigma == -1.0f ? "adaptive" : std::to_string(params.sigma)));
#ifdef ENABLE_POLAR_BOUNDS
	if (params.type == "TV")
		head_fb.push_back(std::make_pair("Fb PB path", params.path_pb));
#endif
	if (params.type == "TV" || params.type == "FILE")
		head_fb.push_back(std::make_pair("Fb path", params.path_fb));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Frozenbits_generator<B_8 >* aff3ct::factory::Frozenbits_generator::build<B_8 >(const aff3ct::factory::Frozenbits_generator::parameters&);
template aff3ct::tools::Frozenbits_generator<B_16>* aff3ct::factory::Frozenbits_generator::build<B_16>(const aff3ct::factory::Frozenbits_generator::parameters&);
template aff3ct::tools::Frozenbits_generator<B_32>* aff3ct::factory::Frozenbits_generator::build<B_32>(const aff3ct::factory::Frozenbits_generator::parameters&);
template aff3ct::tools::Frozenbits_generator<B_64>* aff3ct::factory::Frozenbits_generator::build<B_64>(const aff3ct::factory::Frozenbits_generator::parameters&);
#else
template aff3ct::tools::Frozenbits_generator<B>* aff3ct::factory::Frozenbits_generator::build<B>(const aff3ct::factory::Frozenbits_generator::parameters&);
#ifndef PREC_32_BIT
template aff3ct::tools::Frozenbits_generator<B_32>* aff3ct::factory::Frozenbits_generator::build<B_32>(const aff3ct::factory::Frozenbits_generator::parameters&);
#endif
#endif
// ==================================================================================== explicit template instantiation
