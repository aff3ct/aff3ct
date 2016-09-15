#include <iostream>
#include <cassert>
#include <cmath>

#include "Tools/Display/bash_tools.h"
#include "Simulation/GEN/Polar/Generation_polar.hpp"

#include "Launcher_GEN_polar.hpp"

template <typename B, typename R, typename Q>
Launcher_GEN_polar<B,R,Q>
::Launcher_GEN_polar(const int argc, const char **argv, std::ostream &stream)
: Launcher_GEN<B,R,Q>(argc, argv, stream)
{
	// default parameters
	this->params.code.type                  = "POLAR";
	this->params.decoder.algo               = "SC";

	this->params.simulation.gen_decoder_dir = "../src/Decoder/Polar/SC/Generated";
	this->params.simulation.awgn_codes_dir  = "../awgn_polar_codes/TV";
	this->params.simulation.bin_pb_path     = "../lib/polar_bounds/bin/polar_bounds";
	this->params.simulation.awgn_codes_file = "";
	this->params.code.sigma                 = 0.f;
#ifdef ENABLE_POLAR_BOUNDS
	this->params.code.fb_gen_method         = "TV";
#else
	this->params.code.fb_gen_method         = "GA";
#endif
	this->params.decoder.simd_strategy      = "";
}

template <typename B, typename R, typename Q>
void Launcher_GEN_polar<B,R,Q>
::build_args()
{
	this->req_args["K"              ] = "n_bits";
	this->doc_args["K"              ] = "useful number of bit transmitted (only information bits).";
	this->req_args["N"              ] = "n_bits";
	this->doc_args["N"              ] = "total number of bit transmitted (includes parity bits).";
	this->req_args["snr"            ] = "snr_value";
	this->doc_args["snr"            ] = "signal/noise ratio for the frozen bits generation.";
	this->req_args["code-type"      ] = "code-type";
	this->doc_args["code-type"      ] = "select the code type you want to use (ex: POLAR, TURBO, REPETITION, RA, RSC, UNCODED).";

	this->opt_args["simu-type"      ] = "name";
	this->doc_args["simu-type"      ] = "select the type of simulation to launch (default is BFER).";
#ifdef MULTI_PREC
	this->opt_args["prec"           ] = "prec";
	this->doc_args["prec"           ] = "the simulation precision in bit (ex: 8, 16, 32 or 64).";
#endif
	this->opt_args["gen-decoder-dir"] = "directory";
	this->doc_args["gen-decoder-dir"] = "directory where are located the generated decoders.";
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args["awgn-codes-dir" ] = "directory";
	this->doc_args["awgn-codes-dir" ] = "directory where are located the best channels to use for information bits.";
	this->opt_args["bin-pb-path"    ] = "path";
	this->doc_args["bin-pb-path"    ] = "path of the polar bounds code generator (generates best channels to use).";
#endif
	this->opt_args["awgn-codes-file"] = "path";
	this->doc_args["awgn-codes-file"] = "set the best channels bits by giving path to file.";
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args["fb-gen-method"  ] = "method";
	this->doc_args["fb-gen-method"  ] = "select the frozen bits generation method (ex: GA or TV).";
#endif
}

template <typename B, typename R, typename Q>
void Launcher_GEN_polar<B,R,Q>
::store_args()
{
	// required parameters
	this->params.code.K = std::stoi(this->ar.get_arg("K"));
	this->params.code.N = std::stoi(this->ar.get_arg("N"));
	this->params.code.m = std::ceil(std::log2(this->params.code.N));

	if (this->params.code.K > this->params.code.N)
	{
		std::cerr << bold_red("(EE) K have to be smaller than N, exiting.") << std::endl;
		exit(EXIT_FAILURE);
	}

	this->params.simulation.snr_min = std::stof(this->ar.get_arg("snr"));

	// facultative parameters
	if(this->ar.exist_arg("simu-type"      )) this->params.simulation.type              = this->ar.get_arg("simu-type");
	if(this->ar.exist_arg("code-gen-method")) this->params.code.generation_method       = this->ar.get_arg("code-gen-method");
	if(this->ar.exist_arg("gen-decoder-dir")) this->params.simulation.gen_decoder_dir   = this->ar.get_arg("gen-decoder-dir");
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg("awgn-codes-dir" )) this->params.simulation.awgn_codes_dir    = this->ar.get_arg("awgn-codes-dir");
	if(this->ar.exist_arg("bin-pb-path"    )) this->params.simulation.bin_pb_path       = this->ar.get_arg("bin-pb-path");
#endif
	if(this->ar.exist_arg("awgn-codes-file")) this->params.simulation.awgn_codes_file   = this->ar.get_arg("awgn-codes-file");
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg("fb-gen-method"  )) this->params.code.fb_gen_method           = this->ar.get_arg("fb-gen-method");
#endif
}

template <typename B, typename R, typename Q>
void Launcher_GEN_polar<B,R,Q>
::print_header()
{
	// display configuration and simulation parameters
	this->stream << "# " << bold("-------------------------------------------------")                                       << std::endl;
	this->stream << "# " << bold("---- A FAST FORWARD ERROR CORRECTION TOOL >> ----")                                       << std::endl;
	this->stream << "# " << bold("-------------------------------------------------")                                       << std::endl;
	this->stream << "#"                                                                                                     << std::endl;
	this->stream << "# " << bold_underlined("Simulation parameters:")                                                       << std::endl;
	this->stream << "# " << bold("* Simulation type               ") << " = " << this->params.simulation.type               << std::endl;
	this->stream << "# " << bold("* Code type                     ") << " = " << this->params.code.type << " codes"         << std::endl;
	this->stream << "# " << bold("* Number of information bits (K)") << " = " << this->params.code.K                        << std::endl;
	this->stream << "# " << bold("* Codeword length            (N)") << " = " << this->params.code.N                        << std::endl;
	this->stream << "# " << bold("* SNR                           ") << " = " << this->params.simulation.snr_min   << " dB" << std::endl;
	this->stream << "# " << bold("* Generated decoder directory   ") << " = " << this->params.simulation.gen_decoder_dir    << std::endl;
	if (!this->params.simulation.awgn_codes_file.empty())
	this->stream << "# " << bold("* Path to best channels file    ") << " = " << this->params.simulation.awgn_codes_file    << std::endl;
	this->stream << "# " << bold("* Frozen bits generation method ") << " = " << this->params.code.fb_gen_method            << std::endl;
	
	if (this->params.code.N != exp2(ceil(log2(this->params.code.N))))
	{
		std::cerr << bold_red("(EE) N isn't a power of two.")  << std::endl;
		exit(-1);
	}
}

template <typename B, typename R, typename Q>
void Launcher_GEN_polar<B,R,Q>
::build_simu()
{
	this->simu = new Generation_polar(this->params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_GEN_polar<B_8,R_8,Q_8>;
template class Launcher_GEN_polar<B_16,R_16,Q_16>;
template class Launcher_GEN_polar<B_32,R_32,Q_32>;
template class Launcher_GEN_polar<B_64,R_64,Q_64>;
#else
template class Launcher_GEN_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
