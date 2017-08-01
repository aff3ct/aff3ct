#include <string>
#include <vector>
#include <chrono>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Display/bash_tools.h"

#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER_std_threads.hpp"

using namespace aff3ct;
using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_std_threads<B,R,Q>
::BFER_std_threads(const factory::BFER_std::parameters &params, Codec<B,Q> &codec)
: BFER_std<B,R,Q>(params, codec),

  U_K1(this->params.n_threads, mipp::vector<B>(this->params.src->K     * this->params.src->n_frames)),
  U_K2(this->params.n_threads, mipp::vector<B>(this->params.enc->K     * this->params.enc->n_frames)),
  X_N1(this->params.n_threads, mipp::vector<B>(this->params.enc->N_cw  * this->params.enc->n_frames)),
  X_N2(this->params.n_threads, mipp::vector<B>(this->params.pct->N     * this->params.pct->n_frames)),
  X_N3(this->params.n_threads, mipp::vector<R>(this->params.mdm->N_mod * this->params.mdm->n_frames)),
  H_N (this->params.n_threads, mipp::vector<R>(this->params.chn->N     * this->params.chn->n_frames)),
  Y_N1(this->params.n_threads, mipp::vector<R>(this->params.chn->N     * this->params.chn->n_frames)),
  Y_N2(this->params.n_threads, mipp::vector<R>(this->params.mdm->N_fil * this->params.mdm->n_frames)),
  Y_N3(this->params.n_threads, mipp::vector<R>(this->params.mdm->N     * this->params.mdm->n_frames)),
  Y_N4(this->params.n_threads, mipp::vector<Q>(this->params.qnt->size  * this->params.qnt->n_frames)),
  Y_N5(this->params.n_threads, mipp::vector<Q>(this->params.pct->N_cw  * this->params.pct->n_frames)),
  V_K1(this->params.n_threads, mipp::vector<B>(this->params.dec->K     * this->params.dec->n_frames)),
  V_K2(this->params.n_threads, mipp::vector<B>(this->params.crc->K     * this->params.crc->n_frames))
{
#ifdef ENABLE_MPI
	if (this->params.debug || this->params.benchs)
		throw runtime_error(__FILE__, __LINE__, __func__, "The debug and bench modes are unavailable in MPI.");
#endif

	if (this->params.err_track_revert)
	{
		if (this->params.n_threads != 1)
			std::clog << format_warning("Multi-threading detected with error tracking revert feature!"
			                            " Each thread will play the same frames. Please run with one thread.")
			          << std::endl;
	}

	this->data_sizes[std::make_pair( 0, "Source"      )] = this->U_K1[0].size();
	this->data_sizes[std::make_pair( 1, "CRC build"   )] = this->U_K2[0].size();
	this->data_sizes[std::make_pair( 2, "Encoder"     )] = this->X_N1[0].size();
	this->data_sizes[std::make_pair( 3, "Puncturer"   )] = this->X_N2[0].size();
	this->data_sizes[std::make_pair( 4, "Modulator"   )] = this->X_N3[0].size();
	this->data_sizes[std::make_pair( 5, "Channel"     )] = this->Y_N1[0].size();
	this->data_sizes[std::make_pair( 6, "Filter"      )] = this->Y_N2[0].size();
	this->data_sizes[std::make_pair( 7, "Demodulator" )] = this->Y_N3[0].size();
	this->data_sizes[std::make_pair( 8, "Quantizer"   )] = this->Y_N4[0].size();
	this->data_sizes[std::make_pair( 9, "Depuncturer" )] = this->Y_N5[0].size();
	this->data_sizes[std::make_pair(10, "Coset real"  )] = this->Y_N5[0].size();
	this->data_sizes[std::make_pair(11, "Decoder"     )] = this->V_K1[0].size();
	this->data_sizes[std::make_pair(15, "Coset bit"   )] = this->V_K1[0].size();
	this->data_sizes[std::make_pair(16, "CRC extract" )] = this->V_K2[0].size();
	this->data_sizes[std::make_pair(17, "Check errors")] = this->V_K2[0].size();
}

