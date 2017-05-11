#if !defined(SYSTEMC) && !defined(STARPU)

#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <exception>

#ifdef ENABLE_MPI
#include "Module/Monitor/Standard/Monitor_reduction_mpi.hpp"
#endif

#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Factory/Factory_terminal.hpp"

#include "STD_Simulation_BFER.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::Simulation_BFER(const parameters& params)
: Simulation_BFER_i<B,R,Q>(params),

  threads(this->params.simulation.n_threads -1),

  U_K1(this->params.simulation.n_threads, mipp::vector<B>((params.code.K      - params.crc .size)        * params.simulation.inter_frame_level)),
  U_K2(this->params.simulation.n_threads, mipp::vector<B>( params.code.K                                 * params.simulation.inter_frame_level)),
  X_N1(this->params.simulation.n_threads, mipp::vector<B>((params.code.N_code + params.code.tail_length) * params.simulation.inter_frame_level)),
  X_N2(this->params.simulation.n_threads, mipp::vector<B>((params.code.N      + params.code.tail_length) * params.simulation.inter_frame_level)),
  X_N3(this->params.simulation.n_threads, mipp::vector<R>( params.code.N_mod                             * params.simulation.inter_frame_level)),
  H_N (this->params.simulation.n_threads, mipp::vector<R>( params.code.N_mod                             * params.simulation.inter_frame_level)),
  Y_N1(this->params.simulation.n_threads, mipp::vector<R>( params.code.N_mod                             * params.simulation.inter_frame_level)),
  Y_N2(this->params.simulation.n_threads, mipp::vector<R>( params.code.N_fil                             * params.simulation.inter_frame_level)),
  Y_N3(this->params.simulation.n_threads, mipp::vector<R>((params.code.N      + params.code.tail_length) * params.simulation.inter_frame_level)),
  Y_N4(this->params.simulation.n_threads, mipp::vector<Q>((params.code.N      + params.code.tail_length) * params.simulation.inter_frame_level)),
  Y_N5(this->params.simulation.n_threads, mipp::vector<Q>((params.code.N_code + params.code.tail_length) * params.simulation.inter_frame_level)),
  V_K1(this->params.simulation.n_threads, mipp::vector<B>( params.code.K                                 * params.simulation.inter_frame_level)),
  V_K2(this->params.simulation.n_threads, mipp::vector<B>((params.code.K      - params.crc .size)        * params.simulation.inter_frame_level)),

  monitor_red(nullptr),
  terminal   (nullptr),

  durations(this->params.simulation.n_threads)
{
	if (this->params.simulation.n_threads > 1 && params.simulation.debug)
		std::clog << bold_yellow("(WW) Debug mode will be disabled ")
		          << bold_yellow("because you launched the simulation with more than 1 thread!")
		          << std::endl;
#ifdef ENABLE_MPI
	if (params.simulation.debug || params.simulation.benchs)
		throw std::runtime_error("aff3ct::simulation::Simulation_BFER: debug and bench modes are unavailable in MPI.");
#endif

	// check, if the error tracker is enable, if the given file name is good
	if ((this->params.monitor.err_track_enable || this->params.monitor.err_track_revert) &&
	     !Monitor_reduction<B,R>::check_path(this->params.monitor.err_track_path))
		throw std::runtime_error("aff3ct::simulation::Simulation_BFER: issue while trying to open error tracker "
		                         "log files, check the base path (" + this->params.monitor.err_track_path + ").");

	if (this->params.monitor.err_track_revert)
	{
		if (this->params.simulation.n_threads != 1)
			std::clog << bold_yellow("(WW) Multi-threading detected with error tracking revert feature!")
			          << bold_yellow(" Each thread will play the same frames. Please run with one thread.")
			          << std::endl;

		if (this->params.simulation.inter_frame_level != 1)
			std::clog << bold_yellow("(WW) Inter frame level different than 1 detected with error tracking revert feature!")
			          << bold_yellow(" Each bad frame may be played several times. Please run with an inter frame level of 1.")
			          << std::endl;
	}
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
	if (this->params.monitor.err_track_revert)
	{
		std::string path_src, path_enc, path_noise, path_itl;
		Monitor_reduction<B,R>::get_tracker_paths(this->params.monitor.err_track_path, this->snr,
		                                          path_src, path_enc, path_noise, path_itl);

		// dirty hack to override simulation params
		parameters *params_writable = const_cast<parameters*>(&this->params);

		params_writable->source. path = path_src;
		params_writable->encoder.path = path_enc;
		params_writable->channel.path = path_noise;
	}

	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1] = std::thread(Simulation_BFER<B,R,Q>::Monte_Carlo_method, this, tid);

	// launch the master thread
	Simulation_BFER<B,R,Q>::Monte_Carlo_method(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1].join();

	if (this->params.simulation.mpi_rank == 0 &&
	    !this->params.terminal.disabled       &&
	    !this->params.simulation.benchs       &&
	    terminal != nullptr)
	{
		time_reduction(true);
		terminal->final_report(std::cout);
	}

	if (this->params.monitor.err_track_enable && monitor_red != nullptr)
		monitor_red->dump_bad_frames(this->params.monitor.err_track_path, this->snr);
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
	if (this->params.simulation.time_report &&
	    !this->params.simulation.benchs     &&
	    this->params.simulation.mpi_rank == 0)
		time_report();
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::build_communication_chain(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	Simulation_BFER_i<B,R,Q>::build_communication_chain(simu, tid);

	simu->barrier(tid);
	if (tid == 0)
	{
#ifdef ENABLE_MPI
		// build a monitor to compute BER/FER (reduce the other monitors)
		simu->monitor_red = new Monitor_reduction_mpi<B,R>(simu->params.code.K - simu->params.crc .size,
		                                                   simu->params.code.N + simu->params.code.tail_length,
		                                                   simu->params.code.N_mod,
		                                                   simu->params.monitor.n_frame_errors,
		                                                   simu->monitor,
		                                                   std::this_thread::get_id(),
		                                                   simu->params.simulation.mpi_comm_freq,
		                                                   simu->params.simulation.inter_frame_level);
#else
		// build a monitor to compute BER/FER (reduce the other monitors)
		simu->monitor_red = new Monitor_reduction<B,R>(simu->params.code.K - simu->params.crc .size,
		                                               simu->params.code.N + simu->params.code.tail_length,
		                                               simu->params.code.N_mod,
		                                               simu->params.monitor.n_frame_errors,
		                                               simu->monitor,
		                                               simu->params.simulation.inter_frame_level);
#endif
		// build the terminal to display the BER/FER
		simu->terminal = simu->build_terminal();
		Simulation::check_errors(simu->terminal, "Terminal");
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::Monte_Carlo_method(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	try
	{
		Simulation_BFER<B,R,Q>::build_communication_chain(simu, tid);

		if (tid == 0 && simu->params.simulation.mpi_rank == 0 &&
			(!simu->params.terminal.disabled && simu->snr == simu->params.simulation.snr_min &&
			!(simu->params.simulation.debug && simu->params.simulation.n_threads == 1) &&
			!simu->params.simulation.benchs))
			simu->terminal->legend(std::cout);

		if (simu->params.source.type == "AZCW")
		{
			std::fill(simu->U_K1[tid].begin(), simu->U_K1[tid].end(), (B)0);
			std::fill(simu->U_K2[tid].begin(), simu->U_K2[tid].end(), (B)0);
			std::fill(simu->X_N1[tid].begin(), simu->X_N1[tid].end(), (B)0);
			std::fill(simu->X_N2[tid].begin(), simu->X_N2[tid].end(), (B)0);
			simu->modulator[tid]->modulate(simu->X_N2[tid], simu->X_N3[tid]);
		}

		for (auto& duration : simu->durations[tid])
			duration.second = std::chrono::nanoseconds(0);

		simu->barrier(tid);

		try
		{
			if (simu->params.simulation.n_threads == 1 && simu->params.simulation.debug)
				Simulation_BFER<B,R,Q>::simulation_loop_debug(simu);
			else if (simu->params.simulation.benchs)
				Simulation_BFER<B,R,Q>::simulation_loop_bench(simu, tid);
			else
				Simulation_BFER<B,R,Q>::simulation_loop(simu, tid);
		}
		catch (std::exception const& e)
		{
			Monitor<B,R>::stop();

			if (tid == 0)
				std::cerr << bold_red("(EE) ") << bold_red("An issue was encountered during the simulation loop.")
				          << std::endl
				          << bold_red("(EE) ") << bold_red(e.what()) << std::endl;
		}
	}
	catch (std::exception const& e)
	{
		Monitor<B,R>::stop();

		if (tid == 0)
			std::cerr << bold_red("(EE) ") << bold_red("An issue was encountered when building the ")
			          << bold_red("communication chain.") << std::endl
			          << bold_red("(EE) ") << bold_red(e.what()) << std::endl;
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::simulation_loop(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	using namespace std::chrono;

	if (tid == 0)
		simu->t_simu = steady_clock::now();

	// simulation loop
	while ((!simu->monitor_red->fe_limit_achieved()) && // while max frame error count has not been reached
	        (simu->params.simulation.stop_time == seconds(0) ||
	         (steady_clock::now() - simu->t_snr) < simu->params.simulation.stop_time))
	{
		auto d_sourc = nanoseconds(0);
		auto d_crcbd = nanoseconds(0);
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
			// generate a random K bits vector U_K1
			auto t_sourc = steady_clock::now();
			simu->source[tid]->generate(simu->U_K1[tid]);
			d_sourc = steady_clock::now() - t_sourc;

			// build the CRC from U_K1 into U_K2
			auto t_crcbd = steady_clock::now();
			simu->crc[tid]->build(simu->U_K1[tid], simu->U_K2[tid]);
			d_crcbd = steady_clock::now() - t_crcbd;

			// encode U_K2 into a N bits vector X_N
			auto t_encod = steady_clock::now();
			simu->encoder[tid]->encode(simu->U_K2[tid], simu->X_N1[tid]);
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
		auto t_decod = steady_clock::now();
		simu->decoder[tid]->hard_decode(simu->Y_N5[tid], simu->V_K1[tid]);
		auto d_decod = steady_clock::now() - t_decod;

		// apply the coset to recover the real bits
		if (simu->params.code.coset)
		{
			auto t_cobit = steady_clock::now();
			simu->coset_bit[tid]->apply(simu->U_K2[tid], simu->V_K1[tid], simu->V_K1[tid]);
			d_cobit = steady_clock::now() - t_cobit;
		}

		// extract the CRC bits and keep only the information bits
		auto t_crcex = steady_clock::now();
		simu->crc[tid]->extract(simu->V_K1[tid], simu->V_K2[tid]);
		auto d_crcex = steady_clock::now() - t_crcex;

		// check errors in the frame
		auto t_check = steady_clock::now();
		if (simu->params.monitor.err_track_enable)
			simu->monitor[tid]->check_and_track_errors(simu->U_K1[tid], simu->V_K2[tid], simu->X_N1[tid],
			                                           simu->Y_N1[tid]);
		else
			simu->monitor[tid]->check_errors(simu->U_K1[tid], simu->V_K2[tid]);
		auto d_check = steady_clock::now() - t_check;

		// increment total durations for each operations
		simu->durations[tid][std::make_pair( 0, "Source"      )] += d_sourc;
		simu->durations[tid][std::make_pair( 1, "CRC build"   )] += d_crcbd;
		simu->durations[tid][std::make_pair( 2, "Encoder"     )] += d_encod;
		simu->durations[tid][std::make_pair( 3, "Puncturer"   )] += d_punct;
		simu->durations[tid][std::make_pair( 4, "Modulator"   )] += d_modul;
		simu->durations[tid][std::make_pair( 5, "Channel"     )] += d_chann;
		simu->durations[tid][std::make_pair( 6, "Filter"      )] += d_filte;
		simu->durations[tid][std::make_pair( 7, "Demodulator" )] += d_demod;
		simu->durations[tid][std::make_pair( 8, "Quantizer"   )] += d_quant;
		simu->durations[tid][std::make_pair( 9, "Depuncturer" )] += d_depun;
		simu->durations[tid][std::make_pair(10, "Coset real"  )] += d_corea;
		simu->durations[tid][std::make_pair(11, "Decoder"     )] += d_decod;
		simu->durations[tid][std::make_pair(12, "- load"      )] += simu->decoder[tid]->get_load_duration();
		simu->durations[tid][std::make_pair(13, "- decode"    )] += simu->decoder[tid]->get_decode_duration();
		simu->durations[tid][std::make_pair(14, "- store"     )] += simu->decoder[tid]->get_store_duration();
		simu->durations[tid][std::make_pair(15, "Coset bit"   )] += d_cobit;
		simu->durations[tid][std::make_pair(16, "CRC extract" )] += d_crcex;
		simu->durations[tid][std::make_pair(17, "Check errors")] += d_check;

		// display statistics in terminal
		if (tid == 0 && simu->params.simulation.mpi_rank == 0 &&
		    !simu->params.terminal.disabled && simu->params.terminal.frequency != nanoseconds(0) &&
		    (steady_clock::now() - simu->t_simu) >= simu->params.terminal.frequency)
		{
			simu->time_reduction();
			simu->terminal->temp_report(std::clog);
			simu->t_simu = steady_clock::now();
		}

		if (simu->interleaver[tid] != nullptr && simu->params.interleaver.uniform)
			simu->interleaver[tid]->gen_lookup_tables();
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
	for (auto i = 0; i < simu->params.simulation.benchs; i++)
		simu->decoder[tid]->hard_decode(simu->Y_N4[tid], simu->V_K1[tid]);
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

	// frame trace to display the vectors
	Frame_trace<B> ft(simu->params.simulation.debug_limit, simu->params.simulation.debug_precision);

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
		auto d_crcbd = nanoseconds(0);
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
			// generate a random K bits vector U_K1
			std::clog << "Generate random bits U_K1..." << std::endl;
			auto t_sourc = steady_clock::now();
			simu->source[0]->generate(simu->U_K1[0]);
			d_sourc = steady_clock::now() - t_sourc;

			// display U_K1
			std::clog << "U_K1:" << std::endl;
			ft.display_bit_vector(simu->U_K1[0]);
			std::clog << std::endl;

			// add the CRC to U_K
			std::clog << "Build the CRC from U_K1 into U_K2..." << std::endl;
			auto t_crcbd = steady_clock::now();
			simu->crc[0]->build(simu->U_K1[0], simu->U_K2[0]);
			d_crcbd = steady_clock::now() - t_crcbd;

			// display U_K2
			std::clog << "U_K2:" << std::endl;
			ft.display_bit_vector(simu->U_K2[0]);
			std::clog << std::endl;

			// encode U_K2 into a N bits vector X_N1
			std::clog << "Encode U_K2 in X_N1..." << std::endl;
			auto t_encod = steady_clock::now();
			simu->encoder[0]->encode(simu->U_K2[0], simu->X_N1[0]);
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
			// display U_K1
			std::clog << "U_K1:" << std::endl;
			ft.display_bit_vector(simu->U_K1[0]);
			std::clog << std::endl;

			// display U_K2
			std::clog << "U_K2:" << std::endl;
			ft.display_bit_vector(simu->U_K2[0]);
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
		std::clog << "Decode Y_N5 and generate V_K1..." << std::endl;
		auto t_decod = steady_clock::now();
		simu->decoder[0]->hard_decode(simu->Y_N5[0], simu->V_K1[0]);
		auto d_decod = steady_clock::now() - t_decod;

		// display V_K1
		std::clog << "V_K1:" << std::endl;
		if (simu->params.code.coset)
			ft.display_bit_vector(simu->V_K1[0]);
		else
			ft.display_bit_vector(simu->V_K1[0], simu->U_K2[0]);
		std::clog << std::endl;

		// apply the coset to recover the real bits
		if (simu->params.code.coset)
		{
			std::clog << "Apply the coset approach on V_K1..." << std::endl;
			auto t_cobit = steady_clock::now();
			simu->coset_bit[0]->apply(simu->U_K2[0], simu->V_K1[0], simu->V_K1[0]);
			d_cobit = steady_clock::now() - t_cobit;

			// display V_K1
			std::clog << "V_K1:" << std::endl;
			ft.display_bit_vector(simu->V_K1[0], simu->U_K2[0]);
			std::clog << std::endl;
		}

		// extract the CRC bits and keep only the information bits
		std::clog << "Extract the CRC bits from V_K1 and keep only the info. bits in V_K2..." << std::endl;
		auto t_crcex = steady_clock::now();
		simu->crc[0]->extract(simu->V_K1[0], simu->V_K2[0]);
		auto d_crcex = steady_clock::now() - t_crcex;

		// display V_K2
		std::clog << "V_K2:" << std::endl;
		ft.display_real_vector(simu->V_K2[0], simu->U_K1[0]);
		std::clog << std::endl;

		// check errors in the frame
		auto t_check = steady_clock::now();
		if (simu->params.monitor.err_track_enable)
			simu->monitor[0]->check_and_track_errors(simu->U_K1[0], simu->V_K2[0], simu->X_N1[0], simu->Y_N1[0]);
		else
			simu->monitor[0]->check_errors(simu->U_K1[0], simu->V_K2[0]);
		auto d_check = steady_clock::now() - t_check;

		// increment total durations for each operations
		simu->durations[0][std::make_pair( 0, "Source"      )] += d_sourc;
		simu->durations[0][std::make_pair( 1, "CRC build"   )] += d_crcbd;
		simu->durations[0][std::make_pair( 2, "Encoder"     )] += d_encod;
		simu->durations[0][std::make_pair( 3, "Puncturer"   )] += d_punct;
		simu->durations[0][std::make_pair( 4, "Modulator"   )] += d_modul;
		simu->durations[0][std::make_pair( 5, "Channel"     )] += d_chann;
		simu->durations[0][std::make_pair( 6, "Filter"      )] += d_filte;
		simu->durations[0][std::make_pair( 7, "Demodulator" )] += d_demod;
		simu->durations[0][std::make_pair( 8, "Quantizer"   )] += d_quant;
		simu->durations[0][std::make_pair( 9, "Depuncturer" )] += d_depun;
		simu->durations[0][std::make_pair(10, "Coset real"  )] += d_corea;
		simu->durations[0][std::make_pair(11, "Decoder"     )] += d_decod;
		simu->durations[0][std::make_pair(12, "- load"      )] += simu->decoder[0]->get_load_duration();
		simu->durations[0][std::make_pair(13, "- decode"    )] += simu->decoder[0]->get_decode_duration();
		simu->durations[0][std::make_pair(14, "- store"     )] += simu->decoder[0]->get_store_duration();
		simu->durations[0][std::make_pair(15, "Coset bit"   )] += d_cobit;
		simu->durations[0][std::make_pair(16, "CRC extract" )] += d_crcex;
		simu->durations[0][std::make_pair(17, "Check errors")] += d_check;

		// display statistics in terminal
		if (!simu->params.terminal.disabled && simu->params.terminal.frequency != nanoseconds(0) &&
		    (steady_clock::now() - t_simu) >= simu->params.terminal.frequency)
		{
			simu->terminal->temp_report(std::clog);
			t_simu = steady_clock::now();
		}

		if (simu->interleaver[0] != nullptr && simu->params.interleaver.uniform)
			simu->interleaver[0]->gen_lookup_tables();
	}

	simu->terminal->legend(std::cout);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::time_reduction(const bool is_snr_done)
{
	for (auto& duration : durations_red)
		duration.second = std::chrono::nanoseconds(0);

	for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
		for (auto& duration : durations[tid])
			durations_red[duration.first] += duration.second;

	if (is_snr_done)
		for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
			for (auto& duration : durations[tid])
				durations_sum[duration.first] += duration.second;
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::time_report(std::ostream &stream)
{
	auto d_total = std::chrono::nanoseconds(0);
	for (auto& duration : durations_sum)
		if (!duration.first.second.empty())
			d_total += duration.first.second[0] != '-' ? duration.second : std::chrono::nanoseconds(0);
	auto total_sec = ((float)d_total.count()) * 0.000000001f;

	auto max_chars = 0;
	for (auto& duration : durations_sum)
		max_chars = std::max(max_chars, (int)duration.first.second.length());

	stream << "#" << std::endl;
	stream << "# " << bold_underlined("Time report:") << std::endl;

	auto prev_sec = 0.f;
	for (auto& duration : durations_sum)
	{
		if (duration.second.count() != 0 && !duration.first.second.empty())
		{
			std::string key = "";
			const auto cur_sec = ((float)duration.second.count()) * 0.000000001f;
			auto cur_pc  = 0.f;
			if (duration.first.second[0] != '-')
			{
				cur_pc  = (cur_sec / total_sec) * 100.f;
				key = bold("* " + duration.first.second);
				prev_sec = cur_sec;
			}
			else
			{
				cur_pc  = (prev_sec != 0.f) ? (cur_sec / prev_sec) * 100.f : 0.f;
				key = bold_italic("  " + duration.first.second);
			}

			const auto n_spaces = max_chars - (int)duration.first.second.length();
			std::string str_spaces = "";
			for (auto i = 0; i < n_spaces; i++) str_spaces += " ";

			stream << "# " << key << str_spaces << ": "
			       << std::setw(9) << std::fixed << std::setprecision(3) << cur_sec << " sec ("
			       << std::setw(5) << std::fixed << std::setprecision(2) << cur_pc  << "%)"
			       << std::endl;
		}
	}

	stream << "#   ----------------------------------" << std::endl;
	const std::string total_str = "TOTAL";
	const auto n_spaces = max_chars - (int)total_str.length();
	std::string str_spaces = "";
	for (auto i = 0; i < n_spaces; i++) str_spaces += " ";
	stream << "# " << bold("* " + total_str) << str_spaces << ": "
	       << std::setw(9) << std::fixed << std::setprecision(3) << total_sec << " sec" << std::endl;
	stream << "#" << std::endl;
}

template <typename B, typename R, typename Q>
Terminal* Simulation_BFER<B,R,Q>
::build_terminal()
{
	this->durations_red[std::make_pair(11, "Decoder")] = std::chrono::nanoseconds(0);
	const auto &d_dec = this->durations_red[std::make_pair(11, "Decoder")];

#ifdef ENABLE_MPI
	return Factory_terminal<B,R>::build(this->params, this->snr_s, this->snr_b, monitor_red, this->t_snr);
#else
	return Factory_terminal<B,R>::build(this->params, this->snr_s, this->snr_b, monitor_red, this->t_snr, &d_dec);
#endif
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif /* NOT SYSTEMC NOT STARPU */
