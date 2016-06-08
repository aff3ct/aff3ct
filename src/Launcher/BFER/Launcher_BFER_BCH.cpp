#include <iostream>

#include "../../Simulation/BFER/BCH/Simulation_BCH.hpp"
#include "../../Simulation/BFER/BCH/Simulation_BCH_debug.hpp"

#include "../../Tools/bash_tools.h"

#include "Launcher_BFER_BCH.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER_BCH<B,R,Q>
::Launcher_BFER_BCH(const int argc, const char **argv)
: Launcher_BFER<B,R,Q>(argc, argv)
{
	// override parameters
	this->chan_params.quant_n_bits    = 7;
	this->chan_params.quant_point_pos = 2;

	// default parameters
	this->code_params.type            = "BCH";
	this->deco_params.algo            = "ALGEBRAIC";
	this->deco_params.implem          = "STD";

}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	this->req_args["t"  ] = "value";
	this->doc_args["t"  ] = "correction power of the BCH code.";
	this->req_args["mGF"] = "value";
	this->doc_args["mGF"] = "order of the Galois Field.";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	this->code_params.t   = std::stoi(this->ar.get_arg("t"));
	this->code_params.mGF = std::stoi(this->ar.get_arg("mGF"));
}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();

	// display configuration and simulation parameters
	std::clog << "# " << bold("* Correction power           (t)") << " = " << this->code_params.t   << std::endl;
	std::clog << "# " << bold("* Order of the GField      (mGF)") << " = " << this->code_params.mGF << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::build_simu()
{
	if (this->simu_params.enable_debug)
		this->simu = new Simulation_BCH_debug<B,R,Q>(this->simu_params,
		                                             this->code_params,
		                                             this->enco_params,
		                                             this->chan_params,
		                                             this->deco_params);
	else
		this->simu = new Simulation_BCH<B,R,Q>(this->simu_params,
		                                       this->code_params,
		                                       this->enco_params,
		                                       this->chan_params,
		                                       this->deco_params);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_BCH<B_8,R_8,Q_8>;
template class Launcher_BFER_BCH<B_16,R_16,Q_16>;
template class Launcher_BFER_BCH<B_32,R_32,Q_32>;
template class Launcher_BFER_BCH<B_64,R_64,Q_64>;
#else
template class Launcher_BFER_BCH<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation