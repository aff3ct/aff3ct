#include <iostream>

#if defined(SYSTEMC)
#include "Simulation/BFER/Iterative/SystemC/SC_Simulation_BFER_ite.hpp"
#else
#include "Simulation/BFER/Iterative/Threads/Simulation_BFER_ite_threads.hpp"
#endif
#include "Tools/Codec/Polar/Codec_polar.hpp"

#include "Launcher_BFERI_polar.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher_BFERI_polar<B,R,Q>
::Launcher_BFERI_polar(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFERI<B,R,Q>(argc, argv, stream)
{
	this->params.simulation.bin_pb_path   = "../lib/polar_bounds/bin/polar_bounds";
	this->params.code      .type          = "POLAR";
	this->params.code      .awgn_fb_path  = "../conf/cde/awgn_polar_codes/TV";
	this->params.code      .sigma         = 0.f;
	this->params.code      .fb_gen_method = "GA";
	this->params.crc       .type          = "FAST";
	this->params.encoder   .type          = "POLAR";
	this->params.quantizer .n_bits        = 6;
	this->params.quantizer .n_decimals    = 1;
	this->params.decoder   .type          = "SCAN";
	this->params.decoder   .implem        = "NAIVE";
	this->params.decoder   .n_ite         = 1;
	this->params.decoder   .L             = 8;
	this->params.decoder   .simd_strategy = "";
	this->params.decoder   .polar_nodes   = "{R0,R0L,R1,REP,REPL,SPC}";
	this->params.decoder   .full_adaptive = true;
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_polar<B,R,Q>
::build_args()
{
	Launcher_BFERI<B,R,Q>::build_args();

	// ---------------------------------------------------------------------------------------------------- simulation
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args[{"sim-pb-path"}] =
		{"string",
		 "path of the polar bounds code generator (generates best channels to use)."};
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	this->opt_args[{"cde-sigma"}] =
		{"positive_float",
		 "sigma value for the polar codes generation (adaptative frozen bits if sigma is not set)."};
	this->opt_args[{"cde-fb-gen-method"}] =
		{"string",
		 "select the frozen bits generation method.",
		 "GA, TV"};
	this->opt_args[{"cde-awgn-fb-path"}] =
		{"string",
		 "path to a file or a directory containing the best channels to use for information bits."};

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-type"}][2] += ", POLAR";

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("SCAN");
	this->opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the SCAN decoder."};
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_polar<B,R,Q>
::store_args()
{
	Launcher_BFERI<B,R,Q>::store_args();

	this->params.code.N_code = (int)std::exp2(this->params.code.m);

	// ---------------------------------------------------------------------------------------------------- simulation
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg({"sim-pb-path"})) this->params.simulation.bin_pb_path = this->ar.get_arg({"sim-pb-path"});
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	if(this->ar.exist_arg({"cde-sigma"        })) this->params.code.sigma         = this->ar.get_arg_float({"cde-sigma"});
	if(this->ar.exist_arg({"cde-awgn-fb-path" })) this->params.code.awgn_fb_path  = this->ar.get_arg      ({"cde-awgn-fb-path" });
	if(this->ar.exist_arg({"cde-fb-gen-method"})) this->params.code.fb_gen_method = this->ar.get_arg      ({"cde-fb-gen-method"});

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-ite",      "i"})) this->params.decoder.n_ite      = this->ar.get_arg_int  ({"dec-ite",    "i"});

	// force 1 iteration max if not SCAN (and polar code)
	if (this->params.decoder.type != "SCAN") this->params.decoder.n_ite = 1;
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFERI_polar<B,R,Q>
::build_simu()
{
	this->codec = new Codec_polar<B,Q>(this->params);
#if defined(SYSTEMC)
	return new SC_Simulation_BFER_ite     <B,R,Q>(this->params, *this->codec);
#else
	return new Simulation_BFER_ite_threads<B,R,Q>(this->params, *this->codec);
#endif
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFERI_polar<B,R,Q>
::header_code()
{
	std::string sigma = (this->params.code.sigma == 0.f) ? "adaptative" : std::to_string(this->params.code.sigma);

	auto p = Launcher_BFERI<B,R,Q>::header_code();

	p.push_back(std::make_pair("Sigma for code gen.",     sigma                           ));
	p.push_back(std::make_pair("Frozen bits gen. method", this->params.code.fb_gen_method ));
	if (this->params.code.fb_gen_method != "GA" && !this->params.code.awgn_fb_path.empty())
		p.push_back(std::make_pair("Path to the best channels", this->params.code.awgn_fb_path));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFERI_polar<B,R,Q>
::header_decoder()
{
	auto p = Launcher_BFERI<B,R,Q>::header_decoder();

	if (this->params.decoder.type == "SCAN")
		p.push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->params.decoder.n_ite)));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_BFERI_polar<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_BFERI_polar<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_BFERI_polar<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_BFERI_polar<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_BFERI_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
