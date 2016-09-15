#ifndef SYSTEMC

#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <algorithm>

#include "Tools/bash_tools.h"
#include "Tools/simu_tools.h"
#include "Tools/Frame_trace/Frame_trace.hpp"

#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_CRC.hpp"
#include "Tools/Factory/Factory_modulator.hpp"
#include "Tools/Factory/Factory_channel.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/Factory_error_analyzer.hpp"
#include "Tools/Factory/Factory_terminal.hpp"

#include "Simulation_BFERI.hpp"

template <typename B, typename R, typename Q>
Simulation_BFERI<B,R,Q>
::Simulation_BFERI(const parameters& params)
: Simulation(),
  
  params(params),

//  params.simulation(params.simulation),
//  params.code(params.code),
//  params.encoder(params.encoder),
//  params.modulator (params.modulator ),
//  params.channel(params.channel),
//  params.decoder(params.decoder),

  threads(params.simulation.n_threads -1),
  barrier(params.simulation.n_threads),
  n_frames(1),
 
  snr      (0.f),
  code_rate(0.f),
  sigma    (0.f),

  U_K (params.simulation.n_threads, mipp::vector<B>(params.code.K)),
  X_N1(params.simulation.n_threads, mipp::vector<B>(params.code.N)),
  X_N2(params.simulation.n_threads, mipp::vector<B>(params.code.N)),
  X_N3(params.simulation.n_threads, mipp::vector<R>(params.code.N)),
  Y_N1(params.simulation.n_threads, mipp::vector<R>(params.code.N)),
  Y_N2(params.simulation.n_threads, mipp::vector<R>(params.code.N)),
  Y_N3(params.simulation.n_threads, mipp::vector<Q>(params.code.N)),
  Y_N4(params.simulation.n_threads, mipp::vector<Q>(params.code.N)),
  Y_N5(params.simulation.n_threads, mipp::vector<Q>(params.code.N)),
  Y_N6(params.simulation.n_threads, mipp::vector<Q>(params.code.N)),
  Y_N7(params.simulation.n_threads, mipp::vector<Q>(params.code.N)),
  V_K (params.simulation.n_threads, mipp::vector<B>(params.code.K)),

  source      (params.simulation.n_threads, nullptr),
  crc         (params.simulation.n_threads, nullptr),
  encoder     (params.simulation.n_threads, nullptr),
  modulator   (params.simulation.n_threads, nullptr),
  channel     (params.simulation.n_threads, nullptr),
  quantizer   (params.simulation.n_threads, nullptr),
  interleaver (params.simulation.n_threads, nullptr),
  siso        (params.simulation.n_threads, nullptr),
  decoder     (params.simulation.n_threads, nullptr),
  analyzer    (params.simulation.n_threads, nullptr),
  analyzer_red(                             nullptr),
  terminal    (                             nullptr),

  d_sourc_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_crc_total  (params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_encod_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_inter_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_modul_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_chann_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_filte_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_quant_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_demod_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_decod_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_deint_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_check_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),

  d_sourc_total_sum(std::chrono::nanoseconds(0)),
  d_crc_total_sum  (std::chrono::nanoseconds(0)),
  d_encod_total_sum(std::chrono::nanoseconds(0)),
  d_inter_total_sum(std::chrono::nanoseconds(0)),
  d_modul_total_sum(std::chrono::nanoseconds(0)),
  d_chann_total_sum(std::chrono::nanoseconds(0)),
  d_filte_total_sum(std::chrono::nanoseconds(0)),
  d_quant_total_sum(std::chrono::nanoseconds(0)),
  d_demod_total_sum(std::chrono::nanoseconds(0)),
  d_decod_total_sum(std::chrono::nanoseconds(0)),
  d_deint_total_sum(std::chrono::nanoseconds(0)),
  d_check_total_sum(std::chrono::nanoseconds(0))
{
	assert(params.simulation.n_threads >= 1);

	if (params.simulation.n_threads > 1 && params.simulation.enable_debug)
		std::clog << bold_yellow("(WW) Debug mode will be disabled ")
		          << bold_yellow("because you launched the simulation with more than 1 thread!")
		          << std::endl;
}

