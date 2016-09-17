#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Launcher_BFER_RA.hpp"
#include "../../../Simulation/BFER/RA/Simulation_BFER_RA.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER_RA<B,R,Q>
::Launcher_BFER_RA(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->params.channel.quant_n_bits    = 7;
	this->params.channel.quant_point_pos = 2;

	// default parameters
	this->params.code.type               = "RA";
	this->params.decoder.algo            = "RA";
	this->params.decoder.implem          = "STD";

	this->params.decoder.max_iter        = 10;
	this->params.code.interleaver        = "RANDOM";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	this->opt_args[{"max-iter"}] =
		{"positive_int",
		 "maximal number of iterations in the decoder."};
	this->opt_args[{"interleaver"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, RANDOM, COLUMNS, GOLDEN, NO"};
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	if(this->ar.exist_arg({"max-iter"   })) this->params.decoder.max_iter = this->ar.get_arg_int({"max-iter"});
	if(this->ar.exist_arg({"interleaver"})) this->params.code.interleaver = this->ar.get_arg({"interleaver"});
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::print_header()
{
	Launcher_BFER<B,R,Q>::print_header();

	// display configuration and simulation parameters
	this->stream << "# " << bold("* Decoding iterations per frame ") << " = " << this->params.decoder.max_iter << std::endl;
	this->stream << "# " << bold("* Interleaver                   ") << " = " << this->params.code.interleaver << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::build_simu()
{
	this->simu = new Simulation_BFER_RA<B,R,Q>(this->params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_RA<B_8,R_8,Q_8>;
template class Launcher_BFER_RA<B_16,R_16,Q_16>;
template class Launcher_BFER_RA<B_32,R_32,Q_32>;
template class Launcher_BFER_RA<B_64,R_64,Q_64>;
#else
template class Launcher_BFER_RA<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
