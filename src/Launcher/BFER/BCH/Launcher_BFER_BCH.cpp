#include <iostream>

#include "Tools/Display/bash_tools.h"

#if defined(SYSTEMC)
#include "Simulation/BFER/Standard/SystemC/SC_Simulation_BFER_std.hpp"
#elif defined(STARPU)
#include "Simulation/BFER/Standard/StarPU/SPU_Simulation_BFER_std.hpp"
#else
#include "Simulation/BFER/Standard/Threads/Simulation_BFER_std_threads.hpp"
#endif
#include "Tools/Codec/BCH/Codec_BCH.hpp"

#include "Launcher_BFER_BCH.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher_BFER_BCH<B,R,Q>
::Launcher_BFER_BCH(const int argc, const char **argv, std::ostream &stream)
: Launcher_BFER<B,R,Q>(argc, argv, stream)
{
	this->params.code     .type       = "BCH";
	this->params.encoder  .type       = "BCH";
	this->params.quantizer.n_bits     = 7;
	this->params.quantizer.n_decimals = 2;
	this->params.decoder  .type       = "ALGEBRAIC";
	this->params.decoder  .implem     = "STD";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::build_args()
{
	Launcher_BFER<B,R,Q>::build_args();

	// ------------------------------------------------------------------------------------------------------- encoder
	this->opt_args[{"enc-type"}][2] += ", BCH";

	// ---------------------------------------------------------------------------------------------------------- code
	this->opt_args[{"cde-corr-pow", "T"}] =
		{"positive_int",
		 "correction power of the BCH code."};
}

template <typename B, typename R, typename Q>
void Launcher_BFER_BCH<B,R,Q>
::store_args()
{
	Launcher_BFER<B,R,Q>::store_args();

	// ---------------------------------------------------------------------------------------------------------- code
	if (this->ar.exist_arg({"cde-corr-pow", "T"}))
		this->params.code.t = this->ar.get_arg_int({"cde-corr-pow", "T"});
	else
		this->params.code.t = (this->params.code.N - this->params.code.K) / this->params.code.m;
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFER_BCH<B,R,Q>
::build_simu()
{
	this->codec = new Codec_BCH<B,Q>(this->params);
#if defined(SYSTEMC)
	return new SC_Simulation_BFER_std     <B,R,Q>(this->params, *this->codec);
#elif defined(STARPU)
	return new SPU_Simulation_BFER_std    <B,R,Q>(this->params, *this->codec);
#else
	return new Simulation_BFER_std_threads<B,R,Q>(this->params, *this->codec);
#endif
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher_BFER_BCH<B,R,Q>
::header_code()
{
	auto p = Launcher_BFER<B,R,Q>::header_code();

	p.push_back(std::make_pair("Galois field order (m)", std::to_string(this->params.code.m)));
	p.push_back(std::make_pair("Correction power (T)",   std::to_string(this->params.code.t)));

	return p;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_BFER_BCH<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_BFER_BCH<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_BFER_BCH<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_BFER_BCH<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_BFER_BCH<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
