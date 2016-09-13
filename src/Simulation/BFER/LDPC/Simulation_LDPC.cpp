#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "../../../Tools/Factory/LDPC/Factory_encoder_LDPC.hpp"
#include "../../../Tools/Factory/LDPC/Factory_decoder_LDPC.hpp"

#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "Simulation_LDPC.hpp"

template <typename B, typename R, typename Q>
Simulation_LDPC<B,R,Q>
::Simulation_LDPC(const t_simulation_param& simu_params,
                  const t_code_param&       code_params,
                  const t_encoder_param&    enco_params,
                  const t_mod_param&        mod_params,
                  const t_channel_param&    chan_params,
                  const t_decoder_param&    deco_params)
: Simulation_BFER<B,R,Q>(simu_params, code_params, enco_params, mod_params, chan_params, deco_params)
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
	auto encoder = Factory_encoder_LDPC<B>::build(this->simu_params,
	                                              this->code_params,
	                                              this->enco_params,
	                                              tid);
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
	return Factory_decoder_LDPC<B,Q>::build(this->code_params, this->enco_params, this->deco_params,
	                                        n_variables_per_parity, n_parities_per_variable, transpose,
	                                        this->X_N1[tid], this->code_params.coset);
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_LDPC<B_8,R_8,Q_8>;
template class Simulation_LDPC<B_16,R_16,Q_16>;
template class Simulation_LDPC<B_32,R_32,Q_32>;
template class Simulation_LDPC<B_64,R_64,Q_64>;
#else
template class Simulation_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
