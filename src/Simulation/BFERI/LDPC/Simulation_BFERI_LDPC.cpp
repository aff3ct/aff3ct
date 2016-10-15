#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/LDPC/Factory_decoder_LDPC.hpp"
#include "Tools/Code/LDPC/hard_coded_matrices.h"

#include "Simulation_BFERI_LDPC.hpp"

template <typename B, typename R, typename Q>
Simulation_BFERI_LDPC<B,R,Q>
::Simulation_BFERI_LDPC(const parameters& params)
: Simulation_BFERI<B,R,Q>(params),
  decoder_siso(params.simulation.n_threads, nullptr)
{
	assert(this->params.code.N == (int)n_parities_per_variable_4224.size());
	assert(this->params.code.K == this->params.code.N - (int)n_variables_per_parity_2112.size());

	n_variables_per_parity  = n_variables_per_parity_2112;
	n_parities_per_variable = n_parities_per_variable_4224;
	transpose               = transpose_7392;
}

template <typename B, typename R, typename Q>
Simulation_BFERI_LDPC<B,R,Q>
::~Simulation_BFERI_LDPC()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_LDPC<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_LDPC<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFERI_LDPC<B,R,Q>
::build_encoder(const int tid)
{
	return Simulation_BFERI<B,R,Q>::build_encoder(tid);
}

template <typename B, typename R, typename Q>
SISO<Q>* Simulation_BFERI_LDPC<B,R,Q>
::build_siso(const int tid)
{
	decoder_siso[tid] = Factory_decoder_LDPC<B,Q>::build(this->params, n_variables_per_parity, n_parities_per_variable,
	                                                     transpose);
	return decoder_siso[tid];
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFERI_LDPC<B,R,Q>
::build_decoder(const int tid)
{
	return decoder_siso[tid];
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFERI_LDPC<B_8,R_8,Q_8>;
template class Simulation_BFERI_LDPC<B_16,R_16,Q_16>;
template class Simulation_BFERI_LDPC<B_32,R_32,Q_32>;
template class Simulation_BFERI_LDPC<B_64,R_64,Q_64>;
#else
template class Simulation_BFERI_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
