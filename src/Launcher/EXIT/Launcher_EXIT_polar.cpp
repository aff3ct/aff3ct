#include <iostream>

#include "Tools/bash_tools.h"
#include "Simulation/EXIT/Polar/Simulation_EXIT_polar.hpp"

#include "Launcher_EXIT_polar.hpp"

template <typename B, typename R, typename Q>
Launcher_EXIT_polar<B,R,Q>
::Launcher_EXIT_polar(const int argc, const char **argv, std::ostream &stream)
: Launcher_EXIT<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->chan_params.quant_n_bits    = 6;
	this->chan_params.quant_point_pos = 3;

	// default parameters
	this->code_params.type            = "POLAR";
	this->deco_params.algo            = "SCAN";
	this->deco_params.implem          = "NAIVE";

	this->deco_params.max_iter        = 1;
	this->simu_params.awgn_codes_dir  = "../awgn_polar_codes/TV";
	this->simu_params.bin_pb_path     = "../lib/polar_bounds/bin/polar_bounds";
	this->simu_params.awgn_codes_file = "";
	this->deco_params.L               = 1;
	this->code_params.sigma           = 0.3;
#ifdef ENABLE_POLAR_BOUNDS
	this->code_params.fb_gen_method   = "TV";
#else
	this->code_params.fb_gen_method   = "GA";
#endif
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::build_args()
{
	Launcher_EXIT<B,R,Q>::build_args();

	this->req_args["code-sigma"     ] = "sigma_value";
	this->doc_args["code-sigma"     ] = "sigma value used for the polar codes generation.";

	this->opt_args["max-iter"       ] = "n_iterations";
	this->doc_args["max-iter"       ] = "maximal number of iterations in the SCAN decoder.";
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args["awgn-codes-dir" ] = "directory";
	this->doc_args["awgn-codes-dir" ] = "directory where are located the best channels to use for information bits.";
	this->opt_args["bin-pb-path"    ] = "path";
	this->doc_args["bin-pb-path"    ] = "path of the polar bounds code generator (generates best channels to use).";
#endif
	this->opt_args["awgn-codes-file"] = "path";
	this->doc_args["awgn-codes-file"] = "set the best channels bits by giving path to file.";
	this->opt_args["L"              ] = "L";
	this->doc_args["L"              ] = "maximal number of paths in the SCL decoder.";
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args["fb-gen-method"  ] = "method";
	this->doc_args["fb-gen-method"  ] = "select the frozen bits generation method (ex: GA or TV).";
#endif
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::store_args()
{
	Launcher_EXIT<B,R,Q>::store_args();

	if(this->ar.exist_arg("max-iter"       )) this->deco_params.max_iter        = std::stoi(this->ar.get_arg("max-iter"));
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg("awgn-codes-dir" )) this->simu_params.awgn_codes_dir  = this->ar.get_arg("awgn-codes-dir");
	if(this->ar.exist_arg("bin-pb-path"    )) this->simu_params.bin_pb_path     = this->ar.get_arg("bin-pb-path");
#endif
	if(this->ar.exist_arg("awgn-codes-file")) this->simu_params.awgn_codes_file = this->ar.get_arg("awgn-codes-file");
	if(this->ar.exist_arg("L"              )) this->deco_params.L               = std::stoi(this->ar.get_arg("L"));
	if(this->ar.exist_arg("code-sigma"     )) this->code_params.sigma           = std::stof(this->ar.get_arg("code-sigma"));
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg("fb-gen-method"  )) this->code_params.fb_gen_method   = this->ar.get_arg("fb-gen-method");
#endif

	// force 1 iteration max if not SCAN (and polar code)
	if (this->deco_params.algo != "SCAN") this->deco_params.max_iter = 1;
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::print_header()
{
	Launcher_EXIT<B,R,Q>::print_header();

	this->stream << "# " << bold("* Decoding iterations per frame ") << " = " << this->deco_params.max_iter        << std::endl;
	if (!this->simu_params.awgn_codes_file.empty())
	this->stream << "# " << bold("* Path to best channels file    ") << " = " << this->simu_params.awgn_codes_file << std::endl;
	this->stream << "# " << bold("* Number of lists in the SCL (L)") << " = " << this->deco_params.L               << std::endl;
	this->stream << "# " << bold("* Sigma for code generation     ") << " = " << this->code_params.sigma           << std::endl;
	this->stream << "# " << bold("* Frozen bits generation method ") << " = " << this->code_params.fb_gen_method   << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::build_simu()
{
	this->simu = new Simulation_EXIT_polar<B,R,Q>(this->simu_params, 
	                                              this->code_params, 
	                                              this->enco_params,
	                                              this->mod_params,
	                                              this->chan_params, 
	                                              this->deco_params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_EXIT_polar<B_8,R_8,Q_8>;
template class Launcher_EXIT_polar<B_16,R_16,Q_16>;
template class Launcher_EXIT_polar<B_32,R_32,Q_32>;
template class Launcher_EXIT_polar<B_64,R_64,Q_64>;
#else
template class Launcher_EXIT_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
