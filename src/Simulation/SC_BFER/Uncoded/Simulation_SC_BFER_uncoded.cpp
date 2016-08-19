#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "../../../Encoder/Encoder_NO.hpp"
#include "../../../Decoder/Decoder_NO.hpp"

#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "Simulation_SC_BFER_uncoded.hpp"

template <typename B, typename R, typename Q>
Simulation_SC_BFER_uncoded<B,R,Q>
::Simulation_SC_BFER_uncoded(const t_simulation_param& simu_params,
                             const t_code_param&       code_params,
                             const t_encoder_param&    enco_params,
                             const t_mod_param&        mod_params,
                             const t_channel_param&    chan_params,
                             const t_decoder_param&    deco_params)
: Simulation_SC_BFER<B,R,Q>(simu_params, code_params, enco_params, mod_params, chan_params, deco_params)
{
	assert(code_params.N == code_params.K);
}

template <typename B, typename R, typename Q>
Simulation_SC_BFER_uncoded<B,R,Q>
::~Simulation_SC_BFER_uncoded()
{
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER_uncoded<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER_uncoded<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_SC_BFER_uncoded<B,R,Q>
::build_encoder()
{
	return new Encoder_NO<B>(this->code_params.K, this->code_params.N);
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_SC_BFER_uncoded<B,R,Q>
::build_decoder()
{
	return new Decoder_NO<B,Q>(this->code_params.K, this->code_params.N);
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_SC_BFER_uncoded<B_8,R_8,Q_8>;
template class Simulation_SC_BFER_uncoded<B_16,R_16,Q_16>;
template class Simulation_SC_BFER_uncoded<B_32,R_32,Q_32>;
template class Simulation_SC_BFER_uncoded<B_64,R_64,Q_64>;
#else
template class Simulation_SC_BFER_uncoded<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
