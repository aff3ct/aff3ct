#include <map>
#include <chrono>
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <algorithm>

#include "Tools/bash_tools.h"
#include "Tools/simu_tools.h"

#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "Tools/Factory/Turbo/Factory_encoder_turbo.hpp"
#include "Tools/Factory/Turbo/Factory_scaling_factor.hpp"
#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"
#include "Tools/Factory/Turbo/Factory_decoder_turbo.hpp"

#include "Simulation_turbo.hpp"

template <typename B, typename R, typename Q, typename QD>
Simulation_turbo<B,R,Q,QD>
::Simulation_turbo(const t_simulation_param& simu_params,
                   const t_code_param&       code_params,
                   const t_encoder_param&    enco_params,
                   const t_mod_param&        mod_params,
                   const t_channel_param&    chan_params,
                   const t_decoder_param&    deco_params)
: Simulation_BFER<B,R,Q>(simu_params, code_params, enco_params, mod_params, chan_params, deco_params),
  interleaver(                       nullptr),
  sub_encoder(simu_params.n_threads, nullptr),
  siso       (simu_params.n_threads, nullptr),
  sf         (simu_params.n_threads, nullptr)
{
	assert(code_params.N / code_params.K == 3);

	// build the interleaver for the encoder and the decoder
	interleaver = Factory_interleaver<short>::build(this->code_params, this->code_params.K);
	check_errors(interleaver, "Interleaver<short>");
}

template <typename B, typename R, typename Q, typename QD>
Simulation_turbo<B,R,Q,QD>
::~Simulation_turbo()
{
	release_objects();
	delete interleaver;
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo<B,R,Q,QD>
::release_objects()
{
	Simulation_BFER<B,R,Q>::release_objects();

	int tid = 0;
	const int nthr = this->simu_params.n_threads;
	for (tid = 0; tid < nthr; tid++) if (sub_encoder[tid] != nullptr) { delete sub_encoder[tid]; sub_encoder[tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (siso       [tid] != nullptr) { delete siso       [tid]; siso       [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (sf         [tid] != nullptr) { delete sf         [tid]; sf         [tid] = nullptr; }
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo<B,R,Q,QD>
::launch_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo<B,R,Q,QD>
::snr_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
Encoder<B>* Simulation_turbo<B,R,Q,QD>
::build_encoder(const int tid)
{
	sub_encoder[tid] = Factory_encoder_RSC<B>::build(this->simu_params,
	                                                 this->code_params,
	                                                 this->enco_params,
	                                                 this->deco_params);
	check_errors(sub_encoder[tid], "Encoder_RSC_sys<B>");

	if (tid == 0)
		trellis = sub_encoder[tid]->get_trellis();

	return Factory_encoder_turbo<B>::build(this->code_params,
	                                       this->enco_params,
	                                       interleaver,
	                                       sub_encoder[tid],
	                                       sub_encoder[tid]);
}

template <typename B, typename R, typename Q, typename QD>
Decoder<B,Q>* Simulation_turbo<B,R,Q,QD>
::build_decoder(const int tid)
{
	sf[tid] = Factory_scaling_factor<Q>::build(this->code_params, this->deco_params);
	check_errors(sf[tid], "Scaling_factor<Q>");

	this->barrier(tid);
	siso[tid] = Factory_decoder_RSC<B,Q,QD>::build_siso(this->code_params,
	                                                    this->enco_params,
	                                                    this->chan_params,
	                                                    this->deco_params,
	                                                    trellis);
	check_errors(siso[tid], "SISO<Q>");

	if (tid == 0)
		interleaver->set_n_frames(siso[tid]->get_n_frames_siso());

	return Factory_decoder_turbo<B,Q>::build(this->code_params,
	                                         this->enco_params,
	                                         this->chan_params,
	                                         this->deco_params,
	                                         interleaver,
	                                         siso[tid],
	                                         siso[tid],
	                                         sf[tid],
	                                         this->crc[tid]);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_turbo<B_8,R_8,Q_8,QD_8>;
template class Simulation_turbo<B_16,R_16,Q_16,QD_16>;
template class Simulation_turbo<B_32,R_32,Q_32,QD_32>;
template class Simulation_turbo<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_turbo<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
