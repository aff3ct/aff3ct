#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"

#include "Factory_frozenbits_generator.hpp"

using namespace aff3ct::tools;

template <typename B>
Frozenbits_generator<B>* Factory_frozenbits_generator
::build(const parameters &params)
{
	     if (params.type == "GA"  ) return new Frozenbits_generator_GA  <B>(params.K, params.N_cw,                                 params.sigma);
	else if (params.type == "TV"  ) return new Frozenbits_generator_TV  <B>(params.K, params.N_cw, params.path_fb, params.path_pb, params.sigma);
	else if (params.type == "FILE") return new Frozenbits_generator_file<B>(params.K, params.N_cw, params.path_fb                              );

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_frozenbits_generator
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	req_args[{"enc-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{"enc-cw-size", "N"}] =
		{"positive_int",
		 "the codeword size."};

	opt_args[{"enc-fb-sigma"}] =
		{"positive_float",
		 "sigma value for the polar codes generation (adaptative frozen bits if sigma is not set)."};

	opt_args[{"enc-fb-gen-method"}] =
		{"string",
		 "select the frozen bits generation method.",
		 "GA, FILE, TV"};

	opt_args[{"enc-fb-awgn-path"}] =
		{"string",
		 "path to a file or a directory containing the best channels to use for information bits."};

#ifdef ENABLE_POLAR_BOUNDS
	opt_args[{"enc-fb-pb-path"}] =
		{"string",
		 "path of the polar bounds code generator (generates best channels to use)."};
#endif
}

void Factory_frozenbits_generator
::store_args(const Arguments_reader& ar, parameters &params)
{
	if(ar.exist_arg({"enc-info-bits", "K"})) params.K       = ar.get_arg_int  ({"enc-info-bits", "K"});
	if(ar.exist_arg({"enc-cw-size",   "N"})) params.N_cw    = ar.get_arg_int  ({"enc-cw-size",   "N"});
	if(ar.exist_arg({"enc-fb-sigma"      })) params.sigma   = ar.get_arg_float({"enc-fb-sigma"      });
	if(ar.exist_arg({"enc-fb-awgn-path"  })) params.path_fb = ar.get_arg      ({"enc-fb-awgn-path"  });
	if(ar.exist_arg({"enc-fb-gen-method" })) params.type    = ar.get_arg      ({"enc-fb-gen-method" });

#ifdef ENABLE_POLAR_BOUNDS
	if(ar.exist_arg({"enc-fb-pb-path"})) params.path_pb = ar.get_arg({"enc-fb-pb-path"});
#endif
}

void Factory_frozenbits_generator
::group_args(Arguments_reader::arg_grp& ar)
{
}

void Factory_frozenbits_generator
::header(params_list& head_fb, const parameters& params)
{
	head_fb.push_back(std::make_pair("Fb gen. method", params.type));
	head_fb.push_back(std::make_pair("Fb info. bits (K)", std::to_string(params.K)));
	head_fb.push_back(std::make_pair("Fb codeword size (N)", std::to_string(params.N_cw)));
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
template aff3ct::tools::Frozenbits_generator<B_8 >* aff3ct::tools::Factory_frozenbits_generator::build<B_8 >(const aff3ct::tools::Factory_frozenbits_generator::parameters&);
template aff3ct::tools::Frozenbits_generator<B_16>* aff3ct::tools::Factory_frozenbits_generator::build<B_16>(const aff3ct::tools::Factory_frozenbits_generator::parameters&);
template aff3ct::tools::Frozenbits_generator<B_32>* aff3ct::tools::Factory_frozenbits_generator::build<B_32>(const aff3ct::tools::Factory_frozenbits_generator::parameters&);
template aff3ct::tools::Frozenbits_generator<B_64>* aff3ct::tools::Factory_frozenbits_generator::build<B_64>(const aff3ct::tools::Factory_frozenbits_generator::parameters&);
#else
template aff3ct::tools::Frozenbits_generator<B>* aff3ct::tools::Factory_frozenbits_generator::build<B>(const aff3ct::tools::Factory_frozenbits_generator::parameters&);
#ifndef PREC_32_BIT
template aff3ct::tools::Frozenbits_generator<B_32>* aff3ct::tools::Factory_frozenbits_generator::build<B_32>(const aff3ct::tools::Factory_frozenbits_generator::parameters&);
#endif
#endif
// ==================================================================================== explicit template instantiation
