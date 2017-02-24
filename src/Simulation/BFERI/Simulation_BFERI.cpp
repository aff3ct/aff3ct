#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_CRC.hpp"
#include "Tools/Factory/Factory_encoder_common.hpp"
#include "Tools/Factory/Factory_modulator.hpp"
#include "Tools/Factory/Factory_channel.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/Coset/Factory_coset_real.hpp"
#include "Tools/Factory/Coset/Factory_coset_bit.hpp"
#include "Tools/Factory/Factory_monitor.hpp"
#include "Tools/Factory/Factory_terminal.hpp"

#include "Simulation_BFERI.hpp"

template <typename B, typename R, typename Q>
Simulation_BFERI_i<B,R,Q>
::Simulation_BFERI_i(const parameters& params)
: Simulation(),
  
  params(params),

  barrier(params.simulation.n_threads),
 
  snr      (0.f),
  snr_s    (0.f),
  snr_b    (0.f),
  code_rate(0.f),
  sigma    (0.f),

  rd_engine_seed(params.simulation.n_threads),

  source     (params.simulation.n_threads, nullptr),
  crc        (params.simulation.n_threads, nullptr),
  encoder    (params.simulation.n_threads, nullptr),
  modulator  (params.simulation.n_threads, nullptr),
  channel    (params.simulation.n_threads, nullptr),
  quantizer  (params.simulation.n_threads, nullptr),
  interleaver(params.simulation.n_threads, nullptr),
  coset_real (params.simulation.n_threads, nullptr),
  siso       (params.simulation.n_threads, nullptr),
  decoder    (params.simulation.n_threads, nullptr),
  coset_bit  (params.simulation.n_threads, nullptr),
  monitor    (params.simulation.n_threads, nullptr)
{
	assert(params.simulation.n_threads >= 1);

	for (auto tid = 0; tid < params.simulation.n_threads; tid++)
		rd_engine_seed[tid].seed(params.simulation.seed + tid);
}

