#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "../../../Tools/Factory/Factory_interleaver.hpp"
#include "../../../Encoder/RA/Encoder_RA.hpp"
#include "../../../Decoder/RA/Decoder_RA.hpp"

#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "Simulation_SC_BFER_RA.hpp"

template <typename B, typename R, typename Q>
Simulation_SC_BFER_RA<B,R,Q>
::Simulation_SC_BFER_RA(const t_simulation_param& simu_params,
                        const t_code_param&       code_params,
                        const t_encoder_param&    enco_params,
                        const t_mod_param&        mod_params,
                        const t_channel_param&    chan_params,
                       const t_decoder_param&    deco_params)
: Simulation_SC_BFER<B,R,Q>(simu_params, code_params, enco_params, mod_params, chan_params, deco_params),
  interleaver(nullptr)
{
	assert(code_params.N % code_params.K == 0);

	// build the interleaver for the encoder and the decoder
	interleaver = Factory_interleaver<int>::build(this->code_params, this->code_params.N);
}

template <typename B, typename R, typename Q>
Simulation_SC_BFER_RA<B,R,Q>
::~Simulation_SC_BFER_RA()
{
	delete interleaver;
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER_RA<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER_RA<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_SC_BFER_RA<B,R,Q>
::build_encoder()
{
	Encoder<B>* encoder = new Encoder_RA<B>(this->code_params.K, this->code_params.N, *interleaver);
	return encoder;
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_SC_BFER_RA<B,R,Q>
::build_decoder()
{
	Decoder<B,Q>* decoder = new Decoder_RA<B,Q>(this->code_params.K,
	                                            this->code_params.N,
	                                            *interleaver,
	                                            this->deco_params.max_iter);
	interleaver->set_n_frames(decoder->get_n_frames());

	return decoder;
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_SC_BFER_RA<B_8,R_8,Q_8>;
template class Simulation_SC_BFER_RA<B_16,R_16,Q_16>;
template class Simulation_SC_BFER_RA<B_32,R_32,Q_32>;
template class Simulation_SC_BFER_RA<B_64,R_64,Q_64>;
#else
template class Simulation_SC_BFER_RA<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
