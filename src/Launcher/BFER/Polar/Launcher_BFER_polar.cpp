#include <iostream>
#include <cassert>
#include <cmath>

#include "Tools/Display/bash_tools.h"

#include "Module/CRC/Polynomial/CRC_polynomial.hpp"

#include "Launcher_BFER_polar.hpp"
#include "../../../Simulation/BFER/Polar/Simulation_BFER_polar.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER_polar<B,R,Q>
::Launcher_BFER_polar(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->params.channel.quant_n_bits       = 6;
	this->params.channel.quant_point_pos    = 1;

	// default parameters
	this->params.code.type                  = "POLAR";
	this->params.decoder.algo               = "SC";
	this->params.decoder.implem             = "FAST";

	this->params.decoder.max_iter           = 1;
	this->params.simulation.awgn_codes_dir  = "../awgn_polar_codes/TV";
	this->params.simulation.bin_pb_path     = "../lib/polar_bounds/bin/polar_bounds";
	this->params.simulation.awgn_codes_file = "";
	this->params.decoder.L                  = 1;
	this->params.code.sigma                 = 0.f;
	this->params.code.crc                   = "";
#ifdef ENABLE_POLAR_BOUNDS
	this->params.code.fb_gen_method         = "TV";
#else
	this->params.code.fb_gen_method         = "GA";
#endif
	this->params.decoder.simd_strategy      = "";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_polar<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	// ---------------------------------------------------------------------------------------------------- simulation
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args[{"sim-pb-path"}] =
		{"string",
		 "path of the polar bounds code generator (generates best channels to use)."};
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	this->opt_args[{"cde-awgn-fb-file"}] =
		{"string",
		 "set the best channels bits by giving path to file."};
	this->opt_args[{"cde-sigma"}] =
		{"positive_float",
		 "sigma value for the polar codes generation (adaptative frozen bits if sigma is not set)."};
#ifdef ENABLE_POLAR_BOUNDS
	this->opt_args[{"cde-fb-gen-method"}] =
		{"string",
		 "select the frozen bits generation method.",
		 "GA, TV"};
	this->opt_args[{"cde-awgn-fb-path"}] =
		{"string",
		 "directory where are located the best channels to use for information bits."};
#endif

	// ----------------------------------------------------------------------------------------------------------- crc
	this->opt_args[{"crc-type"}] =
		{"string",
		 "select the crc you want to use.",
		 "1-0x1, 2-0x1, 3-0x3, 4-ITU, 8-DVB-S2, 16-CCITT, 16-IBM, 24-LTEA, 32-GZIP"};

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-no-sys"}] =
		{"",
		 "disable the systematic encoding."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the SCAN decoder."};
	this->opt_args[{"dec-lists", "L"}] =
		{"positive_int",
		 "maximal number of paths in the SCL decoder."};
	this->opt_args[{"dec-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};
}

template <typename B, typename R, typename Q>
void Launcher_BFER_polar<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	// ---------------------------------------------------------------------------------------------------- simulation
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg({"sim-pb-path"})) this->params.simulation.bin_pb_path = this->ar.get_arg({"sim-pb-path"});
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	if(this->ar.exist_arg({"cde-sigma"        })) this->params.code.sigma                 = this->ar.get_arg_float({"cde-sigma"});
	if(this->ar.exist_arg({"cde-awgn-fb-file" })) this->params.simulation.awgn_codes_file = this->ar.get_arg      ({"cde-awgn-fb-file" });
#ifdef ENABLE_POLAR_BOUNDS
	if(this->ar.exist_arg({"cde-awgn-fb-path" })) this->params.simulation.awgn_codes_dir  = this->ar.get_arg      ({"cde-awgn-fb-path" });
	if(this->ar.exist_arg({"cde-fb-gen-method"})) this->params.code.fb_gen_method         = this->ar.get_arg      ({"cde-fb-gen-method"});
#endif

	// ----------------------------------------------------------------------------------------------------------- crc
	if(this->ar.exist_arg({"crc-type"})) this->params.code.crc = this->ar.get_arg({"crc-type"});

	// ------------------------------------------------------------------------------------------------------- encoder
	if(this->ar.exist_arg({"enc-no-sys"})) this->params.encoder.systematic = false;

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-ite",   "i"})) this->params.decoder.max_iter      = this->ar.get_arg_int  ({"dec-ite",   "i"});
	if(this->ar.exist_arg({"dec-lists", "L"})) this->params.decoder.L             = this->ar.get_arg_int  ({"dec-lists", "L"});
	if(this->ar.exist_arg({"dec-simd"      })) this->params.decoder.simd_strategy = this->ar.get_arg      ({"dec-simd"      });

	// force 1 iteration max if not SCAN (and polar code)
	if (this->params.decoder.algo != "SCAN") this->params.decoder.max_iter = 1;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_polar<B,R,Q>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();

	std::string sigma = (this->params.code.sigma == 0.f) ? "adaptative" : std::to_string(this->params.code.sigma);

	// display configuration and simulation parameters
	if (this->params.decoder.algo == "SCAN")
	this->stream << "# " << bold("* Decoding iterations per frame ") << " = " << this->params.decoder.max_iter           << std::endl;
	if (!this->params.simulation.awgn_codes_file.empty())
	this->stream << "# " << bold("* Path to best channels file    ") << " = " << this->params.simulation.awgn_codes_file << std::endl;
	if (this->params.decoder.algo == "SCL")
	this->stream << "# " << bold("* Number of lists in the SCL (L)") << " = " << this->params.decoder.L                  << std::endl;
	this->stream << "# " << bold("* Sigma for code generation     ") << " = " << sigma                                   << std::endl;
	this->stream << "# " << bold("* Frozen bits generation method ") << " = " << this->params.code.fb_gen_method         << std::endl;
	if (!this->params.code.crc.empty())
	this->stream << "# " << bold("* CRC type                      ") << " = " << this->params.code.crc                   << std::endl;
	if (!this->params.decoder.simd_strategy.empty())
	this->stream << "# " << bold("* Decoder SIMD strategy         ") << " = " << this->params.decoder.simd_strategy      << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_polar<B,R,Q>
::build_simu()
{

	// hack for K when there is a CRC
	if (!this->params.code.crc.empty())
	{
		assert(this->params.code.K > CRC_polynomial<B>::size(this->params.code.crc));
		this->params.code.K += CRC_polynomial<B>::size(this->params.code.crc);
	}

	this->simu = new Simulation_BFER_polar<B,R,Q>(this->params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_polar<B_8,R_8,Q_8>;
template class Launcher_BFER_polar<B_16,R_16,Q_16>;
template class Launcher_BFER_polar<B_32,R_32,Q_32>;
template class Launcher_BFER_polar<B_64,R_64,Q_64>;
#else
template class Launcher_BFER_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
