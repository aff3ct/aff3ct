#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>

#include "Tools/Factory/Factory_terminal.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"

#include "Simulation_BFER_std_threads.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_std_threads<B,R,Q>
::Simulation_BFER_std_threads(const parameters& params, Codec<B,Q> &codec)
: Simulation_BFER_std<B,R,Q>(params, codec),

  threads(this->params.simulation.n_threads -1),

  U_K1(this->params.simulation.n_threads, mipp::vector<B>(params.code.K_info * params.simulation.inter_frame_level)),
  U_K2(this->params.simulation.n_threads, mipp::vector<B>(params.code.K      * params.simulation.inter_frame_level)),
  X_N1(this->params.simulation.n_threads, mipp::vector<B>(params.code.N_code * params.simulation.inter_frame_level)),
  X_N2(this->params.simulation.n_threads, mipp::vector<B>(params.code.N      * params.simulation.inter_frame_level)),
  X_N3(this->params.simulation.n_threads, mipp::vector<R>(params.code.N_mod  * params.simulation.inter_frame_level)),
  H_N (this->params.simulation.n_threads, mipp::vector<R>(params.code.N_mod  * params.simulation.inter_frame_level)),
  Y_N1(this->params.simulation.n_threads, mipp::vector<R>(params.code.N_mod  * params.simulation.inter_frame_level)),
  Y_N2(this->params.simulation.n_threads, mipp::vector<R>(params.code.N_fil  * params.simulation.inter_frame_level)),
  Y_N3(this->params.simulation.n_threads, mipp::vector<R>(params.code.N      * params.simulation.inter_frame_level)),
  Y_N4(this->params.simulation.n_threads, mipp::vector<Q>(params.code.N      * params.simulation.inter_frame_level)),
  Y_N5(this->params.simulation.n_threads, mipp::vector<Q>(params.code.N_code * params.simulation.inter_frame_level)),
  V_K1(this->params.simulation.n_threads, mipp::vector<B>(params.code.K      * params.simulation.inter_frame_level)),
  V_K2(this->params.simulation.n_threads, mipp::vector<B>(params.code.K_info * params.simulation.inter_frame_level))
{
	if (this->params.simulation.n_threads > 1 && params.simulation.debug)
		std::clog << bold_yellow("(WW) Debug mode will be disabled ")
		          << bold_yellow("because you launched the simulation with more than 1 thread!")
		          << std::endl;
#ifdef ENABLE_MPI
	if (params.simulation.debug || params.simulation.benchs)
		throw std::runtime_error("aff3ct::simulation::Simulation_BFER_std_threads: debug and bench modes are "
		                         "unavailable in MPI.");
#endif

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
Simulation_BFER_std_threads<B,R,Q>
::~Simulation_BFER_std_threads()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std_threads<B,R,Q>
::build_communication_chain(const int tid)
{
	Simulation_BFER_std<B,R,Q>::build_communication_chain(tid);

	if (this->params.source.type == "AZCW")
	{
		std::fill(this->U_K1[tid].begin(), this->U_K1[tid].end(), (B)0);
		std::fill(this->U_K2[tid].begin(), this->U_K2[tid].end(), (B)0);
		std::fill(this->X_N1[tid].begin(), this->X_N1[tid].end(), (B)0);
		std::fill(this->X_N2[tid].begin(), this->X_N2[tid].end(), (B)0);
		this->modulator[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
	}

	if (this->params.monitor.err_track_enable)
	{
		this->dumper[tid]->set_U_K(U_K1[tid].data());
		this->dumper[tid]->set_X_N(X_N1[tid].data());
		this->dumper[tid]->set_Y_N(Y_N1[tid].data());
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std_threads<B,R,Q>
::_launch()
{
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1] = std::thread(Simulation_BFER_std_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	Simulation_BFER_std_threads<B,R,Q>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1].join();
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std_threads<B,R,Q>
::start_thread(Simulation_BFER_std_threads<B,R,Q> *simu, const int tid)
{
	simu->Monte_Carlo_method(tid);
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std_threads<B,R,Q>
::Monte_Carlo_method(const int tid)
{
	if (this->params.simulation.n_threads == 1 && this->params.simulation.debug)
		this->simulation_loop_debug();
	else if (this->params.simulation.benchs)
		this->simulation_loop_bench(tid);
	else
		this->simulation_loop(tid);
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std_threads<B,R,Q>
::simulation_loop(const int tid)
{
	using namespace std::chrono;

	// simulation loop
	while ((!this->monitor_red->fe_limit_achieved()) && // while max frame error count has not been reached
	        (this->params.simulation.stop_time == seconds(0) ||
	         (steady_clock::now() - this->t_snr) < this->params.simulation.stop_time))
	{
		if (this->params.source.type != "AZCW")
		{
			// generate a random K bits vector U_K1
			auto t_sourc = steady_clock::now();
			this->source[tid]->generate(this->U_K1[tid]);
			this->durations[tid][std::make_pair(0, "Source")] += steady_clock::now() - t_sourc;

			// build the CRC from U_K1 into U_K2
			auto t_crcbd = steady_clock::now();
			this->crc[tid]->build(this->U_K1[tid], this->U_K2[tid]);
			this->durations[tid][std::make_pair(1, "CRC build")] += steady_clock::now() - t_crcbd;

			// encode U_K2 into a N bits vector X_N
			auto t_encod = steady_clock::now();
			this->encoder[tid]->encode(this->U_K2[tid], this->X_N1[tid]);
			this->durations[tid][std::make_pair(2, "Encoder")] += steady_clock::now() - t_encod;

			// puncture X_N1 into X_N2
			auto t_punct = steady_clock::now();
			this->puncturer[tid]->puncture(this->X_N1[tid], this->X_N2[tid]);
			this->durations[tid][std::make_pair(3, "Puncturer")] += steady_clock::now() - t_punct;

			// modulate
			auto t_modul = steady_clock::now();
			this->modulator[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
			this->durations[tid][std::make_pair(4, "Modulator")] += steady_clock::now() - t_modul;
		}

		// Rayleigh channel
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
		{
			// add noise
			auto t_chann = steady_clock::now();
			this->channel[tid]->add_noise(this->X_N3[tid], this->Y_N1[tid], this->H_N[tid]);
			this->durations[tid][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;

			// filtering
			auto t_filte = steady_clock::now();
			this->modulator[tid]->filter(this->Y_N1[tid], this->Y_N2[tid]);
			this->durations[tid][std::make_pair(6, "Filter")] += steady_clock::now() - t_filte;

			// demodulation
			auto t_demod = steady_clock::now();
			this->modulator[tid]->demodulate_with_gains(this->Y_N2[tid], this->H_N[tid], this->Y_N3[tid]);
			this->durations[tid][std::make_pair(7, "Demodulator")] += steady_clock::now() - t_demod;
		}
		else // additive channel (AWGN, USER, NO)
		{
			// add noise
			auto t_chann = steady_clock::now();
			this->channel[tid]->add_noise(this->X_N3[tid], this->Y_N1[tid]);
			this->durations[tid][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;

			// filtering
			auto t_filte = steady_clock::now();
			this->modulator[tid]->filter(this->Y_N1[tid], this->Y_N2[tid]);
			this->durations[tid][std::make_pair(6, "Filter")] += steady_clock::now() - t_filte;

			// demodulation
			auto t_demod = steady_clock::now();
			this->modulator[tid]->demodulate(this->Y_N2[tid], this->Y_N3[tid]);
			this->durations[tid][std::make_pair(7, "Demodulator")] += steady_clock::now() - t_demod;
		}

		// make the quantization
		auto t_quant = steady_clock::now();
		this->quantizer[tid]->process(this->Y_N3[tid], this->Y_N4[tid]);
		this->durations[tid][std::make_pair(8, "Quantizer")] += steady_clock::now() - t_quant;

		// depuncture before the decoding stage
		auto t_depun = steady_clock::now();
		this->puncturer[tid]->depuncture(this->Y_N4[tid], this->Y_N5[tid]);
		this->durations[tid][std::make_pair(9, "Depuncturer")] += steady_clock::now() - t_depun;

		// apply the coset: the decoder will believe to a AZCW
		if (this->params.code.coset)
		{
			auto t_corea = steady_clock::now();
			this->coset_real[tid]->apply(this->X_N1[tid], this->Y_N5[tid], this->Y_N5[tid]);
			this->durations[tid][std::make_pair(10, "Coset real")] += steady_clock::now() - t_corea;
		}

		// launch decoder
		auto t_decod = steady_clock::now();
		this->decoder[tid]->hard_decode(this->Y_N5[tid], this->V_K1[tid]);
		this->durations[tid][std::make_pair(11, "Decoder" )] += steady_clock::now() - t_decod;
		this->durations[tid][std::make_pair(12, "- load"  )] += this->decoder[tid]->get_load_duration();
		this->durations[tid][std::make_pair(13, "- decode")] += this->decoder[tid]->get_decode_duration();
		this->durations[tid][std::make_pair(14, "- store" )] += this->decoder[tid]->get_store_duration();

		// apply the coset to recover the real bits
		if (this->params.code.coset)
		{
			auto t_cobit = steady_clock::now();
			this->coset_bit[tid]->apply(this->U_K2[tid], this->V_K1[tid], this->V_K1[tid]);
			this->durations[tid][std::make_pair(15, "Coset bit")] += steady_clock::now() - t_cobit;
		}

		// extract the CRC bits and keep only the information bits
		auto t_crcex = steady_clock::now();
		this->crc[tid]->extract(this->V_K1[tid], this->V_K2[tid]);
		this->durations[tid][std::make_pair(16, "CRC extract")] += steady_clock::now() - t_crcex;

		// check errors in the frame
		auto t_check = steady_clock::now();
		this->monitor[tid]->check_errors(this->U_K1[tid], this->V_K2[tid]);
		this->durations[tid][std::make_pair(17, "Check errors")] += steady_clock::now() - t_check;
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std_threads<B,R,Q>
::simulation_loop_bench(const int tid)
{
	using namespace std::chrono;

	this->barrier(tid);
	auto t_start = std::chrono::steady_clock::now(); // start time

	this->barrier(tid);
	for (auto i = 0; i < this->params.simulation.benchs; i++)
		this->decoder[tid]->hard_decode(this->Y_N5[tid], this->V_K1[tid]);
	this->barrier(tid);

	auto t_stop = std::chrono::steady_clock::now(); // stop time

	auto frames   = (float)this->params.simulation.benchs *
	                (float)this->params.simulation.inter_frame_level *
	                (float)this->params.simulation.n_threads;
	auto bits     = (float)frames * (float)this->params.code.K;
	auto duration = t_stop - t_start;

	auto  bps = (float)bits / (float)(duration.count() * 0.000000001f);
	auto kbps =  bps * 0.001f;
	auto mbps = kbps * 0.001f;

	auto latency_ns = (float)duration.count() / (float)this->params.simulation.benchs;
	auto latency_us = latency_ns * 0.001f;

	if (tid == 0)
		std::cout << "  SNR (Eb/N0) = "          << std::setw(5) << std::fixed << std::setprecision(2) << this->snr
		          << " dB"   << ", "
		          << "information throughput = " << std::setw(8) << std::fixed << std::setprecision(4) << mbps       
		          << " Mbps" << ", " 
		          << "latency = "                << std::setw(8) << std::fixed << std::setprecision(4) << latency_us 
		          << " us."  << std::endl;
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std_threads<B,R,Q>
::simulation_loop_debug()
{
	using namespace std::chrono;

	// frame trace to display the vectors
	Frame_trace<B> ft(this->params.simulation.debug_limit, this->params.simulation.debug_precision);

	// simulation loop
	while (!this->monitor_red->fe_limit_achieved() && // while max frame error count has not been reached
	       (this->params.simulation.stop_time == seconds(0) ||
	        (steady_clock::now() - this->t_snr) < this->params.simulation.stop_time))
	{
		std::clog << "-------------------------------" << std::endl;
		std::clog << "New encoding/decoding session !" << std::endl;
		std::clog << "Frame n°" << this->monitor_red->get_n_analyzed_fra() << std::endl;
		std::clog << "-------------------------------" << std::endl;

		if (this->params.source.type != "AZCW")
		{
			// generate a random K bits vector U_K1
			std::clog << "Generate random bits U_K1..." << std::endl;
			auto t_sourc = steady_clock::now();
			this->source[0]->generate(this->U_K1[0]);
			this->durations[0][std::make_pair(0, "Source")] += steady_clock::now() - t_sourc;

			// display U_K1
			std::clog << "U_K1:" << std::endl;
			ft.display_bit_vector(this->U_K1[0]);
			std::clog << std::endl;

			// add the CRC to U_K
			std::clog << "Build the CRC from U_K1 into U_K2..." << std::endl;
			auto t_crcbd = steady_clock::now();
			this->crc[0]->build(this->U_K1[0], this->U_K2[0]);
			this->durations[0][std::make_pair(1, "CRC build")] += steady_clock::now() - t_crcbd;

			// display U_K2
			std::clog << "U_K2:" << std::endl;
			ft.display_bit_vector(this->U_K2[0]);
			std::clog << std::endl;

			// encode U_K2 into a N bits vector X_N1
			std::clog << "Encode U_K2 in X_N1..." << std::endl;
			auto t_encod = steady_clock::now();
			this->encoder[0]->encode(this->U_K2[0], this->X_N1[0]);
			this->durations[0][std::make_pair(2, "Encoder")] += steady_clock::now() - t_encod;

			// display X_N1
			std::clog << "X_N1:" << std::endl;
			ft.display_bit_vector(this->X_N1[0]);
			std::clog << std::endl;

			// puncture X_N1 into X_N2
			std::clog << "Puncture X_N1 in X_N2..." << std::endl;
			auto t_punct = steady_clock::now();
			this->puncturer[0]->puncture(this->X_N1[0], this->X_N2[0]);
			this->durations[0][std::make_pair(3, "Puncturer")] += steady_clock::now() - t_punct;

			// display X_N2
			std::clog << "X_N2:" << std::endl;
			ft.display_bit_vector(this->X_N2[0]);
			std::clog << std::endl;

			// modulate
			std::clog << "Modulate X_N2 in X_N3..." << std::endl;
			auto t_modul = steady_clock::now();
			this->modulator[0]->modulate(this->X_N2[0], this->X_N3[0]);
			this->durations[0][std::make_pair(4, "Modulator")] += steady_clock::now() - t_modul;

			// display X_N3
			std::clog << "X_N3:" << std::endl;
			ft.display_real_vector(this->X_N3[0]);
			std::clog << std::endl;
		}
		else
		{
			// display U_K1
			std::clog << "U_K1:" << std::endl;
			ft.display_bit_vector(this->U_K1[0]);
			std::clog << std::endl;

			// display U_K2
			std::clog << "U_K2:" << std::endl;
			ft.display_bit_vector(this->U_K2[0]);
			std::clog << std::endl;

			// display X_N2
			std::clog << "X_N2:" << std::endl;
			ft.display_bit_vector(this->X_N2[0]);
			std::clog << std::endl;

			// display X_N3
			std::clog << "X_N3:" << std::endl;
			ft.display_real_vector(this->X_N3[0]);
			std::clog << std::endl;
		}

		// Rayleigh channel
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
		{
			// add noise
			std::clog << "Add noise from X_N3 to Y_N1..." << std::endl;
			auto t_chann = steady_clock::now();
			this->channel[0]->add_noise(this->X_N3[0], this->Y_N1[0], this->H_N[0]);
			this->durations[0][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;

			// display Y_N1
			std::clog << "Y_N1:" << std::endl;
			ft.display_real_vector(this->Y_N1[0]);
			std::clog << std::endl;

			// display channel gains
			std::clog << "H_N:" << std::endl;
			ft.display_real_vector(this->H_N[0]);
			std::clog << std::endl;

			// filtering
			std::clog << "Filter from Y_N1 to Y_N2..." << std::endl;
			auto t_filte = steady_clock::now();
			this->modulator[0]->filter(this->Y_N1[0], this->Y_N2[0]);
			this->durations[0][std::make_pair(6, "Filter")] += steady_clock::now() - t_filte;

			// display Y_N2
			std::clog << "Y_N2:" << std::endl;
			ft.display_real_vector(this->Y_N2[0]);
			std::clog << std::endl;

			// demodulation
			std::clog << "Demodulate from Y_N2 to Y_N3..." << std::endl;
			auto t_demod = steady_clock::now();
			this->modulator[0]->demodulate_with_gains(this->Y_N2[0], this->H_N[0], this->Y_N3[0]);
			this->durations[0][std::make_pair(7, "Demodulator")] += steady_clock::now() - t_demod;

			// display Y_N3
			std::clog << "Y_N3:" << std::endl;
			ft.display_real_vector(this->Y_N3[0]);
			std::clog << std::endl;
		}
		else // additive channel (AWGN, USER, NO)
		{
			// add noise
			std::clog << "Add noise from X_N3 to Y_N1..." << std::endl;
			auto t_chann = steady_clock::now();
			this->channel[0]->add_noise(this->X_N3[0], this->Y_N1[0]);
			this->durations[0][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;

			// display Y_N1
			std::clog << "Y_N1:" << std::endl;
			ft.display_real_vector(this->Y_N1[0]);
			std::clog << std::endl;

			// filtering
			std::clog << "Filter from Y_N1 to Y_N2..." << std::endl;
			auto t_filte = steady_clock::now();
			this->modulator[0]->filter(this->Y_N1[0], this->Y_N2[0]);
			this->durations[0][std::make_pair(6, "Filter")] += steady_clock::now() - t_filte;

			// display Y_N2
			std::clog << "Y_N2:" << std::endl;
			ft.display_real_vector(this->Y_N2[0]);
			std::clog << std::endl;

			// demodulation
			std::clog << "Demodulate from Y_N2 to Y_N3..." << std::endl;
			auto t_demod = steady_clock::now();
			this->modulator[0]->demodulate(this->Y_N2[0], this->Y_N3[0]);
			this->durations[0][std::make_pair(7, "Demodulator")] += steady_clock::now() - t_demod;

			// display Y_N3
			std::clog << "Y_N3:" << std::endl;
			ft.display_real_vector(this->Y_N3[0]);
			std::clog << std::endl;
		}

		// make the quantization
		std::clog << "Make the quantization from Y_N3 to Y_N4..." << std::endl;
		auto t_quant = steady_clock::now();
		this->quantizer[0]->process(this->Y_N3[0], this->Y_N4[0]);
		this->durations[0][std::make_pair(8, "Quantizer")] += steady_clock::now() - t_quant;

		// display Y_N4
		std::clog << "Y_N4:" << std::endl;
		ft.display_real_vector(this->Y_N4[0]);
		std::clog << std::endl;

		// depuncture before the decoding stage
		std::clog << "Depuncture Y_N4 and generate Y_N5..." << std::endl;
		auto t_depun = steady_clock::now();
		this->puncturer[0]->depuncture(this->Y_N4[0], this->Y_N5[0]);
		this->durations[0][std::make_pair(9, "Depuncturer")] += steady_clock::now() - t_depun;

		// display Y_N5
		std::clog << "Y_N5:" << std::endl;
		ft.display_real_vector(this->Y_N5[0]);
		std::clog << std::endl;

		// apply the coset: the decoder will believe to a AZCW
		if (this->params.code.coset)
		{
			std::clog << "Apply the coset approach on Y_N5..." << std::endl;
			auto t_corea = steady_clock::now();
			this->coset_real[0]->apply(this->X_N1[0], this->Y_N5[0], this->Y_N5[0]);
			this->durations[0][std::make_pair(10, "Coset real")] += steady_clock::now() - t_corea;

			// display Y_N5
			std::clog << "Y_N5:" << std::endl;
			ft.display_real_vector(this->Y_N5[0]);
			std::clog << std::endl;
		}
		
		// launch decoder
		std::clog << "Decode Y_N5 and generate V_K1..." << std::endl;
		auto t_decod = steady_clock::now();
		this->decoder[0]->hard_decode(this->Y_N5[0], this->V_K1[0]);
		this->durations[0][std::make_pair(11, "Decoder" )] += steady_clock::now() - t_decod;
		this->durations[0][std::make_pair(12, "- load"  )] += this->decoder[0]->get_load_duration();
		this->durations[0][std::make_pair(13, "- decode")] += this->decoder[0]->get_decode_duration();
		this->durations[0][std::make_pair(14, "- store" )] += this->decoder[0]->get_store_duration();

		// display V_K1
		std::clog << "V_K1:" << std::endl;
		if (this->params.code.coset)
			ft.display_bit_vector(this->V_K1[0]);
		else
			ft.display_bit_vector(this->V_K1[0], this->U_K2[0]);
		std::clog << std::endl;

		// apply the coset to recover the real bits
		if (this->params.code.coset)
		{
			std::clog << "Apply the coset approach on V_K1..." << std::endl;
			auto t_cobit = steady_clock::now();
			this->coset_bit[0]->apply(this->U_K2[0], this->V_K1[0], this->V_K1[0]);
			this->durations[0][std::make_pair(15, "Coset bit")] += steady_clock::now() - t_cobit;

			// display V_K1
			std::clog << "V_K1:" << std::endl;
			ft.display_bit_vector(this->V_K1[0], this->U_K2[0]);
			std::clog << std::endl;
		}

		// extract the CRC bits and keep only the information bits
		std::clog << "Extract the CRC bits from V_K1 and keep only the info. bits in V_K2..." << std::endl;
		auto t_crcex = steady_clock::now();
		this->crc[0]->extract(this->V_K1[0], this->V_K2[0]);
		this->durations[0][std::make_pair(16, "CRC extract")] += steady_clock::now() - t_crcex;

		// display V_K2
		std::clog << "V_K2:" << std::endl;
		ft.display_real_vector(this->V_K2[0], this->U_K1[0]);
		std::clog << std::endl;

		// check errors in the frame
		auto t_check = steady_clock::now();
		this->monitor[0]->check_errors(this->U_K1[0], this->V_K2[0]);
		this->durations[0][std::make_pair(17, "Check errors")] += steady_clock::now() - t_check;
	}
}

template <typename B, typename R, typename Q>
Terminal* Simulation_BFER_std_threads<B,R,Q>
::build_terminal()
{
	this->durations_red[std::make_pair(11, "Decoder")] = std::chrono::nanoseconds(0);
	const auto &d_dec = this->durations_red[std::make_pair(11, "Decoder")];

#ifdef ENABLE_MPI
	return Factory_terminal<B>::build(this->params, this->snr_s, this->snr_b, this->monitor_red, this->t_snr);
#else
	return Factory_terminal<B>::build(this->params, this->snr_s, this->snr_b, this->monitor_red, this->t_snr, &d_dec);
#endif
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER_std_threads<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_std_threads<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_std_threads<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_std_threads<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_std_threads<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
