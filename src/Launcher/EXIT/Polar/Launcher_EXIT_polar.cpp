#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/EXIT/Polar/Simulation_EXIT_polar.hpp"

#include "Launcher_EXIT_polar.hpp"

template <typename B, typename R, typename Q>
Launcher_EXIT_polar<B,R,Q>
::Launcher_EXIT_polar(const int argc, const char **argv, std::ostream &stream)
: Launcher_EXIT<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->params.channel.quant_n_bits       = 6;
	this->params.channel.quant_point_pos    = 3;

	// default parameters
	this->params.code.type                  = "POLAR";
	this->params.decoder.algo               = "SCAN";
	this->params.decoder.implem             = "NAIVE";

	this->params.decoder.max_iter           = 1;
	this->params.simulation.awgn_codes_dir  = "../awgn_polar_codes/TV";
	this->params.simulation.bin_pb_path     = "../lib/polar_bounds/bin/polar_bounds";
	this->params.simulation.awgn_codes_file = "";
	this->params.decoder.L                  = 1;
	this->params.code.sigma                 = 0.3;
#ifdef ENABLE_POLAR_BOUNDS
	this->params.code.fb_gen_method         = "TV";
#else
	this->params.code.fb_gen_method         = "GA";
#endif
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::build_args()
{
	Launcher_EXIT<B,R,Q>::build_args();

	this->req_args[{"cde-sigma"}] =
		{"float",
		 "sigma value used for the polar codes generation."};

	this->opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the SCAN decoder."};
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args[{"cde-awgn-fb-path"}] =
		{"string",
		 "directory where are located the best channels to use for information bits."};
	this->opt_args[{"sim-pb-path"}] =
		{"string",
		 "path of the polar bounds code generator (generates best channels to use)."};
#endif
	this->opt_args[{"cde-awgn-fb-file"}] =
		{"string",
		 "set the best channels bits by giving path to file."};
	this->opt_args[{"dec-lists", "L"}] =
		{"positive_int",
		 "maximal number of paths in the SCL decoder."};
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args[{"cde-fb-gen-method"}] =
		{"string",
		 "select the frozen bits generation method.",
		 "GA, TV"};
#endif
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::store_args()
{
	Launcher_EXIT<B,R,Q>::store_args();

	if(this->ar.exist_arg({"dec-ite", "i"     })) this->params.decoder.max_iter           = this->ar.get_arg_int  ({"dec-ite", "i"});
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg({"cde-awgn-fb-path" })) this->params.simulation.awgn_codes_dir  = this->ar.get_arg      ({"cde-awgn-fb-path"});
	if(this->ar.exist_arg({"sim-pb-path"      })) this->params.simulation.bin_pb_path     = this->ar.get_arg      ({"sim-pb-path"});
#endif
	if(this->ar.exist_arg({"cde-awgn-fb-file" })) this->params.simulation.awgn_codes_file = this->ar.get_arg      ({"cde-awgn-fb-file"});
	if(this->ar.exist_arg({"dec-lists", "L"   })) this->params.decoder.L                  = this->ar.get_arg_int  ({"dec-lists", "L"});
	if(this->ar.exist_arg({"cde-sigma"        })) this->params.code.sigma                 = this->ar.get_arg_float({"cde-sigma"});
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg({"cde-fb-gen-method"})) this->params.code.fb_gen_method         = this->ar.get_arg      ({"cde-fb-gen-method"});
#endif

	// force 1 iteration max if not SCAN (and polar code)
	if (this->params.decoder.algo != "SCAN") this->params.decoder.max_iter = 1;
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::print_header()
{
	Launcher_EXIT<B,R,Q>::print_header();

	this->stream << "# " << bold("* Decoding iterations per frame ") << " = " << this->params.decoder.max_iter           << std::endl;
	if (!this->params.simulation.awgn_codes_file.empty())
	this->stream << "# " << bold("* Path to best channels file    ") << " = " << this->params.simulation.awgn_codes_file << std::endl;
	this->stream << "# " << bold("* Number of lists in the SCL (L)") << " = " << this->params.decoder.L                  << std::endl;
	this->stream << "# " << bold("* Sigma for code generation     ") << " = " << this->params.code.sigma                 << std::endl;
	this->stream << "# " << bold("* Frozen bits generation method ") << " = " << this->params.code.fb_gen_method         << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher_EXIT_polar<B,R,Q>
::build_simu()
{
	this->simu = new Simulation_EXIT_polar<B,R,Q>(this->params);
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
