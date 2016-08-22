#ifdef SYSTEMC

#include <thread>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "../../Tools/Factory/Factory_source.hpp"
#include "../../Tools/Factory/Factory_CRC.hpp"
#include "../../Tools/Factory/Factory_modulator.hpp"
#include "../../Tools/Factory/Factory_channel.hpp"
#include "../../Tools/Factory/Factory_quantizer.hpp"
#include "../../Tools/Factory/Factory_error_analyzer.hpp"
#include "../../Tools/Factory/Factory_terminal.hpp"

#include "../../Puncturer/Puncturer_NO.hpp"

#include "../../Tools/bash_tools.h"
#include "../../Tools/simu_tools.h"

#include "Simulation_SC_BFER.hpp"

template <typename B, typename R, typename Q>
Simulation_SC_BFER<B,R,Q>
::Simulation_SC_BFER(const t_simulation_param& simu_params,
                     const t_code_param&       code_params,
                     const t_encoder_param&    enco_params,
                     const t_mod_param&        mod_params,
                     const t_channel_param&    chan_params,
                     const t_decoder_param&    deco_params)
: Simulation(),
  
  simu_params(simu_params),
  code_params(code_params),
  enco_params(enco_params),
  mod_params (mod_params ),
  chan_params(chan_params),
  deco_params(deco_params),

  n_frames(1),
 
  snr      (0.f),
  code_rate(0.f),
  sigma    (0.f),

  source   (nullptr),
  crc      (nullptr),
  encoder  (nullptr),
  puncturer(nullptr),
  modulator(nullptr),
  channel  (nullptr),
  quantizer(nullptr),
  decoder  (nullptr),
  analyzer (nullptr),
  terminal (nullptr),

  duplicator(nullptr),
  dbg_B{nullptr, nullptr, nullptr, nullptr, nullptr},
  dbg_R{nullptr, nullptr, nullptr},
  dbg_Q{nullptr, nullptr},

  d_load_total_fake (std::chrono::nanoseconds(0)),
  d_decod_total_fake(std::chrono::nanoseconds(0)),
  d_store_total_fake(std::chrono::nanoseconds(0))
{
	if (simu_params.n_threads > 1)
	{
		std::cerr << bold_red("(EE) SystemC BFER simulation does not support the multi-threaded mode... Exiting.") 
		          << std::endl;
		std::exit(-1);
	}

	if (simu_params.benchs)
	{
		std::cerr << bold_red("(EE) SystemC BFER simulation does not support the bench mode... Exiting") << std::endl;
		std::exit(-1);
	}

	if (simu_params.time_report)
		std::cerr << bold_yellow("(WW) The time report is not available in the SystemC BFER simulation.") << std::endl;
}

