#include <cmath>
#include <regex>
#include <string>
#include <iostream>
#include <sstream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/EXIT/Code/RSC/Simulation_EXIT_RSC.hpp"

#include "Launcher_EXIT_RSC.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q, typename QD>
Launcher_EXIT_RSC<B,R,Q,QD>
::Launcher_EXIT_RSC(const int argc, const char **argv, std::ostream &stream)
: Launcher_EXIT<B,R,Q>(argc, argv, stream)
{
	this->params.code     .type          = "RSC";
	this->params.code     .tail_length   = 2*3;
	this->params.encoder  .type          = "RSC";
	this->params.encoder  .buffered      = false;
	this->params.encoder  .poly          = {013, 015};
	this->params.quantizer.n_bits        = 6;
	this->params.quantizer.n_decimals    = 3;
	this->params.decoder  .type          = "BCJR";
	this->params.decoder  .implem        = "FAST";
	this->params.decoder  .max           = "MAX";
	this->params.decoder  .simd_strategy = "";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::build_args()
{
	Launcher_EXIT<B,R,Q>::build_args();

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-type"}][2] += ", RSC";
	this->opt_args[{"enc-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("BCJR, LTE, CCSDS"             );
	this->opt_args[{"dec-implem"   }].push_back("GENERIC, STD, FAST, VERY_FAST");
	this->opt_args[{"dec-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXS, MAXL"};
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::store_args()
{
	Launcher_EXIT<B,R,Q>::store_args();

	// ------------------------------------------------------------------------------------------------------- encoder
	if(this->ar.exist_arg({"enc-type"})) this->params.encoder.type = this->ar.get_arg({"enc-type"});
	if (this->params.encoder.type == "GENERIC")
	{
		if (this->ar.exist_arg({"enc-poly"}))
		{
			auto poly_str = this->ar.get_arg({"enc-poly"});
//			std::regex pattern("\\{\\d{1-5}\\,\\d{1-5}\\}");
//			assert(std::regex_match(poly_str, pattern));
#ifdef _MSC_VER
			sscanf_s(poly_str.c_str(), "{%o,%o}", &this->params.encoder.poly[0], &this->params.encoder.poly[1]);
#else
			std::sscanf(poly_str.c_str(), "{%o,%o}", &this->params.encoder.poly[0], &this->params.encoder.poly[1]);
#endif
		}
	}

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-max"})) this->params.decoder.max = this->ar.get_arg({"dec-max"});

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

	this->params.code.tail_length = (int)(2 * std::floor(std::log2((float)std::max(this->params.encoder.poly[0],
	                                                                               this->params.encoder.poly[1]))));
}

template <typename B, typename R, typename Q, typename QD>
Simulation* Launcher_EXIT_RSC<B,R,Q,QD>
::build_simu()
{
	return new Simulation_EXIT_RSC<B,R,Q,QD>(this->params);
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_EXIT_RSC<B,R,Q,QD>
::header_encoder()
{
	std::string buff_enc = ((this->params.encoder.buffered) ? "on" : "off");

	std::stringstream poly;
	poly << "{0" << std::oct << this->params.encoder.poly[0] << ",0" << std::oct << this->params.encoder.poly[1] << "}";

	auto p = Launcher_EXIT<B,R,Q>::header_encoder();

	p.push_back(std::make_pair(std::string("Polynomials"), poly.str()));
	p.push_back(std::make_pair(std::string("Buffered"), buff_enc));

	return p;
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_EXIT_RSC<B,R,Q,QD>
::header_decoder()
{
	auto p = Launcher_EXIT<B,R,Q>::header_decoder();

	p.push_back(std::make_pair("Max type", this->params.decoder.max));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_EXIT_RSC<B_32,R_32,Q_32,QD_32>;
template class Launcher_EXIT_RSC<B_64,R_64,Q_64,QD_64>;
#else
#if defined(PREC_32_BIT) || defined(PREC_64_BIT)
template class Launcher_EXIT_RSC<B,R,Q,QD>;
#endif
#endif
// ==================================================================================== explicit template instantiation
