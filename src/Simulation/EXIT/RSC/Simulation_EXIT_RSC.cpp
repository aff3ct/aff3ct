#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>

#include "../../../Tools/Factory/Factory_source.hpp"
#include "../../../Tools/Factory/Factory_encoder_RSC.hpp"
#include "../../../Tools/Factory/Factory_modulator.hpp"
#include "../../../Tools/Factory/Factory_channel.hpp"
#include "../../../Tools/Factory/Factory_quantizer.hpp"
#include "../../../Tools/Factory/Factory_decoder_RSC.hpp"

#include "../../../Decoder/decoder_functions.h"

#include "../../../Tools/params.h"
#include "../../../Tools/bash_tools.h"
#include "../../../Tools/simu_tools.h"

#include "Simulation_EXIT_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Simulation_EXIT_RSC<B,R,Q,QD>
::Simulation_EXIT_RSC(const t_simulation_param& simu_params,
                      const t_code_param&       code_params,
                      const t_encoder_param&    enco_params,
                      const t_channel_param&    chan_params,
                      const t_decoder_param&    deco_params)
: Simulation_EXIT<B,Q>(),

  N(code_params.N),
  K(code_params.K),

  simu_params(simu_params),
  code_params(code_params),
  enco_params(enco_params),
  chan_params(chan_params),
  deco_params(deco_params),

  B_K   (code_params.K),
  B_N   (code_params.N + code_params.tail_length),
  X_K   (code_params.K),
  X_N   (code_params.N + code_params.tail_length),
  Y_N   (code_params.N + code_params.tail_length),
  Y_K   (code_params.K),
  La_K1 (code_params.K),
  Lch_N1(code_params.N + code_params.tail_length),
  La_K2 (code_params.K),
  Lch_N2(code_params.N + code_params.tail_length),
  Le_K  (code_params.K),
  sys   (code_params.K + code_params.tail_length/2),
  par   (code_params.K + code_params.tail_length/2),

  B_buff (0),
  Le_buff(0),
  La_buff(0),

  n_trials (200000 / code_params.K),
  cur_trial(0),

  I_A      (0.0),
  I_E      (0.0),
  code_rate(0.f),
  sigma    (0.f),

  source    (nullptr),
  encoder   (nullptr),
  modulator (nullptr),
  channel   (nullptr),
  channel_a (nullptr),
  quantizer (nullptr),
  siso      (nullptr),
  terminal  (nullptr)
{
}