template <typename B, typename R, typename Q>
Simulation_BFERI<B,R,Q>
::~Simulation_BFERI()
{
	release_objects();
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::launch()
{
	launch_precompute();
	
	// for each SNR to be simulated
	for (snr = params.simulation.snr_min; snr <= params.simulation.snr_max; snr += params.simulation.snr_step)
	{
		t_snr = std::chrono::steady_clock::now();

		code_rate = (float)(params.code.K / (float)(params.code.N + params.code.tail_length));
		sigma     = std::sqrt((float)params.modulator.upsample_factor) /
		            std::sqrt(2.f * code_rate * (float)params.modulator.bits_per_symbol * std::pow(10.f, (snr / 10.f)));

		snr_precompute();

		// launch a group of slave threads (there is "n_threads -1" slave threads)
		for (auto tid = 1; tid < params.simulation.n_threads; tid++)
			threads[tid -1] = std::thread(Simulation_BFERI<B,R,Q>::Monte_Carlo_method, this, tid);

		// launch the master thread
		Simulation_BFERI<B,R,Q>::Monte_Carlo_method(this, 0);

		// join the slave threads with the master thread
		for (auto tid = 1; tid < params.simulation.n_threads; tid++)
			threads[tid -1].join();

		if (!params.simulation.disable_display && !params.simulation.benchs)
		{
			analyzer_red->reduce();
			time_reduction(true);
			terminal->final_report(std::cout);
		}

		// release communication objects
		release_objects();

		// exit simulation (double [ctrl+c])
		if (Error_analyzer<B>::is_over())
			break;
	}

	if (params.simulation.time_report && !params.simulation.benchs)
		time_report();
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::Monte_Carlo_method(Simulation_BFERI<B,R,Q> *simu, const int tid)
{
	Simulation_BFERI<B,R,Q>::build_communication_chain(simu, tid);

	if (tid == 0 && (!simu->params.simulation.disable_display && simu->snr == simu->params.simulation.snr_min &&
	    !(simu->params.simulation.enable_debug && simu->params.simulation.n_threads == 1) && !simu->params.simulation.benchs))
		simu->terminal->legend(std::cout);

	if (simu->params.code.generation_method == "AZCW")
	{
		std::fill(simu->U_K [tid].begin(), simu->U_K [tid].end(), (B)0);
		std::fill(simu->X_N1[tid].begin(), simu->X_N1[tid].end(), (B)0);
		std::fill(simu->X_N2[tid].begin(), simu->X_N2[tid].end(), (B)0);
		simu->modulator[tid]->modulate(simu->X_N2[tid], simu->X_N3[tid]);
	}

	simu->d_sourc_total[tid] = std::chrono::nanoseconds(0);
	simu->d_crc_total  [tid] = std::chrono::nanoseconds(0);
	simu->d_encod_total[tid] = std::chrono::nanoseconds(0);
	simu->d_inter_total[tid] = std::chrono::nanoseconds(0);
	simu->d_modul_total[tid] = std::chrono::nanoseconds(0);
	simu->d_chann_total[tid] = std::chrono::nanoseconds(0);
	simu->d_filte_total[tid] = std::chrono::nanoseconds(0);
	simu->d_quant_total[tid] = std::chrono::nanoseconds(0);
	simu->d_demod_total[tid] = std::chrono::nanoseconds(0);
	simu->d_deint_total[tid] = std::chrono::nanoseconds(0);
	simu->d_decod_total[tid] = std::chrono::nanoseconds(0);
	simu->d_check_total[tid] = std::chrono::nanoseconds(0);

	simu->barrier(tid);

	if (simu->params.simulation.n_threads == 1 && simu->params.simulation.enable_debug)
		Simulation_BFERI<B,R,Q>::simulation_loop_debug(simu);
	else
		Simulation_BFERI<B,R,Q>::simulation_loop(simu, tid);
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::build_communication_chain(Simulation_BFERI<B,R,Q> *simu, const int tid)
{
	// build the objects
	simu->source     [tid] = simu->build_source     (       tid); check_errors(simu->source     [tid], "Source<B>"          );
	simu->crc        [tid] = simu->build_crc        (       tid); check_errors(simu->crc        [tid], "CRC<B>"             );
	simu->encoder    [tid] = simu->build_encoder    (       tid); check_errors(simu->encoder    [tid], "Encoder<B>"         );
	simu->interleaver[tid] = simu->build_interleaver(       tid); check_errors(simu->interleaver[tid], "Interleaver<int>"   );
	simu->modulator  [tid] = simu->build_modulator  (       tid); check_errors(simu->modulator  [tid], "Modulator<B,R>"     );
	
	const auto N     = simu->params.code.N;
	const auto tail  = simu->params.code.tail_length;
	const auto N_mod = simu->modulator[tid]->get_buffer_size_after_modulation(N + tail);
	const auto N_fil = simu->modulator[tid]->get_buffer_size_after_filtering (N + tail);

	simu->channel    [tid] = simu->build_channel    (N_mod, tid); check_errors(simu->channel    [tid], "Channel<R>"         );
	simu->quantizer  [tid] = simu->build_quantizer  (N_fil, tid); check_errors(simu->quantizer  [tid], "Quantizer<R,Q>"     );
	simu->siso       [tid] = simu->build_siso       (       tid); check_errors(simu->siso       [tid], "SISO<Q>"            );
	simu->decoder    [tid] = simu->build_decoder    (       tid); check_errors(simu->decoder    [tid], "Decoder<B,Q>"       );
	simu->analyzer   [tid] = simu->build_analyzer   (       tid); check_errors(simu->analyzer   [tid], "Error_analyzer<B,R>");

	// get the real number of frames per threads (from the decoder)
	auto n_fra = simu->siso[tid]->get_n_frames();
	assert(simu->siso[tid]->get_n_frames() == simu->decoder[tid]->get_n_frames());

	// resize the buffers
	const auto K = simu->params.code.K;
	if (simu->U_K [tid].size() != (unsigned) ( K             * n_fra)) simu->U_K [tid].resize( K              * n_fra);
	if (simu->X_N1[tid].size() != (unsigned) ((N     + tail) * n_fra)) simu->X_N1[tid].resize((N      + tail) * n_fra);
	if (simu->X_N2[tid].size() != (unsigned) ((N     + tail) * n_fra)) simu->X_N2[tid].resize((N      + tail) * n_fra);
	if (simu->X_N3[tid].size() != (unsigned) ( N_mod         * n_fra)) simu->X_N3[tid].resize( N_mod          * n_fra);
	if (simu->Y_N1[tid].size() != (unsigned) ( N_mod         * n_fra)) simu->Y_N1[tid].resize( N_mod          * n_fra);
	if (simu->Y_N2[tid].size() != (unsigned) ( N_fil         * n_fra)) simu->Y_N2[tid].resize( N_fil          * n_fra);
	if (simu->Y_N3[tid].size() != (unsigned) ( N_fil         * n_fra)) simu->Y_N3[tid].resize( N_fil          * n_fra);
	if (simu->Y_N4[tid].size() != (unsigned) ((N     + tail) * n_fra)) simu->Y_N4[tid].resize((N      + tail) * n_fra);
	if (simu->Y_N5[tid].size() != (unsigned) ((N     + tail) * n_fra)) simu->Y_N5[tid].resize((N      + tail) * n_fra);
	if (simu->Y_N6[tid].size() != (unsigned) ((N     + tail) * n_fra)) simu->Y_N6[tid].resize((N      + tail) * n_fra);
	if (simu->Y_N7[tid].size() != (unsigned) ((N     + tail) * n_fra)) simu->Y_N7[tid].resize((N      + tail) * n_fra);
	if (simu->V_K [tid].size() != (unsigned) ( K             * n_fra)) simu->V_K [tid].resize( K              * n_fra);

	// fill the Y_N7 vector with 0 values
	std::fill(simu->Y_N7[tid].begin(), simu->Y_N7[tid].end(), 0);

	// set the real number of frames per thread
	simu->source     [tid]->set_n_frames(n_fra);
	simu->crc        [tid]->set_n_frames(n_fra);
	simu->encoder    [tid]->set_n_frames(n_fra);
	simu->interleaver[tid]->set_n_frames(n_fra);
	simu->modulator  [tid]->set_n_frames(n_fra);
	simu->channel    [tid]->set_n_frames(n_fra);
	simu->quantizer  [tid]->set_n_frames(n_fra);
	simu->analyzer   [tid]->set_n_frames(n_fra);

	simu->barrier(tid);
	if (tid == 0)
	{
		simu->n_frames = n_fra;

		// build an error analyzer to compute BER/FER (reduce the other analyzers)
		simu->analyzer_red = new Error_analyzer_reduction<B>(simu->params.code.K,
		                                                     simu->params.code.N,
		                                                     simu->params.simulation.max_fe,
		                                                     simu->analyzer,
		                                                     simu->n_frames);
		// build the terminal to display the BER/FER
		simu->terminal = simu->build_terminal(tid);
		check_errors(simu->terminal, "Terminal");
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::simulation_loop(Simulation_BFERI<B,R,Q> *simu, const int tid)
{
	using namespace std::chrono;

	if (tid == 0)
		simu->t_simu = steady_clock::now();

	int prev_n_fe = 0;

	// simulation loop
	while ((!simu->analyzer_red->fe_limit_achieved()) && // while max frame error count has not been reached
	        (simu->params.simulation.stop_time == seconds(0) ||
	         (steady_clock::now() - simu->t_snr) < simu->params.simulation.stop_time))
	{
		auto d_sourc = nanoseconds(0);
		auto d_crc   = nanoseconds(0);
		auto d_encod = nanoseconds(0);
		auto d_inter = nanoseconds(0);
		auto d_modul = nanoseconds(0);

		if (simu->params.code.generation_method != "AZCW")
		{
			// generate a random K bits vector U_K
			auto t_sourc = steady_clock::now();
			simu->source[tid]->generate(simu->U_K[tid]);
			d_sourc = steady_clock::now() - t_sourc;

			// add the CRC to U_K
			auto t_crc = steady_clock::now();
			simu->crc[tid]->build(simu->U_K[tid]);
			d_crc = steady_clock::now() - t_crc;

			// encode U_K into a N bits vector X_N
			auto t_encod = steady_clock::now();
			simu->encoder[tid]->encode(simu->U_K[tid], simu->X_N1[tid]);
			d_encod = steady_clock::now() - t_encod;

			auto t_inter = steady_clock::now();
			simu->interleaver[tid]->interleave(simu->X_N1[tid], simu->X_N2[tid]);
			d_inter = steady_clock::now() - t_inter;

			// modulate
			auto t_modul = steady_clock::now();
			simu->modulator[tid]->modulate(simu->X_N2[tid], simu->X_N3[tid]);
			d_modul = steady_clock::now() - t_modul;
		}

		// add noise
		auto t_chann = steady_clock::now();
		simu->channel[tid]->add_noise(simu->X_N3[tid], simu->Y_N1[tid]);
		auto d_chann = steady_clock::now() - t_chann;

		// filtering
		auto t_filte = steady_clock::now();
		simu->modulator[tid]->filter(simu->Y_N1[tid], simu->Y_N2[tid]);
		auto d_filte = steady_clock::now() - t_filte;

		// make the quantization
		auto t_quant = steady_clock::now();
		simu->quantizer[tid]->process(simu->Y_N2[tid], simu->Y_N3[tid]);
		auto d_quant = steady_clock::now() - t_quant;

		auto d_demod = nanoseconds(0);
		auto d_deint = nanoseconds(0);
		auto d_decod = nanoseconds(0);

		std::fill(simu->Y_N7[tid].begin(), simu->Y_N7[tid].end(), 0);
		for (auto ite = 0; ite <= simu->params.modulator.demod_n_ite; ite++)
		{
			// demodulation
			auto t_demod = steady_clock::now();
			simu->modulator[tid]->demodulate(simu->Y_N3[tid], simu->Y_N7[tid], simu->Y_N4[tid]);
			d_demod += steady_clock::now() - t_demod;

			// deinterleaving
			auto t_deint = steady_clock::now();
			simu->interleaver[tid]->deinterleave(simu->Y_N4[tid], simu->Y_N5[tid]);
			d_deint += steady_clock::now() - t_deint;

			// soft decode
			if (ite != simu->params.modulator.demod_n_ite)
			{
				// decode
				auto t_decod = steady_clock::now();
				simu->siso[tid]->decode(simu->Y_N5[tid], simu->Y_N6[tid]);
				d_decod += steady_clock::now() - t_decod;

				// interleaving
				auto t_inter = steady_clock::now();
				simu->interleaver[tid]->interleave(simu->Y_N6[tid], simu->Y_N7[tid]);
				d_inter += steady_clock::now() - t_inter;
			}
			// hard decode
			else
			{
				// decode
				auto t_decod = steady_clock::now();
				simu->decoder[tid]->load  (simu->Y_N5[tid]);
				simu->decoder[tid]->decode(               );
				simu->decoder[tid]->store (simu->V_K [tid]);
				d_decod += steady_clock::now() - t_decod;
			}
		}

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
		simu->d_crc_total  [tid] += d_crc;
		simu->d_encod_total[tid] += d_encod;
		simu->d_inter_total[tid] += d_inter;
		simu->d_modul_total[tid] += d_modul;
		simu->d_chann_total[tid] += d_chann;
		simu->d_filte_total[tid] += d_filte;
		simu->d_quant_total[tid] += d_quant;
		simu->d_demod_total[tid] += d_demod;
		simu->d_deint_total[tid] += d_deint;
		simu->d_decod_total[tid] += d_decod;
		simu->d_check_total[tid] += d_check;

		// display statistics in terminal
		if (tid == 0 && !simu->params.simulation.disable_display && simu->params.simulation.display_freq != nanoseconds(0) &&
		    (steady_clock::now() - simu->t_simu) >= simu->params.simulation.display_freq)
		{
			simu->analyzer_red->reduce();
			simu->time_reduction();
			simu->terminal->temp_report(std::clog);
			simu->t_simu = steady_clock::now();
		}
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::simulation_loop_debug(Simulation_BFERI<B,R,Q> *simu)
{
	using namespace std::chrono;

	Frame_trace<B> ft(simu->params.simulation.debug_limit); // frame trace to display the vectors

	// simulation loop
	auto t_simu = steady_clock::now();
	while (!simu->analyzer_red->fe_limit_achieved() && // while max frame error count has not been reached
	       (simu->params.simulation.stop_time == seconds(0) ||
	        (steady_clock::now() - simu->t_snr) < simu->params.simulation.stop_time))
	{
		std::clog << "-------------------------------" << std::endl;
		std::clog << "New encoding/decoding session !" << std::endl;
		std::clog << "-------------------------------" << std::endl;

		auto d_sourc = nanoseconds(0);
		auto d_crc   = nanoseconds(0);
		auto d_encod = nanoseconds(0);
		auto d_inter = nanoseconds(0);
		auto d_modul = nanoseconds(0);

		if (simu->params.code.generation_method != "AZCW")
		{
			// generate a random K bits vector U_K
			std::clog << "Generate random bits U_K..." << std::endl;
			auto t_sourc = steady_clock::now();
			simu->source[0]->generate(simu->U_K[0]);
			d_sourc = steady_clock::now() - t_sourc;

			// display U_K
			std::clog << "U_K:" << std::endl;
			ft.display_bit_vector(simu->U_K[0]);
			std::clog << std::endl;

			// add the CRC to U_K
			std::clog << "Add the CRC to U_K..." << std::endl;
			auto t_crc = steady_clock::now();
			simu->crc[0]->build(simu->U_K[0]);
			d_crc = steady_clock::now() - t_crc;

			// display U_K
			std::clog << "U_K:" << std::endl;
			ft.display_bit_vector(simu->U_K[0]);
			std::clog << std::endl;

			// encode U_K into a N bits vector X_N1
			std::clog << "Encode U_K in X_N1..." << std::endl;
			auto t_encod = steady_clock::now();
			simu->encoder[0]->encode(simu->U_K[0], simu->X_N1[0]);
			d_encod = steady_clock::now() - t_encod;

			// display X_N1
			std::clog << "X_N1:" << std::endl;
			ft.display_bit_vector(simu->X_N1[0]);
			std::clog << std::endl;

			// puncture X_N1 into X_N2
			std::clog << "Interleaver X_N1 in X_N2..." << std::endl;
			auto t_inter = steady_clock::now();
			simu->interleaver[0]->interleave(simu->X_N1[0], simu->X_N2[0]);
			d_inter = steady_clock::now() - t_inter;

			// display X_N2
			std::clog << "X_N2:" << std::endl;
			ft.display_bit_vector(simu->X_N2[0]);
			std::clog << std::endl;

			// modulate
			std::clog << "Modulate X_N2 in X_N3..." << std::endl;
			auto t_modul = steady_clock::now();
			simu->modulator[0]->modulate(simu->X_N2[0], simu->X_N3[0]);
			d_modul = steady_clock::now() - t_modul;

			// display X_N3
			std::clog << "X_N3:" << std::endl;
			ft.display_real_vector(simu->X_N3[0]);
			std::clog << std::endl;
		}
		else
		{
			// display U_K
			std::clog << "U_K:" << std::endl;
			ft.display_bit_vector(simu->U_K[0]);
			std::clog << std::endl;

			// display X_N2
			std::clog << "X_N2:" << std::endl;
			ft.display_bit_vector(simu->X_N2[0]);
			std::clog << std::endl;

			// display X_N3
			std::clog << "X_N3:" << std::endl;
			ft.display_real_vector(simu->X_N3[0]);
			std::clog << std::endl;
		}

		// add noise
		std::clog << "Add noise from X_N3 to Y_N1..." << std::endl;
		auto t_chann = steady_clock::now();
		simu->channel[0]->add_noise(simu->X_N3[0], simu->Y_N1[0]);
		auto d_chann = steady_clock::now() - t_chann;

		// display Y_N1
		std::clog << "Y_N1:" << std::endl;
		ft.display_real_vector(simu->Y_N1[0]);
		std::clog << std::endl;

		// filtering
		std::clog << "Apply the filtering from Y_N1 to Y_N2..." << std::endl;
		auto t_filte = steady_clock::now();
		simu->modulator[0]->filter(simu->Y_N1[0], simu->Y_N2[0]);
		auto d_filte = steady_clock::now() - t_filte;

		// display Y_N2
		std::clog << "Y_N2:" << std::endl;
		ft.display_real_vector(simu->Y_N2[0]);
		std::clog << std::endl;

		// make the quantization
		std::clog << "Make the quantization from Y_N2 to Y_N3..." << std::endl;
		auto t_quant = steady_clock::now();
		simu->quantizer[0]->process(simu->Y_N2[0], simu->Y_N3[0]);
		auto d_quant = steady_clock::now() - t_quant;

		// display Y_N3
		std::clog << "Y_N3:" << std::endl;
		ft.display_real_vector(simu->Y_N3[0]);
		std::clog << std::endl;

		auto d_demod = nanoseconds(0);
		auto d_deint = nanoseconds(0);
		auto d_decod = nanoseconds(0);

		std::fill(simu->Y_N7[0].begin(), simu->Y_N7[0].end(), 0);
		for (auto ite = 0; ite <= simu->params.modulator.demod_n_ite; ite++)
		{
			std::clog << "*** Turbo demodulation iteration n°" << ite << " ***" << std::endl << std::endl;
			
			// demodulation
			std::clog << "Demodulate from Y_N3 to Y_N4..." << std::endl;
			auto t_demod = steady_clock::now();
			simu->modulator[0]->demodulate(simu->Y_N3[0], simu->Y_N7[0], simu->Y_N4[0]);
			d_demod = steady_clock::now() - t_demod;

			// display Y_N5
			std::clog << "Y_N4:" << std::endl;
			ft.display_real_vector(simu->Y_N4[0]);
			std::clog << std::endl;

			// deinterleaving
			std::clog << "Deinterleave from Y_N4 to Y_N5..." << std::endl;
			auto t_deint = steady_clock::now();
			simu->interleaver[0]->deinterleave(simu->Y_N4[0], simu->Y_N5[0]);
			d_deint += steady_clock::now() - t_deint;

			// display Y_N5
			std::clog << "Y_N5:" << std::endl;
			ft.display_real_vector(simu->Y_N5[0]);
			std::clog << std::endl;

			// soft decode
			if (ite != simu->params.modulator.demod_n_ite)
			{
				// decode
				std::clog << "Soft decode form Y_N5 to Y_N6..." << std::endl;
				auto t_decod = steady_clock::now();
				simu->siso[0]->decode(simu->Y_N5[0], simu->Y_N6[0]);
				d_decod += steady_clock::now() - t_decod;

				// display Y_N6
				std::clog << "Y_N6:" << std::endl;
				ft.display_real_vector(simu->Y_N6[0]);
				std::clog << std::endl;

				// interleaving
				std::clog << "Interleave from Y_N6 to Y_N7..." << std::endl;
				auto t_inter = steady_clock::now();
				simu->interleaver[0]->interleave(simu->Y_N6[0], simu->Y_N7[0]);
				d_inter += steady_clock::now() - t_inter;

				// display Y_N7
				std::clog << "Y_N7:" << std::endl;
				ft.display_real_vector(simu->Y_N7[0]);
				std::clog << std::endl;
			}
			// hard decode
			else
			{
				// decode
				std::clog << "Hard decode form Y_N5 to V_K..." << std::endl;
				auto t_decod = steady_clock::now();
				simu->decoder[0]->load  (simu->Y_N5[0]);
				simu->decoder[0]->decode(               );
				simu->decoder[0]->store (simu->V_K [0]);
				d_decod += steady_clock::now() - t_decod;

				// display V_K
				std::clog << "V_K:" << std::endl;
				ft.display_real_vector(simu->V_K[0]);
				std::clog << std::endl;
			}
		}

		// check errors in the frame
		auto t_check = steady_clock::now();
		simu->analyzer_red->check_errors(simu->U_K[0], simu->V_K[0]);
		auto d_check = steady_clock::now() - t_check;

		// increment total durations for each operations
		simu->d_sourc_total[0] += d_sourc;
		simu->d_crc_total  [0] += d_crc;
		simu->d_encod_total[0] += d_encod;
		simu->d_inter_total[0] += d_inter;
		simu->d_modul_total[0] += d_modul;
		simu->d_chann_total[0] += d_chann;
		simu->d_filte_total[0] += d_filte;
		simu->d_quant_total[0] += d_quant;
		simu->d_demod_total[0] += d_demod;
		simu->d_deint_total[0] += d_deint;
		simu->d_decod_total[0] += d_decod;
		simu->d_check_total[0] += d_check;

		// display statistics in terminal
		if (!simu->params.simulation.disable_display && simu->params.simulation.display_freq != nanoseconds(0) &&
		    (steady_clock::now() - t_simu) >= simu->params.simulation.display_freq)
		{
			simu->terminal->temp_report(std::clog);
			t_simu = steady_clock::now();
		}
	}

	simu->terminal->legend(std::clog);
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::time_reduction(const bool is_snr_done)
{
	using namespace std::chrono;

	d_sourc_total_red = nanoseconds(0);
	d_crc_total_red   = nanoseconds(0);
	d_encod_total_red = nanoseconds(0);
	d_inter_total_red = nanoseconds(0);
	d_modul_total_red = nanoseconds(0);
	d_chann_total_red = nanoseconds(0);
	d_filte_total_red = nanoseconds(0);
	d_quant_total_red = nanoseconds(0);
	d_demod_total_red = nanoseconds(0);
	d_deint_total_red = nanoseconds(0);
	d_decod_total_red = nanoseconds(0);
	d_check_total_red = nanoseconds(0);

	for (auto tid = 0; tid < params.simulation.n_threads; tid++)
	{
		d_sourc_total_red += d_sourc_total[tid];
		d_crc_total_red   += d_crc_total  [tid];
		d_encod_total_red += d_encod_total[tid];
		d_inter_total_red += d_inter_total[tid];
		d_modul_total_red += d_modul_total[tid];
		d_chann_total_red += d_chann_total[tid];
		d_filte_total_red += d_filte_total[tid];
		d_quant_total_red += d_quant_total[tid];
		d_demod_total_red += d_demod_total[tid];
		d_deint_total_red += d_deint_total[tid];
		d_decod_total_red += d_decod_total[tid];
		d_check_total_red += d_check_total[tid];
	}

	if (is_snr_done)
		for (auto tid = 0; tid < params.simulation.n_threads; tid++)
		{
			d_sourc_total_sum += d_sourc_total[tid];
			d_crc_total_sum   += d_crc_total  [tid];
			d_encod_total_sum += d_encod_total[tid];
			d_inter_total_sum += d_inter_total[tid];
			d_modul_total_sum += d_modul_total[tid];
			d_chann_total_sum += d_chann_total[tid];
			d_filte_total_sum += d_filte_total[tid];
			d_quant_total_sum += d_quant_total[tid];
			d_demod_total_sum += d_demod_total[tid];
			d_deint_total_sum += d_deint_total[tid];
			d_decod_total_sum += d_decod_total[tid];
			d_check_total_sum += d_check_total[tid];
		}
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::time_report(std::ostream &stream)
{
	using namespace std::chrono;

	auto d_total = d_sourc_total_sum + 
	               d_crc_total_sum   + 
	               d_encod_total_sum +
	               d_inter_total_sum +
	               d_modul_total_sum +
	               d_chann_total_sum +
	               d_filte_total_sum +
	               d_quant_total_sum + 
	               d_demod_total_sum +
	               d_decod_total_sum +
	               d_deint_total_sum +
	               d_check_total_sum;

	auto sourc_sec = ((float)d_sourc_total_sum.count()) * 0.000000001f;
	auto crc_sec   = ((float)d_crc_total_sum  .count()) * 0.000000001f;
	auto encod_sec = ((float)d_encod_total_sum.count()) * 0.000000001f;
	auto inter_sec = ((float)d_inter_total_sum.count()) * 0.000000001f;
	auto modul_sec = ((float)d_modul_total_sum.count()) * 0.000000001f;
	auto chann_sec = ((float)d_chann_total_sum.count()) * 0.000000001f;
	auto filte_sec = ((float)d_filte_total_sum.count()) * 0.000000001f;
	auto quant_sec = ((float)d_quant_total_sum.count()) * 0.000000001f;
	auto demod_sec = ((float)d_demod_total_sum.count()) * 0.000000001f;
	auto decod_sec = ((float)d_decod_total_sum.count()) * 0.000000001f;
	auto deint_sec = ((float)d_deint_total_sum.count()) * 0.000000001f;
	auto check_sec = ((float)d_check_total_sum.count()) * 0.000000001f;
	auto total_sec = ((float)d_total          .count()) * 0.000000001f;

	auto sourc_pc  = (sourc_sec / total_sec) * 100.f;
	auto crc_pc    = (crc_sec   / total_sec) * 100.f;
	auto encod_pc  = (encod_sec / total_sec) * 100.f;
	auto inter_pc  = (inter_sec / total_sec) * 100.f;
	auto modul_pc  = (modul_sec / total_sec) * 100.f;
	auto chann_pc  = (chann_sec / total_sec) * 100.f;
	auto filte_pc  = (filte_sec / total_sec) * 100.f;
	auto quant_pc  = (quant_sec / total_sec) * 100.f;
	auto demod_pc  = (demod_sec / total_sec) * 100.f;
	auto decod_pc  = (decod_sec / total_sec) * 100.f;
	auto deint_pc  = (deint_sec / total_sec) * 100.f;
	auto check_pc  = (check_sec / total_sec) * 100.f;

	stream << "#" << std::endl;
	stream << "# " << bold_underlined("Time report:") << std::endl;
	stream << "# " << bold("* Source") << "       : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << sourc_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << sourc_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* CRC") << "          : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << crc_sec   << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << crc_pc   << "%)" 
	       << std::endl;
	stream << "# " << bold("* Encoder") << "      : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << encod_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << encod_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* Interleaver") << "  : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << inter_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << inter_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* Modulator") << "    : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << modul_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << modul_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* Channel") << "      : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << chann_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << chann_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* Filter") << "       : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << filte_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << filte_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* Quantizer") << "    : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << quant_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << quant_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* Demodulator") << "  : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << demod_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << demod_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* Decoder") << "      : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << decod_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << decod_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* Deinterleaver") << ": " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << deint_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << deint_pc << "%)" 
	       << std::endl;
	stream << "# " << bold("* Check errors") << " : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << check_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << check_pc << "%)" 
	       << std::endl;
	stream << "#   -----------------------------------" << std::endl;
	stream << "# " << bold("* TOTAL") << "        : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << total_sec << " sec" 
	       << std::endl;
	stream << "#" << std::endl;
}

