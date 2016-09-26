#include <string>
#include <iostream>

#include "Tools/bash_tools.h"

#include "Launcher_EXIT.hpp"

template <typename B, typename R, typename Q>
Launcher_EXIT<B,R,Q>
::Launcher_EXIT(const int argc, const char **argv, std::ostream &stream)
: Launcher<B,R,Q>(argc, argv, stream)
{
	// default parameters
	this->simu_params.type = "EXIT";

	this->enco_params.systematic = true;
	this->simu_params.sig_a_min  = 0.0f;
	this->simu_params.sig_a_max  = 5.0f;
	this->simu_params.sig_a_step = 0.5f;
}

template <typename B, typename R, typename Q>
void Launcher_EXIT<B,R,Q>
::build_args()
{
	Launcher<B,R,Q>::build_args();

	this->req_args["sig-a-min" ] = "sigma_a_min_value";
	this->doc_args["sig-a-min" ] = "sigma min value used in EXIT charts.";
	this->req_args["sig-a-max" ] = "sigma_a_max_value";
	this->doc_args["sig-a-max" ] = "sigma max value used in EXIT charts.";
	
	this->opt_args["sig-a-step"] = "sigma_a_step_value";
	this->doc_args["sig-a-step"] = "sigma step value used in EXIT charts.";
}

template <typename B, typename R, typename Q>
void Launcher_EXIT<B,R,Q>
::store_args()
{
	Launcher<B,R,Q>::store_args();

	if(this->ar.exist_arg("sig-a-min" )) this->simu_params.sig_a_min  = std::stof(this->ar.get_arg("sig-a-min"));
	if(this->ar.exist_arg("sig-a-max" )) this->simu_params.sig_a_max  = std::stof(this->ar.get_arg("sig-a-max"));
	if(this->ar.exist_arg("sig-a-step")) this->simu_params.sig_a_step = std::stof(this->ar.get_arg("sig-a-step"));
}

template <typename B, typename R, typename Q>
void Launcher_EXIT<B,R,Q>
::print_header()
{
	Launcher<B,R,Q>::print_header();

	std::string syst_enc = ((this->enco_params.systematic) ? "on" : "off");

	this->stream << "# " << bold("* Systematic encoding           ") << " = " << syst_enc                     << std::endl;
	this->stream << "# " << bold("* Decoding algorithm            ") << " = " << this->deco_params.algo       << std::endl;
	this->stream << "# " << bold("* Decoding implementation       ") << " = " << this->deco_params.implem     << std::endl;
	this->stream << "# " << bold("* SIG a min                     ") << " = " << this->simu_params.sig_a_min  << std::endl;
	this->stream << "# " << bold("* SIG a max                     ") << " = " << this->simu_params.sig_a_max  << std::endl;
	this->stream << "# " << bold("* SIG a step                    ") << " = " << this->simu_params.sig_a_step << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_EXIT<B_8,R_8,Q_8>;
template class Launcher_EXIT<B_16,R_16,Q_16>;
template class Launcher_EXIT<B_32,R_32,Q_32>;
template class Launcher_EXIT<B_64,R_64,Q_64>;
#else
template class Launcher_EXIT<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
