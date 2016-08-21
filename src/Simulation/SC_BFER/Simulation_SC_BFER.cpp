#ifdef SYSTEMC

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
#include "../../Tools/Frame_trace/Frame_trace.hpp"

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

  d_load_total_fake (std::chrono::nanoseconds(0)),
  d_decod_total_fake(std::chrono::nanoseconds(0)),
  d_store_total_fake(std::chrono::nanoseconds(0))
{
	assert(simu_params.n_threads == 1);
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

		// launch the master thread
		this->Monte_Carlo_method();

		if (!simu_params.disable_display && !simu_params.benchs)
			terminal->final_report(std::cout);

		// release communication objects
		release_objects();

		// exit simulation (double [ctrl+c])
		if (Error_analyzer<B>::is_over())
			break;
	}
}

#include "../../Source/Source_random.hpp"
#include "../../Tools/SystemC/SC_Dummy.hpp"
#include "../../Tools/SystemC/SC_Debug.hpp"

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::Monte_Carlo_method()
{
	this->build_communication_chain();

	if ((!this->simu_params.disable_display && this->snr == this->simu_params.snr_min && 
	    !(this->simu_params.enable_debug && this->simu_params.n_threads == 1) && !this->simu_params.benchs))
		this->terminal->legend(std::cout);

	if (this->simu_params.n_threads == 1 && this->simu_params.enable_debug)
	{
		this->bind_sockets_debug();
		sc_core::sc_start(100, SC_MS); // start simulation
		// sc_core::sc_start(); // start simulation
		this->terminal->legend(std::cout);
	}
	else
	{
		this->bind_sockets();
		sc_core::sc_start(100, SC_MS); // start simulation
		// sc_core::sc_start(); // start simulation
	}
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

	// bind the sockets in the modules (allocate the buffers)
	// this->source   ->register_sockets();
	this->crc      ->register_sockets();
	this->encoder  ->register_sockets();
	this->puncturer->register_sockets();
	this->modulator->register_sockets();
	this->channel  ->register_sockets();
	this->quantizer->register_sockets();
	this->decoder  ->register_sockets();

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
	SC_Dummy<B>* dummy  = new SC_Dummy<B>("Dummy");

	this->source   ->socket_out        (this->crc      ->socket_in        );
	this->crc      ->socket_out        (this->encoder  ->socket_in        );
	this->encoder  ->socket_out        (this->puncturer->socket_in_punct  );
	this->puncturer->socket_out_punct  (this->modulator->socket_in_mod    );
	this->modulator->socket_out_mod    (this->channel  ->socket_in        );
	this->channel  ->socket_out        (this->modulator->socket_in_demod  );
	this->modulator->socket_out_demod  (this->quantizer->socket_in        );
	this->quantizer->socket_out        (this->puncturer->socket_in_depunct);
	this->puncturer->socket_out_depunct(this->decoder  ->socket_in        );
	this->decoder  ->socket_out        (dummy          ->socket_in        );

	// MEMORY LEAK !!! (dummy)
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::bind_sockets_debug()
{
	SC_Debug<B>* dbg_B[4] = {new SC_Debug<B>("Generate random bits U_K...               \nU_K: \n", "Debug_B0"), 
	                         new SC_Debug<B>("Add the CRC to U_K...                     \nU_K: \n", "Debug_B1"), 
	                         new SC_Debug<B>("Encode U_K in X_N1...                     \nX_N1:\n", "Debug_B2"),
	                         new SC_Debug<B>("Puncture X_N1 in X_N2...                  \nX_N2:\n", "Debug_B3")};
	SC_Debug<R>* dbg_R[3] = {new SC_Debug<R>("Modulate X_N2 in X_N3...                  \nX_N3:\n", "Debug_R0"),
	                         new SC_Debug<R>("Add noise from X_N3 to Y_N1...            \nY_N1:\n", "Debug_R1"), 
	                         new SC_Debug<R>("Demodulate from Y_N1 to Y_N2...           \nY_N2:\n", "Debug_R2")};
	SC_Debug<Q>* dbg_Q[3] = {new SC_Debug<Q>("Make the quantization from Y_N2 to Y_N3...\nY_N3:\n", "Debug_Q0"),
	                         new SC_Debug<Q>("Depuncture Y_N3 and generate Y_N4...      \nY_N4:\n", "Debug_Q1"),
	                         new SC_Debug<Q>("Decode Y_N2 and generate V_K...           \nY_N5:\n", "Debug_Q2")};
	SC_Dummy<B>* dummy    =  new SC_Dummy<B>("Dummy");

	this->source   ->socket_out        (dbg_B[0]->socket_in); dbg_B[0]->socket_out(this->crc      ->socket_in        );
	this->crc      ->socket_out        (dbg_B[1]->socket_in); dbg_B[1]->socket_out(this->encoder  ->socket_in        );
	this->encoder  ->socket_out        (dbg_B[2]->socket_in); dbg_B[2]->socket_out(this->puncturer->socket_in_punct  );
	this->puncturer->socket_out_punct  (dbg_B[3]->socket_in); dbg_B[3]->socket_out(this->modulator->socket_in_mod    );
	this->modulator->socket_out_mod    (dbg_R[0]->socket_in); dbg_R[0]->socket_out(this->channel  ->socket_in        );
	this->channel  ->socket_out        (dbg_R[1]->socket_in); dbg_R[1]->socket_out(this->modulator->socket_in_demod  );
	this->modulator->socket_out_demod  (dbg_R[2]->socket_in); dbg_R[2]->socket_out(this->quantizer->socket_in        );
	this->quantizer->socket_out        (dbg_Q[0]->socket_in); dbg_Q[0]->socket_out(this->puncturer->socket_in_depunct);
	this->puncturer->socket_out_depunct(dbg_Q[1]->socket_in); dbg_Q[1]->socket_out(this->decoder  ->socket_in        );
	this->decoder  ->socket_out        (dbg_Q[2]->socket_in); dbg_Q[2]->socket_out(dummy          ->socket_in        );

	// MEMORY LEAK !!! (dbg_B[0-3], dbg_R[0-2], dbg_Q[0-2], dummy)
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::simulation_loop()
{
	// using namespace std::chrono;

	// // simulation loop
	// this->t_simu = steady_clock::now();
	// while ((!this->analyzer->fe_limit_achieved()) && // while max frame error count has not been reached
	//         (this->simu_params.stop_time == seconds(0) || 
	//          (steady_clock::now() - this->t_snr) < this->simu_params.stop_time))
	// {
	// 	// this->source   ->generate    (this->U_K             ); // generate a random K bits vector U_K
	// 	// this->crc      ->build       (this->U_K             ); // add the CRC to U_K
	// 	// this->encoder  ->encode      (this->U_K,  this->X_N1); // encode U_K into a N bits vector X_N
	// 	// this->puncturer->puncture    (this->X_N1, this->X_N2); // puncture X_N1 into X_N2
	// 	// this->modulator->modulate    (this->X_N2, this->X_N3); // modulate
	// 	// this->channel  ->add_noise   (this->X_N3, this->Y_N1); // add noise
	// 	// this->modulator->demodulate  (this->Y_N1, this->Y_N2); // demodulation
	// 	// this->quantizer->process     (this->Y_N2, this->Y_N3); // make the quantization
	// 	// this->puncturer->depuncture  (this->Y_N3, this->Y_N4); // depuncture before the decoding stage
	// 	// this->decoder  ->load        (this->Y_N4            ); // load data in the decoder
	// 	// this->decoder  ->decode      (                      ); // launch decoder
	// 	// this->decoder  ->store       (this->V_K             ); // store results in V_K
	// 	// this->analyzer ->check_errors(this->U_K,  this->V_K ); // check errors in the frame

	// 	// display statistics in terminal
	// 	if (!this->simu_params.disable_display && this->simu_params.display_freq != nanoseconds(0) &&
	// 	    (steady_clock::now() - this->t_simu) >= this->simu_params.display_freq)
	// 	{
	// 		this->terminal->temp_report(std::clog);
	// 		this->t_simu = steady_clock::now();
	// 	}
	// }
}

template <typename B, typename R, typename Q>
void Simulation_SC_BFER<B,R,Q>
::simulation_loop_debug()
{
	// using namespace std::chrono;

	// Frame_trace<B> ft; // frame trace to display the vectors

	// // simulation loop
	// auto t_simu = steady_clock::now();
	// while (!this->analyzer->fe_limit_achieved() && // while max frame error count has not been reached
	//        (this->simu_params.stop_time == seconds(0) ||
	//         (steady_clock::now() - this->t_snr) < this->simu_params.stop_time))
	// {
	// 	// std::clog << "-------------------------------" << std::endl;
	// 	// std::clog << "New encoding/decoding session !" << std::endl;
	// 	// std::clog << "-------------------------------" << std::endl;

	// 	// // generate a random K bits vector U_K
	// 	// std::clog << "Generate random bits U_K..." << std::endl;
	// 	// this->source->generate(this->U_K);

	// 	// // display U_K
	// 	// std::clog << "U_K:" << std::endl;
	// 	// ft.display_bit_vector(this->U_K);
	// 	// std::clog << std::endl;

	// 	// // add the CRC to U_K
	// 	// std::clog << "Add the CRC to U_K..." << std::endl;
	// 	// this->crc->build(this->U_K);

	// 	// // display U_K
	// 	// std::clog << "U_K:" << std::endl;
	// 	// ft.display_bit_vector(this->U_K);
	// 	// std::clog << std::endl;

	// 	// // encode U_K into a N bits vector X_N1
	// 	// std::clog << "Encode U_K in X_N1..." << std::endl;
	// 	// this->encoder->encode(this->U_K, this->X_N1);

	// 	// // display X_N1
	// 	// std::clog << "X_N1:" << std::endl;
	// 	// ft.display_bit_vector(this->X_N1);
	// 	// std::clog << std::endl;

	// 	// // puncture X_N1 into X_N2
	// 	// std::clog << "Puncture X_N1 in X_N2..." << std::endl;
	// 	// this->puncturer->puncture(this->X_N1, this->X_N2);

	// 	// // display X_N2
	// 	// std::clog << "X_N2:" << std::endl;
	// 	// ft.display_bit_vector(this->X_N2);
	// 	// std::clog << std::endl;

	// 	// // modulate
	// 	// std::clog << "Modulate X_N2 in X_N3..." << std::endl;
	// 	// this->modulator->modulate(this->X_N2, this->X_N3);

	// 	// // display X_N3
	// 	// std::clog << "X_N3:" << std::endl;
	// 	// ft.display_real_vector(this->X_N3);
	// 	// std::clog << std::endl;

	// 	// // add noise
	// 	// std::clog << "Add noise from X_N3 to Y_N1..." << std::endl;
	// 	// this->channel->add_noise(this->X_N3, this->Y_N1);

	// 	// // display Y_N1
	// 	// std::clog << "Y_N1:" << std::endl;
	// 	// ft.display_real_vector(this->Y_N1);
	// 	// std::clog << std::endl;

	// 	// // demodulation
	// 	// std::clog << "Demodulate from Y_N1 to Y_N2..." << std::endl;
	// 	// this->modulator->demodulate(this->Y_N1, this->Y_N2);

	// 	// // display Y_N2
	// 	// std::clog << "Y_N2:" << std::endl;
	// 	// ft.display_real_vector(this->Y_N2);
	// 	// std::clog << std::endl;

	// 	// // make the quantization
	// 	// std::clog << "Make the quantization from Y_N2 to Y_N3..." << std::endl;
	// 	// this->quantizer->process(this->Y_N2, this->Y_N3);

	// 	// // display Y_N3
	// 	// std::clog << "Y_N3:" << std::endl;
	// 	// ft.display_real_vector(this->Y_N3);
	// 	// std::clog << std::endl;

	// 	// // depuncture before the decoding stage
	// 	// std::clog << "Depuncture Y_N3 and generate Y_N4..." << std::endl;
	// 	// this->puncturer->depuncture(this->Y_N3, this->Y_N4);

	// 	// // display Y_N4
	// 	// std::clog << "Y_N4:" << std::endl;
	// 	// ft.display_real_vector(this->Y_N4);
	// 	// std::clog << std::endl;

	// 	// // load data in the decoder
	// 	// this->decoder->load(this->Y_N4);
		
	// 	// // launch decoder
	// 	// std::clog << "Decode Y_N2 and generate V_K..." << std::endl;
	// 	// this->decoder->decode();

	// 	// // store results in V_K
	// 	// this->decoder->store(this->V_K);

	// 	// // display V_K
	// 	// std::clog << "V_K:" << std::endl;
	// 	// ft.display_bit_vector(this->V_K);
	// 	// std::clog << std::endl;

	// 	// // check errors in the frame
	// 	// this->analyzer->check_errors(this->U_K, this->V_K);

	// 	// // display statistics in terminal
	// 	// if (!this->simu_params.disable_display && this->simu_params.display_freq != nanoseconds(0) && 
	// 	//     (steady_clock::now() - t_simu) >= this->simu_params.display_freq)
	// 	// {
	// 	// 	this->terminal->temp_report(std::clog);
	// 	// 	t_simu = steady_clock::now();
	// 	// }
	// }

	// this->terminal->legend(std::cout);
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
	auto puncturer = new Puncturer_NO<B,Q>(code_params.K, code_params.N);
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