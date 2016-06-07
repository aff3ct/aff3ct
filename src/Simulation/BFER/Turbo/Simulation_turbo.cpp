#include <map>
#include <chrono>
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <algorithm>

#include "../../../Tools/Factory/Factory_interleaver.hpp"
#include "../../../Tools/Factory/Factory_source.hpp"
#include "../../../Tools/Factory/Factory_CRC.hpp"
#include "../../../Tools/Factory/Factory_encoder_RSC.hpp"
#include "../../../Tools/Factory/Factory_encoder_turbo.hpp"
#include "../../../Tools/Factory/Factory_modulator.hpp"
#include "../../../Tools/Factory/Factory_channel.hpp"
#include "../../../Tools/Factory/Factory_quantizer.hpp"
#include "../../../Tools/Factory/Factory_scaling_factor.hpp"
#include "../../../Tools/Factory/Factory_decoder_RSC.hpp"
#include "../../../Tools/Factory/Factory_decoder_turbo.hpp"
#include "../../../Tools/Factory/Factory_error_analyzer.hpp"
#include "../../../Tools/Factory/Factory_terminal.hpp"

#include "../../../Tools/params.h"
#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "Simulation_turbo.hpp"

template <typename B, typename R, typename Q, typename QD>
Simulation_turbo<B,R,Q,QD>
::Simulation_turbo(const t_simulation_param& simu_params,
                   const t_code_param&       code_params,
                   const t_encoder_param&    enco_params,
                   const t_channel_param&    chan_params,
                   const t_decoder_param&    deco_params)
: Simulation(),
  simu_params(simu_params),
  code_params(code_params),
  enco_params(enco_params),
  chan_params(chan_params),
  deco_params(deco_params),
  U_K(code_params.K),
  X_N (code_params.N + code_params.tail_length),
  Y_N1(code_params.N + code_params.tail_length),
  Y_N2(code_params.N + code_params.tail_length),
  V_K(code_params.K),
  code_rate(0.f),
  sigma(0.f),
  interleaver(nullptr),
  source(nullptr),
  crc(nullptr),
  encoder(nullptr),
  sub_encoder(nullptr),
  modulator(nullptr),
  channel(nullptr),
  quantizer(nullptr),
  sf(nullptr),
  decoder(nullptr),
  siso(nullptr),
  analyzer(nullptr),
  terminal(nullptr),
  n_frames(1)
{
	assert(code_params.N / code_params.K == 3);

	// build the interleaver for the encoder and the decoder
	interleaver = Factory_interleaver<short>::build(code_params, code_params.K);
	check_errors(interleaver, "Interleaver<short>");
}

template <typename B, typename R, typename Q, typename QD>
Simulation_turbo<B,R,Q,QD>
::~Simulation_turbo()
{
	if (interleaver != nullptr) delete interleaver;
	if (source      != nullptr) delete source;
	if (crc         != nullptr) delete crc;
	if (encoder     != nullptr) delete encoder;
	if (sub_encoder != nullptr) delete sub_encoder;
	if (modulator   != nullptr) delete modulator;
	if (channel     != nullptr) delete channel;
	if (quantizer   != nullptr) delete quantizer;
	if (sf          != nullptr) delete sf;
	if (decoder     != nullptr) delete decoder;
	if (siso        != nullptr) delete siso;
	if (analyzer    != nullptr) delete analyzer;
	if (terminal    != nullptr) delete terminal;
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo<B,R,Q,QD>
::build_communication_chain(const R& snr)
{
	// release communication chain objects before allocation
	if (source      != nullptr) delete source;
	if (crc         != nullptr) delete crc;
	if (encoder     != nullptr) delete encoder;
	if (sub_encoder != nullptr) delete sub_encoder;
	if (modulator   != nullptr) delete modulator;
	if (channel     != nullptr) delete channel;
	if (quantizer   != nullptr) delete quantizer;
	if (sf          != nullptr) delete sf;
	if (decoder     != nullptr) delete decoder;
	if (siso        != nullptr) delete siso;
	if (analyzer    != nullptr) delete analyzer;
	if (terminal    != nullptr) delete terminal;

	// build the CRC
	crc = Factory_CRC<B>::build(code_params, deco_params);
	check_errors(crc, "CRC<B>");

	// build the siso for the decoder
	siso = Factory_decoder_RSC<B,Q,QD>::build_siso(code_params, enco_params, chan_params, deco_params);
	check_errors(siso, "SISO<Q>");
	n_frames = siso->get_n_frames();

	// build the sub_encoder
	sub_encoder = Factory_encoder_RSC<B>::build(simu_params, code_params, enco_params, n_frames);
	check_errors(sub_encoder, "Encoder_sys<B>");

	// build the encoder
	encoder = Factory_encoder_turbo<B>::build(code_params, enco_params, interleaver, sub_encoder, sub_encoder, 
	                                          n_frames);
	check_errors(encoder, "Encoder<B>");

	// build the scaling factor for the decoder
	sf = Factory_scaling_factor<Q>::build(code_params, deco_params);
	check_errors(sf, "Scaling_factor<Q>");

	// build the decoder
	decoder = Factory_decoder_turbo<B,Q>::build(code_params, enco_params, chan_params, deco_params, interleaver, 
	                                            siso, siso, sf, crc);
	check_errors(decoder, "Decoder<B,Q>");
	
	interleaver->set_n_frames(n_frames);

	// resize the buffers if needed
	if (U_K .size() != (unsigned) (code_params.K * n_frames)) U_K .resize(code_params.K * n_frames);
	if (X_N .size() != (unsigned) (code_params.N * n_frames)) X_N .resize((code_params.N + code_params.tail_length) * n_frames);
	if (Y_N1.size() != (unsigned) (code_params.N * n_frames)) Y_N1.resize((code_params.N + code_params.tail_length) * n_frames);
	if (Y_N2.size() != (unsigned) (code_params.N * n_frames)) Y_N2.resize((code_params.N + code_params.tail_length) * n_frames);
	if (V_K .size() != (unsigned) (code_params.K * n_frames)) V_K .resize(code_params.K * n_frames);

	// build the source
	source = Factory_source<B>::build(code_params);
	check_errors(source, "Source<B>");

	// build the modulator
	modulator = Factory_modulator<B>::build();
	check_errors(modulator, "Modulator<B>");

	// build the channel
	//channel = Factory_channel<B,R>::build(chan_params, sigma, 0, 1.0 / (2.0 * sigma * sigma));
	channel = Factory_channel<B,R>::build(chan_params, sigma);
	check_errors(channel, "Channel<B,R>");

	// build the quantizer
	quantizer = Factory_quantizer<R,Q>::build(chan_params, sigma);
	check_errors(quantizer, "Quantizer<R,Q>");

	// build the analyser
	analyzer = Factory_error_analyzer<B,R>::build(simu_params, code_params, n_frames);
	check_errors(analyzer, "Error_analyzer<B,R>");

	// build the terminal to display the BER/FER
	terminal = Factory_terminal<B,R>::build(simu_params, snr, analyzer, 
	                                        t_snr, d_load_total, d_decod_total, d_store_total);
	check_errors(terminal, "Terminal");
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo<B,R,Q,QD>
::launch()
{
	this->snr_loop();
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo<B,R,Q,QD>
::snr_loop()
{
	// for each SNR to be simulated
	for (auto snr = simu_params.snr_min; snr <= simu_params.snr_max; snr += simu_params.snr_step)
	{
		t_snr = std::chrono::steady_clock::now();

		code_rate = (float)code_params.K / (float)(code_params.N + code_params.tail_length);
		sigma     = 1.f / sqrt(2.f * code_rate * pow(10.f, (snr / 10.f)));

		// allocate and build all the communication chain for simulate: encoding -> add noise -> decoding
		this->build_communication_chain(snr);

		if (code_params.generation_method == "AZCW")
		{
			std::fill(U_K.begin(), U_K.end(), (B)0);
			std::fill(X_N.begin(), X_N.end(), (B)0);
			modulator->modulate(X_N);
		}

		d_sourc_total = std::chrono::nanoseconds(0);
		d_encod_total = std::chrono::nanoseconds(0);
		d_modul_total = std::chrono::nanoseconds(0);
		d_chann_total = std::chrono::nanoseconds(0);
		d_quant_total = std::chrono::nanoseconds(0);
		d_load_total  = std::chrono::nanoseconds(0);
		d_decod_total = std::chrono::nanoseconds(0);
		d_store_total = std::chrono::nanoseconds(0);
		d_check_total = std::chrono::nanoseconds(0);

		if (!simu_params.disable_display && snr == simu_params.snr_min && !simu_params.enable_debug)
			terminal->legend(std::clog);

		this->simulation_loop();

		if (!simu_params.disable_display)
			terminal->final_report(std::cout);
	}
	// end snr loop
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_turbo<B,R,Q,QD>
::simulation_loop()
{
	using namespace std::chrono;

	// simulation loop
	auto t_simu = steady_clock::now();
	while (!analyzer->fe_limit_achieved() && // while max frame error count has not been reached
	       (simu_params.stop_time == seconds(0) || (steady_clock::now() - t_snr) < simu_params.stop_time))
	{
		auto d_sourc = nanoseconds(0);
		auto d_encod = nanoseconds(0);
		auto d_modul = nanoseconds(0);

		if (code_params.generation_method != "AZCW")
		{
			// generate a random K bits vector U_K
			auto t_sourc = steady_clock::now();
			source->generate(U_K);
			d_sourc = steady_clock::now() - t_sourc;

			// add the CRC to U_K
			crc->build(U_K);

			// encode U_N into a N bits vector X_N
			auto t_encod = steady_clock::now();
			encoder->encode(U_K, X_N);
			d_encod = steady_clock::now() - t_encod;

			// modulate
			auto t_modul = steady_clock::now();
			modulator->modulate(X_N);
			d_modul = steady_clock::now() - t_modul;
		}

		// add noise
		auto t_chann = steady_clock::now();
		channel->add_noise(X_N, Y_N1);
		auto d_chann = steady_clock::now() - t_chann;

		// make the quantization
		auto t_quant = steady_clock::now();
		quantizer->process(Y_N1, Y_N2);
		auto d_quant = steady_clock::now() - t_quant;

		// load data in the decoder
		auto t_load = steady_clock::now();
		decoder->load(Y_N2);
		auto d_load = steady_clock::now() - t_load;

		// launch decoder
		auto t_decod = steady_clock::now();
		decoder->decode();
		auto d_decod = steady_clock::now() - t_decod;

		// store results in V_K
		auto t_store = steady_clock::now();
		decoder->store(V_K);
		auto d_store = steady_clock::now() - t_store;

		// unpack V_K if we used bit packing in the decoder (do nothing else)
		decoder->unpack(V_K);

		// check errors in the frame
		auto t_check = steady_clock::now();
		analyzer->check_errors(U_K, V_K);
		auto d_check = steady_clock::now() - t_check;

		// increment total durations for each operations
		d_sourc_total += d_sourc;
		d_encod_total += d_encod;
		d_modul_total += d_modul;
		d_chann_total += d_chann;
		d_quant_total += d_quant;
		d_load_total  += d_load;
		d_decod_total += d_decod;
		d_store_total += d_store;
		d_check_total += d_check;

		// display statistics in terminal
		if (!simu_params.disable_display && simu_params.display_freq != nanoseconds(0) && 
		    (steady_clock::now() - t_simu) >= simu_params.display_freq)
		{
			terminal->temp_report(std::clog);
			t_simu = steady_clock::now();
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_turbo<B_8,R_8,Q_8,QD_8>;
template class Simulation_turbo<B_16,R_16,Q_16,QD_16>;
template class Simulation_turbo<B_32,R_32,Q_32,QD_32>;
template class Simulation_turbo<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_turbo<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation