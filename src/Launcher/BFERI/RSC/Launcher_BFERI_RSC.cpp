#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/BFERI/RSC/Simulation_BFERI_RSC.hpp"

#include "Launcher_BFERI_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_BFERI_RSC<B,R,Q,QD>
::Launcher_BFERI_RSC(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFERI<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->params.code.tail_length        = 2*3;
	this->params.channel.quant_n_bits    = 6;
	this->params.channel.quant_point_pos = 3;

	// default parameters
	this->params.code.type               = "RSC";
	this->params.decoder.algo            = "BCJR";
	this->params.decoder.implem          = "GENERIC";
	this->params.decoder.max             = "MAXS";

	this->params.encoder.buffered        = true;
	this->params.decoder.simd_strategy   = "";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFERI_RSC<B,R,Q,QD>
::build_args()
{
	Launcher_BFERI<B,R,Q>::build_args();

	this->opt_args[{"disable-buf-enc"}] =
		{"",
		 "disable the buffered encoding."};
	this->opt_args[{"dec-simd-strat"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};
	this->opt_args[{"dec-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXS, MAXL"};
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFERI_RSC<B,R,Q,QD>
::store_args()
{
	Launcher_BFERI<B,R,Q>::store_args();

	if(this->ar.exist_arg({"disable-buf-enc"})) this->params.encoder.buffered      = false;

	if(this->ar.exist_arg({"dec-simd-strat" })) this->params.decoder.simd_strategy = this->ar.get_arg({"dec-simd-strat"});
	if(this->ar.exist_arg({"dec-max"        })) this->params.decoder.max           = this->ar.get_arg({"dec-max"       });

	if (this->params.decoder.algo == "BCJR4" || this->params.decoder.algo == "CCSDS")
		this->params.code.tail_length = 2*4;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFERI_RSC<B,R,Q,QD>
::print_header()
{
	Launcher_BFERI<B,R,Q>::print_header();

	std::string buff_enc = ((this->params.encoder.buffered) ? "on" : "off");

	// display configuration and simulation parameters
	this->stream << "# " << bold("* Buffered encoding             ") << " = " << buff_enc                           << std::endl;
	if (!this->params.decoder.simd_strategy.empty())
	this->stream << "# " << bold("* Decoder SIMD strategy         ") << " = " << this->params.decoder.simd_strategy << std::endl;
	this->stream << "# " << bold("* Decoder MAP implementation    ") << " = " << this->params.decoder.max           << std::endl;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFERI_RSC<B,R,Q,QD>
::build_simu()
{
	this->simu = new Simulation_BFERI_RSC<B,R,Q,QD>(this->params);
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
