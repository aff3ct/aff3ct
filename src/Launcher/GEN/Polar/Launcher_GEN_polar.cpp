#include <iostream>
#include <cassert>
#include <cmath>

#include "Tools/Display/bash_tools.h"
#include "Simulation/GEN/Code/Polar/Generation_polar.hpp"

#include "Launcher_GEN_polar.hpp"
using namespace aff3ct;

template <typename B, typename R, typename Q>
Launcher_GEN_polar<B,R,Q>
::Launcher_GEN_polar(const int argc, const char **argv, std::ostream &stream)
: Launcher_GEN<B,R,Q>(argc, argv, stream)
{
	this->params.simulation.bin_pb_path   = "../lib/polar_bounds/bin/polar_bounds";
	this->params.code      .type          = "POLAR";
	this->params.code      .awgn_fb_path  = "../conf/cde/awgn_polar_codes/TV";
	this->params.code      .sigma         = 0.f;
	this->params.code      .fb_gen_method = "TV";
	this->params.decoder   .simd_strategy = "";
	this->params.decoder   .type          = "SC";
	this->params.decoder   .gen_path      = "../src/Module/Decoder/Polar/SC/Generated";
}

template <typename B, typename R, typename Q>
void Launcher_GEN_polar<B,R,Q>
::build_args()
{
	// ---------------------------------------------------------------------------------------------------- simulation
	this->opt_args[{"sim-type"}] =
		{"string",
		 "select the type of simulation to launch.",
		 "BFER, BFERI, EXIT, GEN"};
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args[{"sim-pb-path"}] =
		{"string",
		 "path of the polar bounds code generator (generates best channels to use)."};
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	this->req_args[{"cde-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (only information bits)."};
	this->req_args[{"cde-size", "N"}] =
		{"positive_int",
		 "total number of bit transmitted (includes parity bits)."};
	this->req_args[{"cde-type"}] =
		{"string",
		 "select the code type you want to use.",
		 "POLAR, TURBO, REPETITION, LDPC, RA, UNCODED"};
	this->opt_args[{"cde-awgn-fb-path"}] =
		{"string",
		 "path to a file or a directory containing the best channels to use for information bits."};
	this->opt_args[{"cde-fb-gen-method"}] =
		{"string",
		 "select the frozen bits generation method.",
		 "GA, TV"};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->req_args[{"dec-snr"}] =
		{"float",
		 "signal/noise ratio for the frozen bits generation."};
	this->opt_args[{"dec-gen-path"}] =
		{"string",
		 "directory where are located the generated decoders."};

	// --------------------------------------------------------------------------------------------------------- other
#ifdef MULTI_PREC
	this->opt_args[{"prec", "p"}] =
		{"positive_int",
		 "the simulation precision in bit.",
		 "8, 16, 32, 64"};
#endif
}

template <typename B, typename R, typename Q>
void Launcher_GEN_polar<B,R,Q>
::store_args()
{
	// ---------------------------------------------------------------------------------------------------- simulation
	this->params.simulation.snr_min = this->ar.get_arg_float({"dec-snr"});

	if(this->ar.exist_arg({"sim-type"   })) this->params.simulation.type         = this->ar.get_arg({"sim-type"   });
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg({"sim-pb-path"})) this->params.simulation.bin_pb_path  = this->ar.get_arg({"sim-pb-path"});
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	this->params.code.K      = this->ar.get_arg_int({"cde-info-bits", "K"});
	this->params.code.N      = this->ar.get_arg_int({"cde-size",      "N"});
	this->params.code.N_code = this->ar.get_arg_int({"cde-size",      "N"});
	this->params.code.m      = (int)std::ceil(std::log2(this->params.code.N));

	if (this->params.code.K > this->params.code.N)
	{
		std::cerr << bold_red("(EE) K have to be smaller than N, exiting.") << std::endl;
		exit(EXIT_FAILURE);
	}
	if(this->ar.exist_arg({"cde-awgn-fb-path" })) this->params.code.awgn_fb_path  = this->ar.get_arg({"cde-awgn-fb-path" });
	if(this->ar.exist_arg({"cde-fb-gen-method"})) this->params.code.fb_gen_method = this->ar.get_arg({"cde-fb-gen-method"});

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-gen-path"})) this->params.decoder.gen_path = this->ar.get_arg({"dec-gen-path"});
}

template <typename B, typename R, typename Q>
Simulation* Launcher_GEN_polar<B,R,Q>
::build_simu()
{
	return new Generation_polar(this->params);
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_GEN_polar<B,R,Q>
::header_simulation()
{
	std::vector<std::pair<std::string,std::string>> p;

	p.push_back(std::make_pair("Type", this->params.simulation.type                           ));
	p.push_back(std::make_pair("SNR",  std::to_string(this->params.simulation.snr_min) + " dB"));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_GEN_polar<B,R,Q>
::header_code()
{
	std::vector<std::pair<std::string,std::string>> p;

	if (this->params.code.N != std::exp2(std::ceil(std::log2(this->params.code.N))))
	{
		std::cerr << bold_red("(EE) N isn't a power of two.")  << std::endl;
		std::exit(-1);
	}

	p.push_back(std::make_pair("Type",              this->params.code.type             ));
	p.push_back(std::make_pair("Info. bits (K)",    std::to_string(this->params.code.K)));
	p.push_back(std::make_pair("Codeword size (N)", std::to_string(this->params.code.N)));
	if (this->params.code.fb_gen_method != "GA" && !this->params.code.awgn_fb_path.empty())
		p.push_back(std::make_pair("Path to the best channels", this->params.code.awgn_fb_path));

	p.push_back(std::make_pair("Frozen bits gen. method", this->params.code.fb_gen_method));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_GEN_polar<B,R,Q>
::header_decoder()
{
	std::vector<std::pair<std::string,std::string>> p;

	p.push_back(std::make_pair("Generated decoder path", this->params.decoder.gen_path));

	return p;
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
