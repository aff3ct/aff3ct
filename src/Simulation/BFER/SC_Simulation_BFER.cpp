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

#include "SC_Simulation_BFER.hpp"

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::Simulation_BFER(const t_simulation_param& simu_params,
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

  barrier(simu_params.n_threads),
  n_frames(1),

  snr      (0.f),
  code_rate(0.f),
  sigma    (0.f),

  source   (1, nullptr),
  crc      (1, nullptr),
  encoder  (1, nullptr),
  puncturer(1, nullptr),
  modulator(1, nullptr),
  channel  (1, nullptr),
  quantizer(1, nullptr),
  decoder  (1, nullptr),
  analyzer (1, nullptr),
  terminal (   nullptr),

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
Simulation_BFER<B,R,Q>
::~Simulation_BFER()
{
	release_objects();
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::launch()
{
	launch_precompute();

	// for each SNR to be simulated
	for (snr = simu_params.snr_min; snr <= simu_params.snr_max; snr += simu_params.snr_step)
	{
		t_snr = std::chrono::steady_clock::now();

		code_rate = (float)(code_params.K / (float)(code_params.N + code_params.tail_length));
		sigma     = std::sqrt((float)mod_params.upsample_factor) / 
		            std::sqrt(2.f * code_rate * (float)mod_params.bits_per_symbol * std::pow(10.f, (snr / 10.f)));

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
void Simulation_BFER<B,R,Q>
::launch_simulation()
{
	this->build_communication_chain();

	if ((!this->simu_params.disable_display && this->snr == this->simu_params.snr_min && 
	    !(this->simu_params.enable_debug && this->simu_params.n_threads == 1) && !this->simu_params.benchs))
		this->terminal->legend(std::cout);

	this->duplicator = new SC_Duplicator<B>("Duplicator");

	if (this->simu_params.n_threads == 1 && this->simu_params.enable_debug)
	{
		this->dbg_B[0] = new SC_Debug<B>("Generate random bits U_K...               \nU_K: \n", "Debug_B0");
		this->dbg_B[1] = new SC_Debug<B>("Add the CRC to U_K...                     \nU_K: \n", "Debug_B1");
		this->dbg_B[2] = new SC_Debug<B>("Encode U_K in X_N1...                     \nX_N1:\n", "Debug_B2");
		this->dbg_B[3] = new SC_Debug<B>("Puncture X_N1 in X_N2...                  \nX_N2:\n", "Debug_B3");
		this->dbg_R[0] = new SC_Debug<R>("Modulate X_N2 in X_N3...                  \nX_N3:\n", "Debug_R0");
		this->dbg_R[1] = new SC_Debug<R>("Add noise from X_N3 to Y_N1...            \nY_N1:\n", "Debug_R1");
		this->dbg_R[2] = new SC_Debug<R>("Demodulate from Y_N1 to Y_N2...           \nY_N2:\n", "Debug_R2");
		this->dbg_R[3] = new SC_Debug<R>("Filter from Y_N2 to Y_N3...               \nY_N3:\n", "Debug_R3");
		this->dbg_Q[0] = new SC_Debug<Q>("Make the quantization from Y_N3 to Y_N4...\nY_N4:\n", "Debug_Q0");
		this->dbg_Q[1] = new SC_Debug<Q>("Depuncture Y_N4 and generate Y_N5...      \nY_N5:\n", "Debug_Q1");
		this->dbg_B[4] = new SC_Debug<B>("Decode Y_N5 and generate V_K...           \nV_K: \n", "Debug_B4");

		this->bind_sockets_debug();
		sc_core::sc_start(); // start simulation
		this->terminal->legend(std::cout);

		for (auto i = 0; i < 5; i++) { delete this->dbg_B[i]; this->dbg_B[i] = nullptr; }
		for (auto i = 0; i < 4; i++) { delete this->dbg_R[i]; this->dbg_R[i] = nullptr; }
		for (auto i = 0; i < 2; i++) { delete this->dbg_Q[i]; this->dbg_Q[i] = nullptr; }
	}
	else
	{
		// launch a thread dedicated to the terminal display
		std::thread thread(Simulation_BFER<B,R,Q>::terminal_temp_report, this);

		this->bind_sockets();
		sc_core::sc_report_handler::set_actions(SC_INFO, SC_DO_NOTHING);
		sc_core::sc_start(); // start simulation

		// wait the terminal thread to finish
		thread.join();
	}

	delete this->duplicator; this->duplicator = nullptr;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// /!\ VERY DIRTY WAY TO CREATE A NEW SIMULATION CONTEXT IN SYSTEMC, BE CAREFUL THIS IS NOT IN THE STANDARD! /!\ //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	sc_core::sc_curr_simcontext = new sc_simcontext();
	sc_core::sc_default_global_context = sc_curr_simcontext;
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::build_communication_chain()
{
	// build the objects
	this->source   [0] = this->build_source   (     ); check_errors(this->source   [0], "Source<B>"          );
	this->crc      [0] = this->build_crc      (     ); check_errors(this->crc      [0], "CRC<B>"             );
	this->encoder  [0] = this->build_encoder  (     ); check_errors(this->encoder  [0], "Encoder<B>"         );
	this->puncturer[0] = this->build_puncturer(     ); check_errors(this->puncturer[0], "Puncturer<B,Q>"     );
	this->modulator[0] = this->build_modulator(     ); check_errors(this->modulator[0], "Modulator<B,R>"     );

	const auto N     = this->code_params.N;
	const auto tail  = this->code_params.tail_length;
	const auto N_mod = this->modulator[0]->get_buffer_size_after_modulation(N + tail);
	
	this->channel  [0] = this->build_channel  (N_mod); check_errors(this->channel  [0], "Channel<R>"         );
	this->quantizer[0] = this->build_quantizer(N    ); check_errors(this->quantizer[0], "Quantizer<R,Q>"     );
	this->decoder  [0] = this->build_decoder  (     ); check_errors(this->decoder  [0], "Decoder<B,Q>"       );
	this->analyzer [0] = this->build_analyzer (     ); check_errors(this->analyzer [0], "Error_analyzer<B,R>");

	// create the sc_module inside the objects of the communication chain
	this->source   [0]->create_sc_module            ();
	this->crc      [0]->create_sc_module            ();
	this->encoder  [0]->create_sc_module            ();
	this->puncturer[0]->create_sc_module_puncturer  ();
	this->puncturer[0]->create_sc_module_depuncturer();
	this->modulator[0]->create_sc_module_modulator  ();
	this->modulator[0]->create_sc_module_filterer   ();
	this->modulator[0]->create_sc_module_demodulator();
	this->channel  [0]->create_sc_module            ();
	this->quantizer[0]->create_sc_module            ();
	this->decoder  [0]->create_sc_module            ();
	this->analyzer [0]->create_sc_module            ();

	// get the real number of frames per threads (from the decoder)
	this->n_frames = this->decoder[0]->get_n_frames();

	// set the real number of frames per thread
	this->source   [0]->set_n_frames(this->n_frames);
	this->crc      [0]->set_n_frames(this->n_frames);
	this->encoder  [0]->set_n_frames(this->n_frames);
	this->puncturer[0]->set_n_frames(this->n_frames);
	this->modulator[0]->set_n_frames(this->n_frames);
	this->channel  [0]->set_n_frames(this->n_frames);
	this->quantizer[0]->set_n_frames(this->n_frames);
	this->analyzer [0]->set_n_frames(this->n_frames);

	// build the terminal to display the BER/FER
	this->terminal = this->build_terminal();
	check_errors(this->terminal, "Terminal");
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::bind_sockets()
{
	this->source    [0]->module        ->s_out (this->duplicator                  ->s_in );
	this->duplicator                   ->s_out1(this->analyzer [0]->module        ->s_in1);
	this->duplicator                   ->s_out2(this->crc      [0]->module        ->s_in );
	this->crc       [0]->module        ->s_out (this->encoder  [0]->module        ->s_in );
	this->encoder   [0]->module        ->s_out (this->puncturer[0]->module_punct  ->s_in );
	this->puncturer [0]->module_punct  ->s_out (this->modulator[0]->module_mod    ->s_in );
	this->modulator [0]->module_mod    ->s_out (this->channel  [0]->module        ->s_in );
	this->channel   [0]->module        ->s_out (this->modulator[0]->module_filt   ->s_in );
	this->modulator [0]->module_filt   ->s_out (this->modulator[0]->module_demod  ->s_in );
	this->modulator [0]->module_demod  ->s_out (this->quantizer[0]->module        ->s_in );
	this->quantizer [0]->module        ->s_out (this->puncturer[0]->module_depunct->s_in );
	this->puncturer [0]->module_depunct->s_out (this->decoder  [0]->module        ->s_in );
	this->decoder   [0]->module        ->s_out (this->analyzer [0]->module        ->s_in2);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::bind_sockets_debug()
{
	this->source    [0]->module        ->s_out(this->dbg_B[0]->s_in); this->dbg_B[0]->s_out (this->duplicator                  ->s_in );
	this->duplicator                                                                ->s_out1(this->analyzer [0]->module        ->s_in1);
	this->duplicator                                                                ->s_out2(this->crc      [0]->module        ->s_in );
	this->crc       [0]->module        ->s_out(this->dbg_B[1]->s_in); this->dbg_B[1]->s_out (this->encoder  [0]->module        ->s_in );
	this->encoder   [0]->module        ->s_out(this->dbg_B[2]->s_in); this->dbg_B[2]->s_out (this->puncturer[0]->module_punct  ->s_in );
	this->puncturer [0]->module_punct  ->s_out(this->dbg_B[3]->s_in); this->dbg_B[3]->s_out (this->modulator[0]->module_mod    ->s_in );
	this->modulator [0]->module_mod    ->s_out(this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel  [0]->module        ->s_in );
	this->channel   [0]->module        ->s_out(this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator[0]->module_filt   ->s_in );
	this->modulator [0]->module_filt   ->s_out(this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->modulator[0]->module_demod  ->s_in );
	this->modulator [0]->module_demod  ->s_out(this->dbg_R[3]->s_in); this->dbg_R[3]->s_out (this->quantizer[0]->module        ->s_in );
	this->quantizer [0]->module        ->s_out(this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->puncturer[0]->module_depunct->s_in );
	this->puncturer [0]->module_depunct->s_out(this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->decoder  [0]->module        ->s_in );
	this->decoder   [0]->module        ->s_out(this->dbg_B[4]->s_in); this->dbg_B[4]->s_out (this->analyzer [0]->module        ->s_in2);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::terminal_temp_report(Simulation_BFER<B,R,Q> *simu)
{
	if (!simu->simu_params.disable_display && simu->simu_params.display_freq != std::chrono::nanoseconds(0))
	{
		while (!simu->analyzer[0]->fe_limit_achieved() && !simu->analyzer[0]->is_interrupt())
		{
			const auto sleep_time = simu->simu_params.display_freq - std::chrono::milliseconds(0);
			std::this_thread::sleep_for(sleep_time);

			// display statistics in terminal
			simu->terminal->temp_report(std::clog);
		}
	}
}

// ---------------------------------------------------------------------------------------------------- virtual methods

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::release_objects()
{
	if (source   [0] != nullptr) { delete source   [0]; source   [0] = nullptr; }
	if (crc      [0] != nullptr) { delete crc      [0]; crc      [0] = nullptr; }
	if (encoder  [0] != nullptr) { delete encoder  [0]; encoder  [0] = nullptr; }
	if (puncturer[0] != nullptr) { delete puncturer[0]; puncturer[0] = nullptr; }
	if (modulator[0] != nullptr) { delete modulator[0]; modulator[0] = nullptr; }
	if (channel  [0] != nullptr) { delete channel  [0]; channel  [0] = nullptr; }
	if (quantizer[0] != nullptr) { delete quantizer[0]; quantizer[0] = nullptr; }
	if (decoder  [0] != nullptr) { delete decoder  [0]; decoder  [0] = nullptr; }
	if (analyzer [0] != nullptr) { delete analyzer [0]; analyzer [0] = nullptr; }
	if (terminal     != nullptr) { delete terminal    ; terminal     = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Source<B>* Simulation_BFER<B,R,Q>
::build_source(const int tid)
{
	return Factory_source<B>::build(code_params, tid);
}

template <typename B, typename R, typename Q>
Puncturer<B,Q>* Simulation_BFER<B,R,Q>
::build_puncturer(const int tid)
{
	auto puncturer = new Puncturer_NO<B,Q>(code_params.K, code_params.N + code_params.tail_length);
	check_errors(puncturer, "Puncturer<B,Q>");
	return puncturer;
}

template <typename B, typename R, typename Q>
CRC<B>* Simulation_BFER<B,R,Q>
::build_crc(const int tid)
{
	return Factory_CRC<B>::build(code_params, deco_params);
}

template <typename B, typename R, typename Q>
Modulator<B,R,R>* Simulation_BFER<B,R,Q>
::build_modulator(const int tid)
{
	return Factory_modulator<B,R,R>::build(code_params, mod_params, sigma);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_BFER<B,R,Q>
::build_channel(const int size, const int tid)
{
	return Factory_channel<R>::build(code_params, chan_params, sigma, size, tid);
}

template <typename B, typename R, typename Q>
Quantizer<R,Q>* Simulation_BFER<B,R,Q>
::build_quantizer(const int size, const int tid)
{
	return Factory_quantizer<R,Q>::build(code_params, chan_params, sigma, size);
}

template <typename B, typename R, typename Q>
Error_analyzer<B>* Simulation_BFER<B,R,Q>
::build_analyzer(const int tid)
{
	return Factory_error_analyzer<B>::build(simu_params, code_params, n_frames);
}

// ------------------------------------------------------------------------------------------------- non-virtual method

template <typename B, typename R, typename Q>
Terminal* Simulation_BFER<B,R,Q>
::build_terminal(const int tid)
{
	return Factory_terminal<B,R>::build(simu_params, snr, analyzer[0], t_snr, d_decod_total_fake);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFER<B_8,R_8,Q_8>;
template class Simulation_BFER<B_16,R_16,Q_16>;
template class Simulation_BFER<B_32,R_32,Q_32>;
template class Simulation_BFER<B_64,R_64,Q_64>;
#else
template class Simulation_BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif /* SYSTEMC*/