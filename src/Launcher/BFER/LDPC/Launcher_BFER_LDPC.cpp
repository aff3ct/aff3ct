#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Launcher_BFER_LDPC.hpp"
#include "../../../Simulation/BFER/LDPC/Simulation_BFER_LDPC.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER_LDPC<B,R,Q>
::Launcher_BFER_LDPC(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	this->params.code     .type       = "LDPC";
	this->params.code     .coset      = false;
	this->params.quantizer.n_bits     = 6;
	this->params.quantizer.n_decimals = 2;
	this->params.decoder  .type       = "BP_FLOODING";
	this->params.decoder  .implem     = "MIN_SUM";
	this->params.decoder  .n_ite      = 10;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_LDPC<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

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
void Launcher_BFER_LDPC<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();
	// ---------------------------------------------------------------------------------------------------------- code
	if(this->ar.exist_arg({"cde-coset", "c"})) this->params.code.coset = true;

	// ------------------------------------------------------------------------------------------------------- decoder
	if(this->ar.exist_arg({"dec-ite", "i"})) this->params.decoder.n_ite = this->ar.get_arg_int({"dec-ite", "i"});
}

template <typename B, typename R, typename Q>
void Launcher_BFER_LDPC<B,R,Q>
::build_simu()
{
	this->simu = new Simulation_BFER_LDPC<B,R,Q>(this->params);
}

template <typename B, typename R, typename Q>
std::vector<std::vector<std::string>> Launcher_BFER_LDPC<B,R,Q>
::header_code()
{
	std::string coset = this->params.code.coset ? "on" : "off";

	auto p = Launcher_BFER<B,R,Q>::header_code();

	p.push_back({"Coset approach (c)", coset});

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::vector<std::string>> Launcher_BFER_LDPC<B,R,Q>
::header_decoder()
{
	auto p = Launcher_BFER<B,R,Q>::header_decoder();

	p.push_back({"Num. of iterations (i)", std::to_string(this->params.decoder.n_ite)});

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER_LDPC<B_8,R_8,Q_8>;
template class Launcher_BFER_LDPC<B_16,R_16,Q_16>;
template class Launcher_BFER_LDPC<B_32,R_32,Q_32>;
template class Launcher_BFER_LDPC<B_64,R_64,Q_64>;
#else
template class Launcher_BFER_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
