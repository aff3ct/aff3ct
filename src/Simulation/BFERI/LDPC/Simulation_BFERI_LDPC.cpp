#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "../../../Encoder/LDPC/Encoder_LDPC_fake.hpp"
#include "../../../Decoder/LDPC/BP/Decoder_LDPC_BP_naive.hpp"

#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "Simulation_BFERI_LDPC.hpp"

template <typename B, typename R, typename Q>
Simulation_BFERI_LDPC<B,R,Q>
::Simulation_BFERI_LDPC(const t_simulation_param& simu_params,
                        const t_code_param&       code_params,
                        const t_encoder_param&    enco_params,
                        const t_mod_param&        mod_params,
                        const t_channel_param&    chan_params,
                        const t_decoder_param&    deco_params)
: Simulation_BFERI<B,R,Q>(simu_params, code_params, enco_params, mod_params, chan_params, deco_params)
{
	assert(code_params.N == code_params.K);
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
	return new Encoder_LDPC_fake<B>();
}

template <typename B, typename R, typename Q>
SISO<Q>* Simulation_BFERI_LDPC<B,R,Q>
::build_siso(const int tid)
{
	return new Decoder_LDPC_BP_naive<B,Q>(this->code_params.K, this->code_params.N);
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFERI_LDPC<B,R,Q>
::build_decoder(const int tid)
{
	return new Decoder_LDPC_BP_naive<B,Q>(this->code_params.K, this->code_params.N);
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFERI_LDPC<B_8,R_8,Q_8>;
template class Simulation_BFERI_LDPC<B_16,R_16,Q_16>;
template class Simulation_BFERI_LDPC<B_32,R_32,Q_32>;
template class Simulation_BFERI_LDPC<B_64,R_64,Q_64>;
#else
template class Simulation_BFERI_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
