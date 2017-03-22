#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Simulation/BFER/Code/Repetition/Simulation_BFER_repetition.hpp"

#include "Launcher_BFER_repetition.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher_BFER_repetition<B,R,Q>
::Launcher_BFER_repetition(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	this->params.code     .type       = "REPETITION";
	this->params.encoder  .type       = "REPETITION";
	this->params.encoder  .buffered   = true;
	this->params.quantizer.n_bits     = 6;
	this->params.quantizer.n_decimals = 2;
	this->params.decoder  .type       = "REPETITION";
	this->params.decoder  .implem     = "STD";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_repetition<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-type"}][2] += ", REPETITON";
	this->opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	// ------------------------------------------------------------------------------------------------------- decoder
	this->opt_args[{"dec-type", "D"}].push_back("REPETITION");
	this->opt_args[{"dec-implem"   }].push_back("STD, FAST");
}

template <typename B, typename R, typename Q>
void Launcher_BFER_repetition<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	// ------------------------------------------------------------------------------------------------------- encoder
	if(this->ar.exist_arg({"enc-no-buff"})) this->params.encoder.buffered = false;
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFER_repetition<B,R,Q>
::build_simu()
{
	return new Simulation_BFER_repetition<B,R,Q>(this->params);
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_repetition<B,R,Q>
::header_encoder()
{
	std::string buff_enc = ((this->params.encoder.buffered) ? "on" : "off");

	auto p = Launcher_BFER<B,R,Q>::header_encoder();

	p.push_back(std::make_pair("Buffered", buff_enc));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_BFER_repetition<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_BFER_repetition<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_BFER_repetition<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_BFER_repetition<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_BFER_repetition<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
