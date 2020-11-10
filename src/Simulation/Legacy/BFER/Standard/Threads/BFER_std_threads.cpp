#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Simulation/Legacy/BFER/Standard/Threads/BFER_std_threads.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_std_threads<B,R,Q>
::BFER_std_threads(const factory::BFER_std &params_BFER_std)
: BFER_std<B,R,Q>(params_BFER_std)
{
	if (this->params_BFER_std.err_track_revert && this->params_BFER_std.n_threads != 1)
		std::clog << rang::tag::warning << "Multi-threading detected with error tracking revert feature! "
		                                   "Each thread will play the same frames. Please run with one thread."
		          << std::endl;
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::_launch()
{
	std::vector<std::thread> threads(this->params_BFER_std.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params_BFER_std.n_threads; tid++)
		threads[tid -1] = std::thread(&BFER_std_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	BFER_std_threads<B,R,Q>::start_thread(0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params_BFER_std.n_threads; tid++)
		threads[tid -1].join();

	if (!this->prev_err_messages_to_display.empty())
		throw std::runtime_error(this->prev_err_messages_to_display.back());
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::start_thread(const int tid)
{
	try
	{
		this->sockets_binding(tid);
		this->simulation_loop(tid);
		this->sockets_binding(tid, true); // unbind
	}
	catch (std::exception const& e)
	{
		tools::Terminal::stop();

		this->mutex_exception.lock();

		auto save = tools::exception::no_backtrace;
		tools::exception::no_backtrace = true;
		std::string msg = e.what(); // get only the function signature
		tools::exception::no_backtrace = save;

		if (std::find(this->prev_err_messages.begin(), this->prev_err_messages.end(), msg) ==
		    this->prev_err_messages.end())
		{
			this->prev_err_messages.push_back(msg); // save only the function signature
			this->prev_err_messages_to_display.push_back(e.what()); // with backtrace if debug mode
		}

		this->mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::sockets_binding(const int tid, const bool unbind)
{
	std::function<void(module::Socket&, module::Socket&)> bind_unbind =
		[&unbind](module::Socket& sin, module::Socket& sout)
		{
			if (unbind) sin.unbind(sout);
			else        sin.  bind(sout);
		};

	using namespace module;

	auto &src = *this->source    [tid];
	auto &crc = *this->crc       [tid];
	auto &enc =  this->codec     [tid]->get_encoder();
	auto &pct =  this->codec     [tid]->get_puncturer();
	auto &mdm = *this->modem     [tid];
	auto &chn = *this->channel   [tid];
	auto &qnt = *this->quantizer [tid];
	auto &csr = *this->coset_real[tid];
	auto &dec =  this->codec     [tid]->get_decoder_siho();
	auto &csb = *this->coset_bit [tid];
	auto &mnt = *this->monitor_er[tid];

	if (this->params_BFER_std.src->type == "AZCW")
	{
		auto src_data = (uint8_t*)(src[src::sck::generate::U_K ].get_dataptr());
		auto crc_data = (uint8_t*)(crc[crc::sck::build   ::U_K2].get_dataptr());
		auto enc_data = (uint8_t*)(enc[enc::sck::encode  ::X_N ].get_dataptr());
		auto pct_data = (uint8_t*)(pct[pct::sck::puncture::X_N2].get_dataptr());

		auto src_bytes = src[src::sck::generate::U_K ].get_databytes();
		auto crc_bytes = crc[crc::sck::build   ::U_K2].get_databytes();
		auto enc_bytes = enc[enc::sck::encode  ::X_N ].get_databytes();
		auto pct_bytes = pct[pct::sck::puncture::X_N2].get_databytes();

		std::fill(src_data, src_data + src_bytes, 0);
		std::fill(crc_data, crc_data + crc_bytes, 0);
		std::fill(enc_data, enc_data + enc_bytes, 0);
		std::fill(pct_data, pct_data + pct_bytes, 0);

		mdm[mdm::sck::modulate::X_N1](pct[pct::sck::puncture::X_N2]);
		mdm[mdm::tsk::modulate].exec();
		mdm[mdm::tsk::modulate].reset();
	}
	else
	{
		if (this->params_BFER_std.crc->type != "NO")
			bind_unbind(crc[crc::sck::build::U_K1], src[src::sck::generate::U_K ]);

		if (this->params_BFER_std.cdc->enc->type != "NO")
		{
			if (this->params_BFER_std.crc->type != "NO")
				bind_unbind(enc[enc::sck::encode::U_K], crc[crc::sck::build::U_K2]);
			else
				bind_unbind(enc[enc::sck::encode::U_K], src[src::sck::generate::U_K]);
		}

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
		{
			if (this->params_BFER_std.cdc->enc->type != "NO")
				bind_unbind(pct[pct::sck::puncture::X_N1], enc[enc::sck::encode::X_N]);
			else if (this->params_BFER_std.crc->type != "NO")
				bind_unbind(pct[pct::sck::puncture::X_N1], crc[crc::sck::build::U_K2]);
			else
				bind_unbind(pct[pct::sck::puncture::X_N1], src[src::sck::generate::U_K]);
		}

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
			bind_unbind(mdm[mdm::sck::modulate::X_N1], pct[pct::sck::puncture::X_N2]);
		else if (this->params_BFER_std.cdc->enc->type != "NO")
			bind_unbind(mdm[mdm::sck::modulate::X_N1], enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_std.crc->type != "NO")
			bind_unbind(mdm[mdm::sck::modulate::X_N1], crc[crc::sck::build::U_K2]);
		else
			bind_unbind(mdm[mdm::sck::modulate::X_N1], src[src::sck::generate::U_K]);
	}

	const auto is_rayleigh = this->params_BFER_std.chn->type.find("RAYLEIGH") != std::string::npos;
	const auto is_optical = this->params_BFER_std.chn->type == "OPTICAL" && this->params_BFER_std.mdm->rop_est_bits > 0;
	if (is_rayleigh)
	{
		if (this->params_BFER_std.chn->type == "NO")
		{
			auto chn_data = (uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr());
			auto chn_bytes = chn[chn::sck::add_noise_wg::H_N].get_databytes();
			std::fill(chn_data, chn_data + chn_bytes, 0);
		}

		if (this->params_BFER_std.chn->type != "NO")
		{
			bind_unbind(chn[chn::sck::add_noise_wg::X_N], mdm[mdm::sck::modulate::X_N2]);
		}

		if (mdm.is_filter())
		{
			if (this->params_BFER_std.chn->type != "NO")
				bind_unbind(mdm[mdm::sck::filter::Y_N1], chn[chn::sck::add_noise_wg::Y_N]);
			else
				bind_unbind(mdm[mdm::sck::filter::Y_N1], mdm[mdm::sck::modulate::X_N2]);
		}

		if (mdm.is_demodulator())
		{
			if (this->params_BFER_std.chn->type != "NO")
				bind_unbind(mdm[mdm::sck::demodulate_wg::H_N], chn[chn::sck::add_noise_wg::H_N]);
			else
				mdm[mdm::sck::demodulate_wg::H_N].bind((uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr()));

			if (mdm.is_filter())
				bind_unbind(mdm[mdm::sck::demodulate_wg::Y_N1], mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
				bind_unbind(mdm[mdm::sck::demodulate_wg::Y_N1], chn[chn::sck::add_noise_wg::Y_N]);
			else
				bind_unbind(mdm[mdm::sck::demodulate_wg::Y_N1], mdm[mdm::sck::modulate::X_N2]);
		}

		if (this->params_BFER_std.qnt->type != "NO")
		{
			if (mdm.is_demodulator())
				bind_unbind(qnt[qnt::sck::process::Y_N1], mdm[mdm::sck::demodulate_wg::Y_N2]);
			else if (mdm.is_filter())
				bind_unbind(qnt[qnt::sck::process::Y_N1], mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
				bind_unbind(qnt[qnt::sck::process::Y_N1], chn[chn::sck::add_noise_wg::Y_N]);
			else
				bind_unbind(qnt[qnt::sck::process::Y_N1], mdm[mdm::sck::modulate::X_N2]);
		}
	}
	else if (is_optical)
	{
		bind_unbind(chn[chn::sck::add_noise    ::X_N ], mdm[mdm::sck::modulate     ::X_N2]);
		bind_unbind(mdm[mdm::sck::demodulate_wg::H_N ], mdm[mdm::sck::modulate     ::X_N2]);
		bind_unbind(mdm[mdm::sck::demodulate_wg::Y_N1], chn[chn::sck::add_noise    ::Y_N ]);
		bind_unbind(qnt[qnt::sck::process      ::Y_N1], mdm[mdm::sck::demodulate_wg::Y_N2]);
	}
	else
	{
		if (this->params_BFER_std.chn->type != "NO")
			bind_unbind(chn[chn::sck::add_noise::X_N], mdm[mdm::sck::modulate::X_N2]);

		if (mdm.is_filter())
		{
			if (this->params_BFER_std.chn->type != "NO")
				bind_unbind(mdm[mdm::sck::filter::Y_N1], chn[chn::sck::add_noise::Y_N]);
			else
				bind_unbind(mdm[mdm::sck::filter::Y_N1], mdm[mdm::sck::modulate::X_N2]);
		}

		if (mdm.is_demodulator())
		{
			if (mdm.is_filter())
				bind_unbind(mdm[mdm::sck::demodulate::Y_N1], mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
				bind_unbind(mdm[mdm::sck::demodulate::Y_N1], chn[chn::sck::add_noise::Y_N]);
			else
				bind_unbind(mdm[mdm::sck::demodulate::Y_N1], mdm[mdm::sck::modulate::X_N2]);
		}

		if (this->params_BFER_std.qnt->type != "NO")
		{
			if (mdm.is_demodulator())
				bind_unbind(qnt[qnt::sck::process::Y_N1], mdm[mdm::sck::demodulate::Y_N2]);
			else if (mdm.is_filter())
				bind_unbind(qnt[qnt::sck::process::Y_N1], mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
				bind_unbind(qnt[qnt::sck::process::Y_N1], chn[chn::sck::add_noise::Y_N]);
			else
				bind_unbind(qnt[qnt::sck::process::Y_N1], mdm[mdm::sck::modulate::X_N2]);
		}
	}

	if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
	{
		if (this->params_BFER_std.qnt->type != "NO")
			bind_unbind(pct[pct::sck::depuncture::Y_N1], qnt[qnt::sck::process::Y_N2]);
		else if (mdm.is_demodulator())
		{
			if (is_rayleigh || is_optical)
				bind_unbind(pct[pct::sck::depuncture::Y_N1], mdm[mdm::sck::demodulate_wg::Y_N2]);
			else
				bind_unbind(pct[pct::sck::depuncture::Y_N1], mdm[mdm::sck::demodulate::Y_N2]);
		}
		else if (mdm.is_filter())
			bind_unbind(pct[pct::sck::depuncture::Y_N1], mdm[mdm::sck::filter::Y_N2]);
		else if (this->params_BFER_std.chn->type != "NO")
		{
			if (is_rayleigh)
				bind_unbind(pct[pct::sck::depuncture::Y_N1], chn[chn::sck::add_noise_wg::Y_N]);
			else
				bind_unbind(pct[pct::sck::depuncture::Y_N1], chn[chn::sck::add_noise::Y_N]);
		}
		else
			bind_unbind(pct[pct::sck::depuncture::Y_N1], mdm[mdm::sck::modulate::X_N2]);
	}

	if (this->params_BFER_std.coset)
	{
		if (this->params_BFER_std.cdc->enc->type != "NO")
			bind_unbind(csr[cst::sck::apply::ref], enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_std.crc->type != "NO")
			bind_unbind(csr[cst::sck::apply::ref], crc[crc::sck::build::U_K2]);
		else
			bind_unbind(csr[cst::sck::apply::ref], src[src::sck::generate::U_K]);

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
			bind_unbind(csr[cst::sck::apply::in], pct[pct::sck::depuncture::Y_N2]);
		else if (this->params_BFER_std.qnt->type != "NO")
			bind_unbind(csr[cst::sck::apply::in], qnt[qnt::sck::process::Y_N2]);
		else if (mdm.is_demodulator())
		{
			if (is_rayleigh || is_optical)
				bind_unbind(csr[cst::sck::apply::in], mdm[mdm::sck::demodulate_wg::Y_N2]);
			else
				bind_unbind(csr[cst::sck::apply::in], mdm[mdm::sck::demodulate::Y_N2]);
		}
		else if (mdm.is_filter())
			bind_unbind(csr[cst::sck::apply::in], mdm[mdm::sck::filter::Y_N2]);
		else if (this->params_BFER_std.chn->type != "NO")
		{
			if (is_rayleigh)
				bind_unbind(csr[cst::sck::apply::in], chn[chn::sck::add_noise_wg::Y_N]);
			else
				bind_unbind(csr[cst::sck::apply::in], chn[chn::sck::add_noise::Y_N]);
		}
		else
			bind_unbind(csr[cst::sck::apply::in], mdm[mdm::sck::modulate::X_N2]);

		if (this->params_BFER_std.coded_monitoring)
		{
			bind_unbind(dec[dec::sck::decode_siho_cw::Y_N], csr[cst::sck::apply::out]);

			if (this->params_BFER_std.cdc->enc->type != "NO")
				bind_unbind(csb[cst::sck::apply::ref], enc[enc::sck::encode::X_N]);
			else if (this->params_BFER_std.crc->type != "NO")
				bind_unbind(csb[cst::sck::apply::ref], crc[crc::sck::build::U_K2]);
			else
				bind_unbind(csb[cst::sck::apply::ref], src[src::sck::generate::U_K]);

			bind_unbind(csb[cst::sck::apply::in], dec[dec::sck::decode_siho_cw::V_N]);
		}
		else
		{
			bind_unbind(dec[dec::sck::decode_siho::Y_N], csr[cst::sck::apply::out]);

			if (this->params_BFER_std.crc->type != "NO")
				bind_unbind(csb[cst::sck::apply::ref], crc[crc::sck::build::U_K2]);
			else
				bind_unbind(csb[cst::sck::apply::ref], src[src::sck::generate::U_K]);

			bind_unbind(csb[cst::sck::apply::in], dec[dec::sck::decode_siho::V_K]);

			if (this->params_BFER_std.crc->type != "NO")
				bind_unbind(crc[crc::sck::extract::V_K1], csb[cst::sck::apply::out]);
		}
	}
	else
	{
		if (this->params_BFER_std.coded_monitoring)
		{
			if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
				bind_unbind(dec[dec::sck::decode_siho_cw::Y_N], pct[pct::sck::depuncture::Y_N2]);
			else if (this->params_BFER_std.qnt->type != "NO")
				bind_unbind(dec[dec::sck::decode_siho_cw::Y_N], qnt[qnt::sck::process::Y_N2]);
			else if (mdm.is_demodulator())
			{
				if (is_rayleigh || is_optical)
					bind_unbind(dec[dec::sck::decode_siho_cw::Y_N], mdm[mdm::sck::demodulate_wg::Y_N2]);
				else
					bind_unbind(dec[dec::sck::decode_siho_cw::Y_N], mdm[mdm::sck::demodulate::Y_N2]);
			}
			else if (mdm.is_filter())
				bind_unbind(dec[dec::sck::decode_siho_cw::Y_N], mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
			{
				if (is_rayleigh)
					bind_unbind(dec[dec::sck::decode_siho_cw::Y_N], chn[chn::sck::add_noise_wg::Y_N]);
				else
					bind_unbind(dec[dec::sck::decode_siho_cw::Y_N], chn[chn::sck::add_noise::Y_N]);
			}
			else
				bind_unbind(dec[dec::sck::decode_siho_cw::Y_N], mdm[mdm::sck::modulate::X_N2]);
		}
		else
		{
			if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
				bind_unbind(dec[dec::sck::decode_siho::Y_N], pct[pct::sck::depuncture::Y_N2]);
			else if (this->params_BFER_std.qnt->type != "NO")
				bind_unbind(dec[dec::sck::decode_siho::Y_N], qnt[qnt::sck::process::Y_N2]);
			else if (mdm.is_demodulator())
			{
				if (is_rayleigh || is_optical)
					bind_unbind(dec[dec::sck::decode_siho::Y_N], mdm[mdm::sck::demodulate_wg::Y_N2]);
				else
					bind_unbind(dec[dec::sck::decode_siho::Y_N], mdm[mdm::sck::demodulate::Y_N2]);
			}
			else if (mdm.is_filter())
				bind_unbind(dec[dec::sck::decode_siho::Y_N], mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
			{
				if (is_rayleigh)
					bind_unbind(dec[dec::sck::decode_siho::Y_N], chn[chn::sck::add_noise_wg::Y_N]);
				else
					bind_unbind(dec[dec::sck::decode_siho::Y_N], chn[chn::sck::add_noise::Y_N]);
			}
			else
				bind_unbind(dec[dec::sck::decode_siho::Y_N], mdm[mdm::sck::modulate::X_N2]);

			if (this->params_BFER_std.crc->type != "NO")
				bind_unbind(crc[crc::sck::extract::V_K1], dec[dec::sck::decode_siho::V_K]);
		}
	}

	if (this->params_BFER_std.coded_monitoring)
	{
		if (this->params_BFER_std.cdc->enc->type != "NO")
			bind_unbind(mnt[mnt::sck::check_errors::U], enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_std.crc->type != "NO")
			bind_unbind(mnt[mnt::sck::check_errors::U], crc[crc::sck::build::U_K2]);
		else
			bind_unbind(mnt[mnt::sck::check_errors::U], src[src::sck::generate::U_K]);

		if (this->params_BFER_std.coset)
			bind_unbind(mnt[mnt::sck::check_errors::V], csb[cst::sck::apply::out]);
		else
			bind_unbind(mnt[mnt::sck::check_errors::V], dec[dec::sck::decode_siho_cw::V_N]);
	}
	else
	{
		bind_unbind(mnt[mnt::sck::check_errors::U], src[src::sck::generate::U_K]);
		if (this->params_BFER_std.crc->type != "NO")
			bind_unbind(mnt[mnt::sck::check_errors::V], crc[crc::sck::extract::V_K2]);
		else if (this->params_BFER_std.coset)
			bind_unbind(mnt[mnt::sck::check_errors::V], csb[cst::sck::apply::out]);
		else
			bind_unbind(mnt[mnt::sck::check_errors::V], dec[dec::sck::decode_siho::V_K]);
	}

	if (this->params_BFER_std.mnt_mutinfo)
	{
		auto &mnt = *this->monitor_mi[tid];

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
			bind_unbind(mnt[mnt::sck::get_mutual_info::X], pct[pct::sck::puncture::X_N2]);
		else if (this->params_BFER_std.cdc->enc->type != "NO")
			bind_unbind(mnt[mnt::sck::get_mutual_info::X], enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_std.crc->type != "NO")
			bind_unbind(mnt[mnt::sck::get_mutual_info::X], crc[crc::sck::build::U_K2]);
		else
			bind_unbind(mnt[mnt::sck::get_mutual_info::X], src[src::sck::generate::U_K]);

		if (mdm.is_demodulator())
		{
			if (is_rayleigh || is_optical)
				bind_unbind(mnt[mnt::sck::get_mutual_info::Y], mdm[mdm::sck::demodulate_wg::Y_N2]);
			else
				bind_unbind(mnt[mnt::sck::get_mutual_info::Y], mdm[mdm::sck::demodulate::Y_N2]);
		}
		else if (mdm.is_filter())
			bind_unbind(mnt[mnt::sck::get_mutual_info::Y], mdm[mdm::sck::filter::Y_N2]);
		else if (this->params_BFER_std.chn->type != "NO")
			bind_unbind(mnt[mnt::sck::get_mutual_info::Y], chn[chn::sck::add_noise_wg::Y_N]);
		else
			bind_unbind(mnt[mnt::sck::get_mutual_info::Y], mdm[mdm::sck::modulate::X_N2]);
	}
}

template <typename B, typename R, typename Q>
void BFER_std_threads<B,R,Q>
::simulation_loop(const int tid)
{
	auto &source     = *this->source    [tid];
	auto &crc        = *this->crc       [tid];
	auto &encoder    =  this->codec     [tid]->get_encoder();
	auto &puncturer  =  this->codec     [tid]->get_puncturer();
	auto &modem      = *this->modem     [tid];
	auto &channel    = *this->channel   [tid];
	auto &quantizer  = *this->quantizer [tid];
	auto &coset_real = *this->coset_real[tid];
	auto &decoder    =  this->codec     [tid]->get_decoder_siho();
	auto &coset_bit  = *this->coset_bit [tid];
	auto &monitor    = *this->monitor_er[tid];

	using namespace module;

	// communication chain execution
	while (this->keep_looping_noise_point())
	{
		if (this->params_BFER_std.debug)
		{
			if (!monitor[mnt::tsk::check_errors].get_n_calls())
				std::cout << "#" << std::endl;

			auto fid = monitor[mnt::tsk::check_errors].get_n_calls();
			std::cout << "# -------------------------------"     << std::endl;
			std::cout << "# New communication (n°" << fid << ")" << std::endl;
			std::cout << "# -------------------------------"     << std::endl;
			std::cout << "#"                                     << std::endl;
		}

		if (this->params_BFER_std.src->type != "AZCW")
		{
			source[src::tsk::generate].exec();
			if (this->params_BFER_std.crc->type != "NO")
				crc[crc::tsk::build].exec();
			if (this->params_BFER_std.cdc->enc->type != "NO")
				encoder[enc::tsk::encode].exec();
			if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
				puncturer[pct::tsk::puncture].exec();
			modem[mdm::tsk::modulate].exec();
		}

		if (this->params_BFER_std.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			if (this->params_BFER_std.chn->type != "NO")
				channel[chn::tsk::add_noise_wg].exec();
			if (modem.is_filter())
				modem[mdm::tsk::filter].exec();
			if (modem.is_demodulator())
				modem[mdm::tsk::demodulate_wg].exec();
			if (this->params_BFER_std.qnt->type != "NO")
				quantizer[qnt::tsk::process].exec();
		}
		else if (this->params_BFER_std.chn->type == "OPTICAL" && this->params_BFER_std.mdm->rop_est_bits > 0)
		{
			channel[chn::tsk::add_noise].exec();
			modem[mdm::tsk::demodulate_wg].exec();
			if (this->params_BFER_std.qnt->type != "NO")
				quantizer[qnt::tsk::process].exec();
		}
		else
		{
			if (this->params_BFER_std.chn->type != "NO")
				channel[chn::tsk::add_noise].exec();
			if (modem.is_filter())
				modem[mdm::tsk::filter].exec();
			if (modem.is_demodulator())
				modem[mdm::tsk::demodulate].exec();
			if (this->params_BFER_std.qnt->type != "NO")
				quantizer[qnt::tsk::process].exec();
		}

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
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
				if (this->params_BFER_std.crc->type != "NO")
					crc[crc::tsk::extract].exec();
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
				if (this->params_BFER_std.crc->type != "NO")
					crc[crc::tsk::extract].exec();
			}
		}

		monitor[mnt::tsk::check_errors].exec();

		if (this->params_BFER_std.mnt_mutinfo)
		{
			auto &monitor = *this->monitor_mi[tid];

			monitor[mnt::tsk::get_mutual_info].exec();
		}
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::simulation::BFER_std_threads<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER_std_threads<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER_std_threads<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER_std_threads<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER_std_threads<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
