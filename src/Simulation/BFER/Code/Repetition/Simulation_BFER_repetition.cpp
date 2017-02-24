#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/Repetition/Factory_encoder_repetition.hpp"
#include "Tools/Factory/Repetition/Factory_decoder_repetition.hpp"

#include "Simulation_BFER_repetition.hpp"
using namespace aff3ct;

template <typename B, typename R, typename Q>
Simulation_BFER_repetition<B,R,Q>
::Simulation_BFER_repetition(const parameters& params)
: Simulation_BFER<B,R,Q>(params)
{
	assert(params.code.N % params.code.K == 0);
}

template <typename B, typename R, typename Q>
Simulation_BFER_repetition<B,R,Q>
::~Simulation_BFER_repetition()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_repetition<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_repetition<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFER_repetition<B,R,Q>
::build_encoder(const int tid)
{
	auto encoder = Simulation_BFER<B,R,Q>::build_encoder(tid);
	if (encoder == nullptr)
		encoder = Factory_encoder_repetition<B>::build(this->params);
	return encoder;
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFER_repetition<B,R,Q>
::build_decoder(const int tid)
{
	return Factory_decoder_repetition<B,Q>::build(this->params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFER_repetition<B_8,R_8,Q_8>;
template class Simulation_BFER_repetition<B_16,R_16,Q_16>;
template class Simulation_BFER_repetition<B_32,R_32,Q_32>;
template class Simulation_BFER_repetition<B_64,R_64,Q_64>;
#else
template class Simulation_BFER_repetition<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
