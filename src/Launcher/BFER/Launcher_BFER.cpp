#include <string>
#include <iostream>

#include "../../Tools/bash_tools.h"

#include "Launcher_BFER.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER<B,R,Q>
::Launcher_BFER(const int argc, const char **argv)
: Launcher<B,R,Q>(argc, argv)
{
	this->simu_params.type            = "BFER";
	this->simu_params.max_fe          = 100;
	this->simu_params.benchs          = 0;
	this->simu_params.enable_debug    = false;
	this->simu_params.enable_leg_term = false;
	this->simu_params.enable_dec_thr  = false;
	this->enco_params.systematic      = true;
}

template <typename B, typename R, typename Q>
void Launcher_BFER<B,R,Q>
::build_args()
{
	Launcher<B,R,Q>::build_args();

	this->opt_args["max-fe"         ] = "n_fe";
	this->doc_args["max-fe"         ] = "max number of frame errors for each SNR simulation.";
	this->opt_args["benchs"         ] = "n_benchs";
	this->doc_args["benchs"         ] = "enable special benchmark mode with a loop around the decoder.";
	this->opt_args["enable-leg-term"] = "";
	this->doc_args["enable-leg-term"] = "enable the legacy display (needed for retro-compatibility with PyBer).";
	this->opt_args["enable-dec-thr" ] = "";
	this->doc_args["enable-dec-thr" ] = "enable the display of the decoder throughput considering only the decoder time.";
	this->opt_args["enable-debug"   ] = "";
	this->doc_args["enable-debug"   ] = "enable debug mode: print array values after each step.";
}

template <typename B, typename R, typename Q>
void Launcher_BFER<B,R,Q>
::store_args()
{
	Launcher<B,R,Q>::store_args();

	// facultative parameters
	if(this->ar.exist_arg("max-fe"         )) this->simu_params.max_fe          = std::stoi(this->ar.get_arg("max-fe"));
	if(this->ar.exist_arg("benchs"         )) this->simu_params.benchs          = std::stoi(this->ar.get_arg("benchs"));
	if(this->ar.exist_arg("enable-leg-term")) this->simu_params.enable_leg_term = true;
	if(this->ar.exist_arg("enable-dec-thr" )) this->simu_params.enable_dec_thr  = true;
	if(this->ar.exist_arg("enable-debug"   )) this->simu_params.enable_debug    = true;
}

template <typename B, typename R, typename Q>
void Launcher_BFER<B,R,Q>
::print_header()
{
	Launcher<B,R,Q>::print_header();

	std::string syst_enc = ((this->enco_params.systematic) ? "on" : "off");

	std::string threads = "unused";
	if (this->simu_params.n_threads)
		threads = std::to_string(this->simu_params.n_threads) + " thread(s)";

	// display configuration and simulation parameters
	std::clog << "# " << bold("* Max frame error count     (FE)") << " = " << this->simu_params.max_fe << std::endl;
	std::clog << "# " << bold("* Systematic encoding           ") << " = " << syst_enc                 << std::endl;
	std::clog << "# " << bold("* Decoding algorithm            ") << " = " << this->deco_params.algo   << std::endl;
	std::clog << "# " << bold("* Decoding implementation       ") << " = " << this->deco_params.implem << std::endl;
	std::clog << "# " << bold("* Multi-threading               ") << " = " << threads                  << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFER<B_8,R_8,Q_8>;
template class Launcher_BFER<B_16,R_16,Q_16>;
template class Launcher_BFER<B_32,R_32,Q_32>;
template class Launcher_BFER<B_64,R_64,Q_64>;
#else
template class Launcher_BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation