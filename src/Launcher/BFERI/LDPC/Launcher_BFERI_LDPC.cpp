#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/BFERI/LDPC/Simulation_BFERI_LDPC.hpp"

#include "Launcher_BFERI_LDPC.hpp"

template <typename B, typename R, typename Q>
Launcher_BFERI_LDPC<B,R,Q>
::Launcher_BFERI_LDPC(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFERI<B,R,Q>(argc, argv, stream)
{
	this->params.code       .type       = "LDPC";
	this->params.code       .coset      = false;
	this->params.interleaver.type       = "RANDOM_HARD";
	this->params.quantizer  .n_bits     = 6;
	this->params.quantizer  .n_decimals = 2;
	this->params.decoder    .n_ite      = 1;
	this->params.decoder    .type       = "BP_FLOODING";
	this->params.decoder    .implem     = "MIN_SUM";
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::build_args()
{
	Launcher_BFERI<B,R,Q>::build_args();

	// ---------------------------------------------------------------------------------------------------------- code
	this->opt_args[{"cde-coset", "c"}] =
		{"",
		 "enable the coset approach."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-ite", "i"}] =
			{"positive_int",
			 "maximal number of iterations in the turbo decoder."};
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::store_args()
{
	Launcher_BFERI<B,R,Q>::store_args();

	// ---------------------------------------------------------------------------------------------------------- code
	if(this->ar.exist_arg({"cde-coset", "c"})) this->params.code.coset = true;

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-ite", "i"})) this->params.decoder.n_ite = this->ar.get_arg_int({"dec-ite", "i"});

}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::print_header()
{
	Launcher_BFERI<B,R,Q>::print_header();

	std::string coset = this->params.code.coset ? "on" : "off";

	this->stream << "# " << bold("* Coset approach                ") << " = " << coset                      << std::endl;
	this->stream << "# " << bold("* Decoding iterations per frame ") << " = " << this->params.decoder.n_ite << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::build_simu()
{
	this->simu = new Simulation_BFERI_LDPC<B,R,Q>(this->params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFERI_LDPC<B_8,R_8,Q_8>;
template class Launcher_BFERI_LDPC<B_16,R_16,Q_16>;
template class Launcher_BFERI_LDPC<B_32,R_32,Q_32>;
template class Launcher_BFERI_LDPC<B_64,R_64,Q_64>;
#else
template class Launcher_BFERI_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
