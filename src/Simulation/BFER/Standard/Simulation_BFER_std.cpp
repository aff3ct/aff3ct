#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <string>

#include "Tools/Sequence/Sequence.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Interface/Interface_set_seed.hpp"
#include "Tools/Interface/Interface_get_set_noise.hpp"
#include "Tools/Interface/Interface_notify_noise_update.hpp"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"
#include "Tools/Display/Dumper/Dumper.hpp"
#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Coset/Coset.hpp"
#include "Factory/Tools/Codec/Codec.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"
#include "Simulation/BFER/Standard/Simulation_BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_std<B,R,Q>
::Simulation_BFER_std(const factory::BFER_std &params_BFER_std)
: Simulation_BFER<B,R>(params_BFER_std),
  params_BFER_std(params_BFER_std)
{
	if (this->params_BFER_std.err_track_revert && this->params_BFER_std.n_threads != 1)
		std::clog << rang::tag::warning << "Multi-threading detected with error tracking revert feature! "
		                                   "Each thread will play the same frames. Please run with one thread."
		          << std::endl;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Source<B>> Simulation_BFER_std<B,R,Q>
::build_source()
{
	auto src = std::unique_ptr<module::Source<B>>(params_BFER_std.src->build<B>());
	src->set_n_frames(this->params.n_frames);
	return src;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::CRC<B>> Simulation_BFER_std<B,R,Q>
::build_crc()
{
	auto crc = std::unique_ptr<module::CRC<B>>(params_BFER_std.crc->build<B>());
	crc->set_n_frames(this->params.n_frames);
	return crc;
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools::Codec_SIHO<B,Q>> Simulation_BFER_std<B,R,Q>
::build_codec(const module::CRC<B> *crc)
{
	std::unique_ptr<factory::Codec> params_cdc(params_BFER_std.cdc->clone());
	auto crc_ptr = this->params_BFER_std.crc->type == "NO" ? nullptr : crc;
	auto param_siho = dynamic_cast<factory::Codec_SIHO*>(params_cdc.get());

	auto cdc = std::unique_ptr<tools::Codec_SIHO<B,Q>>(param_siho->build<B,Q>(crc_ptr));
	cdc->set_n_frames(this->params.n_frames);
	return cdc;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Modem<B,R,R>> Simulation_BFER_std<B,R,Q>
::build_modem(const tools::Distributions<R> *distributions, const tools::Constellation<R> *constellation)
{
	if (distributions != nullptr)
	{
		auto mdm = std::unique_ptr<module::Modem<B,R,R>>(params_BFER_std.mdm->build<B,R,R>(*distributions));
		mdm->set_n_frames(this->params.n_frames);
		return mdm;
	}
	else
	{
		auto mdm =  std::unique_ptr<module::Modem<B,R,R>>(params_BFER_std.mdm->build<B,R,R>(constellation));
		mdm->set_n_frames(this->params.n_frames);
		return mdm;
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Channel<R>> Simulation_BFER_std<B,R,Q>
::build_channel(const tools::Distributions<R> *distributions)
{
	if (distributions != nullptr)
	{
		auto chn = std::unique_ptr<module::Channel<R>>(params_BFER_std.chn->build<R>(*distributions));
		chn->set_n_frames(this->params.n_frames);
		return chn;
	}
	else
	{
		auto chn = std::unique_ptr<module::Channel<R>>(params_BFER_std.chn->build<R>());
		chn->set_n_frames(this->params.n_frames);
		return chn;
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Quantizer<R,Q>> Simulation_BFER_std<B,R,Q>
::build_quantizer()
{
	auto qnt = std::unique_ptr<module::Quantizer<R,Q>>(params_BFER_std.qnt->build<R,Q>());
	qnt->set_n_frames(this->params.n_frames);
	return qnt;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,Q>> Simulation_BFER_std<B,R,Q>
::build_coset_real()
{
	factory::Coset cst_params;
	cst_params.size = params_BFER_std.cdc->N_cw;
	auto cst = std::unique_ptr<module::Coset<B,Q>>(cst_params.build_real<B,Q>());
	cst->set_n_frames(this->params.n_frames);
	return cst;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,B>> Simulation_BFER_std<B,R,Q>
::build_coset_bit()
{
	factory::Coset cst_params;
	cst_params.size = this->params_BFER_std.coded_monitoring ? params_BFER_std.cdc->N_cw : params_BFER_std.cdc->K;
	auto cst = std::unique_ptr<module::Coset<B,B>>(cst_params.build_bit<B,B>());
	cst->set_n_frames(this->params.n_frames);
	return cst;
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std<B,R,Q>
::create_modules()
{
	Simulation_BFER<B,R>::create_modules();

	this->source     = this->build_source    (                                                    );
	this->crc        = this->build_crc       (                                                    );
	this->codec      = this->build_codec     (this->crc.get()                                     );
	this->modem      = this->build_modem     (this->distributions.get(), this->constellation.get());
	this->channel    = this->build_channel   (this->distributions.get()                           );
	this->quantizer  = this->build_quantizer (                                                    );
	this->coset_real = this->build_coset_real(                                                    );
	this->coset_bit  = this->build_coset_bit (                                                    );
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std<B,R,Q>
::bind_sockets()
{
	using namespace module;

	auto &src = *this->source;
	auto &crc = *this->crc;
	auto &enc =  this->codec->get_encoder();
	auto &pct =  this->codec->get_puncturer();
	auto &mdm = *this->modem;
	auto &chn = *this->channel;
	auto &qnt = *this->quantizer;
	auto &csr = *this->coset_real;
	auto &dec =  this->codec->get_decoder_siho();
	auto &csb = *this->coset_bit;
	auto &mnt = *this->monitor_er;
	auto &mni = *this->monitor_mi;

	std::vector<Module*> modules = {&src, &crc, &enc, &pct, &mdm, &chn, &qnt, &csr, &dec, &csb, &mnt, &mni};
	for (auto& mod : modules)
		for (auto& tsk : mod->tasks)
			tsk->set_autoalloc(true);

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

		mdm[mdm::sck::modulate::X_N1] = pct[pct::sck::puncture::X_N2];
		mdm[mdm::tsk::modulate].exec();
		mdm[mdm::tsk::modulate].reset();
	}
	else
	{
		if (this->params_BFER_std.crc->type != "NO")
			crc[crc::sck::build::U_K1] = src[src::sck::generate::U_K];

		if (this->params_BFER_std.cdc->enc->type != "NO")
		{
			if (this->params_BFER_std.crc->type != "NO")
				enc[enc::sck::encode::U_K] = crc[crc::sck::build::U_K2];
			else
				enc[enc::sck::encode::U_K] = src[src::sck::generate::U_K];
		}

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
		{
			if (this->params_BFER_std.cdc->enc->type != "NO")
				pct[pct::sck::puncture::X_N1] = enc[enc::sck::encode::X_N];
			else if (this->params_BFER_std.crc->type != "NO")
				pct[pct::sck::puncture::X_N1] = crc[crc::sck::build::U_K2];
			else
				pct[pct::sck::puncture::X_N1] = src[src::sck::generate::U_K];
		}

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
			mdm[mdm::sck::modulate::X_N1] = pct[pct::sck::puncture::X_N2];
		else if (this->params_BFER_std.cdc->enc->type != "NO")
			mdm[mdm::sck::modulate::X_N1] = enc[enc::sck::encode::X_N];
		else if (this->params_BFER_std.crc->type != "NO")
			mdm[mdm::sck::modulate::X_N1] = crc[crc::sck::build::U_K2];
		else
			mdm[mdm::sck::modulate::X_N1] = src[src::sck::generate::U_K];
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
			chn[chn::sck::add_noise_wg::CP ] = this->channel_params;
			chn[chn::sck::add_noise_wg::X_N] = mdm[mdm::sck::modulate::X_N2];
		}

		if (mdm.is_filter())
		{
			mdm[mdm::sck::filter::CP] = this->channel_params;
			if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::filter::Y_N1] = chn[chn::sck::add_noise_wg::Y_N];
			else
				mdm[mdm::sck::filter::Y_N1] = mdm[mdm::sck::modulate::X_N2];
		}

		if (mdm.is_demodulator())
		{
			mdm[mdm::sck::demodulate_wg::CP] = this->channel_params;
			if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::demodulate_wg::H_N] = chn[chn::sck::add_noise_wg::H_N];
			else
				mdm[mdm::sck::demodulate_wg::H_N] = (uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr());

			if (mdm.is_filter())
				mdm[mdm::sck::demodulate_wg::Y_N1](mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::demodulate_wg::Y_N1] = chn[chn::sck::add_noise_wg::Y_N];
			else
				mdm[mdm::sck::demodulate_wg::Y_N1] = mdm[mdm::sck::modulate::X_N2];
		}

		if (this->params_BFER_std.qnt->type != "NO")
		{
			if (mdm.is_demodulator())
				qnt[qnt::sck::process::Y_N1] = mdm[mdm::sck::demodulate_wg::Y_N2];
			else if (mdm.is_filter())
				qnt[qnt::sck::process::Y_N1] = mdm[mdm::sck::filter::Y_N2];
			else if (this->params_BFER_std.chn->type != "NO")
				qnt[qnt::sck::process::Y_N1] = chn[chn::sck::add_noise_wg::Y_N];
			else
				qnt[qnt::sck::process::Y_N1] = mdm[mdm::sck::modulate::X_N2];
		}
	}
	else if (is_optical)
	{
		chn[chn::sck::add_noise    ::CP  ] = this->channel_params;
		mdm[mdm::sck::demodulate_wg::CP  ] = this->channel_params;
		chn[chn::sck::add_noise    ::X_N ] = mdm[mdm::sck::modulate ::X_N2];
		mdm[mdm::sck::demodulate_wg::H_N ] = mdm[mdm::sck::modulate ::X_N2];
		mdm[mdm::sck::demodulate_wg::Y_N1] = chn[chn::sck::add_noise::Y_N ];
		if (this->params_BFER_std.qnt->type != "NO")
			qnt[qnt::sck::process::Y_N1] = mdm[mdm::sck::demodulate_wg::Y_N2];
	}
	else
	{
		if (this->params_BFER_std.chn->type != "NO")
		{
			chn[chn::sck::add_noise::CP ] = this->channel_params;
			chn[chn::sck::add_noise::X_N] = mdm[mdm::sck::modulate::X_N2];
		}

		if (mdm.is_filter())
		{
			mdm[mdm::sck::filter::CP] = this->channel_params;
			if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::filter::Y_N1] = chn[chn::sck::add_noise::Y_N];
			else
				mdm[mdm::sck::filter::Y_N1] = mdm[mdm::sck::modulate::X_N2];
		}

		if (mdm.is_demodulator())
		{
			mdm[mdm::sck::demodulate::CP] = this->channel_params;
			if (mdm.is_filter())
				mdm[mdm::sck::demodulate::Y_N1] = mdm[mdm::sck::filter::Y_N2];
			else if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::demodulate::Y_N1] = chn[chn::sck::add_noise::Y_N];
			else
				mdm[mdm::sck::demodulate::Y_N1] = mdm[mdm::sck::modulate::X_N2];
		}

		if (this->params_BFER_std.qnt->type != "NO")
		{
			if (mdm.is_demodulator())
				qnt[qnt::sck::process::Y_N1] = mdm[mdm::sck::demodulate::Y_N2];
			else if (mdm.is_filter())
				qnt[qnt::sck::process::Y_N1] = mdm[mdm::sck::filter::Y_N2];
			else if (this->params_BFER_std.chn->type != "NO")
				qnt[qnt::sck::process::Y_N1] = chn[chn::sck::add_noise::Y_N];
			else
				qnt[qnt::sck::process::Y_N1] = mdm[mdm::sck::modulate::X_N2];
		}
	}

	if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
	{
		if (this->params_BFER_std.qnt->type != "NO")
			pct[pct::sck::depuncture::Y_N1] = qnt[qnt::sck::process::Y_N2];
		else if (mdm.is_demodulator() || is_optical)
		{
			if (is_rayleigh || is_optical)
				pct[pct::sck::depuncture::Y_N1] = mdm[mdm::sck::demodulate_wg::Y_N2];
			else
				pct[pct::sck::depuncture::Y_N1] = mdm[mdm::sck::demodulate::Y_N2];
		}
		else if (mdm.is_filter())
			pct[pct::sck::depuncture::Y_N1] = mdm[mdm::sck::filter::Y_N2];
		else if (this->params_BFER_std.chn->type != "NO")
		{
			if (is_rayleigh)
				pct[pct::sck::depuncture::Y_N1] = chn[chn::sck::add_noise_wg::Y_N];
			else
				pct[pct::sck::depuncture::Y_N1] = chn[chn::sck::add_noise::Y_N];
		}
		else
			pct[pct::sck::depuncture::Y_N1] = mdm[mdm::sck::modulate::X_N2];
	}

	if (this->params_BFER_std.coset)
	{
		if (this->params_BFER_std.cdc->enc->type != "NO")
			csr[cst::sck::apply::ref] = enc[enc::sck::encode::X_N];
		else if (this->params_BFER_std.crc->type != "NO")
			csr[cst::sck::apply::ref] = crc[crc::sck::build::U_K2];
		else
			csr[cst::sck::apply::ref] = src[src::sck::generate::U_K];

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
			csr[cst::sck::apply::in] = pct[pct::sck::depuncture::Y_N2];
		else if (this->params_BFER_std.qnt->type != "NO")
			csr[cst::sck::apply::in] = qnt[qnt::sck::process::Y_N2];
		else if (mdm.is_demodulator() || is_optical)
		{
			if (is_rayleigh || is_optical)
				csr[cst::sck::apply::in] = mdm[mdm::sck::demodulate_wg::Y_N2];
			else
				csr[cst::sck::apply::in] = mdm[mdm::sck::demodulate::Y_N2];
		}
		else if (mdm.is_filter())
			csr[cst::sck::apply::in] = mdm[mdm::sck::filter::Y_N2];
		else if (this->params_BFER_std.chn->type != "NO")
		{
			if (is_rayleigh)
				csr[cst::sck::apply::in] = chn[chn::sck::add_noise_wg::Y_N];
			else
				csr[cst::sck::apply::in] = chn[chn::sck::add_noise::Y_N];
		}
		else
			csr[cst::sck::apply::in] = mdm[mdm::sck::modulate::X_N2];

		if (this->params_BFER_std.coded_monitoring)
		{
			dec[dec::sck::decode_siho_cw::Y_N] = csr[cst::sck::apply::out];

			if (this->params_BFER_std.cdc->enc->type != "NO")
				csb[cst::sck::apply::ref] = enc[enc::sck::encode::X_N];
			else if (this->params_BFER_std.crc->type != "NO")
				csb[cst::sck::apply::ref] = crc[crc::sck::build::U_K2];
			else
				csb[cst::sck::apply::ref] = src[src::sck::generate::U_K];

			csb[cst::sck::apply::in] = dec[dec::sck::decode_siho_cw::V_N];
		}
		else
		{
			dec[dec::sck::decode_siho::Y_N] = csr[cst::sck::apply::out];

			if (this->params_BFER_std.crc->type != "NO")
				csb[cst::sck::apply::ref] = crc[crc::sck::build::U_K2];
			else
				csb[cst::sck::apply::ref] = src[src::sck::generate::U_K];

			csb[cst::sck::apply::in] = dec[dec::sck::decode_siho::V_K];

			if (this->params_BFER_std.crc->type != "NO")
				crc[crc::sck::extract::V_K1] = csb[cst::sck::apply::out];
		}
	}
	else
	{
		if (this->params_BFER_std.coded_monitoring)
		{
			if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
				dec[dec::sck::decode_siho_cw::Y_N] = pct[pct::sck::depuncture::Y_N2];
			else if (this->params_BFER_std.qnt->type != "NO")
				dec[dec::sck::decode_siho_cw::Y_N] = qnt[qnt::sck::process::Y_N2];
			else if (mdm.is_demodulator() || is_optical)
			{
				if (is_rayleigh || is_optical)
					dec[dec::sck::decode_siho_cw::Y_N] = mdm[mdm::sck::demodulate_wg::Y_N2];
				else
					dec[dec::sck::decode_siho_cw::Y_N] = mdm[mdm::sck::demodulate::Y_N2];
			}
			else if (mdm.is_filter())
				dec[dec::sck::decode_siho_cw::Y_N] = mdm[mdm::sck::filter::Y_N2];
			else if (this->params_BFER_std.chn->type != "NO")
			{
				if (is_rayleigh)
					dec[dec::sck::decode_siho_cw::Y_N] = chn[chn::sck::add_noise_wg::Y_N];
				else
					dec[dec::sck::decode_siho_cw::Y_N] = chn[chn::sck::add_noise::Y_N];
			}
			else
				dec[dec::sck::decode_siho_cw::Y_N] = mdm[mdm::sck::modulate::X_N2];
		}
		else
		{
			if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
				dec[dec::sck::decode_siho::Y_N] = pct[pct::sck::depuncture::Y_N2];
			else if (this->params_BFER_std.qnt->type != "NO")
				dec[dec::sck::decode_siho::Y_N] = qnt[qnt::sck::process::Y_N2];
			else if (mdm.is_demodulator() || is_optical)
			{
				if (is_rayleigh || is_optical)
					dec[dec::sck::decode_siho::Y_N] = mdm[mdm::sck::demodulate_wg::Y_N2];
				else
					dec[dec::sck::decode_siho::Y_N] = mdm[mdm::sck::demodulate::Y_N2];
			}
			else if (mdm.is_filter())
				dec[dec::sck::decode_siho::Y_N] = mdm[mdm::sck::filter::Y_N2];
			else if (this->params_BFER_std.chn->type != "NO")
			{
				if (is_rayleigh)
					dec[dec::sck::decode_siho::Y_N] = chn[chn::sck::add_noise_wg::Y_N];
				else
					dec[dec::sck::decode_siho::Y_N] = chn[chn::sck::add_noise::Y_N];
			}
			else
				dec[dec::sck::decode_siho::Y_N] = mdm[mdm::sck::modulate::X_N2];

			if (this->params_BFER_std.crc->type != "NO")
				crc[crc::sck::extract::V_K1] = dec[dec::sck::decode_siho::V_K];
		}
	}

	if (this->params_BFER_std.coded_monitoring)
	{
		if (this->params_BFER_std.src->type == "AZCW")
			mnt[mnt::sck::check_errors::U] = enc[enc::sck::encode::X_N].get_dataptr();
		else
		{
			if (this->params_BFER_std.cdc->enc->type != "NO")
				mnt[mnt::sck::check_errors::U] = enc[enc::sck::encode::X_N];
			else if (this->params_BFER_std.crc->type != "NO")
				mnt[mnt::sck::check_errors::U] = crc[crc::sck::build::U_K2];
			else
				mnt[mnt::sck::check_errors::U] = src[src::sck::generate::U_K];
		}

		if (this->params_BFER_std.coset)
			mnt[mnt::sck::check_errors::V] = csb[cst::sck::apply::out];
		else
			mnt[mnt::sck::check_errors::V] = dec[dec::sck::decode_siho_cw::V_N];
	}
	else
	{
		if (this->params_BFER_std.src->type == "AZCW")
			mnt[mnt::sck::check_errors::U] = enc[enc::sck::encode::X_N].get_dataptr();
		else
			mnt[mnt::sck::check_errors::U] = src[src::sck::generate::U_K];
		if (this->params_BFER_std.crc->type != "NO")
			mnt[mnt::sck::check_errors::V] = crc[crc::sck::extract::V_K2];
		else if (this->params_BFER_std.coset)
			mnt[mnt::sck::check_errors::V] = csb[cst::sck::apply::out];
		else
			mnt[mnt::sck::check_errors::V] = dec[dec::sck::decode_siho::V_K];
	}

	if (this->params_BFER_std.mnt_mutinfo)
	{
		if (this->params_BFER_std.src->type == "AZCW")
			mni[mnt::sck::get_mutual_info::X] = enc[enc::sck::encode::X_N].get_dataptr();
		else
		{
			if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
				mni[mnt::sck::get_mutual_info::X] = pct[pct::sck::puncture::X_N2];
			else if (this->params_BFER_std.cdc->enc->type != "NO")
				mni[mnt::sck::get_mutual_info::X] = enc[enc::sck::encode::X_N];
			else if (this->params_BFER_std.crc->type != "NO")
				mni[mnt::sck::get_mutual_info::X] = crc[crc::sck::build::U_K2];
			else
				mni[mnt::sck::get_mutual_info::X] = src[src::sck::generate::U_K];
		}

		if (mdm.is_demodulator())
		{
			if (is_rayleigh || is_optical)
				mni[mnt::sck::get_mutual_info::Y] = mdm[mdm::sck::demodulate_wg::Y_N2];
			else
				mni[mnt::sck::get_mutual_info::Y] = mdm[mdm::sck::demodulate::Y_N2];
		}
		else if (mdm.is_filter())
			mni[mnt::sck::get_mutual_info::Y] = mdm[mdm::sck::filter::Y_N2];
		else if (this->params_BFER_std.chn->type != "NO")
			mni[mnt::sck::get_mutual_info::Y] = chn[chn::sck::add_noise_wg::Y_N];
		else
			mni[mnt::sck::get_mutual_info::Y] = mdm[mdm::sck::modulate::X_N2];
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std<B,R,Q>
::create_sequence()
{
	const auto is_rayleigh = this->params_BFER_std.chn->type.find("RAYLEIGH") != std::string::npos;
	const auto is_optical = this->params_BFER_std.chn->type == "OPTICAL" && this->params_BFER_std.mdm->rop_est_bits > 0;
	const auto t = this->params_BFER.n_threads;
	if (this->params_BFER_std.src->type != "AZCW")
		this->sequence.reset(new tools::Sequence((*this->source)[module::src::tsk::generate], t));
	else if (this->params_BFER_std.chn->type != "NO")
	{
		if (is_rayleigh)
			this->sequence.reset(new tools::Sequence((*this->channel)[module::chn::tsk::add_noise_wg], t));
		else
			this->sequence.reset(new tools::Sequence((*this->channel)[module::chn::tsk::add_noise], t));
	}
	else if (this->modem->is_demodulator())
	{
		if (is_rayleigh || is_optical)
			this->sequence.reset(new tools::Sequence((*this->modem)[module::mdm::tsk::demodulate_wg], t));
		else
			this->sequence.reset(new tools::Sequence((*this->modem)[module::mdm::tsk::demodulate], t));
	}
	else if (this->modem->is_filter())
		this->sequence.reset(new tools::Sequence((*this->modem)[module::mdm::tsk::filter], t));
	else if (this->params_BFER_std.qnt->type != "NO")
		this->sequence.reset(new tools::Sequence((*this->quantizer)[module::qnt::tsk::process], t));
	else if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
		this->sequence.reset(new tools::Sequence(this->codec->get_puncturer()[module::pct::tsk::puncture], t));
	else if (this->params_BFER_std.coset)
		this->sequence.reset(new tools::Sequence((*this->coset_real)[module::cst::tsk::apply], t));
	else
		this->sequence.reset(new tools::Sequence(this->codec->get_decoder_siho()[module::dec::tsk::decode_siho], t));

	// set the noise
	this->codec->set_noise(*this->noise);
	for (auto &m : this->sequence->template get_modules<tools::Interface_get_set_noise>())
		m->set_noise(*this->noise);

	// registering to noise updates
	this->noise->record_callback_update([this](){ this->codec->notify_noise_update(); });
	for (auto &m : this->sequence->template get_modules<tools::Interface_notify_noise_update>())
		this->noise->record_callback_update([m](){ m->notify_noise_update(); });

	// set different seeds in the modules that uses PRNG
	std::mt19937 prng(params_BFER_std.local_seed);
	for (auto &m : this->sequence->template get_modules<tools::Interface_set_seed>())
		m->set_seed(prng());

	auto fb_modules = this->sequence->template get_modules<tools::Interface_get_set_frozen_bits>();
	if (fb_modules.size())
	{
		this->noise->record_callback_update([fb_modules](){
		for (auto &m : fb_modules)
			m->set_frozen_bits(fb_modules[0]->get_frozen_bits());
		});
	}

	bool is_interleaver = true;
	try
	{
		codec->get_interleaver().set_seed(params_BFER_std.cdc->itl->core->seed);
	}
	catch (...)
	{
		is_interleaver = false;
	}
	if (is_interleaver && codec->get_interleaver().is_uniform())
	{
		std::stringstream message;
		message << "Uniform interleaver is not supported at this time in the simulations";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->params_BFER_std.err_track_enable)
	{
		auto sources = this->sequence->template get_modules<module::Source<B>>();
		for (size_t tid = 0; tid < (size_t)this->params_BFER.n_threads; tid++)
		{
			auto &source  = sources.size() ? *sources[tid] : *this->source;
			auto src_data = (B*)(source[module::src::sck::generate::U_K].get_dataptr());
			auto src_bytes = source[module::src::sck::generate::U_K].get_databytes();
			auto src_size = (src_bytes / sizeof(B)) / this->params_BFER_std.n_frames;
			this->dumper[tid]->register_data(src_data,
			                                 (unsigned int)src_size,
			                                 this->params_BFER_std.err_track_threshold,
			                                 "src",
			                                 false,
			                                 this->params_BFER_std.n_frames,
			                                 {});
		}

		auto encoders = this->sequence->template get_modules<module::Encoder<B>>();
		for (size_t tid = 0; tid < (size_t)this->params_BFER.n_threads; tid++)
		{
			auto &encoder = encoders.size() ? *encoders[tid] : this->codec->get_encoder();
			auto enc_data = (B*)(encoder[module::enc::sck::encode::X_N].get_dataptr());
			auto enc_bytes = encoder[module::enc::sck::encode::X_N].get_databytes();
			auto enc_size = (enc_bytes / sizeof(B)) / this->params_BFER_std.n_frames;
			this->dumper[tid]->register_data(enc_data,
			                                 (unsigned int)enc_size,
			                                 this->params_BFER_std.err_track_threshold,
			                                 "enc",
			                                 false,
			                                 this->params_BFER_std.n_frames,
			                                 {(unsigned)this->params_BFER_std.cdc->enc->K});
		}

		auto channels = this->sequence->template get_modules<module::Channel<R>>();
		for (size_t tid = 0; tid < (size_t)this->params_BFER.n_threads; tid++)
		{
			auto &channel = channels.size() ? *channels[tid] : *this->channel;
			this->dumper[tid]->register_data(channel.get_noised_data(),
			                                 this->params_BFER_std.err_track_threshold,
			                                 "chn",
			                                 true,
			                                 this->params_BFER_std.n_frames,
			                                 {});
		}

		auto monitors_er = this->sequence->template get_modules<module::Monitor_BFER<B>>();
		for (size_t tid = 0; tid < (size_t)this->params_BFER.n_threads; tid++)
		{
			monitors_er[tid]->record_callback_fe(std::bind(&tools::Dumper::add,
			                                               this->dumper[tid].get(),
			                                               std::placeholders::_1,
			                                               std::placeholders::_2));
		}
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::simulation::Simulation_BFER_std<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_std<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_std<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_std<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_std<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
