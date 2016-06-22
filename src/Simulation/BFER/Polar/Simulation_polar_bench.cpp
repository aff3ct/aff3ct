#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#include "Simulation_polar_bench.hpp"

template <typename B, typename R, typename Q>
Simulation_polar_bench<B,R,Q>
::Simulation_polar_bench(const t_simulation_param& simu_params,
                         const t_code_param&       code_params,
                         const t_encoder_param&    enco_params,
                         const t_channel_param&    chan_params,
                         const t_decoder_param&    deco_params)
: Simulation_polar<B,R,Q>(simu_params, code_params, enco_params, chan_params, deco_params)
{
}

template <typename B, typename R, typename Q>
Simulation_polar_bench<B,R,Q>
::~Simulation_polar_bench()
{
}

template <typename B, typename R, typename Q>
void Simulation_polar_bench<B,R,Q>
::simulation_loop()
{
	using namespace std::chrono;

	auto t_simu = steady_clock::now();

	if (this->code_params.generation_method != "AZCW")
	{
		// generate a random K bits vector U_K
		auto t_sourc = steady_clock::now();
		this->source->generate(this->U_K);
		this->d_sourc_total = steady_clock::now() - t_sourc;

		// encode U_K into a N bits vector X_N
		auto t_encod = steady_clock::now();
		this->encoder->encode(this->U_K, this->U_N, this->X_N);
		this->d_encod_total = steady_clock::now() - t_encod;

		// modulate
		auto t_modul = steady_clock::now();
		this->modulator->modulate(this->X_N, this->X_N);
		this->d_modul_total = steady_clock::now() - t_modul;
	}

	// add noise
	auto t_chann = steady_clock::now();
	this->channel->add_noise(this->X_N, this->Y_N1);
	this->d_chann_total = steady_clock::now() - t_chann;

	// make the quantization
	auto t_quant = steady_clock::now();
	this->quantizer->process(this->Y_N1, this->Y_N2);
	this->d_quant_total = steady_clock::now() - t_quant;

	auto t_compute_loop = steady_clock::now();
	while (this->analyzer->get_n_analyzed_frames() < this->simu_params.benchs &&
	       (this->simu_params.stop_time == seconds(0) || 
	        (steady_clock::now() - t_compute_loop) < this->simu_params.stop_time))
	{
		// load data in the decoder
		auto t_load = steady_clock::now();
		this->decoder->load(this->Y_N2);
		this->d_load_total += steady_clock::now() - t_load;

		// launch decoder
		auto t_decod = steady_clock::now();
		this->decoder->decode();
		this->d_decod_total += steady_clock::now() - t_decod;

		// store results in V_N
		auto t_store = steady_clock::now();
		this->decoder->store(this->V_N);
		this->d_store_total += steady_clock::now() - t_store;

		// display statistics in terminal
		if (!this->simu_params.disable_display && this->simu_params.display_freq != nanoseconds(0) && 
		    (steady_clock::now() - t_simu) >= this->simu_params.display_freq)
		{
			this->terminal->temp_report(std::clog);
			t_simu = steady_clock::now();
		}

		this->analyzer->update_n_analyzed_frames();
	}
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_polar_bench<B_8,R_8,Q_8>;
template class Simulation_polar_bench<B_16,R_16,Q_16>;
template class Simulation_polar_bench<B_32,R_32,Q_32>;
template class Simulation_polar_bench<B_64,R_64,Q_64>;
#else
template class Simulation_polar_bench<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation