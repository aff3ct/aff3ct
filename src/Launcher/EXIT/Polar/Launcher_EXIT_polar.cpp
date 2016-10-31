#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/EXIT/Polar/Simulation_EXIT_polar.hpp"

#include "Launcher_EXIT_polar.hpp"

template <typename B, typename R, typename Q>
Launcher_EXIT_polar<B,R,Q>
::Launcher_EXIT_polar(const int argc, const char **argv, std::ostream &stream)
: Launcher_EXIT<B,R,Q>(argc, argv, stream)
{
	this->params.simulation.bin_pb_path   = "../lib/polar_bounds/bin/polar_bounds";
	this->params.code      .type          = "POLAR";
	this->params.code      .awgn_fb_path  = "../conf/cde/awgn_polar_codes/TV";
	this->params.code      .sigma         = 0.3f;
	this->params.code      .fb_gen_method = "TV";
	this->params.encoder   .type          = "POLAR";
	this->params.quantizer .n_bits        = 6;
	this->params.quantizer .n_decimals    = 3;
	this->params.decoder   .type          = "SCAN";
	this->params.decoder   .implem        = "NAIVE";
	this->params.decoder   .n_ite         = 1;
	this->params.decoder   .L             = 1;
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::build_args()
{
	Launcher_EXIT<B,R,Q>::build_args();

	// ---------------------------------------------------------------------------------------------------- simulation
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args[{"sim-pb-path"}] =
		{"string",
		 "path of the polar bounds code generator (generates best channels to use)."};
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	this->req_args[{"cde-sigma"}] =
		{"float",
		 "sigma value used for the polar codes generation."};
	this->opt_args[{"cde-awgn-fb-path"}] =
		{"string",
		 "path to a file or a directory containing the best channels to use for information bits."};
	this->opt_args[{"cde-fb-gen-method"}] =
		{"string",
		 "select the frozen bits generation method.",
		 "GA, TV"};

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-type"}][2] += ", POLAR";

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("SCAN" );
	this->opt_args[{"dec-implem"   }].push_back("NAIVE");
	this->opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the SCAN decoder."};
	this->opt_args[{"dec-lists", "L"}] =
		{"positive_int",
		 "maximal number of paths in the SCL decoder."};
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::store_args()
{
	Launcher_EXIT<B,R,Q>::store_args();

	// ---------------------------------------------------------------------------------------------------- simulation
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg({"sim-pb-path"})) this->params.simulation.bin_pb_path = this->ar.get_arg({"sim-pb-path"});
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	if(this->ar.exist_arg({"cde-sigma"        })) this->params.code.sigma         = this->ar.get_arg_float({"cde-sigma"});
	if(this->ar.exist_arg({"cde-awgn-fb-path" })) this->params.code.awgn_fb_path  = this->ar.get_arg      ({"cde-awgn-fb-path"});
	if(this->ar.exist_arg({"cde-fb-gen-method"})) this->params.code.fb_gen_method = this->ar.get_arg      ({"cde-fb-gen-method"});

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-ite",   "i"})) this->params.decoder.n_ite = this->ar.get_arg_int({"dec-ite",   "i"});
	if(this->ar.exist_arg({"dec-lists", "L"})) this->params.decoder.L     = this->ar.get_arg_int({"dec-lists", "L"});

	// force 1 iteration max if not SCAN (and polar code)
	if (this->params.decoder.type != "SCAN") this->params.decoder.n_ite = 1;
}

template <typename B, typename R, typename Q>
Simulation* Launcher_EXIT_polar<B,R,Q>
::build_simu()
{
	return new Simulation_EXIT_polar<B,R,Q>(this->params);
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_EXIT_polar<B,R,Q>
::header_code()
{
	std::string sigma = (this->params.code.sigma == 0.f) ? "adaptative" : std::to_string(this->params.code.sigma);

	auto p = Launcher_EXIT<B,R,Q>::header_code();

	p.push_back(std::make_pair("Sigma for code gen.",     sigma                          ));
	p.push_back(std::make_pair("Frozen bits gen. method", this->params.code.fb_gen_method));
	if (this->params.code.fb_gen_method != "GA" && !this->params.code.awgn_fb_path.empty())
		p.push_back(std::make_pair("Path to the best channels", this->params.code.awgn_fb_path));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_EXIT_polar<B,R,Q>
::header_decoder()
{
	auto p = Launcher_EXIT<B,R,Q>::header_decoder();

	if (!this->params.decoder.simd_strategy.empty())
		p.push_back(std::make_pair("SIMD strategy", this->params.decoder.simd_strategy));

	if (this->params.decoder.type == "SCAN")
		p.push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->params.decoder.n_ite)));

	if (this->params.decoder.type == "SCL")
		p.push_back(std::make_pair("Num. of lists (L)", std::to_string(this->params.decoder.L)));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_EXIT_polar<B_32,R_32,Q_32>;
template class Launcher_EXIT_polar<B_64,R_64,Q_64>;
#else
#if defined(PREC_32_BIT) || defined(PREC_64_BIT)
template class Launcher_EXIT_polar<B,R,Q>;
#endif
#endif
// ==================================================================================== explicit template instantiation
