#include <iostream>

#include "Tools/bash_tools.h"
#include "Simulation/BFERI/Uncoded/Simulation_BFERI_uncoded.hpp"

#include "Launcher_BFERI_uncoded.hpp"

template <typename B, typename R, typename Q>
Launcher_BFERI_uncoded<B,R,Q>
::Launcher_BFERI_uncoded(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFERI<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->chan_params.quant_n_bits    = 6;
	this->chan_params.quant_point_pos = 2;

	// default parameters
	this->code_params.type   = "Channel";
	this->deco_params.algo   = "NONE";
	this->deco_params.implem = "NONE";
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_uncoded<B,R,Q>
::build_args()
{
	Launcher_BFERI<B,R,Q>::build_args();
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_uncoded<B,R,Q>
::store_args()
{
	Launcher_BFERI<B,R,Q>::store_args();
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_uncoded<B,R,Q>
::print_header()
{
	Launcher_BFERI<B,R,Q>::print_header();
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_uncoded<B,R,Q>
::build_simu()
{
	this->simu = new Simulation_BFERI_uncoded<B,R,Q>(this->simu_params, 
	                                                 this->code_params, 
	                                                 this->enco_params, 
	                                                 this->mod_params,
	                                                 this->chan_params,
	                                                 this->deco_params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFERI_uncoded<B_8,R_8,Q_8>;
template class Launcher_BFERI_uncoded<B_16,R_16,Q_16>;
template class Launcher_BFERI_uncoded<B_32,R_32,Q_32>;
template class Launcher_BFERI_uncoded<B_64,R_64,Q_64>;
#else
template class Launcher_BFERI_uncoded<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
