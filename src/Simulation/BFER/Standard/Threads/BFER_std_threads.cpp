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
::BFER_std_threads(const factory::BFER_std::parameters &params_BFER_std)
: BFER_std<B,R,Q>(params_BFER_std)
{
	if (this->params_BFER_std.err_track_revert)
	{
		if (this->params_BFER_std.n_threads != 1)
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
::_launch()
{
	BFER_std<B,R,Q>::_launch();

	std::vector<std::thread> threads(this->params_BFER_std.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params_BFER_std.n_threads; tid++)
		threads[tid -1] = std::thread(BFER_std_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	BFER_std_threads<B,R,Q>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params_BFER_std.n_threads; tid++)
		threads[tid -1].join();

	if (!this->prev_err_messages.empty())
		throw std::runtime_error(this->prev_err_messages.back());
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::start_thread(BFER_std_threads<B,R,Q> *simu, const int tid)
{
	try
	{
		simu->sockets_binding(tid);
		simu->simulation_loop(tid);
	}
	catch (std::exception const& e)
	{
		module::Monitor::stop();

		simu->mutex_exception.lock();
		if (std::find(simu->prev_err_messages.begin(), simu->prev_err_messages.end(), e.what()) == simu->prev_err_messages.end())
			simu->prev_err_messages.push_back(e.what());
		simu->mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::sockets_binding(const int tid)
{
	using namespace module;

	auto &src = *this->source    [tid];
	auto &crc = *this->crc       [tid];
	auto &enc = *this->codec     [tid]->get_encoder();
	auto &pct = *this->codec     [tid]->get_puncturer();
	auto &mdm = *this->modem     [tid];
	auto &chn = *this->channel   [tid];
	auto &qnt = *this->quantizer [tid];
	auto &csr = *this->coset_real[tid];
	auto &dec = *this->codec     [tid]->get_decoder_siho();
	auto &csb = *this->coset_bit [tid];
	auto &mnt = *this->monitor   [tid];

	if (this->params_BFER_std.src->type == "AZCW")
	{
		auto src_data = (uint8_t*)(src[src::tsk::generate][src::sck::generate::U_K ].get_dataptr());
		auto crc_data = (uint8_t*)(crc[crc::tsk::build   ][crc::sck::build   ::U_K2].get_dataptr());
		auto enc_data = (uint8_t*)(enc[enc::tsk::encode  ][enc::sck::encode  ::X_N ].get_dataptr());
		auto pct_data = (uint8_t*)(pct[pct::tsk::puncture][pct::sck::puncture::X_N2].get_dataptr());

		auto src_bytes = src[src::tsk::generate][src::sck::generate::U_K ].get_databytes();
		auto crc_bytes = crc[crc::tsk::build   ][crc::sck::build   ::U_K2].get_databytes();
		auto enc_bytes = enc[enc::tsk::encode  ][enc::sck::encode  ::X_N ].get_databytes();
		auto pct_bytes = pct[pct::tsk::puncture][pct::sck::puncture::X_N2].get_databytes();

		std::fill(src_data, src_data + src_bytes, 0);
		std::fill(crc_data, crc_data + crc_bytes, 0);
		std::fill(enc_data, enc_data + enc_bytes, 0);
		std::fill(pct_data, pct_data + pct_bytes, 0);

		mdm[mdm::tsk::modulate][mdm::sck::modulate::X_N1](pct[pct::tsk::puncture][pct::sck::puncture::X_N2]);
		mdm[mdm::tsk::modulate].exec();
		mdm[mdm::tsk::modulate].reset_stats();
	}
	else
	{
		crc[crc::tsk::build   ][crc::sck::build   ::U_K1](src[src::tsk::generate][src::sck::generate::U_K ]);
		enc[enc::tsk::encode  ][enc::sck::encode  ::U_K ](crc[crc::tsk::build   ][crc::sck::build   ::U_K2]);
		pct[pct::tsk::puncture][pct::sck::puncture::X_N1](enc[enc::tsk::encode  ][enc::sck::encode  ::X_N ]);
		mdm[mdm::tsk::modulate][mdm::sck::modulate::X_N1](pct[pct::tsk::puncture][pct::sck::puncture::X_N2]);
	}

	if (this->params_BFER_std.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		chn[chn::tsk::add_noise_wg ][chn::sck::add_noise_wg ::X_N ](mdm[mdm::tsk::modulate     ][mdm::sck::modulate     ::X_N2]);
		mdm[mdm::tsk::demodulate_wg][mdm::sck::demodulate_wg::H_N ](chn[chn::tsk::add_noise_wg ][chn::sck::add_noise_wg ::H_N ]);
		mdm[mdm::tsk::filter       ][mdm::sck::filter       ::Y_N1](chn[chn::tsk::add_noise_wg ][chn::sck::add_noise_wg ::Y_N ]);
		mdm[mdm::tsk::demodulate_wg][mdm::sck::demodulate_wg::Y_N1](mdm[mdm::tsk::filter       ][mdm::sck::filter       ::Y_N2]);
		qnt[qnt::tsk::process      ][qnt::sck::process      ::Y_N1](mdm[mdm::tsk::demodulate_wg][mdm::sck::demodulate_wg::Y_N2]);
	}
	else
	{
		chn[chn::tsk::add_noise ][chn::sck::add_noise ::X_N ](mdm[mdm::tsk::modulate  ][mdm::sck::modulate  ::X_N2]);
		mdm[mdm::tsk::filter    ][mdm::sck::filter    ::Y_N1](chn[chn::tsk::add_noise ][chn::sck::add_noise ::Y_N ]);
		mdm[mdm::tsk::demodulate][mdm::sck::demodulate::Y_N1](mdm[mdm::tsk::filter    ][mdm::sck::filter    ::Y_N2]);
		qnt[qnt::tsk::process   ][qnt::sck::process   ::Y_N1](mdm[mdm::tsk::demodulate][mdm::sck::demodulate::Y_N2]);
	}

	pct[pct::tsk::depuncture][pct::sck::depuncture::Y_N1](qnt[qnt::tsk::process][qnt::sck::process::Y_N2]);

	if (this->params_BFER_std.coset)
	{
		csr[cst::tsk::apply][cst::sck::apply::ref](enc[enc::tsk::encode    ][enc::sck::encode    ::X_N ]);
		csr[cst::tsk::apply][cst::sck::apply::in ](pct[pct::tsk::depuncture][pct::sck::depuncture::Y_N2]);

		if (this->params_BFER_std.coded_monitoring)
		{
			dec[dec::tsk::decode_siho_cw][dec::sck::decode_siho_cw::Y_N](csr[cst::tsk::apply         ][cst::sck::apply         ::out]);
			csb[cst::tsk::apply         ][cst::sck::apply         ::ref](enc[enc::tsk::encode        ][enc::sck::encode        ::X_N]);
			csb[cst::tsk::apply         ][cst::sck::apply         ::in ](dec[dec::tsk::decode_siho_cw][dec::sck::decode_siho_cw::V_N]);
		}
		else
		{
			dec[dec::tsk::decode_siho][dec::sck::decode_siho::Y_N ](csr[cst::tsk::apply      ][cst::sck::apply      ::out ]);
			csb[cst::tsk::apply      ][cst::sck::apply      ::ref ](crc[crc::tsk::build      ][crc::sck::build      ::U_K2]);
			csb[cst::tsk::apply      ][cst::sck::apply      ::in  ](dec[dec::tsk::decode_siho][dec::sck::decode_siho::V_K ]);
			crc[crc::tsk::extract    ][crc::sck::extract    ::V_K1](csb[cst::tsk::apply      ][cst::sck::apply      ::out ]);
		}
	}
	else
	{
		if (this->params_BFER_std.coded_monitoring)
		{
			dec[dec::tsk::decode_siho_cw][dec::sck::decode_siho_cw::Y_N](pct[pct::tsk::depuncture][pct::sck::depuncture::Y_N2]);
		}
		else
		{
			dec[dec::tsk::decode_siho][dec::sck::decode_siho::Y_N ](pct[pct::tsk::depuncture ][pct::sck::depuncture ::Y_N2]);
			crc[crc::tsk::extract    ][crc::sck::extract    ::V_K1](dec[dec::tsk::decode_siho][dec::sck::decode_siho::V_K ]);
		}
	}

	if (this->params_BFER_std.coded_monitoring)
	{
		mnt[mnt::tsk::check_errors][mnt::sck::check_errors::U](enc[enc::tsk::encode][enc::sck::encode::X_N]);

		if (this->params_BFER_std.coset)
		{
			mnt[mnt::tsk::check_errors][mnt::sck::check_errors::V](csb[cst::tsk::apply][cst::sck::apply::out]);
		}
		else
		{
			mnt[mnt::tsk::check_errors][mnt::sck::check_errors::V](dec[dec::tsk::decode_siho_cw][dec::sck::decode_siho_cw::V_N]);
		}
	}
	else
	{
		mnt[mnt::tsk::check_errors][mnt::sck::check_errors::U](src[src::tsk::generate][src::sck::generate::U_K ]);
		mnt[mnt::tsk::check_errors][mnt::sck::check_errors::V](crc[crc::tsk::extract ][crc::sck::extract ::V_K2]);
	}
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::simulation_loop(const int tid)
{
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

	using namespace module;
	using namespace std::chrono;
	auto t_snr = steady_clock::now();

	// communication chain execution
	while (!this->monitor_red->fe_limit_achieved() && // while max frame error count has not been reached
	       (this->params_BFER_std.stop_time == seconds(0) || 
	       (steady_clock::now() - t_snr) < this->params_BFER_std.stop_time) &&
	       (this->monitor_red->get_n_analyzed_fra() < this->max_fra || this->max_fra == 0))
	{
		if (this->params_BFER_std.debug)
		{
			if (!monitor[mnt::tsk::check_errors].get_n_calls())
				std::cout << "#" << std::endl;

			std::cout << "# -------------------------------" << std::endl;
			std::cout << "# New communication (n°" << monitor[mnt::tsk::check_errors].get_n_calls() << ")" << std::endl;
			std::cout << "# -------------------------------" << std::endl;
			std::cout << "#" << std::endl;
		}

		if (this->params_BFER_std.src->type != "AZCW")
		{
			source   [src::tsk::generate].exec();
			crc      [crc::tsk::build   ].exec();
			encoder  [enc::tsk::encode  ].exec();
			puncturer[pct::tsk::puncture].exec();
			modem    [mdm::tsk::modulate].exec();
		}

		if (this->params_BFER_std.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			channel  [chn::tsk::add_noise_wg ].exec();
			modem    [mdm::tsk::filter       ].exec();
			modem    [mdm::tsk::demodulate_wg].exec();
			quantizer[qnt::tsk::process      ].exec();
		}
		else
		{
			channel  [chn::tsk::add_noise ].exec();
			modem    [mdm::tsk::filter    ].exec();
			modem    [mdm::tsk::demodulate].exec();
			quantizer[qnt::tsk::process   ].exec();
		}

		puncturer[pct::tsk::depuncture].exec();

		if (this->params_BFER_std.coset)
		{
			coset_real[cst::tsk::apply].exec();

			if (this->params_BFER_std.coded_monitoring)
			{
				decoder  [dec::tsk::decode_siho_cw].exec();
				coset_bit[cst::tsk::apply         ].exec();
			}
			else
			{
				decoder  [dec::tsk::decode_siho].exec();
				coset_bit[cst::tsk::apply      ].exec();
				crc      [crc::tsk::extract    ].exec();
			}
		}
		else
		{
			if (this->params_BFER_std.coded_monitoring)
			{
				decoder[dec::tsk::decode_siho_cw].exec();
			}
			else
			{
				decoder[dec::tsk::decode_siho].exec();
				crc    [crc::tsk::extract    ].exec();
			}
		}

		monitor[mnt::tsk::check_errors].exec();
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
