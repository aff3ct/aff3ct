#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Simulation/BFER/Iterative/Threads/BFER_ite_threads.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_ite_threads<B,R,Q>
::BFER_ite_threads(const factory::BFER_ite &params_BFER_ite)
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
void BFER_ite_threads<B,R,Q>
::_launch()
{
	std::vector<std::thread> threads(this->params_BFER_ite.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params_BFER_ite.n_threads; tid++)
		threads[tid -1] = std::thread(&BFER_ite_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	BFER_ite_threads<B,R,Q>::start_thread(0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params_BFER_ite.n_threads; tid++)
		threads[tid -1].join();

	if (!this->prev_err_messages.empty())
		throw std::runtime_error(this->prev_err_messages.back());
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::start_thread(const int tid)
{
	try
	{
		this->sockets_binding(tid);
		this->simulation_loop(tid);
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
void BFER_ite_threads<B,R,Q>
::sockets_binding(const int tid)
{
	auto &src  = *this->source          [tid];
	auto &crc  = *this->crc             [tid];
	auto &cdc1 = *this->codec1          [tid];
	auto &cdc2 = *this->codec2          [tid];
	auto &itb  = *this->interleaver_bit [tid];
	auto &mdm1 = *this->modem1          [tid];
	auto &mdm2 = *this->modem2          [tid];
	auto &chn  = *this->channel         [tid];
	auto &qnt  = *this->quantizer       [tid];
	auto &itl1 = *this->interleaver_llr1[tid];
	auto &itl2 = *this->interleaver_llr2[tid];
	auto &csr1 = *this->coset_real1     [tid];
	auto &csr2 = *this->coset_real2     [tid];
	auto &csr3 = *this->coset_real3     [tid];
	auto &csb  = *this->coset_bit       [tid];
	auto &mnt  = *this->monitor_er      [tid];
	auto &lite = *this->loop_ite        [tid];
	auto &lcrc = *this->loop_crc        [tid];

	auto &enc  = cdc1.get_encoder();
	auto &dcs  = cdc1.get_decoder_siso();
	auto &dch  = cdc1.get_decoder_siho();
	auto &ext1 = cdc1.get_extractor();
	auto &ext2 = cdc2.get_extractor();

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

		mdm1[mdm::sck::modulate::X_N1](itb[itl::sck::interleave::itl]);
		mdm1[mdm::tsk::modulate].exec();
		mdm1[mdm::tsk::modulate].reset();
	}
	else
	{
		if (this->params_BFER_ite.crc->type != "NO")
			crc[crc::sck::build::U_K1](src[src::sck::generate::U_K]);

		if (this->params_BFER_ite.cdc->enc->type != "NO")
		{
			if (this->params_BFER_ite.crc->type != "NO")
				enc[enc::sck::encode::U_K](crc[crc::sck::build::U_K2]);
			else
				enc[enc::sck::encode::U_K](src[src::sck::generate::U_K]);
		}

		if (this->params_BFER_ite.cdc->enc->type != "NO")
			itb[itl::sck::interleave::nat](enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_ite.crc->type != "NO")
			itb[itl::sck::interleave::nat](crc[crc::sck::build::U_K2]);
		else
			itb[itl::sck::interleave::nat](src[src::sck::generate::U_K]);

		mdm1[mdm::sck::modulate::X_N1](itb[itl::sck::interleave::itl]);
	}

	if (this->params_BFER_ite.coset)
	{
		if (this->params_BFER_ite.coded_monitoring)
		{
			if (this->params_BFER_ite.cdc->enc->type != "NO")
				csb[cst::sck::apply::ref](enc[enc::sck::encode::X_N]);
			else if (this->params_BFER_ite.crc->type != "NO")
				csb[cst::sck::apply::ref](crc[crc::sck::build::U_K2]);
			else
				csb[cst::sck::apply::ref](src[src::sck::generate::U_K]);
		}
		else
		{
			if (this->params_BFER_ite.crc->type != "NO")
				csb[cst::sck::apply::ref](crc[crc::sck::build::U_K2]);
			else
				csb[cst::sck::apply::ref](src[src::sck::generate::U_K]);
		}

		if (this->params_BFER_ite.cdc->enc->type != "NO")
		{
			csr1[cst::sck::apply::ref](enc[enc::sck::encode::X_N]);
			csr2[cst::sck::apply::ref](enc[enc::sck::encode::X_N]);
			csr3[cst::sck::apply::ref](enc[enc::sck::encode::X_N]);
		}
		else if (this->params_BFER_ite.crc->type != "NO")
		{
			csr1[cst::sck::apply::ref](crc[crc::sck::build::U_K2]);
			csr2[cst::sck::apply::ref](crc[crc::sck::build::U_K2]);
			csr3[cst::sck::apply::ref](crc[crc::sck::build::U_K2]);
		}
		else
		{
			csr1[cst::sck::apply::ref](src[src::sck::generate::U_K]);
			csr2[cst::sck::apply::ref](src[src::sck::generate::U_K]);
			csr3[cst::sck::apply::ref](src[src::sck::generate::U_K]);
		}
	}

	if (this->params_BFER_ite.coded_monitoring)
	{
		if (this->params_BFER_ite.cdc->enc->type != "NO")
			mnt[mnt::sck::check_errors::U](enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_ite.crc->type != "NO")
			mnt[mnt::sck::check_errors::U](crc[crc::sck::build::U_K2]);
		else
			mnt[mnt::sck::check_errors::U](src[src::sck::generate::U_K]);
	}
	else
		mnt[mnt::sck::check_errors::U](src[src::sck::generate::U_K]);

	const auto is_rayleigh = this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos;
	if (is_rayleigh && this->params_BFER_ite.chn->type == "NO")
	{
		auto chn_data = (uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr());
		auto chn_bytes = chn[chn::sck::add_noise_wg::H_N].get_databytes();
		std::fill(chn_data, chn_data + chn_bytes, 0);
	}

	if (this->params_BFER_ite.chn->type != "NO")
	{
		if (is_rayleigh)
			chn[chn::sck::add_noise_wg::X_N](mdm1[mdm::sck::modulate::X_N2]);
		else
			chn[chn::sck::add_noise::X_N](mdm1[mdm::sck::modulate::X_N2]);
	}

	if (mdm1.is_filter())
	{
		if (this->params_BFER_ite.chn->type != "NO")
		{
			if (is_rayleigh)
				mdm1[mdm::sck::filter::Y_N1](chn[chn::sck::add_noise_wg::Y_N]);
			else
				mdm1[mdm::sck::filter::Y_N1](chn[chn::sck::add_noise::Y_N]);
		}
		else
			mdm1[mdm::sck::filter::Y_N1](mdm1[mdm::sck::modulate::X_N2]);
	}

	if (this->params_BFER_ite.qnt->type != "NO")
	{
		if (mdm1.is_filter())
			qnt[qnt::sck::process::Y_N1](mdm1[mdm::sck::filter::Y_N2]);
		else if (this->params_BFER_ite.chn->type != "NO")
		{
			if (is_rayleigh)
				qnt[qnt::sck::process::Y_N1](chn[chn::sck::add_noise_wg::Y_N]);
			else
				qnt[qnt::sck::process::Y_N1](chn[chn::sck::add_noise::Y_N]);
		}
		else
			qnt[qnt::sck::process::Y_N1](mdm1[mdm::sck::modulate::X_N2]);
	}

	if (mdm1.is_demodulator())
	{
		if (is_rayleigh)
		{
			if (this->params_BFER_ite.chn->type != "NO")
				mdm1[mdm::sck::demodulate_wg::H_N](chn[chn::sck::add_noise_wg::H_N]);
			else
				mdm1[mdm::sck::demodulate_wg::H_N]((uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr()));
		}

		if (this->params_BFER_ite.qnt->type != "NO")
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1](qnt[qnt::sck::process::Y_N2]);
			else
				mdm1[mdm::sck::demodulate::Y_N1](qnt[qnt::sck::process::Y_N2]);
		}
		else if (mdm1.is_filter())
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1](mdm1[mdm::sck::filter::Y_N2]);
			else
				mdm1[mdm::sck::demodulate::Y_N1](mdm1[mdm::sck::filter::Y_N2]);
		}
		else if (this->params_BFER_ite.chn->type != "NO")
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1](chn[chn::sck::add_noise_wg::Y_N]);
			else
				mdm1[mdm::sck::demodulate::Y_N1](chn[chn::sck::add_noise::Y_N]);
		}
		else
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1](mdm1[mdm::sck::modulate::X_N2]);
			else
				mdm1[mdm::sck::demodulate::Y_N1](mdm1[mdm::sck::modulate::X_N2]);
		}
	}

	if (mdm1.is_demodulator())
	{
		if (is_rayleigh)
			itl1[itl::sck::deinterleave::itl](mdm1[mdm::sck::demodulate_wg::Y_N2]);
		else
			itl1[itl::sck::deinterleave::itl](mdm1[mdm::sck::demodulate::Y_N2]);
	}
	else if (this->params_BFER_ite.qnt->type != "NO")
		itl1[itl::sck::deinterleave::itl](qnt[qnt::sck::process::Y_N2]);
	else if (mdm1.is_filter())
		itl1[itl::sck::deinterleave::itl](mdm1[mdm::sck::filter::Y_N2]);
	else if (this->params_BFER_ite.chn->type != "NO")
	{
		if (is_rayleigh)
			itl1[itl::sck::deinterleave::itl](chn[chn::sck::add_noise_wg::Y_N]);
		else
			itl1[itl::sck::deinterleave::itl](chn[chn::sck::add_noise::Y_N]);
	}
	else
		itl1[itl::sck::deinterleave::itl](mdm1[mdm::sck::modulate::X_N2]);

	// ----------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------- turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	if (this->params_BFER_ite.crc->type != "NO")
	{
		ext1[ext::sck::get_sys_bit::Y_N    ](itl1[itl::sck::deinterleave::nat]);
		lcrc[lop::sck::stop       ::in1    ](ext1[ext::sck::get_sys_bit ::V_K]);
		lcrc[lop::sck::stop       ::in_out1](itl1[itl::sck::deinterleave::nat]);
	}
	else
		lite[lop::sck::stop::in_out1](itl1[itl::sck::deinterleave::nat]);

	// ------------------------------------------------------------------------------------------------------- decoding
	if (this->params_BFER_ite.coset)
	{
		if (this->params_BFER_ite.crc->type != "NO")
			csr1[cst::sck::apply::in](lcrc[lop::sck::stop::in_out1]);
		else
			csr1[cst::sck::apply::in](lite[lop::sck::stop::in_out1]);

		dcs [dec::sck::decode_siso::Y_N1](csr1[cst::sck::apply      ::out ]);
		csr2[cst::sck::apply      ::in  ](dcs [dec::sck::decode_siso::Y_N2]);
	}
	else
	{
		if (this->params_BFER_ite.crc->type != "NO")
			dcs[dec::sck::decode_siso::Y_N1](lcrc[lop::sck::stop::in_out1]);
		else
			dcs[dec::sck::decode_siso::Y_N1](lite[lop::sck::stop::in_out1]);
	}

	// --------------------------------------------------------------------------------------------------- interleaving
	if (this->params_BFER_ite.coset)
		itl2[itl::sck::interleave::nat](csr2[cst::sck::apply::out]);
	else
		itl2[itl::sck::interleave::nat](dcs[dec::sck::decode_siso::Y_N2]);

	// --------------------------------------------------------------------------------------------------- demodulation
	if (mdm2.is_demodulator())
	{
		if (is_rayleigh)
		{
			if (this->params_BFER_ite.chn->type != "NO")
				mdm2[mdm::sck::tdemodulate_wg::H_N](chn[chn::sck::add_noise_wg::H_N]);
			else
				mdm2[mdm::sck::tdemodulate_wg::H_N]((uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr()));

			if (this->params_BFER_ite.qnt->type != "NO")
				mdm2[mdm::sck::tdemodulate_wg::Y_N1](qnt[qnt::sck::process::Y_N2]);
			else if (mdm1.is_filter())
				mdm2[mdm::sck::tdemodulate_wg::Y_N1](mdm1[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_ite.chn->type != "NO")
				mdm2[mdm::sck::tdemodulate_wg::Y_N1](chn[chn::sck::add_noise_wg::Y_N]);
			else
				mdm2[mdm::sck::tdemodulate_wg::Y_N1](mdm1[mdm::sck::modulate::X_N2]);

			mdm2[mdm::sck::tdemodulate_wg::Y_N2](itl2[itl::sck::interleave::itl]);
		}
		else
		{
			if (this->params_BFER_ite.qnt->type != "NO")
				mdm2[mdm::sck::tdemodulate::Y_N1](qnt[qnt::sck::process::Y_N2]);
			else if (mdm1.is_filter())
				mdm2[mdm::sck::tdemodulate::Y_N1](mdm1[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_ite.chn->type != "NO")
				mdm2[mdm::sck::tdemodulate::Y_N1](chn[chn::sck::add_noise::Y_N]);
			else
				mdm2[mdm::sck::tdemodulate::Y_N1](mdm1[mdm::sck::modulate::X_N2]);

			mdm2[mdm::sck::tdemodulate::Y_N2](itl2[itl::sck::interleave::itl ]);
		}
	}

	// ------------------------------------------------------------------------------------------------- deinterleaving
	if (mdm2.is_demodulator())
	{
		if (is_rayleigh)
			itl2[itl::sck::deinterleave::itl](mdm2[mdm::sck::tdemodulate_wg::Y_N3]);
		else
			itl2[itl::sck::deinterleave::itl](mdm2[mdm::sck::tdemodulate::Y_N3]);
	}
	else if (this->params_BFER_ite.qnt->type != "NO")
		itl2[itl::sck::deinterleave::itl](qnt[qnt::sck::process::Y_N2]);
	else if (mdm1.is_filter())
		itl2[itl::sck::deinterleave::itl](mdm1[mdm::sck::filter::Y_N2]);
	else if (this->params_BFER_ite.chn->type != "NO")
	{
		if (is_rayleigh)
			itl2[itl::sck::deinterleave::itl](chn[chn::sck::add_noise_wg::Y_N]);
		else
			itl2[itl::sck::deinterleave::itl](chn[chn::sck::add_noise::Y_N]);
	}
	else
		itl2[itl::sck::deinterleave::itl](mdm1[mdm::sck::modulate::X_N2]);

	if (this->params_BFER_ite.crc->type != "NO")
	{
		if (this->params_BFER_ite.coset)
			ext2[ext::sck::get_sys_bit::Y_N](dcs[dec::sck::decode_siso::Y_N2]);
		else
			ext2[ext::sck::get_sys_bit::Y_N](itl2[itl::sck::deinterleave::nat]);

		lcrc[lop::sck::stop::in2    ](ext2[ext::sck::get_sys_bit ::V_K]);
		lcrc[lop::sck::stop::in_out2](itl2[itl::sck::deinterleave::nat]);
	}
	else
		lite[lop::sck::stop::in_out2](itl2[itl::sck::deinterleave::nat]);

	// ----------------------------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------- end of turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	if (this->params_BFER_ite.coset)
	{
		if (this->params_BFER_ite.crc->type != "NO")
			csr3[cst::sck::apply::in](lcrc[lop::sck::stop::in_out2]);
		else
			csr3[cst::sck::apply::in](lite[lop::sck::stop::in_out2]);

		if (this->params_BFER_ite.coded_monitoring)
		{
			dch[dec::sck::decode_siho_cw::Y_N](csr3[cst::sck::apply         ::out]);
			csb[cst::sck::apply         ::in ](dch [dec::sck::decode_siho_cw::V_N]);
		}
		else
		{
			dch[dec::sck::decode_siho::Y_N ](csr3[cst::sck::apply      ::out]);
			csb[cst::sck::apply      ::in  ](dch [dec::sck::decode_siho::V_K]);
			crc[crc::sck::extract    ::V_K1](csb [cst::sck::apply      ::out]);
		}
	}
	else
	{
		if (this->params_BFER_ite.coded_monitoring)
		{
			if (this->params_BFER_ite.crc->type != "NO")
				dch[dec::sck::decode_siho_cw::Y_N](lcrc[lop::sck::stop::in_out2]);
			else
				dch[dec::sck::decode_siho_cw::Y_N](lite[lop::sck::stop::in_out2]);
		}
		else
		{
			if (this->params_BFER_ite.crc->type != "NO")
				dch[dec::sck::decode_siho::Y_N](lcrc[lop::sck::stop::in_out2]);
			else
				dch[dec::sck::decode_siho::Y_N](lite[lop::sck::stop::in_out2]);

			crc[crc::sck::extract::V_K1](dch[dec::sck::decode_siho::V_K]);
		}
	}

	if (this->params_BFER_ite.coded_monitoring)
	{
		if (this->params_BFER_ite.coset)
			mnt[mnt::sck::check_errors::V](csb[cst::sck::apply::out]);
		else
			mnt[mnt::sck::check_errors::V](dch[dec::sck::decode_siho_cw::V_N]);
	}
	else
	{
		if (this->params_BFER_ite.crc->type != "NO")
			mnt[mnt::sck::check_errors::V](crc[crc::sck::extract::V_K2]);
		else
		{
			if (this->params_BFER_ite.coset)
				mnt[mnt::sck::check_errors::V](csb[cst::sck::apply::out]);
			else
				mnt[mnt::sck::check_errors::V](dch[dec::sck::decode_siho::V_K]);
		}
	}
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::simulation_loop(const int tid)
{
	auto &source           = *this->source          [tid];
	auto &crc              = *this->crc             [tid];
	auto &codec1           = *this->codec1          [tid];
	auto &codec2           = *this->codec2          [tid];
	auto &interleaver_bit  = *this->interleaver_bit [tid];
	auto &modem1           = *this->modem1          [tid];
	auto &modem2           = *this->modem2          [tid];
	auto &channel          = *this->channel         [tid];
	auto &quantizer        = *this->quantizer       [tid];
	auto &interleaver_llr1 = *this->interleaver_llr1[tid];
	auto &interleaver_llr2 = *this->interleaver_llr2[tid];
	auto &coset_real1      = *this->coset_real1     [tid];
	auto &coset_real2      = *this->coset_real2     [tid];
	auto &coset_real3      = *this->coset_real3     [tid];
	auto &coset_bit        = *this->coset_bit       [tid];
	auto &monitor          = *this->monitor_er      [tid];
	auto &loop_ite         = *this->loop_ite        [tid];
	auto &loop_crc         = *this->loop_crc        [tid];

	auto &encoder      = codec1.get_encoder();
	auto &decoder_siso = codec1.get_decoder_siso();
	auto &decoder_siho = codec1.get_decoder_siho();
	auto &extractor1   = codec1.get_extractor();
	auto &extractor2   = codec2.get_extractor();

	using namespace module;

	while (this->keep_looping_noise_point())
	{
		if (this->params_BFER_ite.debug)
		{
			if (!monitor[mnt::tsk::check_errors].get_n_calls())
				std::cout << "#" << std::endl;

			auto fid = monitor[mnt::tsk::check_errors].get_n_calls();
			std::cout << "# -------------------------------"     << std::endl;
			std::cout << "# New communication (n°" << fid << ")" << std::endl;
			std::cout << "# -------------------------------"     << std::endl;
			std::cout << "#"                                     << std::endl;
		}

		if (this->params_BFER_ite.src->type != "AZCW")
		{
			source[src::tsk::generate].exec();
			if (this->params_BFER_ite.crc->type != "NO")
				crc[crc::tsk::build].exec();
			if (this->params_BFER_ite.cdc->enc->type != "NO")
				encoder[enc::tsk::encode].exec();

			interleaver_bit[itl::tsk::interleave].exec();
			modem1         [mdm::tsk::modulate  ].exec();
		}

		if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			if (this->params_BFER_ite.chn->type != "NO")
				channel[chn::tsk::add_noise_wg].exec();
			if (modem1.is_filter())
				modem1[mdm::tsk::filter].exec();
			if (this->params_BFER_ite.qnt->type != "NO")
				quantizer[qnt::tsk::process].exec();
			if (modem1.is_demodulator())
				modem1[mdm::tsk::demodulate_wg].exec();
		}
		else
		{
			if (this->params_BFER_ite.chn->type != "NO")
				channel[chn::tsk::add_noise].exec();
			if (modem1.is_filter())
				modem1[mdm::tsk::filter].exec();
			if (this->params_BFER_ite.qnt->type != "NO")
				quantizer[qnt::tsk::process].exec();
			if (modem1.is_demodulator())
				modem1[mdm::tsk::demodulate].exec();
		}

		interleaver_llr1[itl::tsk::deinterleave].exec();
		if (this->params_BFER_ite.crc->type != "NO")
			extractor1[ext::tsk::get_sys_bit].exec();

		// ------------------------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------ turbo demodulation loop
		// ------------------------------------------------------------------------------------------------------------

		auto &loop = this->params_BFER_ite.crc->type != "NO" ? loop_crc : loop_ite;

		while (!loop[lop::tsk::stop].exec())
		{
			// ----------------------------------------------------------------------------------------------- decoding
			if (this->params_BFER_ite.coset)
			{
				coset_real1 [cst::tsk::apply      ].exec();
				decoder_siso[dec::tsk::decode_siso].exec();
				coset_real2 [cst::tsk::apply      ].exec();
			}
			else
			{
				decoder_siso[dec::tsk::decode_siso].exec();
			}

			// ------------------------------------------------------------------------------------------- interleaving
			interleaver_llr2[itl::tsk::interleave].exec();

			// ------------------------------------------------------------------------------------------- demodulation
			if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos)
			{
				if (modem2.is_demodulator())
					modem2[mdm::tsk::tdemodulate_wg].exec();
			}
			else
			{
				if (modem2.is_demodulator())
					modem2[mdm::tsk::tdemodulate].exec();
			}

			// ----------------------------------------------------------------------------------------- deinterleaving
			interleaver_llr2[itl::tsk::deinterleave].exec();
			if (this->params_BFER_ite.crc->type != "NO")
				extractor2[ext::tsk::get_sys_bit].exec();
		}

		if (this->params_BFER_ite.coset)
		{
			coset_real3[cst::tsk::apply].exec();

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
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::simulation::BFER_ite_threads<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER_ite_threads<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER_ite_threads<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER_ite_threads<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER_ite_threads<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
