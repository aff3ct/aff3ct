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

#include "Simulation_BFER_RSC.hpp"
using namespace aff3ct;

template <typename B, typename R, typename Q, typename QD>
Simulation_BFER_RSC<B,R,Q,QD>
::Simulation_BFER_RSC(const parameters& params)
:  Simulation_BFER<B,R,Q>(params)
{
	assert(params.code.N / params.code.K == 2);
}

template <typename B, typename R, typename Q, typename QD>
Simulation_BFER_RSC<B,R,Q,QD>
::~Simulation_BFER_RSC()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_BFER_RSC<B,R,Q,QD>
::launch_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_BFER_RSC<B,R,Q,QD>
::snr_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
Encoder<B>* Simulation_BFER_RSC<B,R,Q,QD>
::build_encoder(const int tid)
{
	auto encoder_RSC = Factory_encoder_RSC<B>::build(this->params);
	Simulation::check_errors(encoder_RSC, "Encoder_RSC_sys<B>");
	if (tid == 0)
		trellis = encoder_RSC->get_trellis();

	auto encoder = Simulation_BFER<B,R,Q>::build_encoder(tid);
	if (encoder == nullptr)
		encoder = encoder_RSC;
	else
		delete encoder_RSC;
	return encoder;
}

template <typename B, typename R, typename Q, typename QD>
Decoder<B,Q>* Simulation_BFER_RSC<B,R,Q,QD>
::build_decoder(const int tid)
{
	this->barrier(tid);
	return Factory_decoder_RSC<B,Q,QD>::build(this->params, trellis);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFER_RSC<B_8,R_8,Q_8,QD_8>;
template class Simulation_BFER_RSC<B_16,R_16,Q_16,QD_16>;
template class Simulation_BFER_RSC<B_32,R_32,Q_32,QD_32>;
template class Simulation_BFER_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_BFER_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
