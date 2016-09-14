#include <iostream>

#include "../../Simulation/BFERI/LDPC/Simulation_BFERI_LDPC.hpp"
#include "../../Tools/bash_tools.h"

#include "Launcher_BFERI_LDPC.hpp"

template <typename B, typename R, typename Q>
Launcher_BFERI_LDPC<B,R,Q>
::Launcher_BFERI_LDPC(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFERI<B,R,Q>(argc, argv, stream)
{
	// override parameters
	this->chan_params.quant_n_bits    = 6;
	this->chan_params.quant_point_pos = 2;

	// default parameters
	this->deco_params.max_iter        = 1;
	this->deco_params.algo            = "BP_FLOODING";
	this->deco_params.implem          = "MIN_SUM";
	this->code_params.interleaver     = "RANDOM_HARD";
	this->code_params.coset           = false;
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::build_args()
{
	Launcher_BFERI<B,R,Q>::build_args();

	this->opt_args["max-iter"] = "n_iterations";
	this->doc_args["max-iter"] = "maximal number of iterations in the turbo decoder.";

	this->opt_args["enable-coset"] = "";
	this->doc_args["enable-coset"] = "enable the coset approach.";
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::store_args()
{
	Launcher_BFERI<B,R,Q>::store_args();

	if(this->ar.exist_arg("max-iter"    )) this->deco_params.max_iter = std::stoi(this->ar.get_arg("max-iter"));
	if(this->ar.exist_arg("enable-coset")) this->code_params.coset    = true;
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::print_header()
{
	Launcher_BFERI<B,R,Q>::print_header();

	std::string coset = this->code_params.coset ? "on" : "off";

	this->stream << "# " << bold("* Coset approach                ") << " = " << coset                      << std::endl;
	this->stream << "# " << bold("* Decoding iterations per frame ") << " = " << this->deco_params.max_iter << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher_BFERI_LDPC<B,R,Q>
::build_simu()
{
	this->simu = new Simulation_BFERI_LDPC<B,R,Q>(this->simu_params, 
	                                              this->code_params, 
	                                              this->enco_params, 
	                                              this->mod_params,
	                                              this->chan_params,
	                                              this->deco_params);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFERI_LDPC<B_8,R_8,Q_8>;
template class Launcher_BFERI_LDPC<B_16,R_16,Q_16>;
template class Launcher_BFERI_LDPC<B_32,R_32,Q_32>;
template class Launcher_BFERI_LDPC<B_64,R_64,Q_64>;
#else
template class Launcher_BFERI_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
