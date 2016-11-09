#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Tools/Display/bash_tools.h"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"

#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_CRC.hpp"
#include "Tools/Factory/Factory_encoder_common.hpp"
#include "Tools/Factory/Factory_modulator.hpp"
#include "Tools/Factory/Factory_channel.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Coset/Factory_coset_real.hpp"
#include "Tools/Factory/Coset/Factory_coset_bit.hpp"
#include "Tools/Factory/Factory_monitor.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"

#include "Simulation_BFER_i.hpp"

template <typename B, typename R, typename Q>
Simulation_BFER_i<B,R,Q>
::Simulation_BFER_i(const parameters& params)
: Simulation(),

  params(params),

  barrier(params.simulation.n_threads),

  snr      (0.f),
  code_rate(0.f),
  sigma    (0.f),

  source     ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  crc        ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  encoder    ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  puncturer  ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  modulator  ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  channel    ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  quantizer  ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  coset_real ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  decoder    ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  coset_bit  ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr),
  monitor    ((params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads, nullptr)
{
	assert(params.simulation.n_threads >= 1);
}

template <typename B, typename R, typename Q>
Simulation_BFER_i<B,R,Q>
::~Simulation_BFER_i()
{
	release_objects();
}

template <typename B, typename R, typename Q>
void Simulation_BFER_i<B,R,Q>
::build_communication_chain(Simulation_BFER_i<B,R,Q> *simu, const int tid)
{
	// build the objects
	simu->source    [tid] = simu->build_source    (        tid); check_errors(simu->source    [tid], "Source<B>"     );
	simu->crc       [tid] = simu->build_crc       (        tid); check_errors(simu->crc       [tid], "CRC<B>"        );
	simu->encoder   [tid] = simu->build_encoder   (        tid); check_errors(simu->encoder   [tid], "Encoder<B>"    );
	simu->puncturer [tid] = simu->build_puncturer (        tid); check_errors(simu->puncturer [tid], "Puncturer<B,Q>");
	simu->modulator [tid] = simu->build_modulator (        tid); check_errors(simu->modulator [tid], "Modulator<B,R>");

	const auto N     = simu->params.code.N;
	const auto tail  = simu->params.code.tail_length;
	const auto N_mod = simu->modulator[tid]->get_buffer_size_after_modulation(N + tail);

	simu->channel   [tid] = simu->build_channel   (N_mod , tid); check_errors(simu->channel   [tid], "Channel<R>"    );
	simu->quantizer [tid] = simu->build_quantizer (N+tail, tid); check_errors(simu->quantizer [tid], "Quantizer<R,Q>");
	simu->coset_real[tid] = simu->build_coset_real(        tid); check_errors(simu->coset_real[tid], "Coset<B,Q>"    );
	simu->decoder   [tid] = simu->build_decoder   (        tid); check_errors(simu->decoder   [tid], "Decoder<B,Q>"  );
	simu->coset_bit [tid] = simu->build_coset_bit (        tid); check_errors(simu->coset_bit [tid], "Coset<B,B>"    );
	simu->monitor   [tid] = simu->build_monitor   (        tid); check_errors(simu->monitor   [tid], "Monitor<B>"    );

	// get the real number of frames per threads (from the decoder)
	const auto n_frames = simu->decoder[tid]->get_n_frames();

	// set the real number of frames per thread
	simu->source    [tid]->set_n_frames(n_frames);
	simu->crc       [tid]->set_n_frames(n_frames);
	simu->encoder   [tid]->set_n_frames(n_frames);
	simu->puncturer [tid]->set_n_frames(n_frames);
	simu->modulator [tid]->set_n_frames(n_frames);
	simu->channel   [tid]->set_n_frames(n_frames);
	simu->quantizer [tid]->set_n_frames(n_frames);
	simu->coset_real[tid]->set_n_frames(n_frames);
	simu->coset_bit [tid]->set_n_frames(n_frames);
	simu->monitor   [tid]->set_n_frames(n_frames);
}

template <typename B, typename R, typename Q>
void Simulation_BFER_i<B,R,Q>
::launch()
{
	this->launch_precompute();

	// for each SNR to be simulated
	for (snr = params.simulation.snr_min; snr <= params.simulation.snr_max; snr += params.simulation.snr_step)
	{
		t_snr = std::chrono::steady_clock::now();

		code_rate = (float)(params.code.K / (float)(params.code.N + params.code.tail_length));
		sigma     = std::sqrt((float)params.modulator.upsample_factor) /
		            std::sqrt(2.f * code_rate * (float)params.modulator.bits_per_symbol * std::pow(10.f, (snr / 10.f)));

		this->snr_precompute ();
		this->_launch        ();
		this->snr_postcompute();
		this->release_objects();

		// exit simulation (double [ctrl+c])
		if (Monitor<B>::is_over())
			break;
	}

	this->launch_postcompute();
}

template <typename B, typename R, typename Q>
void Simulation_BFER_i<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_i<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_i<B,R,Q>
::snr_postcompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_i<B,R,Q>
::release_objects()
{
	const auto nobj = (params.simulation.n_conc_tasks) ? params.simulation.n_conc_tasks : params.simulation.n_threads;
	for (auto i = 0; i < nobj; i++) if (source    [i] != nullptr) { delete source    [i]; source    [i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (crc       [i] != nullptr) { delete crc       [i]; crc       [i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (encoder   [i] != nullptr) { delete encoder   [i]; encoder   [i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (puncturer [i] != nullptr) { delete puncturer [i]; puncturer [i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (modulator [i] != nullptr) { delete modulator [i]; modulator [i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (channel   [i] != nullptr) { delete channel   [i]; channel   [i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (quantizer [i] != nullptr) { delete quantizer [i]; quantizer [i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (coset_real[i] != nullptr) { delete coset_real[i]; coset_real[i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (decoder   [i] != nullptr) { delete decoder   [i]; decoder   [i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (coset_bit [i] != nullptr) { delete coset_bit [i]; coset_bit [i] = nullptr; }
	for (auto i = 0; i < nobj; i++) if (monitor   [i] != nullptr) { delete monitor   [i]; monitor   [i] = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFER_i<B,R,Q>
::launch_postcompute()
{
}

template <typename B, typename R, typename Q>
Source<B>* Simulation_BFER_i<B,R,Q>
::build_source(const int tid)
{
	return Factory_source<B>::build(params, params.simulation.seed + tid);
}

template <typename B, typename R, typename Q>
CRC<B>* Simulation_BFER_i<B,R,Q>
::build_crc(const int tid)
{
	return Factory_CRC<B>::build(params);
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFER_i<B,R,Q>
::build_encoder(const int tid)
{
	return Factory_encoder_common<B>::build(params, params.simulation.seed + tid);
}

template <typename B, typename R, typename Q>
Puncturer<B,Q>* Simulation_BFER_i<B,R,Q>
::build_puncturer(const int tid)
{
	return new Puncturer_NO<B,Q>(params.code.K, params.code.N + params.code.tail_length);
}

template <typename B, typename R, typename Q>
Modulator<B,R,R>* Simulation_BFER_i<B,R,Q>
::build_modulator(const int tid)
{
	return Factory_modulator<B,R,R>::build(params, sigma);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_BFER_i<B,R,Q>
::build_channel(const int size, const int tid)
{
	return Factory_channel<R>::build(params, sigma, size, params.simulation.seed + tid);
}

template <typename B, typename R, typename Q>
Quantizer<R,Q>* Simulation_BFER_i<B,R,Q>
::build_quantizer(const int size, const int tid)
{
	return Factory_quantizer<R,Q>::build(params, sigma, size);
}

template <typename B, typename R, typename Q>
Coset<B,Q>* Simulation_BFER_i<B,R,Q>
::build_coset_real(const int tid)
{
	return Factory_coset_real<B,Q>::build(params);
}

template <typename B, typename R, typename Q>
Coset<B,B>* Simulation_BFER_i<B,R,Q>
::build_coset_bit(const int tid)
{
	return Factory_coset_bit<B>::build(params);
}

template <typename B, typename R, typename Q>
Monitor<B>* Simulation_BFER_i<B,R,Q>
::build_monitor(const int tid)
{
	return Factory_monitor<B>::build(params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFER_i<B_8,R_8,Q_8>;
template class Simulation_BFER_i<B_16,R_16,Q_16>;
template class Simulation_BFER_i<B_32,R_32,Q_32>;
template class Simulation_BFER_i<B_64,R_64,Q_64>;
#else
template class Simulation_BFER_i<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
