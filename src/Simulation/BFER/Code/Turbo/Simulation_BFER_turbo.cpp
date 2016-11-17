#include <map>
#include <chrono>
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "Tools/Factory/Turbo/Factory_encoder_turbo.hpp"
#include "Tools/Factory/Turbo/Factory_scaling_factor.hpp"
#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"
#include "Tools/Factory/Turbo/Factory_decoder_turbo.hpp"


#include "Simulation_BFER_turbo.hpp"

template <typename B, typename R, typename Q, typename QD>
Simulation_BFER_turbo<B,R,Q,QD>
::Simulation_BFER_turbo(const parameters& params)
: Simulation_BFER<B,R,Q>(params),
  interleaver(this->params.simulation.n_threads, nullptr),
  sub_encoder(this->params.simulation.n_threads, nullptr),
  siso       (this->params.simulation.n_threads, nullptr),
  sf         (this->params.simulation.n_threads, nullptr)
{
	assert(params.code.N / params.code.K == 3);

	for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
	{
		auto seed = this->params.simulation.seed;
		seed += (params.interleaver.type == "UNIFORM") ? tid : 0;

		// build the interleaver for the encoder and the decoder
		interleaver[tid] = Factory_interleaver<short>::build(this->params, this->params.code.K, seed);
		Simulation::check_errors(interleaver[tid], "Interleaver<short>");
	}

	if (!params.simulation.json_path.empty())
	{
		assert(this->params.simulation.n_threads == 1);
		json_stream.open(params.simulation.json_path.c_str(), std::ios::out | std::ios::trunc);

		json_stream << "[" << std::endl;
	}
}

template <typename B, typename R, typename Q, typename QD>
Simulation_BFER_turbo<B,R,Q,QD>
::~Simulation_BFER_turbo()
{
	release_objects();
	if (json_stream.is_open())
	{
		json_stream << "[{\"stage\": \"end\"}]]" << std::endl;
		json_stream.close();
	}
	for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
		if (interleaver[tid] != nullptr)
			delete interleaver[tid];
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_BFER_turbo<B,R,Q,QD>
::release_objects()
{
	Simulation_BFER<B,R,Q>::release_objects();

	int tid = 0;
	const int nthr = this->params.simulation.n_threads;
	for (tid = 0; tid < nthr; tid++) if (sub_encoder[tid] != nullptr) { delete sub_encoder[tid]; sub_encoder[tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (siso       [tid] != nullptr) { delete siso       [tid]; siso       [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (sf         [tid] != nullptr) { delete sf         [tid]; sf         [tid] = nullptr; }
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_BFER_turbo<B,R,Q,QD>
::launch_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_BFER_turbo<B,R,Q,QD>
::snr_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
Encoder<B>* Simulation_BFER_turbo<B,R,Q,QD>
::build_encoder(const int tid)
{
	sub_encoder[tid] = Factory_encoder_RSC<B>::build(this->params, 1, json_stream);
	Simulation::check_errors(sub_encoder[tid], "Encoder_RSC_sys<B>");

	if (tid == 0)
		trellis = sub_encoder[tid]->get_trellis();

	Encoder<B>* encoder = nullptr;
	if (this->params.simulation.json_path.empty())
		encoder = Simulation_BFER<B,R,Q>::build_encoder(tid);
	if (encoder == nullptr)
		encoder = Factory_encoder_turbo<B>::build(this->params, interleaver[tid], sub_encoder[tid], sub_encoder[tid]);

	return encoder;
}

template <typename B, typename R, typename Q, typename QD>
Decoder<B,Q>* Simulation_BFER_turbo<B,R,Q,QD>
::build_decoder(const int tid)
{
	sf[tid] = Factory_scaling_factor<Q>::build(this->params);
	Simulation::check_errors(sf[tid], "Scaling_factor<Q>");

	this->barrier(tid);
	siso[tid] = Factory_decoder_RSC<B,Q,QD>::build_siso(this->params, trellis, json_stream);
	Simulation::check_errors(siso[tid], "SISO<Q>");

	interleaver[tid]->set_n_frames(siso[tid]->get_n_frames());

	return Factory_decoder_turbo<B,Q>::build(this->params, interleaver[tid], siso[tid], siso[tid], sf[tid], this->crc[tid]);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFER_turbo<B_8,R_8,Q_8,QD_8>;
template class Simulation_BFER_turbo<B_16,R_16,Q_16,QD_16>;
template class Simulation_BFER_turbo<B_32,R_32,Q_32,QD_32>;
template class Simulation_BFER_turbo<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_BFER_turbo<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
