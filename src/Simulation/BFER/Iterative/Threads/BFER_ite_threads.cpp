#include <cstring>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Display/bash_tools.h"

#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER_ite_threads.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_ite_threads<B,R,Q>
::BFER_ite_threads(const factory::BFER_ite::parameters &params)
: BFER_ite<B,R,Q>(params)
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
BFER_ite_threads<B,R,Q>
::~BFER_ite_threads()
{
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::_launch()
{
	BFER_ite<B,R,Q>::_launch();

	std::vector<std::thread> threads(this->params.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1] = std::thread(BFER_ite_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	BFER_ite_threads<B,R,Q>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1].join();

	if (!this->prev_err_messages.empty())
		throw std::runtime_error(this->prev_err_messages.back());
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::start_thread(BFER_ite_threads<B,R,Q> *simu, const int tid)
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
			if (simu->prev_err_messages.size() && std::strlen(e.what()) > simu->prev_err_messages.back().size())
				simu->prev_err_messages.push_back(e.what());
		simu->mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::sockets_binding(const int tid)
{
	auto &src = *this->source         [tid];
	auto &crc = *this->crc            [tid];
	auto &cdc = *this->codec          [tid];
	auto &itb = *this->interleaver_bit[tid];
	auto &mdm = *this->modem          [tid];
	auto &chn = *this->channel        [tid];
	auto &qnt = *this->quantizer      [tid];
	auto &itl = *this->interleaver_llr[tid];
	auto &csr = *this->coset_real     [tid];
	auto &csb = *this->coset_bit      [tid];
	auto &mnt = *this->monitor        [tid];

	auto &enc = *cdc.get_encoder();
	auto &dcs = *cdc.get_decoder_siso();
	auto &dch = *cdc.get_decoder_siho();

	using namespace module;

	if (this->params.src->type == "AZCW")
	{
		auto src_data = (uint8_t*)(src[src::tsk::generate  ][src::sck::generate  ::U_K ].get_dataptr());
		auto crc_data = (uint8_t*)(crc[crc::tsk::build     ][crc::sck::build     ::U_K2].get_dataptr());
		auto enc_data = (uint8_t*)(enc[enc::tsk::encode    ][enc::sck::encode    ::X_N ].get_dataptr());
		auto itl_data = (uint8_t*)(itb[itl::tsk::interleave][itl::sck::interleave::itl ].get_dataptr());

		auto src_bytes = src[src::tsk::generate  ][src::sck::generate  ::U_K ].get_databytes();
		auto crc_bytes = crc[crc::tsk::build     ][crc::sck::build     ::U_K2].get_databytes();
		auto enc_bytes = enc[enc::tsk::encode    ][enc::sck::encode    ::X_N ].get_databytes();
		auto itl_bytes = itb[itl::tsk::interleave][itl::sck::interleave::itl ].get_databytes();

		std::fill(src_data, src_data + src_bytes, 0);
		std::fill(crc_data, crc_data + crc_bytes, 0);
		std::fill(enc_data, enc_data + enc_bytes, 0);
		std::fill(itl_data, itl_data + itl_bytes, 0);

		mdm[mdm::tsk::modulate][mdm::sck::modulate::X_N1](itb[itl::tsk::interleave][itl::sck::interleave::itl]);
		mdm[mdm::tsk::modulate].exec();
		mdm[mdm::tsk::modulate].reset_stats();
	}
	else
	{
		crc[crc::tsk::build     ][crc::sck::build     ::U_K1](src[src::tsk::generate  ][src::sck::generate  ::U_K ]);
		enc[enc::tsk::encode    ][enc::sck::encode    ::U_K ](crc[crc::tsk::build     ][crc::sck::build     ::U_K2]);
		itb[itl::tsk::interleave][itl::sck::interleave::nat ](enc[enc::tsk::encode    ][enc::sck::encode    ::X_N ]);
		mdm[mdm::tsk::modulate  ][mdm::sck::modulate  ::X_N1](itb[itl::tsk::interleave][itl::sck::interleave::itl ]);
	}

	if (this->params.coset)
	{
		if (this->params.coded_monitoring)
			csb[cst::tsk::apply][cst::sck::apply::ref](enc[enc::tsk::encode][enc::sck::encode::X_N ]);
		else
			csb[cst::tsk::apply][cst::sck::apply::ref](crc[crc::tsk::build ][crc::sck::build ::U_K2]);

		csr[cst::tsk::apply][cst::sck::apply::ref](enc[enc::tsk::encode][enc::sck::encode::X_N]);
	}

	if (this->params.coded_monitoring)
		mnt[mnt::tsk::check_errors][mnt::sck::check_errors::U](enc[enc::tsk::encode  ][enc::sck::encode  ::X_N]);
	else
		mnt[mnt::tsk::check_errors][mnt::sck::check_errors::U](src[src::tsk::generate][src::sck::generate::U_K]);

	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		chn[chn::tsk::add_noise_wg ][chn::sck::add_noise_wg ::X_N ](mdm[mdm::tsk::modulate     ][mdm::sck::modulate    ::X_N2]);
		mdm[mdm::tsk::demodulate_wg][mdm::sck::demodulate_wg::H_N ](chn[chn::tsk::add_noise_wg ][chn::sck::add_noise_wg::H_N ]);
		mdm[mdm::tsk::filter       ][mdm::sck::filter       ::Y_N1](chn[chn::tsk::add_noise_wg ][chn::sck::add_noise_wg::Y_N ]);
		qnt[qnt::tsk::process      ][qnt::sck::process      ::Y_N1](mdm[mdm::tsk::filter       ][mdm::sck::filter      ::Y_N2]);
		mdm[mdm::tsk::demodulate_wg][mdm::sck::demodulate_wg::Y_N1](qnt[qnt::tsk::process      ][qnt::sck::process     ::Y_N2]);
	}
	else
	{
		chn[chn::tsk::add_noise ][chn::sck::add_noise ::X_N ](mdm[mdm::tsk::modulate ][mdm::sck::modulate ::X_N2]);
		mdm[mdm::tsk::filter    ][mdm::sck::filter    ::Y_N1](chn[chn::tsk::add_noise][chn::sck::add_noise::Y_N ]);
		qnt[qnt::tsk::process   ][qnt::sck::process   ::Y_N1](mdm[mdm::tsk::filter   ][mdm::sck::filter   ::Y_N2]);
		mdm[mdm::tsk::demodulate][mdm::sck::demodulate::Y_N1](qnt[qnt::tsk::process  ][qnt::sck::process  ::Y_N2]);
	}

	itl[itl::tsk::deinterleave][itl::sck::deinterleave::itl](mdm[mdm::tsk::demodulate][mdm::sck::demodulate::Y_N2]);

	// ----------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------- turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	// --------------------------------------------------------------------------------------------------- CRC checking
	if (this->params.crc->type != "NO")
	{
		cdc[cdc::tsk::extract_sys_bit][cdc::sck::extract_sys_bit::Y_N](itl[itl::tsk::deinterleave   ][itl::sck::deinterleave   ::nat]);
		crc[crc::tsk::check          ][crc::sck::check          ::V_K](cdc[cdc::tsk::extract_sys_bit][cdc::sck::extract_sys_bit::V_K]);
	}

	// ------------------------------------------------------------------------------------------------------- decoding
	if (this->params.coset)
	{
		// output socket binding (trick to avoid runtime re-binding)
		dcs[dec::tsk::decode_siso][dec::sck::decode_siso::Y_N2](itl[itl::tsk::deinterleave][itl::sck::deinterleave::nat ]);
		csr[cst::tsk::apply      ][cst::sck::apply      ::in  ](itl[itl::tsk::deinterleave][itl::sck::deinterleave::nat ]);
		dcs[dec::tsk::decode_siso][dec::sck::decode_siso::Y_N1](csr[cst::tsk::apply       ][cst::sck::apply       ::out ]);
		csr[cst::tsk::apply      ][cst::sck::apply      ::in  ](dcs[dec::tsk::decode_siso ][dec::sck::decode_siso ::Y_N2]);
	}
	else
	{
		dcs[dec::tsk::decode_siso][dec::sck::decode_siso::Y_N1](itl[itl::tsk::deinterleave][itl::sck::deinterleave::nat]);
	}

	// --------------------------------------------------------------------------------------------------- interleaving
	if (this->params.coset)
		itl[itl::tsk::interleave][itl::sck::interleave::nat](csr[cst::tsk::apply      ][cst::sck::apply      ::out ]);
	else
		itl[itl::tsk::interleave][itl::sck::interleave::nat](dcs[dec::tsk::decode_siso][dec::sck::decode_siso::Y_N2]);

	// --------------------------------------------------------------------------------------------------- demodulation
	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		// output socket binding (trick to avoid runtime re-binding)
		mdm[mdm::tsk::tdemodulate_wg][mdm::sck::tdemodulate_wg::Y_N3](mdm[mdm::tsk::demodulate  ][mdm::sck::demodulate  ::Y_N2]);
		mdm[mdm::tsk::tdemodulate_wg][mdm::sck::tdemodulate_wg::Y_N1](qnt[qnt::tsk::process     ][qnt::sck::process     ::Y_N2]);
		mdm[mdm::tsk::tdemodulate_wg][mdm::sck::tdemodulate_wg::H_N ](chn[chn::tsk::add_noise_wg][chn::sck::add_noise_wg::H_N ]);
		mdm[mdm::tsk::tdemodulate_wg][mdm::sck::tdemodulate_wg::Y_N2](itl[itl::tsk::interleave  ][itl::sck::interleave  ::itl ]);
	}
	else
	{
		// output socket binding (trick to avoid runtime re-binding)
		mdm[mdm::tsk::tdemodulate][mdm::sck::tdemodulate::Y_N3](mdm[mdm::tsk::demodulate][mdm::sck::demodulate::Y_N2]);
		mdm[mdm::tsk::tdemodulate][mdm::sck::tdemodulate::Y_N1](qnt[qnt::tsk::process   ][qnt::sck::process   ::Y_N2]);
		mdm[mdm::tsk::tdemodulate][mdm::sck::tdemodulate::Y_N2](itl[itl::tsk::interleave][itl::sck::interleave::itl ]);
	}

	// ------------------------------------------------------------------------------------------------- deinterleaving
	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
		itl[itl::tsk::deinterleave][itl::sck::deinterleave::itl](mdm[mdm::tsk::tdemodulate_wg][mdm::sck::tdemodulate_wg::Y_N3]);
	else
		itl[itl::tsk::deinterleave][itl::sck::deinterleave::itl](mdm[mdm::tsk::tdemodulate   ][mdm::sck::tdemodulate   ::Y_N3]);

	// ----------------------------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------- end of turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	if (this->params.coset)
	{
		csr[cst::tsk::apply][cst::sck::apply::in](itl[itl::tsk::deinterleave][itl::sck::deinterleave::nat]);

		if (this->params.coded_monitoring)
		{
			dch[dec::tsk::decode_siho_cw][dec::sck::decode_siho_cw::Y_N](csr[cst::tsk::apply         ][cst::sck::apply         ::out]);
			csb[cst::tsk::apply         ][cst::sck::apply         ::in ](dch[dec::tsk::decode_siho_cw][dec::sck::decode_siho_cw::V_N]);
		}
		else
		{
			dch[dec::tsk::decode_siho][dec::sck::decode_siho::Y_N ](csr[cst::tsk::apply      ][cst::sck::apply      ::out]);
			csb[cst::tsk::apply      ][cst::sck::apply      ::in  ](dch[dec::tsk::decode_siho][dec::sck::decode_siho::V_K]);
			crc[crc::tsk::extract    ][crc::sck::extract    ::V_K1](csb[cst::tsk::apply      ][cst::sck::apply      ::out]);
		}
	}
	else
	{
		if (this->params.coded_monitoring)
		{
			dch[dec::tsk::decode_siho_cw][dec::sck::decode_siho_cw::Y_N](itl[itl::tsk::deinterleave][itl::sck::deinterleave::nat]);
		}
		else
		{
			dch[dec::tsk::decode_siho][dec::sck::decode_siho::Y_N ](itl[itl::tsk::deinterleave][itl::sck::deinterleave::nat]);
			crc[crc::tsk::extract    ][crc::sck::extract    ::V_K1](dch[dec::tsk::decode_siho ][dec::sck::decode_siho ::V_K]);
		}
	}

	if (this->params.coded_monitoring)
	{
		if (this->params.coset)
			mnt[mnt::tsk::check_errors][mnt::sck::check_errors::V](csb[cst::tsk::apply         ][cst::sck::apply         ::out]);
		else
			mnt[mnt::tsk::check_errors][mnt::sck::check_errors::V](dch[dec::tsk::decode_siho_cw][dec::sck::decode_siho_cw::V_N]);
	}
	else
	{
		mnt[mnt::tsk::check_errors][mnt::sck::check_errors::V](crc[crc::tsk::extract][crc::sck::extract::V_K2]);
	}
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::simulation_loop(const int tid)
{
	auto &source          = *this->source         [tid];
	auto &crc             = *this->crc            [tid];
	auto &codec           = *this->codec          [tid];
	auto &interleaver_bit = *this->interleaver_bit[tid];
	auto &modem           = *this->modem          [tid];
	auto &channel         = *this->channel        [tid];
	auto &quantizer       = *this->quantizer      [tid];
	auto &interleaver_llr = *this->interleaver_llr[tid];
	auto &coset_real      = *this->coset_real     [tid];
	auto &coset_bit       = *this->coset_bit      [tid];
	auto &monitor         = *this->monitor        [tid];

	auto &encoder      = *codec.get_encoder();
	auto &decoder_siso = *codec.get_decoder_siso();
	auto &decoder_siho = *codec.get_decoder_siho();

	using namespace module;
	using namespace std::chrono;
	auto t_snr = steady_clock::now();

	while ((!this->monitor_red->fe_limit_achieved()) && // while max frame error count has not been reached
	        (this->params.stop_time == seconds(0) || (steady_clock::now() - t_snr) < this->params.stop_time) &&
	        (this->monitor_red->get_n_analyzed_fra() < this->max_fra || this->max_fra == 0))
	{
		if (this->params.debug)
		{
			if (!monitor[mnt::tsk::check_errors].get_n_calls())
				std::cout << "#" << std::endl;

			std::cout << "# -------------------------------" << std::endl;
			std::cout << "# New communication (n°" << monitor[mnt::tsk::check_errors].get_n_calls() << ")" << std::endl;
			std::cout << "# -------------------------------" << std::endl;
			std::cout << "#" << std::endl;
		}

		if (this->params.src->type != "AZCW")
		{
			source         [src::tsk::generate  ].exec();
			crc            [crc::tsk::build     ].exec();
			encoder        [enc::tsk::encode    ].exec();
			interleaver_bit[itl::tsk::interleave].exec();
			modem          [mdm::tsk::modulate  ].exec();
		}

		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			channel  [chn::tsk::add_noise_wg ].exec();
			modem    [mdm::tsk::filter       ].exec();
			quantizer[qnt::tsk::process      ].exec();
			modem    [mdm::tsk::demodulate_wg].exec();
		}
		else
		{
			channel  [chn::tsk::add_noise ].exec();
			modem    [mdm::tsk::filter    ].exec();
			quantizer[qnt::tsk::process   ].exec();
			modem    [mdm::tsk::demodulate].exec();
		}

		interleaver_llr[itl::tsk::deinterleave].exec();

		// ------------------------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------ turbo demodulation loop
		// ------------------------------------------------------------------------------------------------------------
		for (auto ite = 1; ite <= this->params.n_ite; ite++)
		{
			// ------------------------------------------------------------------------------------------- CRC checking
			if (this->params.crc->type != "NO" && ite >= this->params.crc_start)
			{
				codec[cdc::tsk::extract_sys_bit].exec();
				if (crc[crc::tsk::check].exec())
					break;
			}

			// ----------------------------------------------------------------------------------------------- decoding
			if (this->params.coset)
			{
				coset_real  [cst::tsk::apply      ].exec();
				decoder_siso[dec::tsk::decode_siso].exec();
				coset_real  [cst::tsk::apply      ].exec();
			}
			else
			{
				decoder_siso[dec::tsk::decode_siso].exec();
			}

			// ------------------------------------------------------------------------------------------- interleaving
			interleaver_llr[itl::tsk::interleave].exec();

			// ------------------------------------------------------------------------------------------- demodulation
			if (this->params.chn->type.find("RAYLEIGH") != std::string::npos) modem[mdm::tsk::tdemodulate_wg].exec();
			else                                                              modem[mdm::tsk::tdemodulate   ].exec();

			// ----------------------------------------------------------------------------------------- deinterleaving
			interleaver_llr[itl::tsk::deinterleave].exec();
		}

		if (this->params.coset)
		{
			coset_real[cst::tsk::apply].exec();

			if (this->params.coded_monitoring)
			{
				decoder_siho[dec::tsk::decode_siho_cw].exec();
				coset_bit   [cst::tsk::apply         ].exec();
			}
			else
			{
				decoder_siho[dec::tsk::decode_siho].exec();
				coset_bit   [cst::tsk::apply      ].exec();
				crc         [crc::tsk::extract    ].exec();
			}
		}
		else
		{
			if (this->params.coded_monitoring)
			{
				decoder_siho[dec::tsk::decode_siho_cw].exec();
			}
			else
			{
				decoder_siho[dec::tsk::decode_siho].exec();
				crc         [crc::tsk::extract    ].exec();
			}
		}

		monitor[mnt::tsk::check_errors].exec();
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::BFER_ite_threads<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER_ite_threads<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER_ite_threads<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER_ite_threads<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER_ite_threads<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
