#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "Simulation_BFER_BCH.hpp"
#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/BCH/Factory_encoder_BCH.hpp"
#include "Tools/Factory/BCH/Factory_decoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_BCH<B,R,Q>
::Simulation_BFER_BCH(const parameters& params)
: Simulation_BFER<B,R,Q>(params), GF(params.code.mGF, params.code.N, params.code.K, params.code.t)
{
	// assertion are made in the Galois Field (GF)
}

template <typename B, typename R, typename Q>
Simulation_BFER_BCH<B,R,Q>
::~Simulation_BFER_BCH()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_BCH<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_BCH<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFER_BCH<B,R,Q>
::build_encoder(const int tid)
{
	auto encoder = Simulation_BFER<B,R,Q>::build_encoder(tid);
	if (encoder == nullptr)
		encoder = Factory_encoder_BCH<B>::build(this->params, GF);
	return encoder;
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFER_BCH<B,R,Q>
::build_decoder(const int tid)
{
	return Factory_decoder_BCH<B,Q>::build(this->params, GF);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER_BCH<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_BCH<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_BCH<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_BCH<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_BCH<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
