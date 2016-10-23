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
	this->params.encoder  .type          = "RSC";
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
	this->opt_args[{"enc-type"}][2] += ", RSC";
	this->opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};
	this->opt_args[{"enc-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

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

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-simd"   })) this->params.decoder.simd_strategy = this->ar.get_arg({"dec-simd"});
	if(this->ar.exist_arg({"dec-max"    })) this->params.decoder.max           = this->ar.get_arg({"dec-max" });

	if (this->params.decoder.simd_strategy == "INTER" && !this->ar.exist_arg({"sim-inter-lvl"}))
		this->params.simulation.inter_frame_level = mipp::nElReg<Q>();
	else if (this->params.decoder.simd_strategy == "INTRA" && !this->ar.exist_arg({"sim-inter-lvl"}))
		this->params.simulation.inter_frame_level = std::ceil(mipp::nElReg<Q>() / 8.f);

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

	std::stringstream poly;
	poly << "{0" << std::oct << this->params.encoder.poly[0] << ",0" << std::oct << this->params.encoder.poly[1] << "}";

	auto p = Launcher_BFER<B,R,Q>::header_encoder();

	p.push_back(std::make_pair(std::string("Polynomials"), poly.str()));
	p.push_back(std::make_pair(std::string("Buffered"), buff_enc));

	return p;
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