template <typename B, typename R, typename Q>
Simulation_SC_BFER<B,R,Q>
::~Simulation_SC_BFER()
{
	release_objects();
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::launch()
{
	launch_precompute();
	
	// for each SNR to be simulated
	for (snr = simu_params.snr_min; snr <= simu_params.snr_max; snr += simu_params.snr_step)
	{
		t_snr = std::chrono::steady_clock::now();

		code_rate = (float)(code_params.K / (float)(code_params.N + code_params.tail_length));
		sigma     = 1.f / sqrt(2.f * code_rate * (float)mod_params.bits_per_symbol * pow(10.f, (snr / 10.f)));

		snr_precompute();

		// launch the simulation
		this->launch_simulation();

		if (!simu_params.disable_display && !simu_params.benchs)
			terminal->final_report(std::cout);

		// release communication objects
		release_objects();

		// exit simulation (double [ctrl+c])
		if (Error_analyzer<B>::is_over())
			break;
	}
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::launch_simulation()
{
	this->build_communication_chain();

	if ((!this->simu_params.disable_display && this->snr == this->simu_params.snr_min && 
	    !(this->simu_params.enable_debug && this->simu_params.n_threads == 1) && !this->simu_params.benchs))
		this->terminal->legend(std::cout);

	duplicator = new SC_Duplicator<B>("Duplicator");

	if (this->simu_params.n_threads == 1 && this->simu_params.enable_debug)
	{
		dbg_B[0] = new SC_Debug<B>("Generate random bits U_K...               \nU_K: \n", "Debug_B0");
		dbg_B[1] = new SC_Debug<B>("Add the CRC to U_K...                     \nU_K: \n", "Debug_B1");
		dbg_B[2] = new SC_Debug<B>("Encode U_K in X_N1...                     \nX_N1:\n", "Debug_B2");
		dbg_B[3] = new SC_Debug<B>("Puncture X_N1 in X_N2...                  \nX_N2:\n", "Debug_B3");
		dbg_B[4] = new SC_Debug<B>("Decode Y_N2 and generate V_K...           \nY_N5:\n", "Debug_B4");
		dbg_R[0] = new SC_Debug<R>("Modulate X_N2 in X_N3...                  \nX_N3:\n", "Debug_R0");
		dbg_R[1] = new SC_Debug<R>("Add noise from X_N3 to Y_N1...            \nY_N1:\n", "Debug_R1");
		dbg_R[2] = new SC_Debug<R>("Demodulate from Y_N1 to Y_N2...           \nY_N2:\n", "Debug_R2");
		dbg_Q[0] = new SC_Debug<Q>("Make the quantization from Y_N2 to Y_N3...\nY_N3:\n", "Debug_Q0");
		dbg_Q[1] = new SC_Debug<Q>("Depuncture Y_N3 and generate Y_N4...      \nY_N4:\n", "Debug_Q1");

		this->bind_sockets_debug();
		sc_core::sc_start(); // start simulation
		this->terminal->legend(std::cout);

		for (auto i = 0; i < 5; i++) { delete dbg_B[i]; dbg_B[i] = nullptr; }
		for (auto i = 0; i < 3; i++) { delete dbg_R[i]; dbg_R[i] = nullptr; }
		for (auto i = 0; i < 2; i++) { delete dbg_Q[i]; dbg_Q[i] = nullptr; }
	}
	else
	{
		// launch a thread dedicated to the terminal display
		std::thread thread(Simulation_SC_BFER<B,R,Q>::terminal_temp_report, this);

		this->bind_sockets();
		sc_core::sc_report_handler::set_actions(SC_INFO, SC_DO_NOTHING);
		sc_core::sc_start(); // start simulation

		// wait the terminal thread to finish
		thread.join();
	}

	delete duplicator; duplicator = nullptr;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// /!\ VERY DIRTY WAY TO CREATE A NEW SIMULATION CONTEXT IN SYSTEMC, BE CAREFUL THIS IS NOT IN THE STANDARD! /!\ //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	sc_core::sc_curr_simcontext = new sc_simcontext();
	sc_core::sc_default_global_context = sc_curr_simcontext;
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::build_communication_chain()
{
	// build the objects
	this->source    = this->build_source   (); check_errors(this->source   , "Source<B>"          );
	this->crc       = this->build_crc      (); check_errors(this->crc      , "CRC<B>"             );
	this->encoder   = this->build_encoder  (); check_errors(this->encoder  , "Encoder<B>"         );
	this->puncturer = this->build_puncturer(); check_errors(this->puncturer, "Puncturer<B,Q>"     );
	this->modulator = this->build_modulator(); check_errors(this->modulator, "Modulator<B,R>"     );
	this->channel   = this->build_channel  (); check_errors(this->channel  , "Channel<R>"         );
	this->quantizer = this->build_quantizer(); check_errors(this->quantizer, "Quantizer<R,Q>"     );
	this->decoder   = this->build_decoder  (); check_errors(this->decoder  , "Decoder<B,Q>"       );
	this->analyzer  = this->build_analyzer (); check_errors(this->analyzer , "Error_analyzer<B,R>");

	// registering of the sockets in the modules (allocate the buffers)
	this->source   ->create_sc_sockets();
	this->crc      ->create_sc_sockets();
	this->encoder  ->create_sc_sockets();
	this->puncturer->create_sc_sockets();
	this->modulator->create_sc_sockets();
	this->channel  ->create_sc_sockets();
	this->quantizer->create_sc_sockets();
	this->decoder  ->create_sc_sockets();
	this->analyzer ->create_sc_sockets();

	// get the real number of frames per threads (from the decoder)
	this->n_frames = this->decoder->get_n_frames();

	// set the real number of frames per thread
	this->source   ->set_n_frames(this->n_frames);
	this->crc      ->set_n_frames(this->n_frames);
	this->encoder  ->set_n_frames(this->n_frames);
	this->puncturer->set_n_frames(this->n_frames);
	this->modulator->set_n_frames(this->n_frames);
	this->channel  ->set_n_frames(this->n_frames);
	this->quantizer->set_n_frames(this->n_frames);
	this->analyzer ->set_n_frames(this->n_frames);

	// build the terminal to display the BER/FER
	this->terminal = this->build_terminal();
	check_errors(this->terminal, "Terminal");
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::bind_sockets()
{
	this->source    ->sockets->out        (this->duplicator->socket_in          );
	this->duplicator->socket_out1         (this->analyzer  ->sockets->in_source );
	this->duplicator->socket_out2         (this->crc       ->sockets->in        );
	this->crc       ->sockets->out        (this->encoder   ->sockets->in        );
	this->encoder   ->sockets->out        (this->puncturer ->sockets->in_punct  );
	this->puncturer ->sockets->out_punct  (this->modulator ->sockets->in_mod    );
	this->modulator ->sockets->out_mod    (this->channel   ->sockets->in        );
	this->channel   ->sockets->out        (this->modulator ->sockets->in_demod  );
	this->modulator ->sockets->out_demod  (this->quantizer ->sockets->in        );
	this->quantizer ->sockets->out        (this->puncturer ->sockets->in_depunct);
	this->puncturer ->sockets->out_depunct(this->decoder   ->sockets->in        );
	this->decoder   ->sockets->out        (this->analyzer  ->sockets->in_decoder);
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::bind_sockets_debug()
{
	this->source    ->sockets->out        (this->dbg_B[0]->socket_in); this->dbg_B[0]->socket_out (duplicator     ->socket_in          );
	this->duplicator                                                                 ->socket_out1(this->analyzer ->sockets->in_source );
	this->duplicator                                                                 ->socket_out2(this->crc      ->sockets->in        );
	this->crc       ->sockets->out        (this->dbg_B[1]->socket_in); this->dbg_B[1]->socket_out (this->encoder  ->sockets->in        );
	this->encoder   ->sockets->out        (this->dbg_B[2]->socket_in); this->dbg_B[2]->socket_out (this->puncturer->sockets->in_punct  );
	this->puncturer ->sockets->out_punct  (this->dbg_B[3]->socket_in); this->dbg_B[3]->socket_out (this->modulator->sockets->in_mod    );
	this->modulator ->sockets->out_mod    (this->dbg_R[0]->socket_in); this->dbg_R[0]->socket_out (this->channel  ->sockets->in        );
	this->channel   ->sockets->out        (this->dbg_R[1]->socket_in); this->dbg_R[1]->socket_out (this->modulator->sockets->in_demod  );
	this->modulator ->sockets->out_demod  (this->dbg_R[2]->socket_in); this->dbg_R[2]->socket_out (this->quantizer->sockets->in        );
	this->quantizer ->sockets->out        (this->dbg_Q[0]->socket_in); this->dbg_Q[0]->socket_out (this->puncturer->sockets->in_depunct);
	this->puncturer ->sockets->out_depunct(this->dbg_Q[1]->socket_in); this->dbg_Q[1]->socket_out (this->decoder  ->sockets->in        );
	this->decoder   ->sockets->out        (this->dbg_B[4]->socket_in); this->dbg_B[4]->socket_out (this->analyzer ->sockets->in_decoder);
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::terminal_temp_report(Simulation_SC_BFER<B,R,Q> *simu)
{
	using namespace std::chrono;
	simu->t_simu = steady_clock::now();

	if (!simu->simu_params.disable_display && simu->simu_params.display_freq != nanoseconds(0))
	{
		while (!simu->analyzer->fe_limit_achieved() && !simu->analyzer->is_interrupt())
		{
			const auto sleep_time = simu->simu_params.display_freq - std::chrono::milliseconds(0);
			std::this_thread::sleep_for(sleep_time);

			// display statistics in terminal
			simu->terminal->temp_report(std::clog);
			simu->t_simu = steady_clock::now();
		}
	}
}

// ---------------------------------------------------------------------------------------------------- virtual methods

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::release_objects()
{
	if (source    != nullptr) { delete source;    source    = nullptr; }
	if (crc       != nullptr) { delete crc;       crc       = nullptr; }
	if (encoder   != nullptr) { delete encoder;   encoder   = nullptr; }
	if (puncturer != nullptr) { delete puncturer; puncturer = nullptr; }
	if (modulator != nullptr) { delete modulator; modulator = nullptr; }
	if (channel   != nullptr) { delete channel;   channel   = nullptr; }
	if (quantizer != nullptr) { delete quantizer; quantizer = nullptr; }
	if (decoder   != nullptr) { delete decoder;   decoder   = nullptr; }
	if (analyzer  != nullptr) { delete analyzer;  analyzer  = nullptr; }
	if (terminal  != nullptr) { delete terminal;  terminal  = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Source<B>* Simulation_SC_BFER<B,R,Q>
::build_source()
{
	return Factory_source<B>::build(code_params);
}

template <typename B, typename R, typename Q>
Puncturer<B,Q>* Simulation_SC_BFER<B,R,Q>
::build_puncturer()
{
	auto puncturer = new Puncturer_NO<B,Q>(code_params.K, code_params.N + code_params.tail_length);
	check_errors(puncturer, "Puncturer<B,Q>");
	return puncturer;
}

template <typename B, typename R, typename Q>
CRC<B>* Simulation_SC_BFER<B,R,Q>
::build_crc()
{
	return Factory_CRC<B>::build(code_params, deco_params);
}

template <typename B, typename R, typename Q>
Modulator<B,R>* Simulation_SC_BFER<B,R,Q>
::build_modulator()
{
	return Factory_modulator<B,R>::build(code_params, mod_params, sigma);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_SC_BFER<B,R,Q>
::build_channel()
{
	return Factory_channel<R>::build(code_params, chan_params, sigma);
}

template <typename B, typename R, typename Q>
Quantizer<R,Q>* Simulation_SC_BFER<B,R,Q>
::build_quantizer()
{
	return Factory_quantizer<R,Q>::build(code_params, chan_params, sigma);
}

template <typename B, typename R, typename Q>
Error_analyzer<B>* Simulation_SC_BFER<B,R,Q>
::build_analyzer()
{
	return Factory_error_analyzer<B>::build(simu_params, code_params, n_frames);
}

// ------------------------------------------------------------------------------------------------- non-virtual method

template <typename B, typename R, typename Q>
Terminal* Simulation_SC_BFER<B,R,Q>
::build_terminal()
{
	return Factory_terminal<B,R>::build(simu_params, snr, analyzer, t_snr,
	                                    d_load_total_fake, d_decod_total_fake, d_store_total_fake);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_SC_BFER<B_8,R_8,Q_8>;
template class Simulation_SC_BFER<B_16,R_16,Q_16>;
template class Simulation_SC_BFER<B_32,R_32,Q_32>;
template class Simulation_SC_BFER<B_64,R_64,Q_64>;
#else
template class Simulation_SC_BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif