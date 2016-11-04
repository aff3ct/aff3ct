#ifndef SYSTEMC

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
#include "Tools/Factory/Factory_terminal.hpp"

#include "Simulation_BFER.hpp"

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::Simulation_BFER(const parameters& params)
: Simulation_BFER_i<B,R,Q>(params),

  threads(params.simulation.n_threads -1),

  U_K (params.simulation.n_threads),
  X_N1(params.simulation.n_threads),
  X_N2(params.simulation.n_threads),
  X_N3(params.simulation.n_threads),
  H_N (params.simulation.n_threads),
  Y_N1(params.simulation.n_threads),
  Y_N2(params.simulation.n_threads),
  Y_N3(params.simulation.n_threads),
  Y_N4(params.simulation.n_threads),
  Y_N5(params.simulation.n_threads),
  V_K (params.simulation.n_threads),
  V_N (params.simulation.n_threads),

  monitor_red(nullptr),
  terminal   (nullptr),

  d_sourc_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_crc_total  (params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_encod_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_punct_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_modul_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_chann_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_filte_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_demod_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_quant_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_depun_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_corea_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_load_total (params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_decod_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_store_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_cobit_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),
  d_check_total(params.simulation.n_threads, std::chrono::nanoseconds(0)),

  d_sourc_total_sum(std::chrono::nanoseconds(0)),
  d_crc_total_sum  (std::chrono::nanoseconds(0)),
  d_encod_total_sum(std::chrono::nanoseconds(0)),
  d_punct_total_sum(std::chrono::nanoseconds(0)),
  d_modul_total_sum(std::chrono::nanoseconds(0)),
  d_chann_total_sum(std::chrono::nanoseconds(0)),
  d_filte_total_sum(std::chrono::nanoseconds(0)),
  d_demod_total_sum(std::chrono::nanoseconds(0)),
  d_quant_total_sum(std::chrono::nanoseconds(0)),
  d_depun_total_sum(std::chrono::nanoseconds(0)),
  d_corea_total_sum(std::chrono::nanoseconds(0)),
  d_load_total_sum (std::chrono::nanoseconds(0)),
  d_decod_total_sum(std::chrono::nanoseconds(0)),
  d_store_total_sum(std::chrono::nanoseconds(0)),
  d_cobit_total_sum(std::chrono::nanoseconds(0)),
  d_check_total_sum(std::chrono::nanoseconds(0))
{
	if (params.simulation.n_threads > 1 && params.simulation.debug)
		std::clog << bold_yellow("(WW) Debug mode will be disabled ")
		          << bold_yellow("because you launched the simulation with more than 1 thread!")
		          << std::endl;
}

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::~Simulation_BFER()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::_launch()
{
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1] = std::thread(Simulation_BFER<B,R,Q>::Monte_Carlo_method, this, tid);

	// launch the master thread
	Simulation_BFER<B,R,Q>::Monte_Carlo_method(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1].join();

	if (!this->params.terminal.disabled && !this->params.simulation.benchs)
	{
		monitor_red->reduce();
		time_reduction(true);
		terminal->final_report(std::cout);
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::release_objects()
{
	Simulation_BFER_i<B,R,Q>::release_objects();

	if (monitor_red != nullptr) { delete monitor_red; monitor_red = nullptr; }
	if (terminal    != nullptr) { delete terminal;    terminal    = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::launch_postcompute()
{
	if (this->params.simulation.time_report && !this->params.simulation.benchs)
		time_report();
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::build_communication_chain(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	Simulation_BFER_i<B,R,Q>::build_communication_chain(simu, tid);

	// get the real number of frames per threads (from the decoder)
	const auto n_fra = simu->decoder[tid]->get_n_frames();

	// resize the buffers
	const auto N      = simu->params.code.N;
	const auto K      = simu->params.code.K;
	const auto tail   = simu->params.code.tail_length;
	const auto N_mod  = simu->modulator[tid]->get_buffer_size_after_modulation(N + tail);
	const auto N_code = simu->params.code.N_code;
	const auto N_fil  = simu->modulator[tid]->get_buffer_size_after_filtering (N + tail);

	if (simu->U_K [tid].size() != (unsigned) ( K              * n_fra)) simu->U_K [tid].resize( K              * n_fra);
	if (simu->X_N1[tid].size() != (unsigned) ((N_code + tail) * n_fra)) simu->X_N1[tid].resize((N_code + tail) * n_fra);
	if (simu->X_N2[tid].size() != (unsigned) ((N      + tail) * n_fra)) simu->X_N2[tid].resize((N      + tail) * n_fra);
	if (simu->X_N3[tid].size() != (unsigned) ( N_mod          * n_fra)) simu->X_N3[tid].resize( N_mod          * n_fra);
	if (simu->Y_N1[tid].size() != (unsigned) ( N_mod          * n_fra)) simu->Y_N1[tid].resize( N_mod          * n_fra);
	if (simu->H_N [tid].size() != (unsigned) ( N_mod          * n_fra)) simu->H_N [tid].resize( N_mod          * n_fra);
	if (simu->Y_N2[tid].size() != (unsigned) ( N_fil          * n_fra)) simu->Y_N2[tid].resize( N_fil          * n_fra);
	if (simu->Y_N3[tid].size() != (unsigned) ((N      + tail) * n_fra)) simu->Y_N3[tid].resize((N      + tail) * n_fra);
	if (simu->Y_N4[tid].size() != (unsigned) ((N      + tail) * n_fra)) simu->Y_N4[tid].resize((N      + tail) * n_fra);
	if (simu->Y_N5[tid].size() != (unsigned) ((N_code + tail) * n_fra)) simu->Y_N5[tid].resize((N_code + tail) * n_fra);
	if (simu->V_K [tid].size() != (unsigned) ( K              * n_fra)) simu->V_K [tid].resize( K              * n_fra);
	if (simu->V_N [tid].size() != (unsigned) ((N_code + tail) * n_fra)) simu->V_N [tid].resize((N_code + tail) * n_fra);

	simu->barrier(tid);
	if (tid == 0)
	{
		// build a monitor to compute BER/FER (reduce the other monitors)
		simu->monitor_red = new Monitor_reduction<B>(simu->params.code.K,
		                                             simu->params.code.N,
		                                             simu->params.monitor.n_frame_errors,
		                                             simu->monitor,
		                                             n_fra);
		// build the terminal to display the BER/FER
		simu->terminal = simu->build_terminal(tid);
		Simulation::check_errors(simu->terminal, "Terminal");
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::Monte_Carlo_method(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	Simulation_BFER<B,R,Q>::build_communication_chain(simu, tid);

	if (tid == 0 && (!simu->params.terminal.disabled && simu->snr == simu->params.simulation.snr_min &&
	    !(simu->params.simulation.debug && simu->params.simulation.n_threads == 1) && !simu->params.simulation.benchs))
		simu->terminal->legend(std::cout);

	if (simu->params.source.type == "AZCW")
	{
		std::fill(simu->U_K [tid].begin(), simu->U_K [tid].end(), (B)0);
		std::fill(simu->X_N1[tid].begin(), simu->X_N1[tid].end(), (B)0);
		std::fill(simu->X_N2[tid].begin(), simu->X_N2[tid].end(), (B)0);
		simu->modulator[tid]->modulate(simu->X_N2[tid], simu->X_N3[tid]);
	}

	simu->d_sourc_total[tid] = std::chrono::nanoseconds(0);
	simu->d_crc_total  [tid] = std::chrono::nanoseconds(0);
	simu->d_encod_total[tid] = std::chrono::nanoseconds(0);
	simu->d_punct_total[tid] = std::chrono::nanoseconds(0);
	simu->d_modul_total[tid] = std::chrono::nanoseconds(0);
	simu->d_chann_total[tid] = std::chrono::nanoseconds(0);
	simu->d_filte_total[tid] = std::chrono::nanoseconds(0);
	simu->d_demod_total[tid] = std::chrono::nanoseconds(0);
	simu->d_quant_total[tid] = std::chrono::nanoseconds(0);
	simu->d_depun_total[tid] = std::chrono::nanoseconds(0);
	simu->d_corea_total[tid] = std::chrono::nanoseconds(0);
	simu->d_load_total [tid] = std::chrono::nanoseconds(0);
	simu->d_decod_total[tid] = std::chrono::nanoseconds(0);
	simu->d_store_total[tid] = std::chrono::nanoseconds(0);
	simu->d_cobit_total[tid] = std::chrono::nanoseconds(0);
	simu->d_check_total[tid] = std::chrono::nanoseconds(0);

	simu->barrier(tid);

	if (simu->params.simulation.n_threads == 1 && simu->params.simulation.debug)
		Simulation_BFER<B,R,Q>::simulation_loop_debug(simu);
	else if (simu->params.simulation.benchs)
		Simulation_BFER<B,R,Q>::simulation_loop_bench(simu, tid);
	else
		Simulation_BFER<B,R,Q>::simulation_loop(simu, tid);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::simulation_loop(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	using namespace std::chrono;

	if (tid == 0)
		simu->t_simu = steady_clock::now();

	int prev_n_fe = 0;

	// simulation loop
	while ((!simu->monitor_red->fe_limit_achieved()) && // while max frame error count has not been reached
	        (simu->params.simulation.stop_time == seconds(0) ||
	         (steady_clock::now() - simu->t_snr) < simu->params.simulation.stop_time))
	{
		auto d_sourc = nanoseconds(0);
		auto d_crc   = nanoseconds(0);
		auto d_encod = nanoseconds(0);
		auto d_punct = nanoseconds(0);
		auto d_modul = nanoseconds(0);
		auto d_chann = nanoseconds(0);
		auto d_filte = nanoseconds(0);
		auto d_demod = nanoseconds(0);
		auto d_corea = nanoseconds(0);
		auto d_cobit = nanoseconds(0);

		if (simu->params.source.type != "AZCW")
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

			// puncture X_N1 into X_N2
			auto t_punct = steady_clock::now();
			simu->puncturer[tid]->puncture(simu->X_N1[tid], simu->X_N2[tid]);
			d_punct = steady_clock::now() - t_punct;

			// modulate
			auto t_modul = steady_clock::now();
			simu->modulator[tid]->modulate(simu->X_N2[tid], simu->X_N3[tid]);
			d_modul = steady_clock::now() - t_modul;
		}

		// Rayleigh channel
		if (simu->params.channel.type.find("RAYLEIGH") != std::string::npos)
		{
			// add noise
			auto t_chann = steady_clock::now();
			simu->channel[tid]->add_noise(simu->X_N3[tid], simu->Y_N1[tid], simu->H_N[tid]);
			d_chann = steady_clock::now() - t_chann;

			// filtering
			auto t_filte = steady_clock::now();
			simu->modulator[tid]->filter(simu->Y_N1[tid], simu->Y_N2[tid]);
			d_filte = steady_clock::now() - t_filte;

			// demodulation
			auto t_demod = steady_clock::now();
			simu->modulator[tid]->demodulate_with_gains(simu->Y_N2[tid], simu->H_N[tid], simu->Y_N3[tid]);
			d_demod = steady_clock::now() - t_demod;
		}
		else // additive channel (AWGN, USER, NO)
		{
			// add noise
			auto t_chann = steady_clock::now();
			simu->channel[tid]->add_noise(simu->X_N3[tid], simu->Y_N1[tid]);
			d_chann = steady_clock::now() - t_chann;

			// filtering
			auto t_filte = steady_clock::now();
			simu->modulator[tid]->filter(simu->Y_N1[tid], simu->Y_N2[tid]);
			d_filte = steady_clock::now() - t_filte;

			// demodulation
			auto t_demod = steady_clock::now();
			simu->modulator[tid]->demodulate(simu->Y_N2[tid], simu->Y_N3[tid]);
			d_demod = steady_clock::now() - t_demod;
		}

		// make the quantization
		auto t_quant = steady_clock::now();
		simu->quantizer[tid]->process(simu->Y_N3[tid], simu->Y_N4[tid]);
		auto d_quant = steady_clock::now() - t_quant;

		// depuncture before the decoding stage
		auto t_depun = steady_clock::now();
		simu->puncturer[tid]->depuncture(simu->Y_N4[tid], simu->Y_N5[tid]);
		auto d_depun = steady_clock::now() - t_depun;

		// apply the coset: the decoder will believe to a AZCW
		if (simu->params.code.coset)
		{
			auto t_corea = steady_clock::now();
			simu->coset_real[tid]->apply(simu->X_N1[tid], simu->Y_N5[tid], simu->Y_N5[tid]);
			d_corea = steady_clock::now() - t_corea;
		}

		// launch decoder
		simu->decoder[tid]->hard_decode(simu->Y_N5[tid], simu->V_K[tid]);

		// apply the coset to recover the real bits
		if (simu->params.code.coset)
		{
			auto t_cobit = steady_clock::now();
			simu->coset_bit[tid]->apply(simu->U_K[tid], simu->V_K[tid], simu->V_K[tid]);
			d_cobit = steady_clock::now() - t_cobit;
		}

		// check errors in the frame
		auto t_check = steady_clock::now();
		simu->monitor[tid]->check_errors(simu->U_K[tid], simu->V_K[tid]);
		auto d_check = steady_clock::now() - t_check;

		// update the total number of frame errors if needed
		if (simu->monitor[tid]->get_n_fe() > prev_n_fe)
		{
			simu->monitor_red->increment_frame_errors(simu->monitor[tid]->get_n_fe() - prev_n_fe);
			prev_n_fe = simu->monitor[tid]->get_n_fe();
		}

		// increment total durations for each operations
		simu->d_sourc_total[tid] += d_sourc;
		simu->d_crc_total  [tid] += d_crc;
		simu->d_encod_total[tid] += d_encod;
		simu->d_punct_total[tid] += d_punct;
		simu->d_modul_total[tid] += d_modul;
		simu->d_chann_total[tid] += d_chann;
		simu->d_filte_total[tid] += d_filte;
		simu->d_demod_total[tid] += d_demod;
		simu->d_quant_total[tid] += d_quant;
		simu->d_depun_total[tid] += d_depun;
		simu->d_corea_total[tid] += d_corea;
		simu->d_load_total [tid] += simu->decoder[tid]->get_load_duration();
		simu->d_decod_total[tid] += simu->decoder[tid]->get_decode_duration();
		simu->d_store_total[tid] += simu->decoder[tid]->get_store_duration();
		simu->d_cobit_total[tid] += d_cobit;
		simu->d_check_total[tid] += d_check;

		// save trace in file
		if (tid == 0 && !simu->params.simulation.trace_path.empty())
			trace(simu);

		// display statistics in terminal
		if (tid == 0 && !simu->params.terminal.disabled && simu->params.terminal.frequency != nanoseconds(0) &&
		    (steady_clock::now() - simu->t_simu) >= simu->params.terminal.frequency)
		{
			simu->monitor_red->reduce();
			simu->time_reduction();
			simu->terminal->temp_report(std::clog);
			simu->t_simu = steady_clock::now();
		}
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::simulation_loop_bench(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	using namespace std::chrono;

	simu->barrier(tid);
	auto t_start = std::chrono::steady_clock::now(); // start time
	simu->barrier(tid);
	if (simu->params.simulation.benchs_no_ldst)
		for (auto i = 0; i < simu->params.simulation.benchs; i++)
			simu->decoder[tid]->hard_decode(simu->Y_N4[tid], simu->V_N[tid], false, false, false, false);
	else
		for (auto i = 0; i < simu->params.simulation.benchs; i++)
			simu->decoder[tid]->hard_decode(simu->Y_N4[tid], simu->V_N[tid], true, true, true, false);
	simu->barrier(tid);
	auto t_stop = std::chrono::steady_clock::now(); // stop time

	auto frames   = (float)simu->params.simulation.benchs *
	                (float)simu->params.simulation.inter_frame_level *
	                (float)simu->params.simulation.n_threads;
	auto bits     = (float)frames * (float)simu->params.code.K;
	auto duration = t_stop - t_start;

	auto  bps = (float)bits / (float)(duration.count() * 0.000000001f);
	auto kbps =  bps * 0.001f;
	auto mbps = kbps * 0.001f;

	auto latency_ns = (float)duration.count() / (float)simu->params.simulation.benchs;
	auto latency_us = latency_ns * 0.001f;

	if (tid == 0)
		std::cout << "  SNR (Eb/N0) = "          << std::setw(5) << std::fixed << std::setprecision(2) << simu->snr  
		          << " dB"   << ", "
		          << "information throughput = " << std::setw(8) << std::fixed << std::setprecision(4) << mbps       
		          << " Mbps" << ", " 
		          << "latency = "                << std::setw(8) << std::fixed << std::setprecision(4) << latency_us 
		          << " us."  << std::endl;
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::simulation_loop_debug(Simulation_BFER<B,R,Q> *simu)
{
	using namespace std::chrono;

	Frame_trace<B> ft(simu->params.simulation.debug_limit); // frame trace to display the vectors

	// simulation loop
	auto t_simu = steady_clock::now();
	while (!simu->monitor_red->fe_limit_achieved() && // while max frame error count has not been reached
	       (simu->params.simulation.stop_time == seconds(0) ||
	        (steady_clock::now() - simu->t_snr) < simu->params.simulation.stop_time))
	{
		std::clog << "-------------------------------" << std::endl;
		std::clog << "New encoding/decoding session !" << std::endl;
		std::clog << "Frame n°" << simu->monitor_red->get_n_analyzed_fra() << std::endl;
		std::clog << "-------------------------------" << std::endl;

		auto d_sourc = nanoseconds(0);
		auto d_crc   = nanoseconds(0);
		auto d_encod = nanoseconds(0);
		auto d_punct = nanoseconds(0);
		auto d_modul = nanoseconds(0);
		auto d_corea = nanoseconds(0);
		auto d_cobit = nanoseconds(0);
		auto d_chann = nanoseconds(0);
		auto d_filte = nanoseconds(0);
		auto d_demod = nanoseconds(0);

		if (simu->params.source.type != "AZCW")
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
			std::clog << "Puncture X_N1 in X_N2..." << std::endl;
			auto t_punct = steady_clock::now();
			simu->puncturer[0]->puncture(simu->X_N1[0], simu->X_N2[0]);
			d_punct = steady_clock::now() - t_punct;

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

		// Rayleigh channel
		if (simu->params.channel.type.find("RAYLEIGH") != std::string::npos)
		{
			// add noise
			std::clog << "Add noise from X_N3 to Y_N1..." << std::endl;
			auto t_chann = steady_clock::now();
			simu->channel[0]->add_noise(simu->X_N3[0], simu->Y_N1[0], simu->H_N[0]);
			d_chann = steady_clock::now() - t_chann;

			// display Y_N1
			std::clog << "Y_N1:" << std::endl;
			ft.display_real_vector(simu->Y_N1[0]);
			std::clog << std::endl;

			// display channel gains
			std::clog << "H_N:" << std::endl;
			ft.display_real_vector(simu->H_N[0]);
			std::clog << std::endl;

			// filtering
			std::clog << "Filter from Y_N1 to Y_N2..." << std::endl;
			auto t_filte = steady_clock::now();
			simu->modulator[0]->filter(simu->Y_N1[0], simu->Y_N2[0]);
			d_filte = steady_clock::now() - t_filte;

			// display Y_N2
			std::clog << "Y_N2:" << std::endl;
			ft.display_real_vector(simu->Y_N2[0]);
			std::clog << std::endl;

			// demodulation
			std::clog << "Demodulate from Y_N2 to Y_N3..." << std::endl;
			auto t_demod = steady_clock::now();
			simu->modulator[0]->demodulate_with_gains(simu->Y_N2[0], simu->H_N[0], simu->Y_N3[0]);
			d_demod = steady_clock::now() - t_demod;

			// display Y_N3
			std::clog << "Y_N3:" << std::endl;
			ft.display_real_vector(simu->Y_N3[0]);
			std::clog << std::endl;
		}
		else // additive channel (AWGN, USER, NO)
		{
			// add noise
			std::clog << "Add noise from X_N3 to Y_N1..." << std::endl;
			auto t_chann = steady_clock::now();
			simu->channel[0]->add_noise(simu->X_N3[0], simu->Y_N1[0]);
			d_chann = steady_clock::now() - t_chann;

			// display Y_N1
			std::clog << "Y_N1:" << std::endl;
			ft.display_real_vector(simu->Y_N1[0]);
			std::clog << std::endl;

			// filtering
			std::clog << "Filter from Y_N1 to Y_N2..." << std::endl;
			auto t_filte = steady_clock::now();
			simu->modulator[0]->filter(simu->Y_N1[0], simu->Y_N2[0]);
			d_filte = steady_clock::now() - t_filte;

			// display Y_N2
			std::clog << "Y_N2:" << std::endl;
			ft.display_real_vector(simu->Y_N2[0]);
			std::clog << std::endl;

			// demodulation
			std::clog << "Demodulate from Y_N2 to Y_N3..." << std::endl;
			auto t_demod = steady_clock::now();
			simu->modulator[0]->demodulate(simu->Y_N2[0], simu->Y_N3[0]);
			d_demod = steady_clock::now() - t_demod;

			// display Y_N3
			std::clog << "Y_N3:" << std::endl;
			ft.display_real_vector(simu->Y_N3[0]);
			std::clog << std::endl;
		}

		// make the quantization
		std::clog << "Make the quantization from Y_N3 to Y_N4..." << std::endl;
		auto t_quant = steady_clock::now();
		simu->quantizer[0]->process(simu->Y_N3[0], simu->Y_N4[0]);
		auto d_quant = steady_clock::now() - t_quant;

		// display Y_N4
		std::clog << "Y_N4:" << std::endl;
		ft.display_real_vector(simu->Y_N4[0]);
		std::clog << std::endl;

		// depuncture before the decoding stage
		std::clog << "Depuncture Y_N4 and generate Y_N5..." << std::endl;
		auto t_depun = steady_clock::now();
		simu->puncturer[0]->depuncture(simu->Y_N4[0], simu->Y_N5[0]);
		auto d_depun = steady_clock::now() - t_depun;

		// display Y_N5
		std::clog << "Y_N5:" << std::endl;
		ft.display_real_vector(simu->Y_N5[0]);
		std::clog << std::endl;

		// apply the coset: the decoder will believe to a AZCW
		if (simu->params.code.coset)
		{
			std::clog << "Apply the coset approach on Y_N5..." << std::endl;
			auto t_corea = steady_clock::now();
			simu->coset_real[0]->apply(simu->X_N1[0], simu->Y_N5[0], simu->Y_N5[0]);
			d_corea = steady_clock::now() - t_corea;

			// display Y_N5
			std::clog << "Y_N5:" << std::endl;
			ft.display_real_vector(simu->Y_N5[0]);
			std::clog << std::endl;
		}
		
		// launch decoder
		std::clog << "Decode Y_N5 and generate V_K..." << std::endl;
		simu->decoder[0]->hard_decode(simu->Y_N5[0], simu->V_K[0]);

		// display V_K
		std::clog << "V_K:" << std::endl;
		if (simu->params.code.coset)
			ft.display_bit_vector(simu->V_K[0]);
		else
			ft.display_bit_vector(simu->V_K[0], simu->U_K[0]);
		std::clog << std::endl;

		// apply the coset to recover the real bits
		if (simu->params.code.coset)
		{
			std::clog << "Apply the coset approach on V_K..." << std::endl;
			auto t_cobit = steady_clock::now();
			simu->coset_bit[0]->apply(simu->U_K[0], simu->V_K[0], simu->V_K[0]);
			d_cobit = steady_clock::now() - t_cobit;

			// display V_K
			std::clog << "V_K:" << std::endl;
			ft.display_bit_vector(simu->V_K[0], simu->U_K[0]);
			std::clog << std::endl;
		}

		// check errors in the frame
		auto t_check = steady_clock::now();
		simu->monitor_red->check_errors(simu->U_K[0], simu->V_K[0]);
		auto d_check = steady_clock::now() - t_check;

		// increment total durations for each operations
		simu->d_sourc_total[0] += d_sourc;
		simu->d_crc_total  [0] += d_crc;
		simu->d_encod_total[0] += d_encod;
		simu->d_punct_total[0] += d_punct;
		simu->d_modul_total[0] += d_modul;
		simu->d_chann_total[0] += d_chann;
		simu->d_filte_total[0] += d_filte;
		simu->d_demod_total[0] += d_demod;
		simu->d_quant_total[0] += d_quant;
		simu->d_depun_total[0] += d_depun;
		simu->d_corea_total[0] += d_corea;
		simu->d_load_total [0] += simu->decoder[0]->get_load_duration();
		simu->d_decod_total[0] += simu->decoder[0]->get_decode_duration();
		simu->d_store_total[0] += simu->decoder[0]->get_store_duration();
		simu->d_cobit_total[0] += d_cobit;
		simu->d_check_total[0] += d_check;

		// save trace in file
		if (!simu->params.simulation.trace_path.empty())
			trace(simu);

		// display statistics in terminal
		if (!simu->params.terminal.disabled && simu->params.terminal.frequency != nanoseconds(0) &&
		    (steady_clock::now() - t_simu) >= simu->params.terminal.frequency)
		{
			simu->terminal->temp_report(std::clog);
			t_simu = steady_clock::now();
		}
	}

	simu->terminal->legend(std::cout);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::trace(Simulation_BFER<B,R,Q> *simu)
{
	std::ofstream myfile;
	myfile.open(simu->params.simulation.trace_path);
	myfile << "U_K, \t X_N1, \t X_N2, \t X_N3_I, \t X_N3_Q, \t Y_N1_I, \t Y_N1_Q, \t Y_N2, \t Y_N3, \t Y_N4, \t V_K" 
	       << std::endl;

	for (unsigned l_idx = 0; l_idx < simu->Y_N2[0].size(); l_idx++)
	{
		if (l_idx < simu->U_K [0].size()) myfile << simu->U_K [0][l_idx]; myfile << ", \t ";
		if (l_idx < simu->X_N1[0].size()) myfile << simu->X_N1[0][l_idx]; myfile << ", \t ";
		if (l_idx < simu->X_N2[0].size()) myfile << simu->X_N2[0][l_idx]; myfile << ", \t ";
		if (simu->params.modulator.type == "PAM"  ||
		    simu->params.modulator.type == "BPSK" ||
		    simu->params.modulator.type == "BPSK_FAST")
		{
			if (l_idx < simu->X_N3[0].size()) myfile << simu->X_N3[0][l_idx] << ", \t " << 0; 
			else myfile << ", \t ";
			myfile << ", \t ";

			if (l_idx < simu->Y_N1[0].size()) myfile << simu->Y_N1[0][l_idx] << ", \t " << 0; 
			else myfile << ", \t ";
			myfile << ", \t ";
		}
		else
		{
			if (2*l_idx < simu->X_N3[0].size()) myfile << simu->X_N3[0][2*l_idx] << ", \t " << simu->X_N3[0][2*l_idx+1];
			else myfile << ", \t ";
			myfile << ", \t ";

			if (2*l_idx < simu->Y_N1[0].size()) myfile << simu->Y_N1[0][2*l_idx] << ", \t " << simu->Y_N1[0][2*l_idx+1];
			else myfile << ", \t ";
			myfile << ", \t ";
		}
		if (l_idx < simu->Y_N2[0].size()) myfile << simu->Y_N2[0][l_idx]; myfile << ", \t ";
		if (l_idx < simu->Y_N3[0].size()) myfile << simu->Y_N3[0][l_idx]; myfile << ", \t ";
		if (l_idx < simu->Y_N4[0].size()) myfile << simu->Y_N4[0][l_idx]; myfile << ", \t ";
		if (l_idx < simu->V_K [0].size()) myfile << simu->V_K [0][l_idx]; myfile << std::endl;
	}
	myfile.close();
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::time_reduction(const bool is_snr_done)
{
	using namespace std::chrono;

	d_sourc_total_red = nanoseconds(0);
	d_crc_total_red   = nanoseconds(0);
	d_encod_total_red = nanoseconds(0);
	d_punct_total_red = nanoseconds(0);
	d_modul_total_red = nanoseconds(0);
	d_chann_total_red = nanoseconds(0);
	d_demod_total_red = nanoseconds(0);
	d_filte_total_red = nanoseconds(0);
	d_quant_total_red = nanoseconds(0);
	d_depun_total_red = nanoseconds(0);
	d_corea_total_red = nanoseconds(0);
	d_load_total_red  = nanoseconds(0);
	d_decod_total_red = nanoseconds(0);
	d_store_total_red = nanoseconds(0);
	d_decod_all_red   = nanoseconds(0);
	d_cobit_total_red = nanoseconds(0);
	d_check_total_red = nanoseconds(0);

	for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
	{
		d_sourc_total_red += d_sourc_total[tid];
		d_crc_total_red   += d_crc_total  [tid];
		d_encod_total_red += d_encod_total[tid];
		d_punct_total_red += d_punct_total[tid];
		d_modul_total_red += d_modul_total[tid];
		d_chann_total_red += d_chann_total[tid];
		d_filte_total_red += d_filte_total[tid];
		d_demod_total_red += d_demod_total[tid];
		d_quant_total_red += d_quant_total[tid];
		d_depun_total_red += d_depun_total[tid];
		d_corea_total_red += d_corea_total[tid];
		d_load_total_red  += d_load_total [tid];
		d_decod_total_red += d_decod_total[tid];
		d_store_total_red += d_store_total[tid];
		d_decod_all_red   += (d_load_total[tid] + d_decod_total[tid] + d_store_total[tid]);
		d_cobit_total_red += d_cobit_total[tid];
		d_check_total_red += d_check_total[tid];
	}

	if (is_snr_done)
		for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
		{
			d_sourc_total_sum += d_sourc_total[tid];
			d_crc_total_sum   += d_crc_total  [tid];
			d_encod_total_sum += d_encod_total[tid];
			d_punct_total_sum += d_punct_total[tid];
			d_modul_total_sum += d_modul_total[tid];
			d_chann_total_sum += d_chann_total[tid];
			d_filte_total_sum += d_filte_total[tid];
			d_demod_total_sum += d_demod_total[tid];
			d_quant_total_sum += d_quant_total[tid];
			d_depun_total_sum += d_depun_total[tid];
			d_corea_total_sum += d_corea_total[tid];
			d_load_total_sum  += d_load_total [tid];
			d_decod_total_sum += d_decod_total[tid];
			d_store_total_sum += d_store_total[tid];
			d_cobit_total_sum += d_cobit_total[tid];
			d_check_total_sum += d_check_total[tid];
		}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::time_report(std::ostream &stream)
{
	using namespace std::chrono;

	auto d_total = d_sourc_total_sum + 
	               d_crc_total_sum   + 
	               d_encod_total_sum +
	               d_punct_total_sum +
	               d_modul_total_sum +
	               d_chann_total_sum +
	               d_filte_total_sum +
	               d_demod_total_sum +
	               d_quant_total_sum + 
	               d_depun_total_sum + 
	               d_corea_total_sum +
	               d_load_total_sum  +
	               d_decod_total_sum +
	               d_store_total_sum +
	               d_cobit_total_sum +
	               d_check_total_sum;

	auto sourc_sec     = ((float)d_sourc_total_sum.count()) * 0.000000001f;
	auto crc_sec       = ((float)d_crc_total_sum  .count()) * 0.000000001f;
	auto encod_sec     = ((float)d_encod_total_sum.count()) * 0.000000001f;
	auto punct_sec     = ((float)d_punct_total_sum.count()) * 0.000000001f;
	auto modul_sec     = ((float)d_modul_total_sum.count()) * 0.000000001f;
	auto chann_sec     = ((float)d_chann_total_sum.count()) * 0.000000001f;
	auto filte_sec     = ((float)d_filte_total_sum.count()) * 0.000000001f;
	auto demod_sec     = ((float)d_demod_total_sum.count()) * 0.000000001f;
	auto quant_sec     = ((float)d_quant_total_sum.count()) * 0.000000001f;
	auto depun_sec     = ((float)d_depun_total_sum.count()) * 0.000000001f;
	auto corea_sec     = ((float)d_corea_total_sum.count()) * 0.000000001f;
	auto load_sec      = ((float)d_load_total_sum .count()) * 0.000000001f;
	auto decod_sec     = ((float)d_decod_total_sum.count()) * 0.000000001f;
	auto store_sec     = ((float)d_store_total_sum.count()) * 0.000000001f;
	auto decod_tot_sec = load_sec + decod_sec + store_sec;
	auto cobit_sec     = ((float)d_cobit_total_sum.count()) * 0.000000001f;
	auto check_sec     = ((float)d_check_total_sum.count()) * 0.000000001f;
	auto total_sec     = ((float)d_total          .count()) * 0.000000001f;

	auto sourc_pc     = (sourc_sec     / total_sec)     * 100.f;
	auto crc_pc       = (crc_sec       / total_sec)     * 100.f;
	auto encod_pc     = (encod_sec     / total_sec)     * 100.f;
	auto punct_pc     = (punct_sec     / total_sec)     * 100.f;
	auto modul_pc     = (modul_sec     / total_sec)     * 100.f;
	auto chann_pc     = (chann_sec     / total_sec)     * 100.f;
	auto filte_pc     = (filte_sec     / total_sec)     * 100.f;
	auto demod_pc     = (demod_sec     / total_sec)     * 100.f;
	auto quant_pc     = (quant_sec     / total_sec)     * 100.f;
	auto depun_pc     = (depun_sec     / total_sec)     * 100.f;
	auto corea_pc     = (corea_sec     / total_sec)     * 100.f;
	auto decod_tot_pc = (decod_tot_sec / total_sec)     * 100.f;
	auto load_pc      = (load_sec      / decod_tot_sec) * 100.f;
	auto decod_pc     = (decod_sec     / decod_tot_sec) * 100.f;
	auto store_pc     = (store_sec     / decod_tot_sec) * 100.f;
	auto cobit_pc     = (cobit_sec     / total_sec)     * 100.f;
	auto check_pc     = (check_sec     / total_sec)     * 100.f;

	stream << "#" << std::endl;
	stream << "# " << bold_underlined("Time report:") << std::endl;
	stream << "# " << bold           ("* Source") << "      : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << sourc_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << sourc_pc     << "%)" 
	       << std::endl;
	stream << "# " << bold           ("* CRC") << "         : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << crc_sec       << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << crc_pc       << "%)" 
	       << std::endl;
	stream << "# " << bold           ("* Encoder") << "     : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << encod_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << encod_pc     << "%)" 
	       << std::endl;
	stream << "# " << bold           ("* Puncturer") << "   : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << punct_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << punct_pc     << "%)" 
	       << std::endl;
	stream << "# " << bold           ("* Modulator") << "   : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << modul_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << modul_pc     << "%)" 
	       << std::endl;
	stream << "# " << bold           ("* Channel") << "     : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << chann_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << chann_pc     << "%)" 
	       << std::endl;
	stream << "# " << bold           ("* Filter") << "      : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << filte_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << filte_pc     << "%)" 
	       << std::endl;
	stream << "# " << bold           ("* Demodulator") << " : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << demod_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << demod_pc     << "%)" 
	       << std::endl;
	stream << "# " << bold           ("* Quantizer") << "   : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << quant_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << quant_pc     << "%)" 
	       << std::endl;
	stream << "# " << bold           ("* Depuncturer") << " : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << depun_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << depun_pc     << "%)" 
	       << std::endl;
	if (this->params.code.coset)
	stream << "# " << bold           ("* Coset real") << "  : " << std::setw(9) << std::fixed << std::setprecision(3)
	       << corea_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << corea_pc     << "%)"
	       << std::endl;
	stream << "# " << bold           ("* Decoder") << "     : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << decod_tot_sec << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << decod_tot_pc << "%)" 
	       << std::endl;
	stream << "# " << bold_italic    ("  - load") << "      : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << load_sec      << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << load_pc      << "%)" 
	       << std::endl;
	stream << "# " << bold_italic    ("  - decode") << "    : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << decod_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << decod_pc     << "%)" 
	       << std::endl;
	stream << "# " << bold_italic    ("  - store") << "     : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << store_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << store_pc     << "%)" 
	       << std::endl;
	if (this->params.code.coset)
	stream << "# " << bold           ("* Coset bit") << "   : " << std::setw(9) << std::fixed << std::setprecision(3)
	       << cobit_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << cobit_pc     << "%)"
	       << std::endl;
	stream << "# " << bold           ("* Check errors") << ": " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << check_sec     << " sec (" << std::setw(5) << std::fixed << std::setprecision(2) << check_pc     << "%)" 
	       << std::endl;
	stream << "#   ----------------------------------" << std::endl;
	stream << "# " << bold           ("* TOTAL") << "       : " << std::setw(9) << std::fixed << std::setprecision(3) 
	       << total_sec     << " sec" 
	       << std::endl;
	stream << "#" << std::endl;
}

template <typename B, typename R, typename Q>
Terminal* Simulation_BFER<B,R,Q>
::build_terminal(const int tid)
{
	return Factory_terminal<B,R>::build(this->params, this->snr, monitor_red, this->t_snr, d_decod_all_red);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFER<B_8,R_8,Q_8>;
template class Simulation_BFER<B_16,R_16,Q_16>;
template class Simulation_BFER<B_32,R_32,Q_32>;
template class Simulation_BFER<B_64,R_64,Q_64>;
#else
template class Simulation_BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif /* NOT SYSTEMC */
