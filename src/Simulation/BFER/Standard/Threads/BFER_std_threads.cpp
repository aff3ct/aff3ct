#include <string>
#include <vector>
#include <chrono>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Display/bash_tools.h"

#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER_std_threads.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_std_threads<B,R,Q>
::BFER_std_threads(const factory::BFER_std::parameters &params, tools::Codec<B,Q> &codec)
: BFER_std<B,R,Q>(params, codec)
{
#ifdef ENABLE_MPI
	if (this->params.debug || this->params.benchs)
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "The debug and bench modes are unavailable in MPI.");
#endif

	if (this->params.err_track_revert)
	{
		if (this->params.n_threads != 1)
			std::clog << tools::format_warning("Multi-threading detected with error tracking revert feature!"
			                                   " Each thread will play the same frames. Please run with one thread.")
			          << std::endl;
	}

//	this->data_sizes[std::make_pair( 0, "Source"      )] = (unsigned)this->U_K1[0].size();
//	this->data_sizes[std::make_pair( 1, "CRC build"   )] = (unsigned)this->U_K2[0].size();
//	this->data_sizes[std::make_pair( 2, "Encoder"     )] = (unsigned)this->X_N1[0].size();
//	this->data_sizes[std::make_pair( 3, "Puncturer"   )] = (unsigned)this->X_N2[0].size();
//	this->data_sizes[std::make_pair( 4, "Modulator"   )] = (unsigned)this->X_N3[0].size();
//	this->data_sizes[std::make_pair( 5, "Channel"     )] = (unsigned)this->Y_N1[0].size();
//	this->data_sizes[std::make_pair( 6, "Filter"      )] = (unsigned)this->Y_N2[0].size();
//	this->data_sizes[std::make_pair( 7, "Demodulator" )] = (unsigned)this->Y_N3[0].size();
//	this->data_sizes[std::make_pair( 8, "Quantizer"   )] = (unsigned)this->Y_N4[0].size();
//	this->data_sizes[std::make_pair( 9, "Depuncturer" )] = (unsigned)this->Y_N5[0].size();
//	this->data_sizes[std::make_pair(10, "Coset real"  )] = (unsigned)this->Y_N5[0].size();
//	this->data_sizes[std::make_pair(11, "Decoder"     )] = (unsigned)this->V_K1[0].size();
//	this->data_sizes[std::make_pair(15, "Coset bit"   )] = (unsigned)this->V_K1[0].size();
//	this->data_sizes[std::make_pair(16, "CRC extract" )] = (unsigned)this->V_K2[0].size();
//	this->data_sizes[std::make_pair(17, "Check errors")] = (unsigned)this->V_K2[0].size();
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
		auto src_data = (uint8_t*)((*this->source   [tid])["generate"]["U_K" ].get_dataptr());
		auto crc_data = (uint8_t*)((*this->crc      [tid])["build"   ]["U_K2"].get_dataptr());
		auto enc_data = (uint8_t*)((*this->encoder  [tid])["encode"  ]["X_N" ].get_dataptr());
		auto pct_data = (uint8_t*)((*this->puncturer[tid])["puncture"]["X_N2"].get_dataptr());

		auto src_bytes = (*this->source   [tid])["generate"]["U_K" ].get_databytes();
		auto crc_bytes = (*this->crc      [tid])["build"   ]["U_K2"].get_databytes();
		auto enc_bytes = (*this->encoder  [tid])["encode"  ]["X_N" ].get_databytes();
		auto pct_bytes = (*this->puncturer[tid])["puncture"]["X_N2"].get_databytes();

		std::fill(src_data, src_data + src_bytes, 0);
		std::fill(crc_data, crc_data + crc_bytes, 0);
		std::fill(enc_data, enc_data + enc_bytes, 0);
		std::fill(pct_data, pct_data + pct_bytes, 0);

		(*this->puncturer[tid])["puncture"]["X_N2"].bind((*this->modem[tid])["modulate"]["X_N1"]);
	}

	if (this->params.err_track_enable)
	{
		if (this->params.src->type != "AZCW")
		{
			auto src_data = (B*)((*this->source[tid])["generate"]["U_K"].get_dataptr());
			auto src_size = (*this->source[tid])["generate"]["U_K"].get_databytes() / sizeof(B);
			this->dumper[tid]->register_data(src_data, src_size, "src", false, {});
		}

		if (this->params.coset)
		{
			auto enc_data = (B*)((*this->encoder[tid])["encode"]["X_N"].get_dataptr());
			auto enc_size = (*this->encoder[tid])["encode"]["X_N"].get_databytes() / sizeof(B);
			this->dumper[tid]->register_data(enc_data, enc_size, "enc", false, {(unsigned)this->params.enc->K});
		}

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
		module::Monitor<B>::stop();

		simu->mutex_exception.lock();
		if (simu->prev_err_message != e.what())
		{
			std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
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

	auto &source     = *this->source    [tid];
	auto &crc        = *this->crc       [tid];
	auto &encoder    = *this->encoder   [tid];
	auto &puncturer  = *this->puncturer [tid];
	auto &modem      = *this->modem     [tid];
	auto &channel    = *this->channel   [tid];
	auto &quantizer  = *this->quantizer [tid];
	auto &coset_real = *this->coset_real[tid];
	auto &decoder    = *this->decoder   [tid];
	auto &coset_bit  = *this->coset_bit [tid];
	auto &monitor    = *this->monitor   [tid];

	// simulation loop
	while (!this->monitor_red->fe_limit_achieved() && // while max frame error count has not been reached
	       (this->params.stop_time == seconds(0) || (steady_clock::now() - t_snr) < this->params.stop_time) &&
	       (this->monitor_red->get_n_analyzed_fra() < this->max_fra || this->max_fra == 0))
	{
		if (this->params.src->type != "AZCW")
		{
			source   ["generate"].exec();
			source   ["generate"]["U_K" ].bind(crc      ["build"   ]["U_K1"]);
			crc      ["build"   ]["U_K2"].bind(encoder  ["encode"  ]["U_K" ]);
			encoder  ["encode"  ]["X_N" ].bind(puncturer["puncture"]["X_N1"]);
			puncturer["puncture"]["X_N2"].bind(modem    ["modulate"]["X_N1"]);
		}

		// Rayleigh channel
		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			modem  ["modulate"             ]["X_N2"].bind(channel  ["add_noise_with_gains" ]["X_N" ]);
			channel["add_noise_with_gains" ]["Y_N" ].bind(modem    ["filter"               ]["Y_N1"]);
			modem  ["filter"               ]["Y_N2"].bind(modem    ["demodulate_with_gains"]["Y_N1"]);
			channel["add_noise_with_gains" ]["H_N" ].bind(modem    ["demodulate_with_gains"]["H_N" ]);
			modem  ["demodulate_with_gains"]["Y_N2"].bind(quantizer["process"              ]["Y_N1"]);
		}
		else // additive channel (AWGN, USER, NO)
		{
			modem  ["modulate"  ]["X_N2"].bind(channel  ["add_noise" ]["X_N" ]);
			channel["add_noise" ]["Y_N" ].bind(modem    ["filter"    ]["Y_N1"]);
			modem  ["filter"    ]["Y_N2"].bind(modem    ["demodulate"]["Y_N1"]);
			modem  ["demodulate"]["Y_N2"].bind(quantizer["process"   ]["Y_N1"]);
		}

		quantizer["process"]["Y_N2"].bind(puncturer["depuncture"]["Y_N1"]);

		if (this->params.coset)
		{
			encoder  ["encode"    ]["X_N" ].bind(coset_real["apply"]["ref"    ]);
			puncturer["depuncture"]["Y_N2"].bind(coset_real["apply"]["in_data"]);

			if (this->params.coded_monitoring)
			{
				coset_real["apply"            ]["out_data"].bind(decoder  ["decode_siho_coded"]["Y_N"    ]);
				encoder   ["encode"           ]["X_N"     ].bind(coset_bit["apply"            ]["ref"    ]);
				decoder   ["decode_siho_coded"]["V_N"     ].bind(coset_bit["apply"            ]["in_data"]);
			}
			else
			{
				coset_real["apply"      ]["out_data"].bind(decoder  ["decode_siho"]["Y_N"    ]);
				crc       ["build"      ]["U_K2"    ].bind(coset_bit["apply"      ]["ref"    ]);
				decoder   ["decode_siho"]["V_K"     ].bind(coset_bit["apply"      ]["in_data"]);
				coset_bit ["apply"      ]["out_data"].bind(crc      ["extract"    ]["V_K1"   ]);
			}
		}
		else
		{
			if (this->params.coded_monitoring)
			{
				puncturer["depuncture"]["Y_N2"].bind(decoder["decode_siho_coded"]["Y_N"]);
			}
			else
			{
				puncturer["depuncture" ]["Y_N2"].bind(decoder["decode_siho"]["Y_N" ]);
				decoder  ["decode_siho"]["V_K" ].bind(crc    ["extract"    ]["V_K1"]);
			}
		}

		if (this->params.coded_monitoring)
		{
			encoder["encode"]["X_N"].bind(monitor["check_errors"]["U"]);

			if (this->params.coset)
			{
				coset_bit["apply"]["out_data"].bind(monitor["check_errors"]["V"]);
			}
			else
			{
				decoder["decode_siho_coded"]["V_N"].bind(monitor["check_errors"]["V"]);
			}
		}
		else
		{
			source["generate"]["U_K" ].bind(monitor["check_errors"]["U"]);
			crc   ["extract" ]["V_K2"].bind(monitor["check_errors"]["V"]);
		}
	}
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::simulation_loop_bench(const int tid)
{
//	using namespace std::chrono;
//
//	this->barrier(tid);
//	auto t_start = std::chrono::steady_clock::now(); // start time
//
//	this->barrier(tid);
//	if (this->params.coded_monitoring)
//		for (auto i = 0; i < this->params.benchs; i++)
//			this->decoder[tid]->decode_siho_coded(this->Y_N5[tid], this->V_N1[tid]);
//	else
//		for (auto i = 0; i < this->params.benchs; i++)
//			this->decoder[tid]->decode_siho(this->Y_N5[tid], this->V_K1[tid]);
//	this->barrier(tid);
//
//	auto t_stop = std::chrono::steady_clock::now(); // stop time
//
//	auto frames   = (float)this->params.benchs *
//	                (float)this->params.src->n_frames *
//	                (float)this->params.n_threads;
//	auto bits     = (float)frames * (float)this->params.dec->N_cw;
//	auto duration = t_stop - t_start;
//
//	auto  bps = (float)bits / (float)(duration.count() * 0.000000001f);
//	auto kbps =  bps * 0.001f;
//	auto mbps = kbps * 0.001f;
//
//	auto latency_ns = (float)duration.count() / (float)this->params.benchs;
//	auto latency_us = latency_ns * 0.001f;
//
//	if (tid == 0)
//		std::cout << "  SNR (Eb/N0) = "    << std::setw(5) << std::fixed << std::setprecision(2) << this->snr
//		          << " dB"   << ", "
//		          << "coded throughput = " << std::setw(8) << std::fixed << std::setprecision(4) << mbps
//		          << " Mbps" << ", "
//		          << "latency = "          << std::setw(8) << std::fixed << std::setprecision(4) << latency_us
//		          << " us."  << std::endl;
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::display_debug()
{
//	this->mutex_debug.lock();
//
//	const auto tid = this->thread_id[std::this_thread::get_id()];
//	tools::Frame_trace<B> ft(this->params.debug_limit, this->params.debug_precision);
//
//	std::cout << "-------------------------------" << std::endl;
//	std::cout << "New encoding/decoding session !" << std::endl;
//	std::cout << "Thread id: " << tid              << std::endl;
//	std::cout << "Frame n°" << this->monitor_red->get_n_analyzed_fra() << std::endl;
//	std::cout << "-------------------------------" << std::endl;
//
//	if (this->params.src->type != "AZCW")
//	{
//		std::cout << "Generate random bits U_K1..." << std::endl
//		          << "U_K1:" << std::endl;
//		ft.display_bit_vector(this->U_K1[tid]);
//		std::cout << std::endl;
//
//		std::cout << "Build the CRC from U_K1 into U_K2..." << std::endl
//		          << "U_K2:" << std::endl;
//		ft.display_bit_vector(this->U_K2[tid]);
//		std::cout << std::endl;
//
//		std::cout << "Encode U_K2 in X_N1..." << std::endl
//		          << "X_N1:" << std::endl;
//		ft.display_bit_vector(this->X_N1[tid]);
//		std::cout << std::endl;
//
//		std::cout << "Puncture X_N1 in X_N2..." << std::endl
//		          << "X_N2:" << std::endl;
//		ft.display_bit_vector(this->X_N2[tid]);
//		std::cout << std::endl;
//
//		// modulate
//		std::cout << "Modulate X_N2 in X_N3..." << std::endl
//		          << "X_N3:" << std::endl;
//		ft.display_real_vector(this->X_N3[tid]);
//		std::cout << std::endl;
//	}
//	else
//	{
//		std::cout << "U_K1:" << std::endl;
//		ft.display_bit_vector(this->U_K1[tid]);
//		std::cout << std::endl;
//
//		std::cout << "U_K2:" << std::endl;
//		ft.display_bit_vector(this->U_K2[tid]);
//		std::cout << std::endl;
//
//		std::cout << "X_N2:" << std::endl;
//		ft.display_bit_vector(this->X_N2[tid]);
//		std::cout << std::endl;
//
//		std::cout << "X_N3:" << std::endl;
//		ft.display_real_vector(this->X_N3[tid]);
//		std::cout << std::endl;
//	}
//
//	// Rayleigh channel
//	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
//	{
//		std::cout << "Add noise from X_N3 to Y_N1..." << std::endl
//		          << "Y_N1:" << std::endl;
//		ft.display_real_vector(this->Y_N1[tid]);
//		std::cout << std::endl;
//		std::cout << "H_N:" << std::endl;
//		ft.display_real_vector(this->H_N[tid]);
//		std::cout << std::endl;
//
//		std::cout << "Filter from Y_N1 to Y_N2..." << std::endl
//		          << "Y_N2:" << std::endl;
//		ft.display_real_vector(this->Y_N2[tid]);
//		std::cout << std::endl;
//
//		std::cout << "Demodulate from Y_N2 to Y_N3..." << std::endl
//		          << "Y_N3:" << std::endl;
//		ft.display_real_vector(this->Y_N3[tid]);
//		std::cout << std::endl;
//	}
//	else // additive channel (AWGN, USER, NO)
//	{
//		std::cout << "Add noise from X_N3 to Y_N1..." << std::endl
//		          << "Y_N1:" << std::endl;
//		ft.display_real_vector(this->Y_N1[tid]);
//		std::cout << std::endl;
//
//		std::cout << "Filter from Y_N1 to Y_N2..." << std::endl
//		          << "Y_N2:" << std::endl;
//		ft.display_real_vector(this->Y_N2[tid]);
//		std::cout << std::endl;
//
//		std::cout << "Demodulate from Y_N2 to Y_N3..." << std::endl
//		          << "Y_N3:" << std::endl;
//		ft.display_real_vector(this->Y_N3[tid]);
//		std::cout << std::endl;
//	}
//
//	std::cout << "Make the quantization from Y_N3 to Y_N4..." << std::endl
//	          << "Y_N4:" << std::endl;
//	ft.display_real_vector(this->Y_N4[tid]);
//	std::cout << std::endl;
//
//	std::cout << "Depuncture Y_N4 and generate Y_N5..." << std::endl
//	          << "Y_N5:" << std::endl;
//	ft.display_real_vector(this->Y_N5[tid]);
//	std::cout << std::endl;
//
////	if (this->simu_params.coset)
////	{
////		std::cout << "Apply the coset approach on Y_N5..." << std::endl
////		          << "Y_N5:" << std::endl;
////		ft.display_real_vector(this->Y_N5[tid]);
////		std::cout << std::endl;
////	}
//
//	if (this->params.coded_monitoring)
//	{
//		std::cout << "Decode Y_N5 and generate V_N1..." << std::endl
//		          << "V_N1:" << std::endl;
//		if (this->params.coset)
//			ft.display_bit_vector(this->V_N1[tid]);
//		else
//			ft.display_bit_vector(this->V_N1[tid], this->X_N1[tid]);
//		std::cout << std::endl;
//
////		if (this->simu_params.coset)
////		{
////			std::cout << "Apply the coset approach on V_N1..." << std::endl
////			          << "V_N1:" << std::endl;
////			ft.display_bit_vector(this->V_N1[tid], this->X_N1[tid]);
////			std::cout << std::endl;
////		}
//	}
//	else
//	{
//		std::cout << "Decode Y_N5 and generate V_K1..." << std::endl
//		          << "V_K1:" << std::endl;
//		if (this->params.coset)
//			ft.display_bit_vector(this->V_K1[tid]);
//		else
//			ft.display_bit_vector(this->V_K1[tid], this->U_K2[tid]);
//		std::cout << std::endl;
//
////		if (this->simu_params.coset)
////		{
////			std::cout << "Apply the coset approach on V_K1..." << std::endl
////			          << "V_K1:" << std::endl;
////			ft.display_bit_vector(this->V_K1[tid], this->U_K2[tid]);
////			std::cout << std::endl;
////		}
//
//		std::cout << "Extract the CRC bits from V_K1 and keep only the info. bits in V_K2..." << std::endl
//		          << "V_K2:" << std::endl;
//		ft.display_bit_vector(this->V_K2[tid], this->U_K1[tid]);
//		std::cout << std::endl;
//	}
//
//	this->mutex_debug.unlock();
}

template <typename B, typename R, typename Q>
tools::Terminal_BFER<B>* BFER_std_threads<B,R,Q>
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
