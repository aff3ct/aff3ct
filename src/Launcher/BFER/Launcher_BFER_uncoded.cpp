#include <iostream>

#ifdef SYSTEMC
#include "../../Simulation/SC_BFER/Uncoded/Simulation_SC_BFER_uncoded.hpp"
#else
#include "../../Simulation/BFER/Uncoded/Simulation_uncoded.hpp"
#endif
#include "../../Tools/bash_tools.h"

#include "Launcher_BFER_uncoded.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER_uncoded<B,R,Q>
::Launcher_BFER_uncoded(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->chan_params.quant_n_bits    = 6;
	this->chan_params.quant_point_pos = 2;

	// default parameters
	this->code_params.type            = "Channel";
	this->deco_params.algo            = "NONE";
	this->deco_params.implem          = "NONE";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_uncoded<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();
}

template <typename B, typename R, typename Q>
void Launcher_BFER_uncoded<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();
}

template <typename B, typename R, typename Q>
void Launcher_BFER_uncoded<B,R,Q>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();
}

template <typename B, typename R, typename Q>
void Launcher_BFER_uncoded<B,R,Q>
::build_simu()
{
#ifdef SYSTEMC
	this->simu = new Simulation_SC_BFER_uncoded<B,R,Q>(this->simu_params, 
	                                                   this->code_params, 
	                                                   this->enco_params, 
	                                                   this->mod_params,
	                                                   this->chan_params,
	                                                   this->deco_params);
#else
	this->simu = new Simulation_uncoded<B,R,Q>(this->simu_params, 
	                                           this->code_params, 
	                                           this->enco_params, 
	                                           this->mod_params,
	                                           this->chan_params,
	                                           this->deco_params);
#endif
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_uncoded<B_8,R_8,Q_8>;
template class Launcher_BFER_uncoded<B_16,R_16,Q_16>;
template class Launcher_BFER_uncoded<B_32,R_32,Q_32>;
template class Launcher_BFER_uncoded<B_64,R_64,Q_64>;
#else
template class Launcher_BFER_uncoded<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
