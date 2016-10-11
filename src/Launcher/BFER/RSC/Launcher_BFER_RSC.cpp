#include <cmath>
#include <regex>
#include <string>
#include <iostream>
#include <sstream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/BFER/RSC/Simulation_BFER_RSC.hpp"

#include "Launcher_BFER_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_BFER_RSC<B,R,Q,QD>
::Launcher_BFER_RSC(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	this->params.code     .tail_length   = 2*3;
	this->params.code     .type          = "RSC";
	this->params.encoder  .type          = "GENERIC";
	this->params.encoder  .buffered      = true;
	this->params.encoder  .poly          = {013, 015};
	this->params.quantizer.n_bits        = 6;
	this->params.quantizer.n_decimals    = 3;
	this->params.decoder  .type          = "BCJR";
	this->params.decoder  .implem        = "FAST";
	this->params.decoder  .max           = "MAX";
	this->params.decoder  .simd_strategy = "";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_RSC<B,R,Q,QD>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};
	this->opt_args[{"enc-type"}] =
		{"string",
		 "the type of the RSC encoder.",
		 "GENERIC"};
	this->opt_args[{"enc-poly"}] =
		{"string",
		 "the polynomials describing RSC code (used only with --enc-type set to GENERIC), should be of the form \"{A,B}\"."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("BCJR, LTE, CCSDS"             );
	this->opt_args[{"dec-implem"   }].push_back("GENERIC, STD, FAST, VERY_FAST");
	this->opt_args[{"dec-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};
	this->opt_args[{"dec-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXL, MAXS"};
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_RSC<B,R,Q,QD>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	// ------------------------------------------------------------------------------------------------------- encoder
	if(this->ar.exist_arg({"enc-no-buff"})) this->params.encoder.buffered = false;
	if(this->ar.exist_arg({"enc-type"   })) this->params.encoder.type     = this->ar.get_arg({"enc-type"});
	if (this->params.encoder.type == "GENERIC")
	{
		if (this->ar.exist_arg({"enc-poly"}))
		{
			auto poly_str = this->ar.get_arg({"enc-poly"});
//			std::regex pattern("\\{\\d{1-5}\\,\\d{1-5}\\}");
//			assert(std::regex_match(poly_str, pattern));
			std::sscanf(poly_str.c_str(), "{%o,%o}", &this->params.encoder.poly[0], &this->params.encoder.poly[1]);
		}
	}

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-simd"   })) this->params.decoder.simd_strategy = this->ar.get_arg({"dec-simd"});
	if(this->ar.exist_arg({"dec-max"    })) this->params.decoder.max           = this->ar.get_arg({"dec-max" });

	if (this->params.decoder.type == "LTE")
	{
		this->params.decoder.type = "BCJR";
		this->params.encoder.poly = {013, 015};
	}

	if (this->params.decoder.type == "CCSDS")
	{
		this->params.decoder.type = "BCJR";
		this->params.encoder.poly = {023, 033};
	}

	if (!(this->params.encoder.poly[0] == 013 && this->params.encoder.poly[1] == 015)) // if not LTE BCJR
	{
		this->params.decoder.type          = "BCJR";
		this->params.decoder.implem        = "GENERIC";
		this->params.decoder.simd_strategy = "";
	}

	this->params.code.tail_length = 2 * std::floor(std::log2((float)std::max(this->params.encoder.poly[0],
	                                                                         this->params.encoder.poly[1])));
}

template <typename B, typename R, typename Q, typename QD>
Simulation* Launcher_BFER_RSC<B,R,Q,QD>
::build_simu()
{
	return new Simulation_BFER_RSC<B,R,Q,QD>(this->params);
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_RSC<B,R,Q,QD>
::header_encoder()
{
	std::string buff_enc = ((this->params.encoder.buffered) ? "on" : "off");

	std::stringstream type;
	type << this->params.encoder.type;
	if (this->params.encoder.type == "GENERIC")
		type << " {0" << std::oct << this->params.encoder.poly[0] << ",0" << std::oct << this->params.encoder.poly[1]
		     << "}";

	auto p = Launcher_BFER<B,R,Q>::header_encoder();

	std::vector<std::pair<std::string,std::string>> p_new;

	p_new.push_back(std::make_pair(std::string("Type"), type.str()));

	for (auto i = 0; i < (int)p.size(); i++)
		p_new.push_back(p[i]);

	p_new.push_back(std::make_pair(std::string("Buffered"), buff_enc));

	return p_new;
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_RSC<B,R,Q,QD>
::header_decoder()
{
	auto p = Launcher_BFER<B,R,Q>::header_decoder();

	if (!this->params.decoder.simd_strategy.empty())
		p.push_back(std::make_pair(std::string("SIMD strategy"), this->params.decoder.simd_strategy));

	p.push_back(std::make_pair(std::string("Max type"), this->params.decoder.max));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_RSC<B_8,R_8,Q_8,QD_8>;
template class Launcher_BFER_RSC<B_16,R_16,Q_16,QD_16>;
template class Launcher_BFER_RSC<B_32,R_32,Q_32,QD_32>;
template class Launcher_BFER_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Launcher_BFER_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
