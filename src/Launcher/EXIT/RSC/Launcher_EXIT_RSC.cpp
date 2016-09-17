#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/EXIT/RSC/Simulation_EXIT_RSC.hpp"

#include "Launcher_EXIT_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_EXIT_RSC<B,R,Q,QD>
::Launcher_EXIT_RSC(const int argc, const char **argv, std::ostream &stream)
: Launcher_EXIT<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->params.code.tail_length        = 2*3;
	this->params.channel.quant_n_bits    = 6;
	this->params.channel.quant_point_pos = 3;

	// default parameters
	this->params.code.type               = "RSC";
	this->params.decoder.algo            = "BCJR";
	this->params.decoder.implem          = "FAST";
	this->params.decoder.max             = "MAX";

}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::build_args()
{
	Launcher_EXIT<B,R,Q>::build_args();

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

	if(this->ar.exist_arg({"dec-max"})) this->params.decoder.max = this->ar.get_arg({"dec-max"});

	if (this->params.decoder.algo == "BCJR4" || this->params.decoder.algo == "CCSDS")
		this->params.code.tail_length = 2*4;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::print_header()
{
	Launcher_EXIT<B,R,Q>::print_header();

	this->stream << "# " << bold("* Decoder MAP implementation    ") << " = " << this->params.decoder.max << std::endl;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::build_simu()
{
	this->simu = new Simulation_EXIT_RSC<B,R,Q,QD>(this->params);
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
