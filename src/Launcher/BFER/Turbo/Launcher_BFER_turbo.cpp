#include <cmath>
#include <regex>
#include <string>
#include <iostream>
#include <sstream>

#include "Module/CRC/Polynomial/CRC_polynomial.hpp"
#include "Simulation/BFER/Code/Turbo/Simulation_BFER_turbo.hpp"

#include "Launcher_BFER_turbo.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
using namespace aff3ct::launcher;
using namespace aff3ct::module;

template <typename B, typename R, typename Q, typename QD>
Launcher_BFER_turbo<B,R,Q,QD>
::Launcher_BFER_turbo(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	this->params.simulation .json_path      = "";
	this->params.code       .type           = "TURBO";
	this->params.code       .tail_length    = 4 * 3;
	this->params.crc        .type           = "STD";
	this->params.encoder    .type           = "TURBO";
	this->params.encoder    .buffered       = true;
	this->params.encoder    .poly           = {013, 015};
	this->params.interleaver.type           = "LTE";
	this->params.interleaver.path           = "";
	this->params.interleaver.n_cols         = 4;
	this->params.interleaver.uniform        = false;
	this->params.quantizer  .n_bits         = 6;
	this->params.quantizer  .n_decimals     = (typeid(Q) == typeid(short)) ? 3 : 2;
	this->params.decoder    .type           = "BCJR";
	this->params.decoder    .implem         = "FAST";
	this->params.decoder    .max            = "MAX";
	this->params.decoder    .n_ite          = 6;
	this->params.decoder    .scaling_factor = "LTE_VEC";
	this->params.decoder    .simd_strategy  = "";
	this->params.decoder    .self_corrected = false;
	this->params.decoder    .fnc            = false;
	this->params.decoder    .fnc_q          = 10;
	this->params.decoder    .fnc_ite_min    = 3;
	this->params.decoder    .fnc_ite_max    = this->params.decoder.n_ite;
	this->params.decoder    .fnc_ite_step   = 1;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	// ---------------------------------------------------------------------------------------------------- simulation
	this->opt_args[{"sim-json-path"}] =
		{"string",
		 "path to store the encoder and decoder traces formated in JSON."};

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
	this->opt_args[{"enc-type"}][2] += ", TURBO";
	this->opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};
	this->opt_args[{"enc-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	// --------------------------------------------------------------------------------------------------- interleaver
	this->opt_args[{"itl-type"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, RANDOM, GOLDEN, USER, RAND_COL, ROW_COL, NO"};

	this->opt_args[{"itl-path"}] =
		{"string",
		 "specify the path to the interleaver file (to use with \"--itl-type USER\"."};

	this->opt_args[{"itl-cols"}] =
		{"positive_int",
		 "specify the number of columns used for the COLUMNS interleaver."};

	this->opt_args[{"itl-uni"}] =
		{"",
		 "enable the regeneration of the interleaver for each new frame."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("BCJR, LTE, CCSDS"             );
	this->opt_args[{"dec-implem"   }].push_back("GENERIC, STD, FAST, VERY_FAST");
	this->opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo decoder."};
	this->opt_args[{"dec-sf"}] =
		{"string",
		 "scaling factor type.",
		 "NO, LTE, LTE_VEC, ARRAY"};
	this->opt_args[{"dec-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};
	this->opt_args[{"dec-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXS, MAXL"};
	this->opt_args[{"dec-sc"}] =
		{"",
		 "enables the self corrected decoder (requires \"--crc-type\")."};
	this->opt_args[{"dec-fnc"}] =
		{"",
		 "enables the flip and check decoder (requires \"--crc-type\")."};
	this->opt_args[{"dec-fnc-q"}] =
		{"positive_int",
		 "set the search's space for the fnc algorithm."};
	this->opt_args[{"dec-fnc-ite-m"}] =
		{"positive_int",
		 "set first iteration at which the fnc is used."};
	this->opt_args[{"dec-fnc-ite-M"}] =
		{"positive_int",
		 "set last iteration at which the fnc is used."};
	this->opt_args[{"dec-fnc-ite-s"}] =
		{"positive_int",
		 "set iteration step for the fnc algorithm."};

}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	// ---------------------------------------------------------------------------------------------------- simulation
	if(this->ar.exist_arg({"sim-json-path"})) this->params.simulation.json_path = this->ar.get_arg({"sim-json-path"});

	// ----------------------------------------------------------------------------------------------------------- crc
	if(this->ar.exist_arg({"crc-type"})) this->params.crc.type = this->ar.get_arg    ({"crc-type"});
	if(this->ar.exist_arg({"crc-poly"})) this->params.crc.poly = this->ar.get_arg    ({"crc-poly"});
	if(this->ar.exist_arg({"crc-size"})) this->params.crc.size = this->ar.get_arg_int({"crc-size"});
	if(this->ar.exist_arg({"crc-rate"})) this->params.crc.inc_code_rate = true;

	if (!this->params.crc.poly.empty() && !this->params.crc.size)
		this->params.crc.size = CRC_polynomial<B>::size(this->params.crc.poly);

	// ------------------------------------------------------------------------------------------------------- encoder
	if(this->ar.exist_arg({"enc-no-buff"})) this->params.encoder.buffered = false;
	if(this->ar.exist_arg({"enc-type"   })) this->params.encoder.type     = this->ar.get_arg({"enc-type"});

	if (!this->params.simulation.json_path.empty())
		this->params.encoder.type = "TURBO_JSON";

	if (this->ar.exist_arg({"enc-poly"}))
	{
		auto poly_str = this->ar.get_arg({"enc-poly"});
//		std::regex pattern("\\{\\d{1-5}\\,\\d{1-5}\\}");
//		assert(std::regex_match(poly_str, pattern));
#ifdef _MSC_VER
		sscanf_s(poly_str.c_str(), "{%o,%o}", &this->params.encoder.poly[0], &this->params.encoder.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &this->params.encoder.poly[0], &this->params.encoder.poly[1]);
#endif
	}

	// --------------------------------------------------------------------------------------------------- interleaver
	if(this->ar.exist_arg({"itl-type"})) this->params.interleaver.type    = this->ar.get_arg    ({"itl-type"});
	if(this->ar.exist_arg({"itl-path"})) this->params.interleaver.path    = this->ar.get_arg    ({"itl-path"});
	if(this->ar.exist_arg({"itl-cols"})) this->params.interleaver.n_cols  = this->ar.get_arg_int({"itl-cols"});
	if(this->ar.exist_arg({"itl-uni" })) this->params.interleaver.uniform = true;

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-ite", "i"})) this->params.decoder.n_ite          = this->ar.get_arg_int({"dec-ite", "i"});
	if(this->ar.exist_arg({"dec-sf"      })) this->params.decoder.scaling_factor = this->ar.get_arg    ({"dec-sf"      });
	if(this->ar.exist_arg({"dec-simd"    })) this->params.decoder.simd_strategy  = this->ar.get_arg    ({"dec-simd"    });
	if(this->ar.exist_arg({"dec-max"     })) this->params.decoder.max            = this->ar.get_arg    ({"dec-max"     });

	if (this->ar.exist_arg({"crc-poly"}))
	{
		if (this->ar.exist_arg({"dec-sc"}))
			this->params.decoder.self_corrected = true;
		else if (this->ar.exist_arg({"dec-fnc"}))
		{
			this->params.decoder.fnc = true;
			if (this->ar.exist_arg({"dec-fnc-q"}))
				this->params.decoder.fnc_q = this->ar.get_arg_int({"dec-fnc-q"});
			if (this->ar.exist_arg({"dec-fnc-ite-m"}))
				this->params.decoder.fnc_ite_min = this->ar.get_arg_int({"dec-fnc-ite-m"});
			if (this->ar.exist_arg({"dec-fnc-ite-M"}))
				this->params.decoder.fnc_ite_max = this->ar.get_arg_int({"dec-fnc-ite-M"});
			else
				this->params.decoder.fnc_ite_max = this->params.decoder.n_ite;
			if (this->ar.exist_arg({"dec-fnc-ite-s"}))
				this->params.decoder.fnc_ite_step = this->ar.get_arg_int({"dec-fnc-ite-s"});
		}
	}

	if (this->params.decoder.simd_strategy == "INTER" && !this->ar.exist_arg({"sim-inter-lvl"}))
		this->params.simulation.inter_frame_level = mipp::nElReg<Q>();
	else if (this->params.decoder.simd_strategy == "INTRA" && !this->ar.exist_arg({"sim-inter-lvl"}))
		this->params.simulation.inter_frame_level = (int)std::ceil(mipp::nElReg<Q>() / 8.f);

	if (this->params.decoder.type == "LTE")
	{
		this->params.decoder.type = "BCJR";
		this->params.encoder.poly = {013, 015};
		if (!this->ar.exist_arg({"itl-type"}))
			this->params.interleaver.type = "LTE";
	}

	if (this->params.decoder.type == "CCSDS")
	{
		this->params.decoder.type = "BCJR";
		this->params.encoder.poly = {023, 033};
		if (!this->ar.exist_arg({"itl-type"}))
			this->params.interleaver.type = "CCSDS";
	}

	if (!(this->params.encoder.poly[0] == 013 && this->params.encoder.poly[1] == 015)) // if not LTE BCJR
	{
		this->params.decoder.type          = "BCJR";
		this->params.decoder.implem        = "GENERIC";
		this->params.decoder.simd_strategy = "";
	}

	if (!this->params.simulation.json_path.empty())
	{
		this->params.decoder.type          = "BCJR";
		this->params.decoder.implem        = "GENERIC_JSON";
		this->params.decoder.simd_strategy = "";
	}

	this->params.code.tail_length = (int)(4 * std::floor(std::log2((float)std::max(this->params.encoder.poly[0],
	                                                                               this->params.encoder.poly[1]))));
}

template <typename B, typename R, typename Q, typename QD>
Simulation* Launcher_BFER_turbo<B,R,Q,QD>
::build_simu()
{
	return new Simulation_BFER_turbo<B,R,Q,QD>(this->params);
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_turbo<B,R,Q,QD>
::header_simulation()
{
	auto p = Launcher_BFER<B,R,Q>::header_simulation();

	if (!this->params.simulation.json_path.empty())
		p.push_back(std::make_pair("Path to the JSON file", this->params.simulation.json_path));

	return p;
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_turbo<B,R,Q,QD>
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

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_turbo<B,R,Q,QD>
::header_encoder()
{
	std::string buff_enc = ((this->params.encoder.buffered) ? "on" : "off");

	std::stringstream poly;
	poly << "{0" << std::oct << this->params.encoder.poly[0] << ",0" << std::oct << this->params.encoder.poly[1] << "}";

	auto p = Launcher_BFER<B,R,Q>::header_encoder();

	p.push_back(std::make_pair(std::string("Polynomials"), poly.str()));
	p.push_back(std::make_pair(std::string("Buffered"), buff_enc));

	return p;
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_turbo<B,R,Q,QD>
::header_decoder()
{
	auto p = Launcher_BFER<B,R,Q>::header_decoder();

	std::string sc  = ((this->params.decoder.self_corrected) ? "on" : "off");
	std::string fnc = ((this->params.decoder.fnc) ? "on" : "off");

	if (!this->params.decoder.simd_strategy.empty())
		p.push_back(std::make_pair("SIMD strategy", this->params.decoder.simd_strategy));

	p.push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->params.decoder.n_ite)));
	p.push_back(std::make_pair("Scaling factor",         this->params.decoder.scaling_factor       ));
	p.push_back(std::make_pair("Max type",               this->params.decoder.max                  ));
	if (this->ar.exist_arg({"crc-type"}))
	{
		p.push_back(std::make_pair("Self-corrected", sc));
		p.push_back(std::make_pair("Flip aNd Check (FNC)", fnc));
		if (this->params.decoder.fnc)
		{
			p.push_back(std::make_pair("FNC q"       ,  std::to_string(this->params.decoder.fnc_q)));
			p.push_back(std::make_pair("FNC ite min" ,  std::to_string(this->params.decoder.fnc_ite_min)));
			p.push_back(std::make_pair("FNC ite max" ,  std::to_string(this->params.decoder.fnc_ite_max)));
			p.push_back(std::make_pair("FNC ite step",  std::to_string(this->params.decoder.fnc_ite_step)));
		}
	}
	return p;
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_turbo<B,R,Q,QD>
::header_interleaver()
{
	auto p = Launcher_BFER<B,R,Q>::header_interleaver();

	p.push_back(std::make_pair("Type", this->params.interleaver.type));

	if (this->params.interleaver.type == "USER")
		p.push_back(std::make_pair("Path", this->params.interleaver.path));

	if (this->params.interleaver.type == "RAND_COL" || this->params.interleaver.type == "ROW_COL")
		p.push_back(std::make_pair("Number of columns", std::to_string(this->params.interleaver.n_cols)));

	p.push_back(std::make_pair("Uniform", (this->params.interleaver.uniform ? "on" : "off")));

	return p;
}
// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_BFER_turbo<B_8,R_8,Q_8,QD_8>;
template class aff3ct::launcher::Launcher_BFER_turbo<B_16,R_16,Q_16,QD_16>;
template class aff3ct::launcher::Launcher_BFER_turbo<B_32,R_32,Q_32,QD_32>;
template class aff3ct::launcher::Launcher_BFER_turbo<B_64,R_64,Q_64,QD_64>;
#else
template class aff3ct::launcher::Launcher_BFER_turbo<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
