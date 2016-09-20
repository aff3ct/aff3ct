#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/EXIT/RSC/Simulation_EXIT_RSC.hpp"

#include "Launcher_EXIT_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_EXIT_RSC<B,R,Q,QD>
::Launcher_EXIT_RSC(const int argc, const char **argv, std::ostream &stream)
: Launcher_EXIT<B,R,Q>(argc, argv, stream)
{
	this->params.code     .type        = "RSC";
	this->params.code     .tail_length = 2*3;
	this->params.quantizer.n_bits      = 6;
	this->params.quantizer.n_decimals  = 3;
	this->params.decoder  .type        = "BCJR";
	this->params.decoder  .implem      = "FAST";
	this->params.decoder  .max         = "MAX";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::build_args()
{
	Launcher_EXIT<B,R,Q>::build_args();

	// ------------------------------------------------------------------------------------------------------- decoder
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

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-max"})) this->params.decoder.max = this->ar.get_arg({"dec-max"});

	if (this->params.decoder.type == "BCJR4" || this->params.decoder.type == "CCSDS")
		this->params.code.tail_length = 2*4;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::build_simu()
{
	this->simu = new Simulation_EXIT_RSC<B,R,Q,QD>(this->params);
}

template <typename B, typename R, typename Q, typename QD>
std::vector<std::vector<std::string>> Launcher_EXIT_RSC<B,R,Q,QD>
::header_decoder()
{
	auto p = Launcher_EXIT<B,R,Q>::header_decoder();

	p.push_back({"Max type", this->params.decoder.max});

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_EXIT_RSC<B_8,R_8,Q_8,QD_8>;
template class Launcher_EXIT_RSC<B_16,R_16,Q_16,QD_16>;
template class Launcher_EXIT_RSC<B_32,R_32,Q_32,QD_32>;
template class Launcher_EXIT_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Launcher_EXIT_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