template <typename B, typename R, typename Q>
BFER_std_threads<B,R,Q>
::~BFER_std_threads()
{
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::_build_communication_chain(const int tid)
{
	BFER_std<B,R,Q>::_build_communication_chain(tid);

	if (this->params.src->type == "AZCW")
	{
		std::fill(this->U_K1[tid].begin(), this->U_K1[tid].end(), (B)0);
		std::fill(this->U_K2[tid].begin(), this->U_K2[tid].end(), (B)0);
		std::fill(this->X_N1[tid].begin(), this->X_N1[tid].end(), (B)0);
		std::fill(this->X_N2[tid].begin(), this->X_N2[tid].end(), (B)0);
		this->modem[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
	}

	if (this->params.err_track_enable)
	{
		if (this->params.src->type != "AZCW")
			this->dumper[tid]->register_data(U_K1[tid], "src", false, {});

		if (this->params.coset)
			this->dumper[tid]->register_data(X_N1[tid], "enc", false, {(unsigned)this->params.enc->K});

		this->dumper[tid]->register_data(this->channel[tid]->get_noise(), "chn", true, {});

		if (this->interleaver[tid] != nullptr && this->interleaver[tid]->is_uniform())
			this->dumper[tid]->register_data(this->interleaver[tid]->get_lut(), "itl", false, {});
	}

	if (this->params.debug)
	{
		if (this->params.debug_fe)
			this->monitor[tid]->add_handler_fe   (std::bind(&BFER_std_threads::display_debug, this));
		else
			this->monitor[tid]->add_handler_check(std::bind(&BFER_std_threads::display_debug, this));
	}
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::_launch()
{
	std::vector<std::thread> threads(this->params.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1] = std::thread(BFER_std_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	BFER_std_threads<B,R,Q>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1].join();
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::start_thread(BFER_std_threads<B,R,Q> *simu, const int tid)
{
	try
	{
		simu->thread_id[std::this_thread::get_id()] = tid;
		simu->Monte_Carlo_method(tid);
	}
	catch (std::exception const& e)
	{
		Monitor<B>::stop();

		simu->mutex_exception.lock();
		if (simu->prev_err_message != e.what())
		{
			std::cerr << apply_on_each_line(e.what(), &format_error) << std::endl;
			simu->prev_err_message = e.what();
		}
		simu->mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::Monte_Carlo_method(const int tid)
{
	if (this->params.benchs)
		this->simulation_loop_bench(tid);
	else
		this->simulation_loop(tid);
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::simulation_loop(const int tid)
{
	using namespace std::chrono;
	auto t_snr = steady_clock::now();

	// simulation loop
	while (!this->monitor_red->fe_limit_achieved() && // while max frame error count has not been reached
	       (this->params.stop_time == seconds(0) || (steady_clock::now() - t_snr) < this->params.stop_time) &&
	       (this->monitor_red->get_n_analyzed_fra() < this->max_fra || this->max_fra == 0))
	{
		if (this->params.src->type != "AZCW")
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
			this->modem[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
			this->durations[tid][std::make_pair(4, "Modulator")] += steady_clock::now() - t_modul;
		}

		// Rayleigh channel
		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			// add noise
			auto t_chann = steady_clock::now();
			this->channel[tid]->add_noise(this->X_N3[tid], this->Y_N1[tid], this->H_N[tid]);
			this->durations[tid][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;

			// filtering
			auto t_filte = steady_clock::now();
			this->modem[tid]->filter(this->Y_N1[tid], this->Y_N2[tid]);
			this->durations[tid][std::make_pair(6, "Filter")] += steady_clock::now() - t_filte;

			// demodulation
			auto t_demod = steady_clock::now();
			this->modem[tid]->demodulate_with_gains(this->Y_N2[tid], this->H_N[tid], this->Y_N3[tid]);
			this->durations[tid][std::make_pair(7, "Demodulator")] += steady_clock::now() - t_demod;

			// make the quantization
			auto t_quant = steady_clock::now();
			this->quantizer[tid]->process(this->Y_N3[tid], this->Y_N4[tid]);
			this->durations[tid][std::make_pair(8, "Quantizer")] += steady_clock::now() - t_quant;
		}
		else // additive channel (AWGN, USER, NO)
		{
			// add noise
			auto t_chann = steady_clock::now();
			this->channel[tid]->add_noise(this->X_N3[tid], this->Y_N1[tid]);
			this->durations[tid][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;

			// filtering
			auto t_filte = steady_clock::now();
			this->modem[tid]->filter(this->Y_N1[tid], this->Y_N2[tid]);
			this->durations[tid][std::make_pair(6, "Filter")] += steady_clock::now() - t_filte;

			// demodulation
			auto t_demod = steady_clock::now();
			this->modem[tid]->demodulate(this->Y_N2[tid], this->Y_N3[tid]);
			this->durations[tid][std::make_pair(7, "Demodulator")] += steady_clock::now() - t_demod;

			// make the quantization
			auto t_quant = steady_clock::now();
			this->quantizer[tid]->process(this->Y_N3[tid], this->Y_N4[tid]);
			this->durations[tid][std::make_pair(8, "Quantizer")] += steady_clock::now() - t_quant;
		}


		// depuncture before the decoding stage
		auto t_depun = steady_clock::now();
		this->puncturer[tid]->depuncture(this->Y_N4[tid], this->Y_N5[tid]);
		this->durations[tid][std::make_pair(9, "Depuncturer")] += steady_clock::now() - t_depun;

		// apply the coset: the decoder will believe to a AZCW
		if (this->params.coset)
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
		if (this->params.coset)
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
void BFER_std_threads<B,R,Q>
::simulation_loop_bench(const int tid)
{
	using namespace std::chrono;

	this->barrier(tid);
	auto t_start = std::chrono::steady_clock::now(); // start time

	this->barrier(tid);
	for (auto i = 0; i < this->params.benchs; i++)
		this->decoder[tid]->hard_decode(this->Y_N5[tid], this->V_K1[tid]);
	this->barrier(tid);

	auto t_stop = std::chrono::steady_clock::now(); // stop time

	auto frames   = (float)this->params.benchs *
	                (float)this->params.src->n_frames *
	                (float)this->params.n_threads;
	auto bits     = (float)frames * (float)this->params.dec->K;
	auto duration = t_stop - t_start;

	auto  bps = (float)bits / (float)(duration.count() * 0.000000001f);
	auto kbps =  bps * 0.001f;
	auto mbps = kbps * 0.001f;

	auto latency_ns = (float)duration.count() / (float)this->params.benchs;
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
void BFER_std_threads<B,R,Q>
::display_debug()
{
	this->mutex_debug.lock();

	const auto tid = this->thread_id[std::this_thread::get_id()];
	Frame_trace<B> ft(this->params.debug_limit, this->params.debug_precision);

	std::cout << "-------------------------------" << std::endl;
	std::cout << "New encoding/decoding session !" << std::endl;
	std::cout << "Thread id: " << tid              << std::endl;
	std::cout << "Frame n°" << this->monitor_red->get_n_analyzed_fra() << std::endl;
	std::cout << "-------------------------------" << std::endl;

	if (this->params.src->type != "AZCW")
	{
		std::cout << "Generate random bits U_K1..." << std::endl
		          << "U_K1:" << std::endl;
		ft.display_bit_vector(this->U_K1[tid]);
		std::cout << std::endl;

		std::cout << "Build the CRC from U_K1 into U_K2..." << std::endl
		          << "U_K2:" << std::endl;
		ft.display_bit_vector(this->U_K2[tid]);
		std::cout << std::endl;

		std::cout << "Encode U_K2 in X_N1..." << std::endl
		          << "X_N1:" << std::endl;
		ft.display_bit_vector(this->X_N1[tid]);
		std::cout << std::endl;

		std::cout << "Puncture X_N1 in X_N2..." << std::endl
		          << "X_N2:" << std::endl;
		ft.display_bit_vector(this->X_N2[tid]);
		std::cout << std::endl;

		// modulate
		std::cout << "Modulate X_N2 in X_N3..." << std::endl
		          << "X_N3:" << std::endl;
		ft.display_real_vector(this->X_N3[tid]);
		std::cout << std::endl;
	}
	else
	{
		std::cout << "U_K1:" << std::endl;
		ft.display_bit_vector(this->U_K1[tid]);
		std::cout << std::endl;

		std::cout << "U_K2:" << std::endl;
		ft.display_bit_vector(this->U_K2[tid]);
		std::cout << std::endl;

		std::cout << "X_N2:" << std::endl;
		ft.display_bit_vector(this->X_N2[tid]);
		std::cout << std::endl;

		std::cout << "X_N3:" << std::endl;
		ft.display_real_vector(this->X_N3[tid]);
		std::cout << std::endl;
	}

	// Rayleigh channel
	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		std::cout << "Add noise from X_N3 to Y_N1..." << std::endl
		          << "Y_N1:" << std::endl;
		ft.display_real_vector(this->Y_N1[tid]);
		std::cout << std::endl;
		std::cout << "H_N:" << std::endl;
		ft.display_real_vector(this->H_N[tid]);
		std::cout << std::endl;

		std::cout << "Filter from Y_N1 to Y_N2..." << std::endl
		          << "Y_N2:" << std::endl;
		ft.display_real_vector(this->Y_N2[tid]);
		std::cout << std::endl;

		std::cout << "Demodulate from Y_N2 to Y_N3..." << std::endl
		          << "Y_N3:" << std::endl;
		ft.display_real_vector(this->Y_N3[tid]);
		std::cout << std::endl;
	}
	else // additive channel (AWGN, USER, NO)
	{
		std::cout << "Add noise from X_N3 to Y_N1..." << std::endl
		          << "Y_N1:" << std::endl;
		ft.display_real_vector(this->Y_N1[tid]);
		std::cout << std::endl;

		std::cout << "Filter from Y_N1 to Y_N2..." << std::endl
		          << "Y_N2:" << std::endl;
		ft.display_real_vector(this->Y_N2[tid]);
		std::cout << std::endl;

		std::cout << "Demodulate from Y_N2 to Y_N3..." << std::endl
		          << "Y_N3:" << std::endl;
		ft.display_real_vector(this->Y_N3[tid]);
		std::cout << std::endl;
	}

	std::cout << "Make the quantization from Y_N3 to Y_N4..." << std::endl
	          << "Y_N4:" << std::endl;
	ft.display_real_vector(this->Y_N4[tid]);
	std::cout << std::endl;

	std::cout << "Depuncture Y_N4 and generate Y_N5..." << std::endl
	          << "Y_N5:" << std::endl;
	ft.display_real_vector(this->Y_N5[tid]);
	std::cout << std::endl;

//	if (this->simu_params.coset)
//	{
//		std::cout << "Apply the coset approach on Y_N5..." << std::endl
//		          << "Y_N5:" << std::endl;
//		ft.display_real_vector(this->Y_N5[tid]);
//		std::cout << std::endl;
//	}

	std::cout << "Decode Y_N5 and generate V_K1..." << std::endl
	          << "V_K1:" << std::endl;
	if (this->params.coset)
		ft.display_bit_vector(this->V_K1[tid]);
	else
		ft.display_bit_vector(this->V_K1[tid], this->U_K2[tid]);
	std::cout << std::endl;

//	if (this->simu_params.coset)
//	{
//		std::cout << "Apply the coset approach on V_K1..." << std::endl
//		          << "V_K1:" << std::endl;
//		ft.display_bit_vector(this->V_K1[tid], this->U_K2[tid]);
//		std::cout << std::endl;
//	}

	std::cout << "Extract the CRC bits from V_K1 and keep only the info. bits in V_K2..." << std::endl
	          << "V_K2:" << std::endl;
	ft.display_real_vector(this->V_K2[tid], this->U_K1[tid]);
	std::cout << std::endl;

	this->mutex_debug.unlock();
}

template <typename B, typename R, typename Q>
Terminal_BFER<B>* BFER_std_threads<B,R,Q>
::build_terminal()
{
#ifdef ENABLE_MPI
	return BFER_std<B,R,Q>::build_terminal();
#else
	this->durations_red[std::make_pair(11, "Decoder")] = std::chrono::nanoseconds(0);
	const auto &d_dec = this->durations_red[std::make_pair(11, "Decoder")];

	return factory::Terminal_BFER::build<B>(*this->params.ter, *this->monitor_red, &d_dec);
#endif
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::BFER_std_threads<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER_std_threads<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER_std_threads<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER_std_threads<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER_std_threads<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
