#include <iostream>
#include <cmath>

#include "Simulation/GEN/Code/Polar/Generation_polar.hpp"

#include "Launcher_GEN_polar.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
using namespace aff3ct::launcher;

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
	this->params.decoder   .polar_nodes   = "{R0,R0L,R1,REP,REPL,SPC}";
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
	this->opt_args[{"dec-type"}] =
		{"string",
		 "type of the decoder to generate.",
		 "SC, SCL"};
	this->opt_args[{"dec-polar-nodes"}] =
		{"string",
		 "the type of nodes you want to detect in the Polar tree (ex: {R0,R1,R0L,REP_2-8,REPL,SPC_4+})."};

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

	if(this->ar.exist_arg({"cde-awgn-fb-path" })) this->params.code.awgn_fb_path  = this->ar.get_arg({"cde-awgn-fb-path" });
	if(this->ar.exist_arg({"cde-fb-gen-method"})) this->params.code.fb_gen_method = this->ar.get_arg({"cde-fb-gen-method"});

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-type"       })) this->params.decoder.type        = this->ar.get_arg({"dec-type"       });
	if(this->params.decoder.type == "SCL")      this->params.decoder.gen_path    = "../src/Module/Decoder/Polar/SCL/CRC/Generated";
	if(this->ar.exist_arg({"dec-gen-path"   })) this->params.decoder.gen_path    = this->ar.get_arg({"dec-gen-path"   });
	if(this->ar.exist_arg({"dec-polar-nodes"})) this->params.decoder.polar_nodes = this->ar.get_arg({"dec-polar-nodes"});
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

	p.push_back(std::make_pair("Generated decoder path", this->params.decoder.gen_path   ));
	p.push_back(std::make_pair("Type",                   this->params.decoder.type       ));
	p.push_back(std::make_pair("Polar node types",       this->params.decoder.polar_nodes));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_GEN_polar<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_GEN_polar<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_GEN_polar<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_GEN_polar<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_GEN_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
