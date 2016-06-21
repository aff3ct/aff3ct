#include <map>
#include <chrono>
#include <vector>
#include <string>
#include <cassert>
#include <utility>
#include <cstdlib>
#include <algorithm>

#include "../../../Tools/Factory/Factory_source.hpp"
#include "../../../Tools/Factory/Factory_encoder_RSC.hpp"
#include "../../../Tools/Factory/Factory_modulator.hpp"
#include "../../../Tools/Factory/Factory_channel.hpp"
#include "../../../Tools/Factory/Factory_quantizer.hpp"
#include "../../../Tools/Factory/Factory_decoder_RSC.hpp"
#include "../../../Tools/Factory/Factory_error_analyzer.hpp"
#include "../../../Tools/Factory/Factory_terminal.hpp"

#include "../../../Tools/params.h"
#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "Simulation_RSC_mt.hpp"

template <typename B, typename R, typename Q, typename QD>
Simulation_RSC_mt<B,R,Q,QD>
::Simulation_RSC_mt(const t_simulation_param& simu_params,
                    const t_code_param&       code_params,
                    const t_encoder_param&    enco_params,
                    const t_channel_param&    chan_params,
                    const t_decoder_param&    deco_params,
                    const int n_threads)
: Simulation(),
  simu_params(simu_params),
  code_params(code_params),
  enco_params(enco_params),
  chan_params(chan_params),
  deco_params(deco_params),
  n_threads(n_threads),
  threads(n_threads -1),
  U_K(n_threads),
  X_N(n_threads),
  Y_N1(n_threads),
  Y_N2(n_threads),
  V_K(n_threads),
  trellis(n_threads),
  code_rate(0.f),
  sigma(0.f),
  source(n_threads),
  encoder(n_threads),
  modulator(n_threads),
  channel(n_threads),
  quantizer(n_threads),
  decoder(n_threads),
  analyzer(n_threads),
  analyzer_red(nullptr),
  terminal(nullptr),
  n_frames(1),
  d_sourc_total(n_threads),
  d_encod_total(n_threads),
  d_modul_total(n_threads),
  d_chann_total(n_threads),
  d_quant_total(n_threads),
  d_load_total(n_threads),
  d_decod_total(n_threads),
  d_store_total(n_threads),
  d_check_total(n_threads),
  barrier(n_threads)
{
	assert(code_params.N / code_params.K == 2);
	assert(n_threads > 0);

	for (auto tid = 0; tid < n_threads; tid++)
	{
		U_K [tid].resize(code_params.K);
		X_N [tid].resize(code_params.N + code_params.tail_length);
		Y_N1[tid].resize(code_params.N + code_params.tail_length);
		Y_N2[tid].resize(code_params.N + code_params.tail_length);
		V_K [tid].resize(code_params.K);
	}
}