// ---------------------------------------------------------------------------------------------------- virtual methods

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::release_objects()
{
	int tid;
	const auto nthr = params.simulation.n_threads;
	for (tid = 0; tid < nthr; tid++) if (source     [tid] != nullptr) { delete source     [tid]; source     [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (crc        [tid] != nullptr) { delete crc        [tid]; crc        [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (encoder    [tid] != nullptr) { delete encoder    [tid]; encoder    [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (interleaver[tid] != nullptr) { delete interleaver[tid]; interleaver[tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (modulator  [tid] != nullptr) { delete modulator  [tid]; modulator  [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (channel    [tid] != nullptr) { delete channel    [tid]; channel    [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (quantizer  [tid] != nullptr) { delete quantizer  [tid]; quantizer  [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++)
		if (siso[tid] != nullptr)
		{
			// do not delete the siso if the decoder and the siso are the same pointers
			if (decoder[tid] == nullptr || siso[tid] != dynamic_cast<SISO<Q>*>(decoder[tid]))
				delete siso[tid];
			siso[tid] = nullptr;
		}
	for (tid = 0; tid < nthr; tid++) if (decoder    [tid] != nullptr) { delete decoder    [tid]; decoder    [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (analyzer   [tid] != nullptr) { delete analyzer   [tid]; analyzer   [tid] = nullptr; }
	
	if (analyzer_red != nullptr) { delete analyzer_red; analyzer_red = nullptr; }
	if (terminal     != nullptr) { delete terminal;     terminal     = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Source<B>* Simulation_BFERI<B,R,Q>
::build_source(const int tid)
{
	return Factory_source<B>::build(params, tid);
}

template <typename B, typename R, typename Q>
CRC<B>* Simulation_BFERI<B,R,Q>
::build_crc(const int tid)
{
	return Factory_CRC<B>::build(params);
}

template <typename B, typename R, typename Q>
Interleaver<int>* Simulation_BFERI<B,R,Q>
::build_interleaver(const int tid)
{
	return Factory_interleaver<int>::build(params, params.code.N + params.code.tail_length, 0);
}

template <typename B, typename R, typename Q>
Modulator<B,R,Q>* Simulation_BFERI<B,R,Q>
::build_modulator(const int tid)
{
	return Factory_modulator<B,R,Q>::build(params, sigma);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_BFERI<B,R,Q>
::build_channel(const int size, const int tid)
{
	return Factory_channel<R>::build(params, sigma, size, tid);
}

template <typename B, typename R, typename Q>
Quantizer<R,Q>* Simulation_BFERI<B,R,Q>
::build_quantizer(const int size, const int tid)
{
	return Factory_quantizer<R,Q>::build(params, sigma, size);
}

template <typename B, typename R, typename Q>
Error_analyzer<B>* Simulation_BFERI<B,R,Q>
::build_analyzer(const int tid)
{
	return Factory_error_analyzer<B>::build(params, n_frames);
}

// ------------------------------------------------------------------------------------------------- non-virtual method

template <typename B, typename R, typename Q>
Terminal* Simulation_BFERI<B,R,Q>
::build_terminal(const int tid)
{
	return Factory_terminal<B,R>::build(params, snr, analyzer_red, t_snr, d_decod_total_red);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFERI<B_8,R_8,Q_8>;
template class Simulation_BFERI<B_16,R_16,Q_16>;
template class Simulation_BFERI<B_32,R_32,Q_32>;
template class Simulation_BFERI<B_64,R_64,Q_64>;
#else
template class Simulation_BFERI<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif /* NOT SYSTEMC */
