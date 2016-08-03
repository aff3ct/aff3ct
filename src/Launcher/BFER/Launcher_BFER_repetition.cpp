#include <iostream>

#include "../../Simulation/BFER/Repetition/Simulation_repetition.hpp"
#include "../../Tools/bash_tools.h"

#include "Launcher_BFER_repetition.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER_repetition<B,R,Q>
::Launcher_BFER_repetition(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->chan_params.quant_n_bits    = 6;
	this->chan_params.quant_point_pos = 2;

	// default parameters
	this->code_params.type            = "Repetition";
	this->deco_params.algo            = "REPETITION";
	this->deco_params.implem          = "STD";

	this->enco_params.buffered        = true;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_repetition<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	this->opt_args["disable-buf-enc"] = "";
	this->doc_args["disable-buf-enc"] = "disable the buffered encoding.";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_repetition<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	if(this->ar.exist_arg("disable-buf-enc")) this->enco_params.buffered = false;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_repetition<B,R,Q>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();

	std::string buff_enc = ((this->enco_params.buffered) ? "on" : "off");

	// display configuration and simulation parameters
	this->stream << "# " << bold("* Buffered encoding             ") << " = " << buff_enc << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_repetition<B,R,Q>
::build_simu()
{
	this->simu = new Simulation_repetition<B,R,Q>(this->simu_params, 
	                                              this->code_params, 
	                                              this->enco_params, 
	                                              this->mod_params,
	                                              this->chan_params,
	                                              this->deco_params);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_repetition<B_8,R_8,Q_8>;
template class Launcher_BFER_repetition<B_16,R_16,Q_16>;
template class Launcher_BFER_repetition<B_32,R_32,Q_32>;
template class Launcher_BFER_repetition<B_64,R_64,Q_64>;
#else
template class Launcher_BFER_repetition<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