template <typename B, typename R, typename Q, typename QD>
Simulation_RSC_mt<B,R,Q,QD>
::~Simulation_RSC_mt()
{
	for (auto tid = 0; tid < n_threads; tid++) if (source   [tid] != nullptr) delete source   [tid];
	for (auto tid = 0; tid < n_threads; tid++) if (encoder  [tid] != nullptr) delete encoder  [tid];
	for (auto tid = 0; tid < n_threads; tid++) if (modulator[tid] != nullptr) delete modulator[tid];
	for (auto tid = 0; tid < n_threads; tid++) if (channel  [tid] != nullptr) delete channel  [tid];
	for (auto tid = 0; tid < n_threads; tid++) if (quantizer[tid] != nullptr) delete quantizer[tid];
	for (auto tid = 0; tid < n_threads; tid++) if (decoder  [tid] != nullptr) delete decoder  [tid];
	for (auto tid = 0; tid < n_threads; tid++) if (analyzer [tid] != nullptr) delete analyzer [tid];
	                                           if (analyzer_red   != nullptr) delete analyzer_red;
	                                           if (terminal       != nullptr) delete terminal;
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_RSC_mt<B,R,Q,QD>
::launch()
{
	this->snr_loop();
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_RSC_mt<B,R,Q,QD>
::snr_loop()
{
	// for each SNR to be simulated
	for (auto snr = simu_params.snr_min; snr <= simu_params.snr_max; snr += simu_params.snr_step)
	{
		t_snr = std::chrono::steady_clock::now();

		code_rate = (float)code_params.K / (float)(code_params.N + code_params.tail_length);
		sigma     = 1.f / sqrt(2.f * code_rate * pow(10.f, (snr / 10.f)));

		// launch a group of slave threads (there is "n_threads -1" slave threads)
		for (auto tid = 1; tid < n_threads; tid++)
			threads[tid -1] = std::thread(Simulation_RSC_mt<B,R,Q,QD>::simulation_Monte_Carlo, this, snr, tid);

		// launch the master thread
		Simulation_RSC_mt<B,R,Q,QD>::simulation_Monte_Carlo(this, snr, 0);

		// join the slave threads with the master thread
		for (auto tid = 1; tid < n_threads; tid++)
			threads[tid -1].join();

		if (!simu_params.disable_display)
		{
			analyzer_red->reduce();
			Simulation_RSC_mt<B,R,Q,QD>::time_reduction(this);
			terminal->final_report(std::cout);
		}
	}
	// end snr loop
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_RSC_mt<B,R,Q,QD>
::simulation_Monte_Carlo(Simulation_RSC_mt<B,R,Q,QD> *simu, const R snr, const int tid)
{
	// allocate and build all the communication chain for simulate: encoding -> add noise -> decoding...
	Simulation_RSC_mt<B,R,Q,QD>::build_communication_chain(simu, snr, tid);

	if (simu->code_params.generation_method == "AZCW")
	{
		std::fill(simu->U_K[tid].begin(), simu->U_K[tid].end(), (B)0);
		std::fill(simu->X_N[tid].begin(), simu->X_N[tid].end(), (B)0);
		simu->modulator[tid]->modulate(simu->X_N[tid]);
	}

	simu->d_sourc_total[tid] = std::chrono::nanoseconds(0);
	simu->d_encod_total[tid] = std::chrono::nanoseconds(0);
	simu->d_modul_total[tid] = std::chrono::nanoseconds(0);
	simu->d_chann_total[tid] = std::chrono::nanoseconds(0);
	simu->d_quant_total[tid] = std::chrono::nanoseconds(0);
	simu->d_load_total [tid] = std::chrono::nanoseconds(0);
	simu->d_decod_total[tid] = std::chrono::nanoseconds(0);
	simu->d_store_total[tid] = std::chrono::nanoseconds(0);
	simu->d_check_total[tid] = std::chrono::nanoseconds(0);


	if (tid == 0 && !simu->simu_params.disable_display 
	             && snr == simu->simu_params.snr_min 
	             && !simu->simu_params.enable_debug)
		simu->terminal->legend(std::clog);

	simu->barrier(tid);

	Simulation_RSC_mt<B,R,Q,QD>::simulation_loop(simu, tid);
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_RSC_mt<B,R,Q,QD>
::build_communication_chain(Simulation_RSC_mt<B,R,Q,QD> *simu, const R& snr, const int tid)
{
	// release communication chain objects before allocation
	if (simu->source   [tid] != nullptr) delete simu->source   [tid];
	if (simu->encoder  [tid] != nullptr) delete simu->encoder  [tid];
	if (simu->modulator[tid] != nullptr) delete simu->modulator[tid];
	if (simu->channel  [tid] != nullptr) delete simu->channel  [tid];
	if (simu->quantizer[tid] != nullptr) delete simu->quantizer[tid];
	if (simu->decoder  [tid] != nullptr) delete simu->decoder  [tid];
	if (simu->analyzer [tid] != nullptr) delete simu->analyzer [tid];
	if (tid == 0)
	{
		if (simu->analyzer_red != nullptr) delete simu->analyzer_red;
		if (simu->terminal     != nullptr) delete simu->terminal;
	}

	// build the encoder
	simu->encoder[tid] = Factory_encoder_RSC<B>::build(simu->simu_params, simu->code_params, simu->enco_params, 
	                                                   simu->deco_params);
	check_errors(simu->encoder[tid], "Encoder_RSC<B>", tid);

	// build the the decoder
	simu->trellis[tid] = simu->encoder[tid]->get_trellis();
	simu->decoder[tid] = Factory_decoder_RSC<B,Q,QD>::build(simu->code_params, simu->enco_params, simu->chan_params, 
	                                                        simu->deco_params, simu->trellis[tid]);
	check_errors(simu->decoder[tid], "Decoder<B,Q>", tid);
	
	const auto n_fra = simu->decoder[tid]->get_n_frames();
	if (tid == 0)
		simu->n_frames = n_fra;

	// set the right number of frames to the encoder
	simu->encoder[tid]->set_n_frames(n_fra);

	// resize the buffers if needed
	if ((int)simu->U_K [tid].size() != (simu->code_params.K * n_fra)) 
		simu->U_K [tid].resize( simu->code_params.K * n_fra);
	if ((int)simu->X_N [tid].size() != (simu->code_params.N * n_fra)) 
		simu->X_N [tid].resize((simu->code_params.N + simu->code_params.tail_length) * n_fra);
	if ((int)simu->Y_N1[tid].size() != (simu->code_params.N * n_fra)) 
		simu->Y_N1[tid].resize((simu->code_params.N + simu->code_params.tail_length) * n_fra);
	if ((int)simu->Y_N2[tid].size() != (simu->code_params.N * n_fra)) 
		simu->Y_N2[tid].resize((simu->code_params.N + simu->code_params.tail_length) * n_fra);
	if ((int)simu->V_K [tid].size() != (simu->code_params.K * n_fra)) 
		simu->V_K [tid].resize( simu->code_params.K * n_fra);
	
	// build the source
	simu->source[tid] = Factory_source<B>::build(simu->code_params);
	check_errors(simu->source[tid], "Source<B>", tid);

	// build the modulator
	simu->modulator[tid] = Factory_modulator<B>::build();
	check_errors(simu->modulator[tid], "Modulator<B>", tid);

	// build the channel
	simu->channel[tid] = Factory_channel<B,R>::build(simu->chan_params, simu->sigma, tid,
	                                                 2.0 / (simu->sigma * simu->sigma));
	check_errors(simu->channel[tid], "Channel<B,R>", tid);

	// build the quantizer
	simu->quantizer[tid] = Factory_quantizer<R,Q>::build(simu->chan_params, simu->sigma);
	check_errors(simu->quantizer[tid], "Quantizer<R,Q>", tid);

	// build the analyser
	simu->analyzer[tid] = Factory_error_analyzer<B,R>::build(simu->simu_params, simu->code_params, n_fra);
	check_errors(simu->analyzer[tid], "Error_analyzer<B,R>", tid);

	simu->barrier(tid);

	if (tid == 0)
	{
		// build an error analyzer to compute BER/FER (reduce the other analyzers)
		simu->analyzer_red = new Error_analyzer_reduction<B,R>(simu->code_params.K, simu->code_params.N, 
		                                                       simu->simu_params.max_fe, simu->analyzer, n_fra);

		// build the terminal to display the BER/FER
		simu->terminal = Factory_terminal<B,R>::build(simu->simu_params, snr, simu->analyzer_red, 
		                                              simu->t_snr, simu->d_load_total_red, simu->d_decod_total_red, 
		                                              simu->d_store_total_red);
		check_errors(simu->terminal, "Terminal");
	}
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_RSC_mt<B,R,Q,QD>
::simulation_loop(Simulation_RSC_mt<B,R,Q,QD> *simu, const int tid)
{
	using namespace std::chrono;

	if (tid == 0)
		simu->t_simu = steady_clock::now();

	int prev_n_fe = 0;

	// simulation loop
	while ((!simu->analyzer_red->fe_limit_achieved()) && // while max frame error count has not been reached
	        (simu->simu_params.stop_time == seconds(0) || 
	         (steady_clock::now() - simu->t_snr) < simu->simu_params.stop_time))
	{
		auto d_sourc = nanoseconds(0);
		auto d_encod = nanoseconds(0);
		auto d_modul = nanoseconds(0);

		if (simu->code_params.generation_method != "AZCW")
		{
			// generate a random K bits vector U_K
			auto t_sourc = steady_clock::now();
			simu->source[tid]->generate(simu->U_K[tid]);
			d_sourc = steady_clock::now() - t_sourc;

			// encode U_N into a N bits vector X_N
			auto t_encod = steady_clock::now();
			simu->encoder[tid]->encode(simu->U_K[tid], simu->X_N[tid]);
			d_encod = steady_clock::now() - t_encod;

			// modulate
			auto t_modul = steady_clock::now();
			simu->modulator[tid]->modulate(simu->X_N[tid]);
			d_modul = steady_clock::now() - t_modul;
		}

		// add noise
		auto t_chann = steady_clock::now();
		simu->channel[tid]->add_noise(simu->X_N[tid], simu->Y_N1[tid]);
		auto d_chann = steady_clock::now() - t_chann;

		// make the quantization
		auto t_quant = steady_clock::now();
		simu->quantizer[tid]->process(simu->Y_N1[tid], simu->Y_N2[tid]);
		auto d_quant = steady_clock::now() - t_quant;
	
		// load data in the decoder
		auto t_load = steady_clock::now();
		simu->decoder[tid]->load(simu->Y_N2[tid]);
		auto d_load = steady_clock::now() - t_load;

		// launch decoder
		auto t_decod = steady_clock::now();
		simu->decoder[tid]->decode();
		auto d_decod = steady_clock::now() - t_decod;

		// store results in V_K
		auto t_store = steady_clock::now();
		simu->decoder[tid]->store(simu->V_K[tid]);
		auto d_store = steady_clock::now() - t_store;

		// unpack V_K if we used bit packing in the decoder (do nothing else)
		simu->decoder[tid]->unpack(simu->V_K[tid]);

		// check errors in the frame
		auto t_check = steady_clock::now();
		simu->analyzer[tid]->check_errors(simu->U_K[tid], simu->V_K[tid]);
		auto d_check = steady_clock::now() - t_check;

		// update the total number of frame errors if needed
		if (simu->analyzer[tid]->get_n_fe() > prev_n_fe)
		{
			simu->analyzer_red->increment_frame_errors(simu->analyzer[tid]->get_n_fe() - prev_n_fe);
			prev_n_fe = simu->analyzer[tid]->get_n_fe();
		}

		// increment total durations for each operations
		simu->d_sourc_total[tid] += d_sourc;
		simu->d_encod_total[tid] += d_encod;
		simu->d_modul_total[tid] += d_modul;
		simu->d_chann_total[tid] += d_chann;
		simu->d_quant_total[tid] += d_quant;
		simu->d_load_total [tid] += d_load;
		simu->d_decod_total[tid] += d_decod;
		simu->d_store_total[tid] += d_store;
		simu->d_check_total[tid] += d_check;

		// display statistics in terminal
		if (tid == 0 && !simu->simu_params.disable_display && simu->simu_params.display_freq != nanoseconds(0) &&
		    (steady_clock::now() - simu->t_simu) >= simu->simu_params.display_freq)
		{
			simu->analyzer_red->reduce();
			Simulation_RSC_mt<B,R,Q,QD>::time_reduction(simu);
			simu->terminal->temp_report(std::clog);
			simu->t_simu = steady_clock::now();
		}
	}
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_RSC_mt<B,R,Q,QD>
::time_reduction(Simulation_RSC_mt<B,R,Q,QD> *simu)
{
	simu->d_sourc_total_red = std::chrono::nanoseconds(0);
	simu->d_encod_total_red = std::chrono::nanoseconds(0);
	simu->d_modul_total_red = std::chrono::nanoseconds(0);
	simu->d_chann_total_red = std::chrono::nanoseconds(0);
	simu->d_quant_total_red = std::chrono::nanoseconds(0);
	simu->d_load_total_red  = std::chrono::nanoseconds(0);
	simu->d_decod_total_red = std::chrono::nanoseconds(0);
	simu->d_store_total_red = std::chrono::nanoseconds(0);
	simu->d_check_total_red = std::chrono::nanoseconds(0);

	for (auto cur_tid = 0; cur_tid < simu->n_threads; cur_tid++)
	{
		simu->d_sourc_total_red += simu->d_sourc_total[cur_tid];
		simu->d_encod_total_red += simu->d_encod_total[cur_tid];
		simu->d_modul_total_red += simu->d_modul_total[cur_tid];
		simu->d_chann_total_red += simu->d_chann_total[cur_tid];
		simu->d_quant_total_red += simu->d_quant_total[cur_tid];
		simu->d_load_total_red  += simu->d_load_total [cur_tid];
		simu->d_decod_total_red += simu->d_decod_total[cur_tid];
		simu->d_store_total_red += simu->d_store_total[cur_tid];
		simu->d_check_total_red += simu->d_check_total[cur_tid];
	}
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_RSC_mt<B_8,R_8,Q_8,QD_8>;
template class Simulation_RSC_mt<B_16,R_16,Q_16,QD_16>;
template class Simulation_RSC_mt<B_32,R_32,Q_32,QD_32>;
template class Simulation_RSC_mt<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_RSC_mt<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation