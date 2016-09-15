#include <map>
#include <chrono>
#include <vector>
#include <string>
#include <cassert>
#include <utility>
#include <cstdlib>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"

#include "Simulation_BFERI_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Simulation_BFERI_RSC<B,R,Q,QD>
::Simulation_BFERI_RSC(const parameters& params)
: Simulation_BFERI<B,R,Q>(params)
{
	assert(params.code.N / params.code.K == 2);
}

template <typename B, typename R, typename Q, typename QD>
Simulation_BFERI_RSC<B,R,Q,QD>
::~Simulation_BFERI_RSC()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_BFERI_RSC<B,R,Q,QD>
::launch_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_BFERI_RSC<B,R,Q,QD>
::snr_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
Encoder<B>* Simulation_BFERI_RSC<B,R,Q,QD>
::build_encoder(const int tid)
{
	auto encoder = Factory_encoder_RSC<B>::build(this->params);
	if (tid == 0)
		trellis = encoder->get_trellis();
	return encoder;
}

template <typename B, typename R, typename Q, typename QD>
SISO<Q>* Simulation_BFERI_RSC<B,R,Q, QD>
::build_siso(const int tid)
{
	this->barrier(tid);
	return Factory_decoder_RSC<B,Q,QD>::build_siso(this->params, trellis);
}

template <typename B, typename R, typename Q, typename QD>
Decoder<B,Q>* Simulation_BFERI_RSC<B,R,Q,QD>
::build_decoder(const int tid)
{
	this->barrier(tid);
	return Factory_decoder_RSC<B,Q,QD>::build(this->params, trellis);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFERI_RSC<B_8,R_8,Q_8,QD_8>;
template class Simulation_BFERI_RSC<B_16,R_16,Q_16,QD_16>;
template class Simulation_BFERI_RSC<B_32,R_32,Q_32,QD_32>;
template class Simulation_BFERI_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_BFERI_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
