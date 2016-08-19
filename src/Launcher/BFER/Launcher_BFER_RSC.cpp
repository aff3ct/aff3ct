#include <iostream>

#ifdef SYSTEMC
#include "../../Simulation/SC_BFER/RSC/Simulation_SC_BFER_RSC.hpp"
#else
#include "../../Simulation/BFER/RSC/Simulation_RSC.hpp"
#endif
#include "../../Tools/bash_tools.h"

#include "Launcher_BFER_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_BFER_RSC<B,R,Q,QD>
::Launcher_BFER_RSC(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->code_params.tail_length     = 2*3;
	this->chan_params.quant_n_bits    = 6;
	this->chan_params.quant_point_pos = 3;

	// default parameters
	this->code_params.type            = "RSC";
	this->deco_params.algo            = "BCJR";
	this->deco_params.implem          = "FAST";
	this->deco_params.map             = "MAX";

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
	this->doc_args["dec-simd-strat" ] = "the SIMD strategy you want to use (ex: INTRA, INTER).";
	this->opt_args["dec-map"        ] = "map_type";
	this->doc_args["dec-map"        ] = "the MAP implementation for the nodes (ex: MAX, MAXS, MAXL).";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_RSC<B,R,Q,QD>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	if(this->ar.exist_arg("disable-buf-enc")) this->enco_params.buffered = false;

	if(this->ar.exist_arg("dec-simd-strat" )) this->deco_params.simd_strategy = this->ar.get_arg("dec-simd-strat");
	if(this->ar.exist_arg("dec-map"        )) this->deco_params.map           = this->ar.get_arg("dec-map"       );

	if (this->deco_params.algo == "BCJR4" || this->deco_params.algo == "CCSDS")
		this->code_params.tail_length = 2*4;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_RSC<B,R,Q,QD>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();

	std::string buff_enc = ((this->enco_params.buffered) ? "on" : "off");

	// display configuration and simulation parameters
	this->stream << "# " << bold("* Buffered encoding             ") << " = " << buff_enc                        << std::endl;
	if (!this->deco_params.simd_strategy.empty())
	this->stream << "# " << bold("* Decoder SIMD strategy         ") << " = " << this->deco_params.simd_strategy << std::endl;
	this->stream << "# " << bold("* Decoder MAP implementation    ") << " = " << this->deco_params.map           << std::endl;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_RSC<B,R,Q,QD>
::build_simu()
{
#ifdef SYSTEMC
	this->simu = new Simulation_SC_BFER_RSC<B,R,Q,QD>(this->simu_params, 
	                                                  this->code_params, 
	                                                  this->enco_params, 
	                                                  this->mod_params,
	                                                  this->chan_params, 
	                                                  this->deco_params);
#else
	this->simu = new Simulation_RSC<B,R,Q,QD>(this->simu_params, 
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
template class Launcher_BFER_RSC<B_8,R_8,Q_8,QD_8>;
template class Launcher_BFER_RSC<B_16,R_16,Q_16,QD_16>;
template class Launcher_BFER_RSC<B_32,R_32,Q_32,QD_32>;
template class Launcher_BFER_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Launcher_BFER_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
