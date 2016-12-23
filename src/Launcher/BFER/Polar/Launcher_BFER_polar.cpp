#include <iostream>
#include <cassert>
#include <sstream>
#include <cmath>

#include "Tools/Display/bash_tools.h"
#include "Module/CRC/Polynomial/CRC_polynomial.hpp"
#include "Simulation/BFER/Code/Polar/Simulation_BFER_polar.hpp"

#include "Launcher_BFER_polar.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER_polar<B,R,Q>
::Launcher_BFER_polar(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	this->params.simulation.bin_pb_path   = "../lib/polar_bounds/bin/polar_bounds";
	this->params.code      .type          = "POLAR";
	this->params.code      .awgn_fb_path  = "../conf/cde/awgn_polar_codes/TV";
	this->params.code      .sigma         = 0.f;
	this->params.code      .fb_gen_method = "TV";
	this->params.crc       .type          = "STD";
	this->params.encoder   .type          = "POLAR";
	this->params.quantizer .n_bits        = 6;
	this->params.quantizer .n_decimals    = 1;
	this->params.decoder   .type          = "SC";
	this->params.decoder   .implem        = "FAST";
	this->params.decoder   .n_ite         = 1;
	this->params.decoder   .L             = 1;
	this->params.decoder   .simd_strategy = "";
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

	// ----------------------------------------------------------------------------------------------------------- crc
	this->opt_args[{"crc-type"}] =
		{"string",
		 "select the CRC implementation you want to use.",
		 "STD, FAST"};

	this->opt_args[{"crc-poly"}] =
		{"string",
		 "select the CRC polynomial you want to use (ex: \"8-DVB-S2\": 0xD5, \"16-IBM\": 0x8005, \"24-LTEA\": 0x864CFB, \"32-GZIP\": 0x04C11DB7)."};

	this->opt_args[{"crc-size"}] =
		{"positive_int",
		 "size of the CRC (divisor size in bit -1), required if you selected an unknown CRC."};

	this->opt_args[{"crc-rate"}] =
		{"",
		 "enable the CRC to be counted in the code rate computation."};

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-type"}][2] += ", POLAR";
	this->opt_args[{"enc-no-sys"}] =
		{"",
		 "disable the systematic encoding."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("SC, SCL, SCAN");
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
	if(this->ar.exist_arg({"cde-sigma"        })) this->params.code.sigma         = this->ar.get_arg_float({"cde-sigma"});
	if(this->ar.exist_arg({"cde-awgn-fb-path" })) this->params.code.awgn_fb_path  = this->ar.get_arg      ({"cde-awgn-fb-path" });
	if(this->ar.exist_arg({"cde-fb-gen-method"})) this->params.code.fb_gen_method = this->ar.get_arg      ({"cde-fb-gen-method"});

	// ----------------------------------------------------------------------------------------------------------- crc
	if(this->ar.exist_arg({"crc-type"})) this->params.crc.type = this->ar.get_arg    ({"crc-type"});
	if(this->ar.exist_arg({"crc-poly"})) this->params.crc.poly = this->ar.get_arg    ({"crc-poly"});
	if(this->ar.exist_arg({"crc-size"})) this->params.crc.size = this->ar.get_arg_int({"crc-size"});
	if(this->ar.exist_arg({"crc-rate"})) this->params.crc.inc_code_rate = true;

	// ------------------------------------------------------------------------------------------------------- encoder
	if(this->ar.exist_arg({"enc-no-sys"})) this->params.encoder.systematic = false;

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-ite",   "i"})) this->params.decoder.n_ite         = this->ar.get_arg_int({"dec-ite",   "i"});
	if(this->ar.exist_arg({"dec-lists", "L"})) this->params.decoder.L             = this->ar.get_arg_int({"dec-lists", "L"});
	if(this->ar.exist_arg({"dec-simd"      })) this->params.decoder.simd_strategy = this->ar.get_arg    ({"dec-simd"      });

	if (this->params.decoder.simd_strategy == "INTER" && !this->ar.exist_arg({"sim-inter-lvl"}))
		this->params.simulation.inter_frame_level = mipp::nElReg<Q>();

	// force 1 iteration max if not SCAN (and polar code)
	if (this->params.decoder.type != "SCAN") this->params.decoder.n_ite = 1;

	// hack for K when there is a CRC
	if (!this->params.crc.poly.empty())
	{
		if (!this->params.crc.size)
			this->params.crc.size = CRC_polynomial<B>::size(this->params.crc.poly);

		assert(this->params.code.K > this->params.crc.size);
		this->params.code.K += this->params.crc.size;
		assert(this->params.code.K <= this->params.code.N);
	}
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFER_polar<B,R,Q>
::build_simu()
{
	return new Simulation_BFER_polar<B,R,Q>(this->params);
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_polar<B,R,Q>
::header_code()
{
	std::string sigma = (this->params.code.sigma == 0.f) ? "adaptative" : std::to_string(this->params.code.sigma);

	auto p = Launcher_BFER<B,R,Q>::header_code();

	p.push_back(std::make_pair("Sigma for code gen.",     sigma                           ));
	p.push_back(std::make_pair("Frozen bits gen. method", this->params.code.fb_gen_method ));
	if (this->params.code.fb_gen_method != "GA" && !this->params.code.awgn_fb_path.empty())
		p.push_back(std::make_pair("Path to the best channels", this->params.code.awgn_fb_path));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_polar<B,R,Q>
::header_crc()
{
	auto p = Launcher_BFER<B,R,Q>::header_crc();

	if (!this->params.crc.poly.empty())
	{
		std::string crc_inc_rate = (this->params.crc.inc_code_rate) ? "on" : "off";

		auto poly_name = CRC_polynomial<B>::name (this->params.crc.poly);
		std::stringstream poly_val;
		poly_val << "0x" << std::hex << CRC_polynomial<B>::value(this->params.crc.poly);
		auto poly_size = CRC_polynomial<B>::size (this->params.crc.poly);

		p.push_back(std::make_pair("Type", this->params.crc.type));
		if (!poly_name.empty())
			p.push_back(std::make_pair("Name", poly_name));
		p.push_back(std::make_pair("Polynomial (hexadecimal)", poly_val.str()));
		p.push_back(std::make_pair("Size (in bit)", std::to_string(poly_size ? poly_size : this->params.crc.size)));
		p.push_back(std::make_pair("Add CRC in the code rate", crc_inc_rate));
	}

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_polar<B,R,Q>
::header_decoder()
{
	auto p = Launcher_BFER<B,R,Q>::header_decoder();

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
template class Launcher_BFER_polar<B_8,R_8,Q_8>;
template class Launcher_BFER_polar<B_16,R_16,Q_16>;
template class Launcher_BFER_polar<B_32,R_32,Q_32>;
template class Launcher_BFER_polar<B_64,R_64,Q_64>;
#else
template class Launcher_BFER_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