template <typename B, typename R, typename Q, typename QD>
Simulation_EXIT_RSC<B,R,Q,QD>
::~Simulation_EXIT_RSC()
{
	if (source    != nullptr) delete source;
	if (encoder   != nullptr) delete encoder;
	if (modulator != nullptr) delete modulator;
	if (channel   != nullptr) delete channel;
	if (channel_a != nullptr) delete channel_a;
	if (quantizer != nullptr) delete quantizer;
	if (siso      != nullptr) delete siso;
	if (terminal  != nullptr) delete terminal;
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_EXIT_RSC<B,R,Q,QD>
::build_communication_chain(const R& snr)
{
	// release communication chain objects before allocation
	if (source    != nullptr) delete source;
	if (encoder   != nullptr) delete encoder;
	if (modulator != nullptr) delete modulator;
	if (channel   != nullptr) delete channel;
	if (channel_a != nullptr) delete channel_a;
	if (quantizer != nullptr) delete quantizer;
	if (siso      != nullptr) delete siso;
	if (terminal  != nullptr) delete terminal;

	// build the encoder
	encoder = Factory_encoder_RSC<B>::build(simu_params, code_params, enco_params, deco_params);
	check_errors(encoder, "Encoder_RSC<B>");

	//build the decoder
	trellis = encoder->get_trellis();
	siso = Factory_decoder_RSC<B,Q,QD>::build_siso(code_params, enco_params, chan_params, deco_params, trellis);
	check_errors(siso, "SISO<R>");

	if (siso->get_n_frames() > 1)
	{
		std::cout << bold_red("(EE) EXIT_RSC does not support inter frame SIMD... Exiting.") << std::endl;
		exit(-1);
	}

	// build the generator
	source = Factory_source<B>::build(code_params);
	check_errors(source, "Source<B>");

	// build the modulator
	modulator = Factory_modulator<B>::build();
	check_errors(modulator, "Modulator<B>");

	// build the channel
	channel = Factory_channel<B,R>::build(chan_params, sigma, 0, 2.0 / (sigma * sigma));
	check_errors(channel, "Channel<B,R>");

	// build the channel "a"
	channel_a = Factory_channel<B,R>::build(chan_params, 2.0 / sig_a, 0, (sig_a * sig_a) / 2.0);
	check_errors(channel, "Channel_a<B,R>");

	// build the quantizer
	quantizer = Factory_quantizer<R,Q>::build(chan_params, sigma);
	check_errors(quantizer, "Quantizer<R,Q>");

	// build the terminal
	terminal = new Terminal_EXIT<B,R>(code_params.N, snr, sig_a, t_snr, cur_trial, n_trials, I_A, I_E);
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_EXIT_RSC<B,R,Q,QD>
::launch()
{
	this->snr_loop();
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_EXIT_RSC<B,R,Q,QD>
::snr_loop()
{	
	bool first_loop = true;

	// for each channel SNR to be simulated	
	for (auto cur_snr = simu_params.snr_min; cur_snr <= simu_params.snr_max; cur_snr += simu_params.snr_step)
	{
		// For EXIT simulation, SNR is considered as Es/N0
		code_rate = 1.f;
		sigma     = 1.f / sqrt(2.f * code_rate * pow(10.f, (cur_snr / 10.f)));

		// for each "a" standard deviation (sig_a) to be simulated
		for (sig_a = simu_params.sig_a_min; sig_a <= simu_params.sig_a_max; sig_a += simu_params.sig_a_step)
		{
			I_A = 0.0;
			I_E = 0.0;

			t_snr = std::chrono::steady_clock::now();

			// allocate and build all the communication chain to generate EXIT chart
			this->build_communication_chain(cur_snr);

			// if sig_a = 0, La_K2 = 0
			if (sig_a == 0)
				std::fill(La_K2.begin(), La_K2.end(), chan_params.domain == "LLR" ? init_LLR<R>() : init_LR<R>());

			if (!simu_params.disable_display && first_loop && !simu_params.enable_debug)
			{
				terminal->legend(std::clog);
				first_loop = false;
			}

			this->simulation_loop();

			if (!simu_params.disable_display)
				terminal->final_report(std::cout);
		}
	}
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_EXIT_RSC<B,R,Q,QD>
::simulation_loop()
{
	using namespace std::chrono;

	// simulation loop
	auto t_simu = steady_clock::now();

	Le_buff.clear();
	B_buff .clear();
	La_buff.clear();
	
	for (cur_trial = 0; cur_trial < n_trials; cur_trial++) 
	{
		// generate a random binary value
		source->generate(B_K);

		// encode
		encoder->encode(B_K, X_N);

		// X_K used to generate La_K vector
		X_K = B_K;

		// modulate
		modulator->modulate(X_K);
		modulator->modulate(X_N);

		//if sig_a = 0, La_K = 0, no noise to add
		if (sig_a != 0)
		{
			channel_a->add_noise(X_K, La_K1);
			quantizer->process(La_K1, La_K2);
		}

		channel->add_noise(X_N, Lch_N1);
		quantizer->process(Lch_N1, Lch_N2);

		// extract systematic and parity information
		for (auto i = 0; i < K + code_params.tail_length/2; i++)
		{
			sys[i] = Lch_N2[i*2 +0];
			par[i] = Lch_N2[i*2 +1];
		}

		// add other siso's extrinsic
		for(auto i = 0 ; i < K ; i ++)
			sys[i] += La_K2[i];

		// decode
		siso->decode(sys, par, Le_K);

		// store B_K, La_K and Le_K in buffers 
		this->store_results();

		// display statistics in terminal
		if (!simu_params.disable_display && (steady_clock::now() - t_simu) >= simu_params.display_freq)
		{
			terminal->temp_report(std::clog);
			t_simu = steady_clock::now();
		}
	}

	// measure mutual information and store it in I_A, I_E, sig_a_array
	this->analyze_results();
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_EXIT_RSC<B,R,Q,QD>
::store_results()
{
	// add current B and L to the buffers
	B_buff .insert(B_buff .end(), B_K  .begin(), B_K  .end());
	Le_buff.insert(Le_buff.end(), Le_K .begin(), Le_K .end());
	La_buff.insert(La_buff.end(), La_K2.begin(), La_K2.end());
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_EXIT_RSC<B,R,Q,QD>
::analyze_results()
{
	I_A = Simulation_EXIT<B,Q>::measure_mutual_info_avg  (La_buff, B_buff) / (K * n_trials);
	I_E = Simulation_EXIT<B,Q>::measure_mutual_info_histo(Le_buff, B_buff, 1000);
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_EXIT_RSC<B_8,R_8,Q_8,QD_8>;
template class Simulation_EXIT_RSC<B_16,R_16,Q_16,QD_16>;
template class Simulation_EXIT_RSC<B_32,R_32,Q_32,QD_32>;
template class Simulation_EXIT_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_EXIT_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation