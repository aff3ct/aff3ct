#include <iostream>

#include "../../Simulation/BFER/RA/Simulation_RA.hpp"
#include "../../Tools/bash_tools.h"

#include "Launcher_BFER_RA.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER_RA<B,R,Q>
::Launcher_BFER_RA(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->chan_params.quant_n_bits    = 7;
	this->chan_params.quant_point_pos = 2;

	// default parameters
	this->code_params.type            = "RA";
	this->deco_params.algo            = "RA";
	this->deco_params.implem          = "STD";

	this->deco_params.max_iter        = 10;
	this->code_params.interleaver     = "RANDOM";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	this->opt_args["max-iter"   ] = "n_iterations";
	this->doc_args["max-iter"   ] = "maximal number of iterations in the decoder.";
	this->opt_args["interleaver"] = "interleaver";
	this->doc_args["interleaver"] = "specify the type of the interleaver (ex: LTE, RANDOM, COLUMNS, GOLDEN, NO).";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	if(this->ar.exist_arg("max-iter"   )) this->deco_params.max_iter    = std::stoi(this->ar.get_arg("max-iter"));
	if(this->ar.exist_arg("interleaver")) this->code_params.interleaver = this->ar.get_arg("interleaver");
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();

	// display configuration and simulation parameters
	this->stream << "# " << bold("* Decoding iterations per frame ") << " = " << this->deco_params.max_iter    << std::endl;
	this->stream << "# " << bold("* Interleaver                   ") << " = " << this->code_params.interleaver << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::build_simu()
{
	this->simu = new Simulation_RA<B,R,Q>(this->simu_params,
	                                      this->code_params,
	                                      this->enco_params,
	                                      this->mod_params,
	                                      this->chan_params,
	                                      this->deco_params);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_RA<B_8,R_8,Q_8>;
template class Launcher_BFER_RA<B_16,R_16,Q_16>;
template class Launcher_BFER_RA<B_32,R_32,Q_32>;
template class Launcher_BFER_RA<B_64,R_64,Q_64>;
#else
template class Launcher_BFER_RA<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
