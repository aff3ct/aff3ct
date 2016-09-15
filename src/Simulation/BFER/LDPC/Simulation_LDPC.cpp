#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/LDPC/Factory_encoder_LDPC.hpp"
#include "Tools/Factory/LDPC/Factory_decoder_LDPC.hpp"

#include "Simulation_LDPC.hpp"

template <typename B, typename R, typename Q>
Simulation_LDPC<B,R,Q>
::Simulation_LDPC(const parameters& params)
: Simulation_BFER<B,R,Q>(params)
{
}

template <typename B, typename R, typename Q>
Simulation_LDPC<B,R,Q>
::~Simulation_LDPC()
{
}

template <typename B, typename R, typename Q>
void Simulation_LDPC<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_LDPC<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_LDPC<B,R,Q>
::build_encoder(const int tid)
{
	auto encoder = Factory_encoder_LDPC<B>::build(this->params, tid);
	if (tid == 0)
	{
		n_variables_per_parity  = encoder->get_n_variables_per_parity();
		n_parities_per_variable = encoder->get_n_parities_per_variable();
		transpose               = encoder->get_transpose();
	}

	return encoder;
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_LDPC<B,R,Q>
::build_decoder(const int tid)
{
	this->barrier(tid);
	return Factory_decoder_LDPC<B,Q>::build(this->params, n_variables_per_parity, n_parities_per_variable, transpose,
	                                        this->X_N1[tid]);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_LDPC<B_8,R_8,Q_8>;
template class Simulation_LDPC<B_16,R_16,Q_16>;
template class Simulation_LDPC<B_32,R_32,Q_32>;
template class Simulation_LDPC<B_64,R_64,Q_64>;
#else
template class Simulation_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