template <typename B, typename R, typename Q>
Simulation_BFERI_i<B,R,Q>
::~Simulation_BFERI_i()
{
	release_objects();
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_i<B,R,Q>
::build_communication_chain(Simulation_BFERI_i<B,R,Q> *simu, const int tid)
{
	// build the objects
	simu->source     [tid] = simu->build_source     (       tid); check_errors(simu->source     [tid], "Source<B>"       );
	simu->crc        [tid] = simu->build_crc        (       tid); check_errors(simu->crc        [tid], "CRC<B>"          );
	simu->encoder    [tid] = simu->build_encoder    (       tid); check_errors(simu->encoder    [tid], "Encoder<B>"      );
	simu->interleaver[tid] = simu->build_interleaver(       tid); check_errors(simu->interleaver[tid], "Interleaver<int>");
	simu->modulator  [tid] = simu->build_modulator  (       tid); check_errors(simu->modulator  [tid], "Modulator<B,R>"  );

	const auto N     = simu->params.code.N;
	const auto tail  = simu->params.code.tail_length;
	const auto N_mod = simu->modulator[tid]->get_buffer_size_after_modulation(N + tail);
	const auto N_fil = simu->modulator[tid]->get_buffer_size_after_filtering (N + tail);

	simu->channel    [tid] = simu->build_channel    (N_mod, tid); check_errors(simu->channel    [tid], "Channel<R>"      );
	simu->quantizer  [tid] = simu->build_quantizer  (N_fil, tid); check_errors(simu->quantizer  [tid], "Quantizer<R,Q>"  );
	simu->coset_real [tid] = simu->build_coset_real (       tid); check_errors(simu->coset_real [tid], "Coset<B,Q>"      );
	simu->siso       [tid] = simu->build_siso       (       tid); check_errors(simu->siso       [tid], "SISO<Q>"         );
	simu->decoder    [tid] = simu->build_decoder    (       tid); check_errors(simu->decoder    [tid], "Decoder<B,Q>"    );
	simu->coset_bit  [tid] = simu->build_coset_bit  (       tid); check_errors(simu->coset_bit  [tid], "Coset<B,B>"      );
	simu->monitor    [tid] = simu->build_monitor    (       tid); check_errors(simu->monitor    [tid], "Monitor<B>"      );

	// get the real number of frames per threads (from the decoder)
	const auto n_frames = simu->siso[tid]->get_n_frames();
	assert(simu->siso[tid]->get_n_frames() == simu->decoder[tid]->get_n_frames());

	// set the real number of frames per thread
	simu->source     [tid]->set_n_frames(n_frames);
	simu->crc        [tid]->set_n_frames(n_frames);
	simu->encoder    [tid]->set_n_frames(n_frames);
	simu->interleaver[tid]->set_n_frames(n_frames);
	simu->modulator  [tid]->set_n_frames(n_frames);
	simu->channel    [tid]->set_n_frames(n_frames);
	simu->quantizer  [tid]->set_n_frames(n_frames);
	simu->coset_real [tid]->set_n_frames(n_frames);
	simu->coset_bit  [tid]->set_n_frames(n_frames);
	simu->monitor    [tid]->set_n_frames(n_frames);
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_i<B,R,Q>
::launch()
{
	launch_precompute();
	
	// for each SNR to be simulated
	for (snr = params.simulation.snr_min; snr <= params.simulation.snr_max; snr += params.simulation.snr_step)
	{
		t_snr = std::chrono::steady_clock::now();

		code_rate = (float)(params.code.K / (float)(params.code.N + params.code.tail_length));

		if (params.simulation.snr_type == "EB")
		{
			snr_b = snr;
			snr_s = snr + 10.f * std::log10(code_rate * (float)params.modulator.bits_per_symbol);
		}
		else //if(params.simulation.snr_type == "ES")
		{
			snr_s = snr;
			snr_b = snr - 10.f * std::log10(code_rate * (float)params.modulator.bits_per_symbol);
		}
		sigma = std::sqrt((float)params.modulator.upsample_factor) / std::sqrt(2.f * std::pow(10.f, (snr_s / 10.f)));

		this->snr_precompute ();
		this->_launch        ();
		this->snr_postcompute();
		this->release_objects();

		// exit simulation (double [ctrl+c])
		if (Monitor<B,R>::is_over())
			break;
	}

	this->launch_postcompute();
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_i<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_i<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_i<B,R,Q>
::snr_postcompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_i<B,R,Q>
::release_objects()
{
	const auto nthr = params.simulation.n_threads;
	for (auto i = 0; i < nthr; i++) if (source     [i] != nullptr) { delete source     [i]; source     [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (crc        [i] != nullptr) { delete crc        [i]; crc        [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (encoder    [i] != nullptr) { delete encoder    [i]; encoder    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (interleaver[i] != nullptr) { delete interleaver[i]; interleaver[i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (modulator  [i] != nullptr) { delete modulator  [i]; modulator  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (channel    [i] != nullptr) { delete channel    [i]; channel    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (quantizer  [i] != nullptr) { delete quantizer  [i]; quantizer  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_real [i] != nullptr) { delete coset_real [i]; coset_real [i] = nullptr; }
	for (auto i = 0; i < nthr; i++)
		if (siso[i] != nullptr)
		{
			// do not delete the siso if the decoder and the siso are the same pointers
			if (decoder[i] == nullptr || siso[i] != dynamic_cast<SISO<Q>*>(decoder[i]))
				delete siso[i];
			siso[i] = nullptr;
		}
	for (auto i = 0; i < nthr; i++) if (decoder    [i] != nullptr) { delete decoder    [i]; decoder    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_bit  [i] != nullptr) { delete coset_bit  [i]; coset_bit  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (monitor    [i] != nullptr) { delete monitor    [i]; monitor    [i] = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_i<B,R,Q>
::launch_postcompute()
{
}

template <typename B, typename R, typename Q>
Source<B>* Simulation_BFERI_i<B,R,Q>
::build_source(const int tid)
{
	return Factory_source<B>::build(params, rd_engine_seed[tid]());
}

template <typename B, typename R, typename Q>
CRC<B>* Simulation_BFERI_i<B,R,Q>
::build_crc(const int tid)
{
	return Factory_CRC<B>::build(params);
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFERI_i<B,R,Q>
::build_encoder(const int tid)
{
	return Factory_encoder_common<B>::build(params, rd_engine_seed[tid]());
}

template <typename B, typename R, typename Q>
Interleaver<int>* Simulation_BFERI_i<B,R,Q>
::build_interleaver(const int tid)
{
	return Factory_interleaver<int>::build(params, params.code.N + params.code.tail_length, params.simulation.seed);
}

template <typename B, typename R, typename Q>
Modulator<B,R,Q>* Simulation_BFERI_i<B,R,Q>
::build_modulator(const int tid)
{
	return Factory_modulator<B,R,Q>::build(params, sigma);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_BFERI_i<B,R,Q>
::build_channel(const int size, const int tid)
{
	return Factory_channel<R>::build(params, sigma, size, params.simulation.seed + tid);
}

template <typename B, typename R, typename Q>
Quantizer<R,Q>* Simulation_BFERI_i<B,R,Q>
::build_quantizer(const int size, const int tid)
{
	return Factory_quantizer<R,Q>::build(params, sigma, size);
}

template <typename B, typename R, typename Q>
Coset<B,Q>* Simulation_BFERI_i<B,R,Q>
::build_coset_real(const int tid)
{
	return Factory_coset_real<B,Q>::build(params);
}

template <typename B, typename R, typename Q>
Coset<B,B>* Simulation_BFERI_i<B,R,Q>
::build_coset_bit(const int tid)
{
	return Factory_coset_bit<B>::build(params);
}

template <typename B, typename R, typename Q>
Monitor<B,R>* Simulation_BFERI_i<B,R,Q>
::build_monitor(const int tid)
{
	return Factory_monitor<B,R>::build(params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFERI_i<B_8,R_8,Q_8>;
template class Simulation_BFERI_i<B_16,R_16,Q_16>;
template class Simulation_BFERI_i<B_32,R_32,Q_32>;
template class Simulation_BFERI_i<B_64,R_64,Q_64>;
#else
template class Simulation_BFERI_i<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
