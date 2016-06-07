#include <iostream>

#include "../../Simulation/EXIT/RSC/Simulation_EXIT_RSC.hpp"

#include "../../Tools/bash_tools.h"

#include "Launcher_EXIT_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_EXIT_RSC<B,R,Q,QD>
::Launcher_EXIT_RSC(const int argc, const char **argv)
: Launcher_EXIT<B,R,Q>(argc, argv)
{
	// override parameters
	this->code_params.tail_length     = 2*3;
	this->chan_params.quant_n_bits    = 6;
	this->chan_params.quant_point_pos = 3;

	// default parameters
	this->code_params.type   = "RSC";
	this->deco_params.algo   = "BCJR";
	this->deco_params.implem = "FAST";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::build_args()
{
	Launcher_EXIT<B,R,Q>::build_args();
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::store_args()
{
	Launcher_EXIT<B,R,Q>::store_args();
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::print_header()
{
	Launcher_EXIT<B,R,Q>::print_header();
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_EXIT_RSC<B,R,Q,QD>
::build_simu()
{
	this->simu = new Simulation_EXIT_RSC<B,R,Q,QD>(this->simu_params, 
	                                               this->code_params, 
	                                               this->enco_params, 
	                                               this->chan_params, 
	                                               this->deco_params);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_EXIT_RSC<B_8,R_8,Q_8,QD_8>;
template class Launcher_EXIT_RSC<B_16,R_16,Q_16,QD_16>;
template class Launcher_EXIT_RSC<B_32,R_32,Q_32,QD_32>;
template class Launcher_EXIT_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Launcher_EXIT_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation