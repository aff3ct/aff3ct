#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "../../../Tools/Factory/Factory_frozenbits_generator.hpp"
#include "../../../Tools/Factory/Factory_source.hpp"
#include "../../../Tools/Factory/Factory_CRC.hpp"
#include "../../../Tools/Factory/Factory_encoder_polar.hpp"
#include "../../../Tools/Factory/Factory_modulator.hpp"
#include "../../../Tools/Factory/Factory_channel.hpp"
#include "../../../Tools/Factory/Factory_puncturer_polar.hpp"
#include "../../../Tools/Factory/Factory_quantizer.hpp"
#include "../../../Tools/Factory/Factory_decoder_polar.hpp"
#include "../../../Tools/Factory/Factory_decoder_polar_gen.hpp"
#include "../../../Tools/Factory/Factory_error_analyzer.hpp"
#include "../../../Tools/Factory/Factory_terminal.hpp"

#include "../../../Tools/params.h"
#include "../../../Tools/MIPP/mipp.h"
#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "../../../CRC/CRC_polynomial.hpp"

#include "Simulation_polar.hpp"

template <typename B, typename R, typename Q>
Simulation_polar<B,R,Q>
::Simulation_polar(const t_simulation_param& simu_params,
                   const t_code_param&       code_params,
                   const t_encoder_param&    enco_params,
                   const t_channel_param&    chan_params,
                   const t_decoder_param&    deco_params)
: Simulation(),
  N_2((int)std::exp2(code_params.m)),
  simu_params(simu_params),
  code_params(code_params),
  enco_params(enco_params),
  chan_params(chan_params),
  deco_params(deco_params),
  frozen_bits(N_2),
  U_K(code_params.K),
  U_N(N_2),
  X_N(N_2),
  Y_N1(N_2),
  Y_N2(N_2),
  V_N(N_2),
  code_rate(0.f),
  sigma(0.f),
  is_generated_decoder((deco_params.implem.find("_SNR") != std::string::npos) && (deco_params.algo == "SC")),
  fb_generator(nullptr),
  source(nullptr),
  crc(nullptr),
  encoder(nullptr),
  modulator(nullptr),
  channel(nullptr),
  puncturer(nullptr),
  quantizer(nullptr),
  decoder(nullptr),
  analyzer(nullptr),
  terminal(nullptr),
  n_frames(1)
{
	if (!is_generated_decoder)
	{
		// build the frozen bits generator
		fb_generator = Factory_frozenbits_generator<B>::build(simu_params, code_params);
		check_errors(fb_generator, "Frozenbits_generator<B>");
	}
}

template <typename B, typename R, typename Q>
Simulation_polar<B,R,Q>
::~Simulation_polar()
{
	if (fb_generator != nullptr) delete fb_generator;

	if (source    != nullptr) delete source;
	if (crc       != nullptr) delete crc;
	if (encoder   != nullptr) delete encoder;
	if (modulator != nullptr) delete modulator;
	if (channel   != nullptr) delete channel;
	if (puncturer != nullptr) delete puncturer;
	if (quantizer != nullptr) delete quantizer;
	if (decoder   != nullptr) delete decoder;
	if (analyzer  != nullptr) delete analyzer;
	if (terminal  != nullptr) delete terminal;
}

template <typename B, typename R, typename Q>
void Simulation_polar<B,R,Q>
::build_communication_chain(const R& snr)
{
	// release communication chain objects before allocation
	if (source    != nullptr) delete source;
	if (crc       != nullptr) delete crc;
	if (encoder   != nullptr) delete encoder;
	if (modulator != nullptr) delete modulator;
	if (channel   != nullptr) delete channel;
	if (puncturer != nullptr) delete puncturer;
	if (quantizer != nullptr) delete quantizer;
	if (decoder   != nullptr) delete decoder;
	if (analyzer  != nullptr) delete analyzer;
	if (terminal  != nullptr) delete terminal;

	if (!code_params.puncturer.empty())
	{
		// build the puncturer
		puncturer = Factory_puncturer_polar<B,Q>::build(code_params, deco_params, fb_generator);
		check_errors(puncturer, "Puncturer<B,Q>");
		puncturer->gen_frozen_bits(frozen_bits);
	}

	// build the CRC
	crc = Factory_CRC<B>::build(code_params, deco_params);
	check_errors(crc, "CRC<B>");

	// build the decoder
	if (is_generated_decoder)
		decoder = Factory_decoder_polar_gen<B,Q>::build(code_params, enco_params, chan_params, deco_params, frozen_bits);
	else
		decoder = Factory_decoder_polar<B,Q>::build(code_params, enco_params, chan_params, deco_params, frozen_bits, crc);

	check_errors(decoder, "Decoder<B,Q>");
	n_frames = decoder->get_n_frames();

	if (U_K .size() != (unsigned) (code_params.K * n_frames)) U_K .resize(code_params.K * n_frames);
	if (U_N .size() != (unsigned) (N_2 * n_frames)) U_N .resize(N_2 * n_frames);
	if (X_N .size() != (unsigned) (N_2 * n_frames)) X_N .resize(N_2 * n_frames);
	if (Y_N1.size() != (unsigned) (N_2 * n_frames)) Y_N1.resize(N_2 * n_frames);
	if (Y_N2.size() != (unsigned) (N_2 * n_frames)) Y_N2.resize(N_2 * n_frames);
	if (V_N .size() != (unsigned) (N_2 * n_frames)) V_N .resize(N_2 * n_frames);

	// build the source
	source = Factory_source<B>::build(code_params);
	check_errors(source, "Source<B>");

	// build the encoder
	encoder = Factory_encoder_polar<B>::build(code_params, enco_params, deco_params, frozen_bits, n_frames);
	check_errors(encoder, "Encoder_polar<B>");

	// build the modulator
	modulator = Factory_modulator<B,R>::build();
	check_errors(modulator, "Modulator<B,R>");

	// build the channel
	channel = Factory_channel<B,R>::build(chan_params, sigma, 0, 2.0 / (sigma * sigma));
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

template <typename B, typename R, typename Q>
void Simulation_polar<B,R,Q>
::launch()
{
	this->snr_loop();
}

template <typename B, typename R, typename Q>
void Simulation_polar<B,R,Q>
::snr_loop()
{
	// generated decoder
	if (is_generated_decoder)
		Factory_decoder_polar_gen<B,Q>::get_frozen_bits(code_params, deco_params, frozen_bits);
	else
		// fixed frozen bits generation
		if (!simu_params.awgn_codes_file.empty() || code_params.sigma != 0.f)
			fb_generator->generate(frozen_bits);

	// for each SNR to be simulated
	for (auto snr = simu_params.snr_min; snr <= simu_params.snr_max; snr += simu_params.snr_step)
	{
		t_snr = std::chrono::steady_clock::now();

		code_rate = (float)(code_params.K - CRC_polynomial<B>::size(code_params.crc)) / (float)code_params.N;
		sigma     = 1.f / sqrt(2.f * code_rate * pow(10.f, (snr / 10.f)));

		// adaptative frozen bits generation
		if (simu_params.awgn_codes_file.empty() && code_params.sigma == 0.f && !is_generated_decoder)
		{
			fb_generator->set_sigma(sigma);
			fb_generator->generate(frozen_bits);
		}

		// allocate and build all the communication chain for the simulation
		this->build_communication_chain(snr);

		if (code_params.generation_method == "AZCW")
		{
			std::fill(U_K.begin(), U_K.end(), (B)0);
			std::fill(U_N.begin(), U_N.end(), (B)0);
			std::fill(X_N.begin(), X_N.end(), (B)0);
			modulator->modulate(X_N, X_N);
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

template <typename B, typename R, typename Q>
void Simulation_polar<B,R,Q>
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
		auto d_punct = nanoseconds(0);

		if (code_params.generation_method != "AZCW")
		{
			// generate a random K bits vector U_K
			auto t_sourc = steady_clock::now();
			source->generate(U_K);
			d_sourc = steady_clock::now() - t_sourc;

			// add the CRC to U_K
			crc->build(U_K);

			// encode U_K into a N bits vector X_N
			auto t_encod = steady_clock::now();
			encoder->encode(U_K, U_N, X_N);
			d_encod = steady_clock::now() - t_encod;

			// modulate
			auto t_modul = steady_clock::now();
			modulator->modulate(X_N, X_N);
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
		
		if (!code_params.puncturer.empty())
		{
			// puncture
			auto t_punct = steady_clock::now();
			puncturer->puncture(Y_N2);
			d_punct = steady_clock::now() - t_punct;
		}

		// load data in the decoder
		auto t_load = steady_clock::now();
		decoder->load(Y_N2);
		auto d_load = steady_clock::now() - t_load;

		// launch decoder
		auto t_decod = steady_clock::now();
		decoder->decode();
		auto d_decod = steady_clock::now() - t_decod;

		// store results in V_N
		auto t_store = steady_clock::now();
		decoder->store(V_N);
		auto d_store = steady_clock::now() - t_store;

		// unpack V_N
		decoder->unpack(V_N);

		// check errors in the frame
		auto t_check = steady_clock::now();
		analyzer->check_errors(U_N, V_N);
		auto d_check = steady_clock::now() - t_check;

		// increment total durations for each operations
		d_sourc_total += d_sourc;
		d_encod_total += d_encod;
		d_modul_total += d_modul;
		d_chann_total += d_chann;
		d_quant_total += d_quant;
		d_punct_total += d_punct;
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
template class Simulation_polar<B_8,R_8,Q_8>;
template class Simulation_polar<B_16,R_16,Q_16>;
template class Simulation_polar<B_32,R_32,Q_32>;
template class Simulation_polar<B_64,R_64,Q_64>;
#else
template class Simulation_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation