#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/BFERI/RSC/Simulation_BFERI_RSC.hpp"

#include "Launcher_BFERI_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_BFERI_RSC<B,R,Q,QD>
::Launcher_BFERI_RSC(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFERI<B,R,Q>(argc, argv, stream)
{
	this->params.code     .type          = "RSC";
	this->params.code     .tail_length   = 2*3;
	this->params.encoder  .buffered      = true;
	this->params.encoder  .poly          = {13,11};
	this->params.quantizer.n_bits        = 6;
	this->params.quantizer.n_decimals    = 3;
	this->params.decoder  .type          = "BCJR";
	this->params.decoder  .implem        = "GENERIC";
	this->params.decoder  .max           = "MAXS";
	this->params.decoder  .simd_strategy = "";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFERI_RSC<B,R,Q,QD>
::build_args()
{
	Launcher_BFERI<B,R,Q>::build_args();

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("BCJR, BCJR4, BCJR_G, LTE, CCSDS"      );
	this->opt_args[{"dec-implem"   }].push_back("GENERIC, STD, FAST, VERY_FAST");
	this->opt_args[{"dec-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};
	this->opt_args[{"dec-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXS, MAXL"};
	this->opt_args[{"enc-poly"}] =
			{"string",
			 "the polynomials describing RSC code (Used only with --dec-type set to BCJR_G). Should be of the form (A,B)."};
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFERI_RSC<B,R,Q,QD>
::store_args()
{
	Launcher_BFERI<B,R,Q>::store_args();

	// ------------------------------------------------------------------------------------------------------- encoder
	if(this->ar.exist_arg({"enc-no-buff"})) this->params.encoder.buffered      = false;

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-simd"})) this->params.decoder.simd_strategy = this->ar.get_arg({"dec-simd"});
	if(this->ar.exist_arg({"dec-max" })) this->params.decoder.max           = this->ar.get_arg({"dec-max" });

	if (this->params.decoder.type == "BCJR4" || this->params.decoder.type == "CCSDS")
		this->params.code.tail_length = 2*4;

	if (this->params.decoder.type == "BCJR_G")
	{
		std::string poly_str;
		if(this->ar.exist_arg({"enc-poly"    }))
			{
				poly_str           = this->ar.get_arg({"enc-poly" });
				std::sscanf (poly_str.c_str(), "(%o,%o)", &this->params.encoder.poly[0] , &this->params.encoder.poly[1]);
				this->params.code.tail_length = 2*std::floor(std::log2((float)std::max(this->params.encoder.poly[0],this->params.encoder.poly[1])));
			}
	}
}

template <typename B, typename R, typename Q, typename QD>
Simulation* Launcher_BFERI_RSC<B,R,Q,QD>
::build_simu()
{
	return new Simulation_BFERI_RSC<B,R,Q,QD>(this->params);
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_BFERI_RSC<B,R,Q,QD>
::header_encoder()
{
	std::string buff_enc = ((this->params.encoder.buffered) ? "on" : "off");

	auto p = Launcher_BFERI<B,R,Q>::header_encoder();

	p.push_back(std::make_pair("Buffered", buff_enc));

	return p;
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::pair<std::string,std::string>> Launcher_BFERI_RSC<B,R,Q,QD>
::header_decoder()
{
	auto p = Launcher_BFERI<B,R,Q>::header_decoder();

	if (!this->params.decoder.simd_strategy.empty())
		p.push_back(std::make_pair("SIMD strategy", this->params.decoder.simd_strategy));
	p.push_back(std::make_pair("Max type", this->params.decoder.max));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFERI_RSC<B_8,R_8,Q_8,QD_8>;
template class Launcher_BFERI_RSC<B_16,R_16,Q_16,QD_16>;
template class Launcher_BFERI_RSC<B_32,R_32,Q_32,QD_32>;
template class Launcher_BFERI_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Launcher_BFERI_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
