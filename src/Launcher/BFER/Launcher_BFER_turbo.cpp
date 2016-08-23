#include <iostream>

#include "../../Simulation/BFER/Turbo/Simulation_turbo.hpp"
#include "../../Tools/bash_tools.h"

#include "Launcher_BFER_turbo.hpp"

template <typename B, typename R, typename Q, typename QD>
Launcher_BFER_turbo<B,R,Q,QD>
::Launcher_BFER_turbo(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->code_params.tail_length = 4 * 3;
	if (typeid(Q) == typeid(short))
	{
		this->chan_params.quant_n_bits    = 6;
		this->chan_params.quant_point_pos = 3;
	}
	if (typeid(Q) == typeid(signed char))
	{
		this->chan_params.quant_n_bits    = 6;
		this->chan_params.quant_point_pos = 2;
	}

	// default parameters
	this->code_params.type            = "TURBO";
	this->deco_params.algo            = "LTE";
	this->deco_params.implem          = "FAST";
	this->deco_params.map             = "MAX";

	this->deco_params.max_iter        = 6;
	this->enco_params.buffered        = true;
	this->code_params.interleaver     = "LTE";
	this->code_params.crc             = "";
	this->deco_params.scaling_factor  = "LTE_VEC";
	this->deco_params.simd_strategy   = "";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	this->opt_args["max-iter"       ] = "n_iterations";
	this->doc_args["max-iter"       ] = "maximal number of iterations in the turbo decoder.";
	this->opt_args["disable-buf-enc"] = "";
	this->doc_args["disable-buf-enc"] = "disable the buffered encoding.";
	this->opt_args["interleaver"    ] = "interleaver";
	this->doc_args["interleaver"    ] = "specify the type of the interleaver (ex: LTE, RANDOM, COLUMNS, GOLDEN, NO).";
	this->opt_args["crc-type"       ] = "crc_type";
	this->doc_args["crc-type"       ] = "select the crc you want to use (ex: CRC-LTEA-24).";
	this->opt_args["scaling-factor" ] = "scaling-factor";
	this->doc_args["scaling-factor" ] = "scaling factor type (ex: NO, LTE, LTE_VEC, ARRAY, etc.)";

	this->opt_args["dec-simd-strat" ] = "simd_type";
	this->doc_args["dec-simd-strat" ] = "the SIMD strategy you want to use (ex: INTRA, INTER).";
	this->opt_args["dec-map"        ] = "map_type";
	this->doc_args["dec-map"        ] = "the MAP implementation for the nodes (ex: MAX, MAXS, MAXL).";
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	if(this->ar.exist_arg("max-iter"       )) this->deco_params.max_iter       = std::stoi(this->ar.get_arg("max-iter"));
	if(this->ar.exist_arg("disable-buf-enc")) this->enco_params.buffered       = false;
	if(this->ar.exist_arg("interleaver"    )) this->code_params.interleaver    = this->ar.get_arg("interleaver");
	if(this->ar.exist_arg("crc-type"       )) this->code_params.crc            = this->ar.get_arg("crc-type");
	if(this->ar.exist_arg("scaling-factor" )) this->deco_params.scaling_factor = this->ar.get_arg("scaling-factor");

	if(this->ar.exist_arg("dec-simd-strat" )) this->deco_params.simd_strategy  = this->ar.get_arg("dec-simd-strat");
	if(this->ar.exist_arg("dec-map"        )) this->deco_params.map            = this->ar.get_arg("dec-map"       );

	if (this->deco_params.algo == "BCJR4" || this->deco_params.algo == "CCSDS")
		this->code_params.tail_length = 4*4;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();

	std::string buff_enc = (this->enco_params.buffered) ? "on" : "off";

	// display configuration and simulation parameters
	this->stream << "# " << bold("* Decoding iterations per frame ") << " = " << this->deco_params.max_iter       << std::endl;
	this->stream << "# " << bold("* Buffered encoding             ") << " = " << buff_enc                         << std::endl;
	this->stream << "# " << bold("* Interleaver                   ") << " = " << this->code_params.interleaver    << std::endl;
	if (!this->code_params.crc.empty())
	this->stream << "# " << bold("* CRC type                      ") << " = " << this->code_params.crc            << std::endl;
	this->stream << "# " << bold("* Scaling factor                ") << " = " << this->deco_params.scaling_factor << std::endl;
	if (!this->deco_params.simd_strategy.empty())
	this->stream << "# " << bold("* Decoder SIMD strategy         ") << " = " << this->deco_params.simd_strategy  << std::endl;
	this->stream << "# " << bold("* Decoder MAP implementation    ") << " = " << this->deco_params.map            << std::endl;
}

template <typename B, typename R, typename Q, typename QD>
void Launcher_BFER_turbo<B,R,Q,QD>
::build_simu()
{
	this->simu = new Simulation_turbo<B,R,Q,QD>(this->simu_params, 
	                                            this->code_params, 
	                                            this->enco_params,
	                                            this->mod_params,
	                                            this->chan_params, 
	                                            this->deco_params);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_turbo<B_8,R_8,Q_8,QD_8>;
template class Launcher_BFER_turbo<B_16,R_16,Q_16,QD_16>;
template class Launcher_BFER_turbo<B_32,R_32,Q_32,QD_32>;
template class Launcher_BFER_turbo<B_64,R_64,Q_64,QD_64>;
#else
template class Launcher_BFER_turbo<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
