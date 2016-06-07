#include <iostream>

#include "../../Simulation/BFER/RSC/Simulation_RSC.hpp"
#include "../../Simulation/BFER/RSC/Simulation_RSC_mt.hpp"
#include "../../Simulation/BFER/RSC/Simulation_RSC_bench.hpp"
#include "../../Simulation/BFER/RSC/Simulation_RSC_debug.hpp"

#include "../../Tools/bash_tools.h"

#include "Launcher_BFER_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_BFER_RSC<B,R,Q,QD>
::Launcher_BFER_RSC(const int argc, const char **argv)
: Launcher_BFER<B,R,Q>(argc, argv)
{
	// override parameters
	this->code_params.tail_length     = 2*3;
	this->chan_params.quant_n_bits    = 6;
	this->chan_params.quant_point_pos = 3;

	// default parameters
	this->code_params.type            = "RSC";
	this->deco_params.algo            = "BCJR";
	this->deco_params.implem          = "FAST";

	this->enco_params.buffered        = true;
	this->deco_params.simd_strategy   = "";

}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_RSC<B,R,Q,QD>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	this->opt_args["disable-buf-enc"] = "";
	this->doc_args["disable-buf-enc"] = "disable the buffered encoding.";

	this->opt_args["dec-simd-strat" ] = "simd_type";
	this->doc_args["dec-simd-strat" ] = "the SIMD strategy you want to use (ex: INTRA, INTER, NONE).";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_RSC<B,R,Q,QD>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	if(this->ar.exist_arg("disable-buf-enc")) this->enco_params.buffered = false;

	if(this->ar.exist_arg("dec-simd-strat" )) this->deco_params.simd_strategy   = this->ar.get_arg("dec-simd-strat");
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_RSC<B,R,Q,QD>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();

	std::string buff_enc = ((this->enco_params.buffered) ? "on" : "off");

	// display configuration and simulation parameters
	std::clog << "# " << bold("* Buffered encoding             ") << " = " << buff_enc                        << std::endl;
	if (!this->deco_params.simd_strategy.empty())
	std::clog << "# " << bold("* Decoder SIMD strategy         ") << " = " << this->deco_params.simd_strategy << std::endl;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_RSC<B,R,Q,QD>
::build_simu()
{
	if (this->simu_params.enable_debug)
		this->simu = new Simulation_RSC_debug<B,R,Q,QD>(this->simu_params, 
		                                                this->code_params, 
		                                                this->enco_params, 
		                                                this->chan_params, 
		                                                this->deco_params);
	else if (this->simu_params.benchs)
		this->simu = new Simulation_RSC_bench<B,R,Q,QD>(this->simu_params, 
		                                                this->code_params, 
		                                                this->enco_params, 
		                                                this->chan_params, 
		                                                this->deco_params,
		                                                this->simu_params.n_threads);
	else if (this->simu_params.n_threads)
		this->simu = new Simulation_RSC_mt<B,R,Q,QD>(this->simu_params, 
		                                             this->code_params, 
		                                             this->enco_params, 
		                                             this->chan_params, 
		                                             this->deco_params, 
		                                             this->simu_params.n_threads);
	else
		this->simu = new Simulation_RSC<B,R,Q,QD>(this->simu_params, 
		                                          this->code_params, 
		                                          this->enco_params, 
		                                          this->chan_params, 
		                                          this->deco_params);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_RSC<B_8,R_8,Q_8,QD_8>;
template class Launcher_BFER_RSC<B_16,R_16,Q_16,QD_16>;
template class Launcher_BFER_RSC<B_32,R_32,Q_32,QD_32>;
template class Launcher_BFER_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Launcher_BFER_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation