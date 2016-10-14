#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Launcher_BFER_RA.hpp"
#include "../../../Simulation/BFER/RA/Simulation_BFER_RA.hpp"

template <typename B, typename R, typename Q>
Launcher_BFER_RA<B,R,Q>
::Launcher_BFER_RA(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	this->params.code       .type       = "RA";
	this->params.interleaver.type       = "RANDOM";
	this->params.interleaver.path       = "";
	this->params.quantizer  .n_bits     = 7;
	this->params.quantizer  .n_decimals = 2;
	this->params.decoder    .type       = "RA";
	this->params.decoder    .implem     = "STD";
	this->params.decoder    .n_ite      = 10;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	// --------------------------------------------------------------------------------------------------- interleaver
	this->opt_args[{"itl-type"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, RANDOM, COLUMNS, GOLDEN, USER, NO"};

	this->opt_args[{"itl-path"}] =
		{"string",
		 "specify the path to the interleaver file (to use with \"--itl-type USER\"."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the decoder."};
}

template <typename B, typename R, typename Q>
void Launcher_BFER_RA<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	// --------------------------------------------------------------------------------------------------- interleaver
	if(this->ar.exist_arg({"itl-type"})) this->params.interleaver.type = this->ar.get_arg({"itl-type"});
	if(this->ar.exist_arg({"itl-path"})) this->params.interleaver.path = this->ar.get_arg({"itl-path"});

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("RA" );
	this->opt_args[{"dec-implem"   }].push_back("STD");
	if(this->ar.exist_arg({"dec-ite", "i"})) this->params.decoder.n_ite = this->ar.get_arg_int({"dec-ite", "i"});
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFER_RA<B,R,Q>
::build_simu()
{
	return new Simulation_BFER_RA<B,R,Q>(this->params);
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_RA<B,R,Q>
::header_interleaver()
{
	auto p = Launcher_BFER<B,R,Q>::header_interleaver();

	p.push_back(std::make_pair("Type", this->params.interleaver.type));

	if (this->params.interleaver.type == "USER")
		p.push_back(std::make_pair("Path", this->params.interleaver.path));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_RA<B,R,Q>
::header_decoder()
{
	auto p = Launcher_BFER<B,R,Q>::header_decoder();

	p.push_back(std::make_pair("Num. of iterations (i)", std::to_string(this->params.decoder.n_ite)));

	return p;
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
