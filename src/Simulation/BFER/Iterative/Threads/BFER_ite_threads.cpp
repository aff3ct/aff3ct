#include <cstring>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Display/rang_format/rang_format.h"

#include "BFER_ite_threads.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_ite_threads<B,R,Q>
::BFER_ite_threads(const factory::BFER_ite::parameters &params_BFER_ite)
: BFER_ite<B,R,Q>(params_BFER_ite)
{
	if (this->params_BFER_ite.err_track_revert)
	{
		if (this->params_BFER_ite.n_threads != 1)
			std::clog << rang::tag::warning << "Multi-threading detected with error tracking revert feature! "
			                                      "Each thread will play the same frames. Please run with one thread."
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

	std::vector<std::thread> threads(this->params_BFER_ite.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params_BFER_ite.n_threads; tid++)
		threads[tid -1] = std::thread(BFER_ite_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	BFER_ite_threads<B,R,Q>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params_BFER_ite.n_threads; tid++)
		threads[tid -1].join();

	this->monitor_red->reduce(true);

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
		tools::Terminal::stop();

		simu->mutex_exception.lock();

		auto save = tools::exception::no_backtrace;
		tools::exception::no_backtrace = true;
		std::string msg = e.what(); // get only the function signature
		tools::exception::no_backtrace = save;

		if (std::find(simu->prev_err_messages.begin(), simu->prev_err_messages.end(), msg) == simu->prev_err_messages.end())
		{
			simu->prev_err_messages.push_back(msg); // save only the function signature
			simu->prev_err_messages_to_display.push_back(e.what()); // with backtrace if debug mode
		}

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

	if (this->params_BFER_ite.src->type == "AZCW")
	{
		auto src_data = (uint8_t*)(src[src::sck::generate  ::U_K ].get_dataptr());
		auto crc_data = (uint8_t*)(crc[crc::sck::build     ::U_K2].get_dataptr());
		auto enc_data = (uint8_t*)(enc[enc::sck::encode    ::X_N ].get_dataptr());
		auto itl_data = (uint8_t*)(itb[itl::sck::interleave::itl ].get_dataptr());

		auto src_bytes = src[src::sck::generate  ::U_K ].get_databytes();
		auto crc_bytes = crc[crc::sck::build     ::U_K2].get_databytes();
		auto enc_bytes = enc[enc::sck::encode    ::X_N ].get_databytes();
		auto itl_bytes = itb[itl::sck::interleave::itl ].get_databytes();

		std::fill(src_data, src_data + src_bytes, 0);
		std::fill(crc_data, crc_data + crc_bytes, 0);
		std::fill(enc_data, enc_data + enc_bytes, 0);
		std::fill(itl_data, itl_data + itl_bytes, 0);

		mdm[mdm::sck::modulate::X_N1](itb[itl::sck::interleave::itl]);
		mdm[mdm::tsk::modulate].exec();
		mdm[mdm::tsk::modulate].reset_stats();
	}
	else
	{
		if (this->params_BFER_ite.crc->type == "NO")
			crc[crc::sck::build::U_K2](src[src::sck::generate::U_K]);
		if (this->params_BFER_ite.cdc->enc->type == "NO")
			enc[enc::sck::encode::X_N](crc[crc::sck::build::U_K2]);

		crc[crc::sck::build     ::U_K1](src[src::sck::generate  ::U_K ]);
		enc[enc::sck::encode    ::U_K ](crc[crc::sck::build     ::U_K2]);
		itb[itl::sck::interleave::nat ](enc[enc::sck::encode    ::X_N ]);
		mdm[mdm::sck::modulate  ::X_N1](itb[itl::sck::interleave::itl ]);
	}

	if (this->params_BFER_ite.coset)
	{
		if (this->params_BFER_ite.coded_monitoring)
			csb[cst::sck::apply::ref](enc[enc::sck::encode::X_N ]);
		else
			csb[cst::sck::apply::ref](crc[crc::sck::build ::U_K2]);

		csr[cst::sck::apply::ref](enc[enc::sck::encode::X_N]);
	}

	if (this->params_BFER_ite.coded_monitoring)
		mnt[mnt::sck::check_errors::U](enc[enc::sck::encode  ::X_N]);
	else
		mnt[mnt::sck::check_errors::U](src[src::sck::generate::U_K]);

	if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		if (this->params_BFER_ite.chn->type == "NO")
		{
			chn[chn::sck::add_noise_wg::Y_N](mdm[mdm::sck::modulate::X_N2]);
			auto chn_data = (uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr());
			auto chn_bytes = chn[chn::sck::add_noise_wg::H_N].get_databytes();
			std::fill(chn_data, chn_data + chn_bytes, 0);
		}
		if (!mdm.is_filter())
			mdm[mdm::sck::filter::Y_N2](chn[chn::sck::add_noise_wg::Y_N]);
		if (this->params_BFER_ite.qnt->type == "NO")
			qnt[qnt::sck::process::Y_N2](mdm[mdm::sck::filter::Y_N2]);
		if (!mdm.is_demodulator())
			mdm[mdm::sck::demodulate_wg::Y_N2](qnt[qnt::sck::process::Y_N2]);

		chn[chn::sck::add_noise_wg ::X_N ](mdm[mdm::sck::modulate    ::X_N2]);
		mdm[mdm::sck::demodulate_wg::H_N ](chn[chn::sck::add_noise_wg::H_N ]);
		mdm[mdm::sck::filter       ::Y_N1](chn[chn::sck::add_noise_wg::Y_N ]);
		qnt[qnt::sck::process      ::Y_N1](mdm[mdm::sck::filter      ::Y_N2]);
		mdm[mdm::sck::demodulate_wg::Y_N1](qnt[qnt::sck::process     ::Y_N2]);
	}
	else
	{
		if (this->params_BFER_ite.chn->type == "NO")
			chn[chn::sck::add_noise::Y_N](mdm[mdm::sck::modulate::X_N2]);
		if (!mdm.is_filter())
			mdm[mdm::sck::filter::Y_N2](chn[chn::sck::add_noise::Y_N]);
		if (this->params_BFER_ite.qnt->type == "NO")
			qnt[qnt::sck::process::Y_N2](mdm[mdm::sck::filter::Y_N2]);
		if (!mdm.is_demodulator())
			mdm[mdm::sck::demodulate::Y_N2](qnt[qnt::sck::process::Y_N2]);

		chn[chn::sck::add_noise ::X_N ](mdm[mdm::sck::modulate ::X_N2]);
		mdm[mdm::sck::filter    ::Y_N1](chn[chn::sck::add_noise::Y_N ]);
		qnt[qnt::sck::process   ::Y_N1](mdm[mdm::sck::filter   ::Y_N2]);
		mdm[mdm::sck::demodulate::Y_N1](qnt[qnt::sck::process  ::Y_N2]);
	}

	itl[itl::sck::deinterleave::itl](mdm[mdm::sck::demodulate::Y_N2]);

	// ----------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------- turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	// --------------------------------------------------------------------------------------------------- CRC checking
	if (this->params_BFER_ite.crc->type != "NO")
	{
		cdc[cdc::sck::extract_sys_bit::Y_N](itl[itl::sck::deinterleave   ::nat]);
		crc[crc::sck::check          ::V_K](cdc[cdc::sck::extract_sys_bit::V_K]);
	}

	// ------------------------------------------------------------------------------------------------------- decoding
	if (this->params_BFER_ite.coset)
	{
		// output socket binding (trick to avoid runtime re-binding)
		dcs[dec::sck::decode_siso::Y_N2](itl[itl::sck::deinterleave::nat ]);
		csr[cst::sck::apply      ::in  ](itl[itl::sck::deinterleave::nat ]);
		dcs[dec::sck::decode_siso::Y_N1](csr[cst::sck::apply       ::out ]);
		csr[cst::sck::apply      ::in  ](dcs[dec::sck::decode_siso ::Y_N2]);
	}
	else
	{
		dcs[dec::sck::decode_siso::Y_N1](itl[itl::sck::deinterleave::nat]);
	}

	// --------------------------------------------------------------------------------------------------- interleaving
	if (this->params_BFER_ite.coset)
		itl[itl::sck::interleave::nat](csr[cst::sck::apply      ::out ]);
	else
		itl[itl::sck::interleave::nat](dcs[dec::sck::decode_siso::Y_N2]);

	// --------------------------------------------------------------------------------------------------- demodulation
	if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		if (!mdm.is_demodulator())
			mdm[mdm::sck::tdemodulate_wg::Y_N3](qnt[qnt::sck::process::Y_N2]);
		else // output socket binding (trick to avoid runtime re-binding)
			mdm[mdm::sck::tdemodulate_wg::Y_N3](mdm[mdm::sck::demodulate::Y_N2]);

		mdm[mdm::sck::tdemodulate_wg::Y_N1](qnt[qnt::sck::process     ::Y_N2]);
		mdm[mdm::sck::tdemodulate_wg::H_N ](chn[chn::sck::add_noise_wg::H_N ]);
		mdm[mdm::sck::tdemodulate_wg::Y_N2](itl[itl::sck::interleave  ::itl ]);
	}
	else
	{
		if (!mdm.is_demodulator())
			mdm[mdm::sck::tdemodulate::Y_N3](qnt[qnt::sck::process::Y_N2]);
		else // output socket binding (trick to avoid runtime re-binding)
			mdm[mdm::sck::tdemodulate::Y_N3](mdm[mdm::sck::demodulate::Y_N2]);

		mdm[mdm::sck::tdemodulate::Y_N1](qnt[qnt::sck::process   ::Y_N2]);
		mdm[mdm::sck::tdemodulate::Y_N2](itl[itl::sck::interleave::itl ]);
	}

	// ------------------------------------------------------------------------------------------------- deinterleaving
	if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos)
		itl[itl::sck::deinterleave::itl](mdm[mdm::sck::tdemodulate_wg::Y_N3]);
	else
		itl[itl::sck::deinterleave::itl](mdm[mdm::sck::tdemodulate   ::Y_N3]);

	// ----------------------------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------- end of turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	if (this->params_BFER_ite.coset)
	{
		csr[cst::sck::apply::in](itl[itl::sck::deinterleave::nat]);

		if (this->params_BFER_ite.coded_monitoring)
		{
			dch[dec::sck::decode_siho_cw::Y_N](csr[cst::sck::apply         ::out]);
			csb[cst::sck::apply         ::in ](dch[dec::sck::decode_siho_cw::V_N]);
		}
		else
		{
			if (this->params_BFER_ite.crc->type == "NO")
				crc[crc::sck::extract::V_K2](csb[cst::sck::apply::out]);

			dch[dec::sck::decode_siho::Y_N ](csr[cst::sck::apply      ::out]);
			csb[cst::sck::apply      ::in  ](dch[dec::sck::decode_siho::V_K]);
			crc[crc::sck::extract    ::V_K1](csb[cst::sck::apply      ::out]);
		}
	}
	else
	{
		if (this->params_BFER_ite.coded_monitoring)
		{
			dch[dec::sck::decode_siho_cw::Y_N](itl[itl::sck::deinterleave::nat]);
		}
		else
		{
			if (this->params_BFER_ite.crc->type == "NO")
				crc[crc::sck::extract::V_K2](dch[dec::sck::decode_siho::V_K]);

			dch[dec::sck::decode_siho::Y_N ](itl[itl::sck::deinterleave::nat]);
			crc[crc::sck::extract    ::V_K1](dch[dec::sck::decode_siho ::V_K]);
		}
	}

	if (this->params_BFER_ite.coded_monitoring)
	{
		if (this->params_BFER_ite.coset)
			mnt[mnt::sck::check_errors::V](csb[cst::sck::apply         ::out]);
		else
			mnt[mnt::sck::check_errors::V](dch[dec::sck::decode_siho_cw::V_N]);
	}
	else
	{
		mnt[mnt::sck::check_errors::V](crc[crc::sck::extract::V_K2]);
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

	while (!tools::Terminal::is_interrupt()
	       && !this->monitor_red->fe_limit_achieved() // while max frame error count has not been reached
	       && (this->params_BFER_ite.stop_time == seconds(0)
	           || (steady_clock::now() - t_snr) < this->params_BFER_ite.stop_time)
	       && (this->params_BFER_ite.max_frame == 0
	       	   || this->monitor_red->get_n_analyzed_fra() < this->params_BFER_ite.max_frame))
	{
		if (this->params_BFER_ite.debug)
		{
			if (!monitor[mnt::tsk::check_errors].get_n_calls())
				std::cout << "#" << std::endl;

			std::cout << "# -------------------------------" << std::endl;
			std::cout << "# New communication (n°" << monitor[mnt::tsk::check_errors].get_n_calls() << ")" << std::endl;
			std::cout << "# -------------------------------" << std::endl;
			std::cout << "#" << std::endl;
		}

		if (this->params_BFER_ite.src->type != "AZCW")
		{
			source[src::tsk::generate].exec();
			if (this->params_BFER_ite.crc->type != "NO")
				crc[crc::tsk::build].exec();
			if (this->params_BFER_ite.cdc->enc->type != "NO")
				encoder[enc::tsk::encode].exec();

			interleaver_bit[itl::tsk::interleave].exec();
			modem          [mdm::tsk::modulate  ].exec();
		}

		if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			if (this->params_BFER_ite.chn->type != "NO")
				channel[chn::tsk::add_noise_wg].exec();
			if (modem.is_filter())
				modem [mdm::tsk::filter].exec();
			if (this->params_BFER_ite.qnt->type != "NO")
				quantizer[qnt::tsk::process].exec();
			if (modem.is_demodulator())
				modem[mdm::tsk::demodulate_wg].exec();
		}
		else
		{
			if (this->params_BFER_ite.chn->type != "NO")
				channel[chn::tsk::add_noise].exec();
			if (modem.is_filter())
				modem[mdm::tsk::filter].exec();
			if (this->params_BFER_ite.qnt->type != "NO")
				quantizer[qnt::tsk::process].exec();
			if (modem.is_demodulator())
				modem[mdm::tsk::demodulate].exec();
		}

		interleaver_llr[itl::tsk::deinterleave].exec();

		// ------------------------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------ turbo demodulation loop
		// ------------------------------------------------------------------------------------------------------------
		for (auto ite = 1; ite <= this->params_BFER_ite.n_ite; ite++)
		{
			// ------------------------------------------------------------------------------------------- CRC checking
			if (this->params_BFER_ite.crc->type != "NO" && ite >= this->params_BFER_ite.crc_start)
			{
				codec[cdc::tsk::extract_sys_bit].exec();
				if (crc[crc::tsk::check].exec())
					break;
			}

			// ----------------------------------------------------------------------------------------------- decoding
			if (this->params_BFER_ite.coset)
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
			if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos)
			{
				if (modem.is_demodulator())
					modem[mdm::tsk::tdemodulate_wg].exec();
			}
			else
			{
				if (modem.is_demodulator())
					modem[mdm::tsk::tdemodulate].exec();
			}

			// ----------------------------------------------------------------------------------------- deinterleaving
			interleaver_llr[itl::tsk::deinterleave].exec();
		}

		if (this->params_BFER_ite.coset)
		{
			coset_real[cst::tsk::apply].exec();

			if (this->params_BFER_ite.coded_monitoring)
			{
				decoder_siho[dec::tsk::decode_siho_cw].exec();
				coset_bit   [cst::tsk::apply         ].exec();
			}
			else
			{
				decoder_siho[dec::tsk::decode_siho].exec();
				coset_bit   [cst::tsk::apply      ].exec();

				if (this->params_BFER_ite.crc->type != "NO")
					crc[crc::tsk::extract].exec();
			}
		}
		else
		{
			if (this->params_BFER_ite.coded_monitoring)
			{
				decoder_siho[dec::tsk::decode_siho_cw].exec();
			}
			else
			{
				decoder_siho[dec::tsk::decode_siho].exec();

				if (this->params_BFER_ite.crc->type != "NO")
					crc[crc::tsk::extract].exec();
			}
		}

		monitor[mnt::tsk::check_errors].exec();

		if (tid == 0)
			this->monitor_red->reduce(false);
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
