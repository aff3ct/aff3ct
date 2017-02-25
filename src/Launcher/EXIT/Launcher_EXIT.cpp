#include <string>
#include <iostream>

#include "Tools/Display/bash_tools.h"

#include "Launcher_EXIT.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher_EXIT<B,R,Q>
::Launcher_EXIT(const int argc, const char **argv, std::ostream &stream)
: Launcher<B,R,Q>(argc, argv, stream)
{
	this->params.simulation.type       = "EXIT";
	this->params.simulation.sig_a_min  = 0.0f;
	this->params.simulation.sig_a_max  = 5.0f;
	this->params.simulation.sig_a_step = 0.5f;
	this->params.simulation.snr_type   = "ES";
	this->params.encoder   .type       = "";
	this->params.encoder   .path       = "";
	this->params.encoder   .systematic = true;
}

template <typename B, typename R, typename Q>
void Launcher_EXIT<B,R,Q>
::build_args()
{
	Launcher<B,R,Q>::build_args();

	// ---------------------------------------------------------------------------------------------------- simulation
	this->req_args[{"sim-siga-min", "a"}] =
		{"positive_float",
		 "sigma min value used in EXIT charts."};
	this->req_args[{"sim-siga-max", "A"}] =
		{"positive_float",
		 "sigma max value used in EXIT charts."};
	this->opt_args[{"sim-siga-step"}] =
		{"positive_float",
		 "sigma step value used in EXIT charts."};
	this->opt_args[{"sim-inter-lvl"}].push_back("1");

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-type"}] =
		{"string",
		 "select the type of encoder you want to use.",
		 "AZCW, USER" };
	this->opt_args[{"enc-path"}] =
		{"string",
		 "path to a file containing one or a set of pre-computed codewords, to use with \"--enc-type USER\"."};
}

template <typename B, typename R, typename Q>
void Launcher_EXIT<B,R,Q>
::store_args()
{
	Launcher<B,R,Q>::store_args();

	// ---------------------------------------------------------------------------------------------------- simulation
	if(this->ar.exist_arg({"sim-siga-min", "a"})) this->params.simulation.sig_a_min  = this->ar.get_arg_float({"sim-siga-min", "a"});
	if(this->ar.exist_arg({"sim-siga-max", "A"})) this->params.simulation.sig_a_max  = this->ar.get_arg_float({"sim-siga-max", "A"});
	if(this->ar.exist_arg({"sim-siga-step"    })) this->params.simulation.sig_a_step = this->ar.get_arg_float({"sim-siga-step"    });

	// ------------------------------------------------------------------------------------------------------- encoder
	if(this->ar.exist_arg({"enc-type"})) this->params.encoder.type = this->ar.get_arg({"enc-type"});
	if (this->params.encoder.type == "AZCW")
		this->params.source.type = "AZCW";
	if (this->params.encoder.type == "USER")
		this->params.source.type = "USER";
	if(this->ar.exist_arg({"enc-path"})) this->params.encoder.path = this->ar.get_arg({"enc-path"});
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_EXIT<B,R,Q>
::header_simulation()
{
	auto p = Launcher<B,R,Q>::header_simulation();

	p.push_back(std::make_pair("Sigma-a min (a)", std::to_string(this->params.simulation.sig_a_min )));
	p.push_back(std::make_pair("Sigma-a max (A)", std::to_string(this->params.simulation.sig_a_max )));
	p.push_back(std::make_pair("Sigma-a step",    std::to_string(this->params.simulation.sig_a_step)));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_EXIT<B,R,Q>
::header_encoder()
{
	std::string syst_enc = ((this->params.encoder.systematic) ? "on" : "off");

	auto p = Launcher<B,R,Q>::header_encoder();

	p.push_back(std::make_pair("Type", this->params.encoder.type));

	if (this->params.encoder.type == "USER")
		p.push_back(std::make_pair("Path", this->params.encoder.path));

	p.push_back(std::make_pair("Systematic", syst_enc));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_EXIT<B,R,Q>
::header_decoder()
{
	auto p = Launcher<B,R,Q>::header_decoder();

	p.push_back(std::make_pair("Type (D)",       this->params.decoder.type  ));
	p.push_back(std::make_pair("Implementation", this->params.decoder.implem));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_EXIT<B_32,R_32,Q_32>;
template class Launcher_EXIT<B_64,R_64,Q_64>;
#else
#if defined(PREC_32_BIT) || defined(PREC_64_BIT)
template class Launcher_EXIT<B,R,Q>;
#endif
#endif
// ==================================================================================== explicit template instantiation
