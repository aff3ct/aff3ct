#include <map>
#include <chrono>
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <algorithm>

#include "../../../Tools/Factory/Factory_interleaver.hpp"
#include "../../../Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "../../../Tools/Factory/Turbo/Factory_encoder_turbo.hpp"
#include "../../../Tools/Factory/Turbo/Factory_scaling_factor.hpp"
#include "../../../Tools/Factory/RSC/Factory_decoder_RSC.hpp"
#include "../../../Tools/Factory/Turbo/Factory_decoder_turbo.hpp"

#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "Simulation_SC_BFER_turbo.hpp"

template <typename B, typename R, typename Q, typename QD>
Simulation_SC_BFER_turbo<B,R,Q,QD>
::Simulation_SC_BFER_turbo(const t_simulation_param& simu_params,
                           const t_code_param&       code_params,
                           const t_encoder_param&    enco_params,
                           const t_mod_param&        mod_params,
                           const t_channel_param&    chan_params,
                           const t_decoder_param&    deco_params)
: Simulation_SC_BFER<B,R,Q>(simu_params, code_params, enco_params, mod_params, chan_params, deco_params),
  interleaver(nullptr),
  sub_encoder(nullptr),
  siso       (nullptr),
  sf         (nullptr)
{
	assert(code_params.N / code_params.K == 3);

	// build the interleaver for the encoder and the decoder
	interleaver = Factory_interleaver<short>::build(this->code_params, this->code_params.K);
	check_errors(interleaver, "Interleaver<short>");
}

template <typename B, typename R, typename Q, typename QD>
Simulation_SC_BFER_turbo<B,R,Q,QD>
::~Simulation_SC_BFER_turbo()
{
	release_objects();
	delete interleaver;
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_SC_BFER_turbo<B,R,Q,QD>
::release_objects()
{
	Simulation_SC_BFER<B,R,Q>::release_objects();

	if (sub_encoder != nullptr) { delete sub_encoder; sub_encoder = nullptr; }
	if (siso        != nullptr) { delete siso;        siso        = nullptr; }
	if (sf          != nullptr) { delete sf;          sf          = nullptr; }
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_SC_BFER_turbo<B,R,Q,QD>
::launch_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_SC_BFER_turbo<B,R,Q,QD>
::snr_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
Encoder<B>* Simulation_SC_BFER_turbo<B,R,Q,QD>
::build_encoder()
{
	sub_encoder = Factory_encoder_RSC<B>::build(this->simu_params,
	                                            this->code_params,
	                                            this->enco_params,
	                                            this->deco_params);
	check_errors(sub_encoder, "Encoder_RSC_sys<B>");

	trellis = sub_encoder->get_trellis();

	return Factory_encoder_turbo<B>::build(this->code_params,
	                                       this->enco_params,
	                                       interleaver,
	                                       sub_encoder,
	                                       sub_encoder);
}

template <typename B, typename R, typename Q, typename QD>
Decoder<B,Q>* Simulation_SC_BFER_turbo<B,R,Q,QD>
::build_decoder()
{
	sf = Factory_scaling_factor<Q>::build(this->code_params, this->deco_params);
	check_errors(sf, "Scaling_factor<Q>");

	siso = Factory_decoder_RSC<B,Q,QD>::build_siso(this->code_params,
	                                               this->enco_params,
	                                               this->chan_params,
	                                               this->deco_params,
	                                               trellis);
	check_errors(siso, "SISO<Q>");

	interleaver->set_n_frames(siso->get_n_frames_siso());

	return Factory_decoder_turbo<B,Q>::build(this->code_params,
	                                         this->enco_params,
	                                         this->chan_params,
	                                         this->deco_params,
	                                         interleaver,
	                                         siso,
	                                         siso,
	                                         sf,
	                                         this->crc);
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_SC_BFER_turbo<B_8,R_8,Q_8,QD_8>;
template class Simulation_SC_BFER_turbo<B_16,R_16,Q_16,QD_16>;
template class Simulation_SC_BFER_turbo<B_32,R_32,Q_32,QD_32>;
template class Simulation_SC_BFER_turbo<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_SC_BFER_turbo<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
