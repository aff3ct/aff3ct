#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"

#include "Simulation_EXIT_RSC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q, typename QD>
Simulation_EXIT_RSC<B,R,Q,QD>
::Simulation_EXIT_RSC(const parameters& params)
: Simulation_EXIT<B,R,Q>(params)
{
}

template <typename B, typename R, typename Q, typename QD>
Simulation_EXIT_RSC<B,R,Q,QD>
::~Simulation_EXIT_RSC()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_EXIT_RSC<B,R,Q,QD>
::extract_sys_par(const mipp::vector<R> &Lch_N, 
                  const mipp::vector<R> &La_K, 
                        mipp::vector<R> &sys, 
                        mipp::vector<R> &par)
{
	// extract systematic and parity information
	for (auto i = 0; i < this->params.code.K + this->params.code.tail_length/2; i++)
	{
		sys[i] = Lch_N[i*2 +0];
		par[i] = Lch_N[i*2 +1];
	}

	// add other siso's extrinsic
	for(auto i = 0 ; i < this->params.code.K ; i ++)
		sys[i] += La_K[i];
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_EXIT_RSC<B,R,Q,QD>
::launch_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_EXIT_RSC<B,R,Q,QD>
::snr_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
Encoder<B>* Simulation_EXIT_RSC<B,R,Q,QD>
::build_encoder()
{
	auto encoder_RSC = Factory_encoder_RSC<B>::build(this->params);
	trellis = encoder_RSC->get_trellis();

	auto encoder = Simulation_EXIT<B,R,Q>::build_encoder();
	if (encoder == nullptr)
		encoder = encoder_RSC;
	else
		delete encoder_RSC;
	return encoder;
}

template <typename B, typename R, typename Q, typename QD>
SISO<R>* Simulation_EXIT_RSC<B,R,Q,QD>
::build_siso()
{
	return Factory_decoder_RSC<B,R,R>::build_siso(this->params, trellis);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_EXIT_RSC<B_32,R_32,Q_32,QD_32>;
template class Simulation_EXIT_RSC<B_64,R_64,Q_64,QD_64>;
#else
#if defined(PREC_32_BIT) || defined(PREC_64_BIT)
template class Simulation_EXIT_RSC<B,R,Q,QD>;
#endif
#endif
// ==================================================================================== explicit template instantiation
