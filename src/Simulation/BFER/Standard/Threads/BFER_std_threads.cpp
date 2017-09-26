#include <cstring>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Display/bash_tools.h"

#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER_std_threads.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_std_threads<B,R,Q>
::BFER_std_threads(const factory::BFER_std::parameters &params)
: BFER_std<B,R,Q>(params)
{
	if (this->params.err_track_revert)
	{
		if (this->params.n_threads != 1)
			std::clog << tools::format_warning("Multi-threading detected with error tracking revert feature! "
			                                   "Each thread will play the same frames. Please run with one thread.")
			          << std::endl;
	}
}

template <typename B, typename R, typename Q>
BFER_std_threads<B,R,Q>
::~BFER_std_threads()
{
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::__build_communication_chain(const int tid)
{
	BFER_std<B,R,Q>::__build_communication_chain(tid);

	auto &source    = *this->source[tid];
	auto &crc       = *this->crc   [tid];
	auto &encoder   = *this->codec [tid]->get_encoder();
	auto &puncturer = *this->codec [tid]->get_puncturer();
	auto &modem     = *this->modem [tid];

	if (this->params.src->type == "AZCW")
	{
		auto src_data = (uint8_t*)(source   ["generate"]["U_K" ].get_dataptr());
		auto crc_data = (uint8_t*)(crc      ["build"   ]["U_K2"].get_dataptr());
		auto enc_data = (uint8_t*)(encoder  ["encode"  ]["X_N" ].get_dataptr());
		auto pct_data = (uint8_t*)(puncturer["puncture"]["X_N2"].get_dataptr());

		auto src_bytes = source   ["generate"]["U_K" ].get_databytes();
		auto crc_bytes = crc      ["build"   ]["U_K2"].get_databytes();
		auto enc_bytes = encoder  ["encode"  ]["X_N" ].get_databytes();
		auto pct_bytes = puncturer["puncture"]["X_N2"].get_databytes();

		std::fill(src_data, src_data + src_bytes, 0);
		std::fill(crc_data, crc_data + crc_bytes, 0);
		std::fill(enc_data, enc_data + enc_bytes, 0);
		std::fill(pct_data, pct_data + pct_bytes, 0);

		puncturer["puncture"]["X_N2"].bind(modem["modulate"]["X_N1"]);
	}

	if (this->params.err_track_enable)
	{
		if (this->params.src->type != "AZCW")
		{
			auto src_data = (B*)(source["generate"]["U_K"].get_dataptr());
			auto src_size = source["generate"]["U_K"].get_databytes() / sizeof(B);
			this->dumper[tid]->register_data(src_data, src_size, "src", false, {});
		}

		if (this->params.coset)
		{
			auto enc_data = (B*)(encoder["encode"]["X_N"].get_dataptr());
			auto enc_size = encoder["encode"]["X_N"].get_databytes() / sizeof(B);
			this->dumper[tid]->register_data(enc_data, enc_size, "enc", false, {(unsigned)this->params.cdc->enc->K});
		}
	}
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::_launch()
{
	BFER_std<B,R,Q>::_launch();

	std::vector<std::thread> threads(this->params.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1] = std::thread(BFER_std_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	BFER_std_threads<B,R,Q>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1].join();

	if (!this->prev_err_message.empty())
		throw std::runtime_error(this->prev_err_message);
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::start_thread(BFER_std_threads<B,R,Q> *simu, const int tid)
{
	try
	{
		simu->simulation_loop(tid);
	}
	catch (std::exception const& e)
	{
		module::Monitor::stop();

		simu->mutex_exception.lock();
		if (simu->prev_err_message != e.what())
			if (std::strlen(e.what()) > simu->prev_err_message.size())
				simu->prev_err_message = e.what();
		simu->mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::simulation_loop(const int tid)
{
	using namespace std::chrono;
	auto t_snr = steady_clock::now();

	auto &source     = *this->source    [tid];
	auto &crc        = *this->crc       [tid];
	auto &encoder    = *this->codec     [tid]->get_encoder();
	auto &puncturer  = *this->codec     [tid]->get_puncturer();
	auto &modem      = *this->modem     [tid];
	auto &channel    = *this->channel   [tid];
	auto &quantizer  = *this->quantizer [tid];
	auto &coset_real = *this->coset_real[tid];
	auto &decoder    = *this->codec     [tid]->get_decoder_siho();
	auto &coset_bit  = *this->coset_bit [tid];
	auto &monitor    = *this->monitor   [tid];

	while (!this->monitor_red->fe_limit_achieved() && // while max frame error count has not been reached
	       (this->params.stop_time == seconds(0) || (steady_clock::now() - t_snr) < this->params.stop_time) &&
	       (this->monitor_red->get_n_analyzed_fra() < this->max_fra || this->max_fra == 0))
	{
		if (this->params.debug)
		{
			if (!monitor["check_errors"].get_n_calls())
				std::cout << "#" << std::endl;

			std::cout << "# -------------------------------" << std::endl;
			std::cout << "# New communication (n°" << monitor["check_errors"].get_n_calls() << ")" << std::endl;
			std::cout << "# -------------------------------" << std::endl;
			std::cout << "#" << std::endl;
		}

		if (this->params.src->type != "AZCW")
		{
			source   ["generate"].exec();
			source   ["generate"]["U_K" ].bind(crc      ["build"   ]["U_K1"]);
			crc      ["build"   ]["U_K2"].bind(encoder  ["encode"  ]["U_K" ]);
			encoder  ["encode"  ]["X_N" ].bind(puncturer["puncture"]["X_N1"]);
			puncturer["puncture"]["X_N2"].bind(modem    ["modulate"]["X_N1"]);
		}

		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			modem  ["modulate"     ]["X_N2"].bind(channel  ["add_noise_wg" ]["X_N" ]);
			channel["add_noise_wg" ]["Y_N" ].bind(modem    ["filter"       ]["Y_N1"]);
			modem  ["filter"       ]["Y_N2"].bind(modem    ["demodulate_wg"]["Y_N1"]);
			channel["add_noise_wg" ]["H_N" ].bind(modem    ["demodulate_wg"]["H_N" ]);
			modem  ["demodulate_wg"]["Y_N2"].bind(quantizer["process"      ]["Y_N1"]);
		}
		else
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

