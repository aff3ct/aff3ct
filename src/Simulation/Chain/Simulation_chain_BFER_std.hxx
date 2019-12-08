#include <random>

#include "Tools/Display/Reporter/MI/Reporter_MI.hpp"
#include "Tools/Display/Reporter/BFER/Reporter_BFER.hpp"
#include "Tools/Display/Reporter/Noise/Reporter_noise.hpp"
#include "Tools/Display/Reporter/Throughput/Reporter_throughput.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Display/Statistics/Statistics.hpp"
#include "Tools/Interface/Interface_set_seed.hpp"
#include "Tools/Interface/Interface_get_set_noise.hpp"
#include "Tools/Interface/Interface_notify_noise_update.hpp"
#include "Factory/Module/Coset/Coset.hpp"
#include "Simulation/Chain/Simulation_chain_BFER_std.hpp"

namespace aff3ct
{
namespace simulation
{

template <typename B, typename R, typename Q>
Simulation_chain_BFER_std<B,R,Q>
::Simulation_chain_BFER_std(const factory::BFER_std &params_BFER_std)
: Simulation(),
  params(params_BFER_std),
  params_BFER(params_BFER_std),
  params_BFER_std(params_BFER_std),
  noise(params_BFER.noise->build<>()),
  dumper(params_BFER.n_threads)
{
	if (params_BFER.n_threads < 1)
	{
		std::stringstream message;
		message << "'n_threads' has to be greater than 0 ('n_threads' = " << params_BFER.n_threads << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->params_BFER_std.err_track_revert && this->params_BFER_std.n_threads != 1)
		std::clog << rang::tag::warning << "Multi-threading detected with error tracking revert feature! "
		                                   "Each thread will play the same frames. Please run with one thread."
		          << std::endl;

	if (params_BFER.err_track_enable)
	{
		for (auto tid = 0; tid < params_BFER.n_threads; tid++)
			dumper[tid].reset(new tools::Dumper());
		dumper_red.reset(new tools::Dumper_reduction(dumper));
	}

	if (!params_BFER.noise->pdf_path.empty())
		this->distributions.reset(new tools::Distributions<R>(params_BFER.noise->pdf_path,
		                                                      tools::Distribution_mode::SUMMATION));
	try
	{
		this->constellation.reset(params_BFER.mdm->build_constellation<R>());
	}
	catch (tools::cannot_allocate&) {}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Source<B>> Simulation_chain_BFER_std<B,R,Q>
::build_source()
{
	return std::unique_ptr<module::Source<B>>(params_BFER_std.src->build<B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::CRC<B>> Simulation_chain_BFER_std<B,R,Q>
::build_crc()
{
	return std::unique_ptr<module::CRC<B>>(params_BFER_std.crc->build<B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools::Codec_SIHO<B,Q>> Simulation_chain_BFER_std<B,R,Q>
::build_codec(const module::CRC<B> *crc)
{
	std::unique_ptr<factory::Codec> params_cdc(params_BFER_std.cdc->clone());
	auto crc_ptr = this->params_BFER_std.crc->type == "NO" ? nullptr : crc;
	auto param_siho = dynamic_cast<factory::Codec_SIHO*>(params_cdc.get());
	return std::unique_ptr<tools::Codec_SIHO<B,Q>>(param_siho->build<B,Q>(crc_ptr));
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Modem<B,R,R>> Simulation_chain_BFER_std<B,R,Q>
::build_modem(const tools::Distributions<R> *distributions, const tools::Constellation<R> *constellation)
{
	if (distributions != nullptr)
		return std::unique_ptr<module::Modem<B,R,R>>(params_BFER_std.mdm->build<B,R,R>(*distributions));
	else
		return std::unique_ptr<module::Modem<B,R,R>>(params_BFER_std.mdm->build<B,R,R>(constellation));
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Channel<R>> Simulation_chain_BFER_std<B,R,Q>
::build_channel(const tools::Distributions<R> *distributions)
{
	if (distributions != nullptr)
		return std::unique_ptr<module::Channel<R>>(params_BFER_std.chn->build<R>(*distributions));
	else
		return std::unique_ptr<module::Channel<R>>(params_BFER_std.chn->build<R>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Quantizer<R,Q>> Simulation_chain_BFER_std<B,R,Q>
::build_quantizer()
{
	return std::unique_ptr<module::Quantizer<R,Q>>(params_BFER_std.qnt->build<R,Q>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,Q>> Simulation_chain_BFER_std<B,R,Q>
::build_coset_real()
{
	factory::Coset cst_params;
	cst_params.size = params_BFER_std.cdc->N_cw;
	cst_params.n_frames = params_BFER_std.src->n_frames;
	return std::unique_ptr<module::Coset<B,Q>>(cst_params.build_real<B,Q>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,B>> Simulation_chain_BFER_std<B,R,Q>
::build_coset_bit()
{
	factory::Coset cst_params;
	cst_params.size = this->params_BFER_std.coded_monitoring ? params_BFER_std.cdc->N_cw : params_BFER_std.cdc->K;
	cst_params.n_frames = params_BFER_std.src->n_frames;
	return std::unique_ptr<module::Coset<B,B>>(cst_params.build_bit<B,B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Monitor_MI<B,R>> Simulation_chain_BFER_std<B,R,Q>
::build_monitor_mi()
{
	return std::unique_ptr<module::Monitor_MI<B,R>>(params_BFER.mnt_mi->build<B,R>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Monitor_BFER<B>> Simulation_chain_BFER_std<B,R,Q>
::build_monitor_er()
{
	bool count_unknown_values = params_BFER.noise->type == "EP";

	auto mnt_tmp = params_BFER.mnt_er->build<B>(count_unknown_values);
	auto mnt = std::unique_ptr<module::Monitor_BFER<B>>(mnt_tmp);
	mnt->activate_err_histogram(params_BFER.mnt_er->err_hist != -1);

	return mnt;
}

template <typename B, typename R, typename Q>
std::vector<std::unique_ptr<tools::Reporter>> Simulation_chain_BFER_std<B,R,Q>
::build_reporters(const tools ::Noise       < > *noise,
	              const module::Monitor_BFER<B> *monitor_er,
	              const module::Monitor_MI<B,R> *monitor_mi)
{
	std::vector<std::unique_ptr<tools::Reporter>> reporters;
	auto reporter_noise = new tools::Reporter_noise<>(*noise, this->params_BFER.ter_sigma);
	reporters.push_back(std::unique_ptr<tools::Reporter_noise<>>(reporter_noise));

	if (params_BFER.mnt_mutinfo)
	{
		auto reporter_MI = new tools::Reporter_MI<B,R>(*monitor_mi);
		reporters.push_back(std::unique_ptr<tools::Reporter_MI<B,R>>(reporter_MI));
	}

	auto reporter_BFER = new tools::Reporter_BFER<B>(*monitor_er);
	reporters.push_back(std::unique_ptr<tools::Reporter_BFER<B>>(reporter_BFER));
	auto reporter_thr = new tools::Reporter_throughput<uint64_t>(*monitor_er);
	reporters.push_back(std::unique_ptr<tools::Reporter_throughput<uint64_t>>(reporter_thr));

	return reporters;
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools::Terminal> Simulation_chain_BFER_std<B,R,Q>
::build_terminal(const std::vector<std::unique_ptr<tools::Reporter>> &reporters)
{
	return std::unique_ptr<tools::Terminal>(params_BFER.ter->build(reporters));
}

template <typename B, typename R, typename Q>
void Simulation_chain_BFER_std<B,R,Q>
::create_modules()
{
	this->source     = this->build_source    (                                                    );
	this->crc        = this->build_crc       (                                                    );
	this->codec      = this->build_codec     (this->crc.get()                                     );
	this->modem      = this->build_modem     (this->distributions.get(), this->constellation.get());
	this->channel    = this->build_channel   (this->distributions.get()                           );
	this->quantizer  = this->build_quantizer (                                                    );
	this->coset_real = this->build_coset_real(                                                    );
	this->coset_bit  = this->build_coset_bit (                                                    );
	this->monitor_er = this->build_monitor_er(                                                    );
	this->monitor_mi = this->build_monitor_mi(                                                    );
}

template <typename B, typename R, typename Q>
void Simulation_chain_BFER_std<B,R,Q>
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

	std::vector<module::Module*> modules = {&src, &crc, &enc, &pct, &mdm, &chn, &qnt, &csr, &dec, &csb, &mnt, &mni};
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

		mdm[mdm::sck::modulate::X_N1].bind(pct[pct::sck::puncture::X_N2]);
		mdm[mdm::tsk::modulate].exec();
		mdm[mdm::tsk::modulate].reset();
	}
	else
	{
		if (this->params_BFER_std.crc->type != "NO")
			crc[crc::sck::build::U_K1].bind(src[src::sck::generate::U_K]);

		if (this->params_BFER_std.cdc->enc->type != "NO")
		{
			if (this->params_BFER_std.crc->type != "NO")
				enc[enc::sck::encode::U_K].bind(crc[crc::sck::build::U_K2]);
			else
				enc[enc::sck::encode::U_K].bind(src[src::sck::generate::U_K]);
		}

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
		{
			if (this->params_BFER_std.cdc->enc->type != "NO")
				pct[pct::sck::puncture::X_N1].bind(enc[enc::sck::encode::X_N]);
			else if (this->params_BFER_std.crc->type != "NO")
				pct[pct::sck::puncture::X_N1].bind(crc[crc::sck::build::U_K2]);
			else
				pct[pct::sck::puncture::X_N1].bind(src[src::sck::generate::U_K]);
		}

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
			mdm[mdm::sck::modulate::X_N1].bind(pct[pct::sck::puncture::X_N2]);
		else if (this->params_BFER_std.cdc->enc->type != "NO")
			mdm[mdm::sck::modulate::X_N1].bind(enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_std.crc->type != "NO")
			mdm[mdm::sck::modulate::X_N1].bind(crc[crc::sck::build::U_K2]);
		else
			mdm[mdm::sck::modulate::X_N1].bind(src[src::sck::generate::U_K]);
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
			chn[chn::sck::add_noise_wg::X_N].bind(mdm[mdm::sck::modulate::X_N2]);
		}

		if (mdm.is_filter())
		{
			if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::filter::Y_N1].bind(chn[chn::sck::add_noise_wg::Y_N]);
			else
				mdm[mdm::sck::filter::Y_N1].bind(mdm[mdm::sck::modulate::X_N2]);
		}

		if (mdm.is_demodulator())
		{
			if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::demodulate_wg::H_N].bind(chn[chn::sck::add_noise_wg::H_N]);
			else
				mdm[mdm::sck::demodulate_wg::H_N].bind((uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr()));

			if (mdm.is_filter())
				mdm[mdm::sck::demodulate_wg::Y_N1](mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::demodulate_wg::Y_N1].bind(chn[chn::sck::add_noise_wg::Y_N]);
			else
				mdm[mdm::sck::demodulate_wg::Y_N1].bind(mdm[mdm::sck::modulate::X_N2]);
		}

		if (this->params_BFER_std.qnt->type != "NO")
		{
			if (mdm.is_demodulator())
				qnt[qnt::sck::process::Y_N1].bind(mdm[mdm::sck::demodulate_wg::Y_N2]);
			else if (mdm.is_filter())
				qnt[qnt::sck::process::Y_N1].bind(mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
				qnt[qnt::sck::process::Y_N1].bind(chn[chn::sck::add_noise_wg::Y_N]);
			else
				qnt[qnt::sck::process::Y_N1].bind(mdm[mdm::sck::modulate::X_N2]);
		}
	}
	else if (is_optical)
	{
		chn[chn::sck::add_noise    ::X_N ].bind(mdm[mdm::sck::modulate     ::X_N2]);
		mdm[mdm::sck::demodulate_wg::H_N ].bind(mdm[mdm::sck::modulate     ::X_N2]);
		mdm[mdm::sck::demodulate_wg::Y_N1].bind(chn[chn::sck::add_noise    ::Y_N ]);
		qnt[qnt::sck::process      ::Y_N1].bind(mdm[mdm::sck::demodulate_wg::Y_N2]);
	}
	else
	{
		if (this->params_BFER_std.chn->type != "NO")
			chn[chn::sck::add_noise::X_N].bind(mdm[mdm::sck::modulate::X_N2]);

		if (mdm.is_filter())
		{
			if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::filter::Y_N1].bind(chn[chn::sck::add_noise::Y_N]);
			else
				mdm[mdm::sck::filter::Y_N1].bind(mdm[mdm::sck::modulate::X_N2]);
		}

		if (mdm.is_demodulator())
		{
			if (mdm.is_filter())
				mdm[mdm::sck::demodulate::Y_N1].bind(mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
				mdm[mdm::sck::demodulate::Y_N1].bind(chn[chn::sck::add_noise::Y_N]);
			else
				mdm[mdm::sck::demodulate::Y_N1].bind(mdm[mdm::sck::modulate::X_N2]);
		}

		if (this->params_BFER_std.qnt->type != "NO")
		{
			if (mdm.is_demodulator())
				qnt[qnt::sck::process::Y_N1].bind(mdm[mdm::sck::demodulate::Y_N2]);
			else if (mdm.is_filter())
				qnt[qnt::sck::process::Y_N1].bind(mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
				qnt[qnt::sck::process::Y_N1].bind(chn[chn::sck::add_noise::Y_N]);
			else
				qnt[qnt::sck::process::Y_N1].bind(mdm[mdm::sck::modulate::X_N2]);
		}
	}

	if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
	{
		if (this->params_BFER_std.qnt->type != "NO")
			pct[pct::sck::depuncture::Y_N1].bind(qnt[qnt::sck::process::Y_N2]);
		else if (mdm.is_demodulator())
		{
			if (is_rayleigh || is_optical)
				pct[pct::sck::depuncture::Y_N1].bind(mdm[mdm::sck::demodulate_wg::Y_N2]);
			else
				pct[pct::sck::depuncture::Y_N1].bind(mdm[mdm::sck::demodulate::Y_N2]);
		}
		else if (mdm.is_filter())
			pct[pct::sck::depuncture::Y_N1].bind(mdm[mdm::sck::filter::Y_N2]);
		else if (this->params_BFER_std.chn->type != "NO")
		{
			if (is_rayleigh)
				pct[pct::sck::depuncture::Y_N1].bind(chn[chn::sck::add_noise_wg::Y_N]);
			else
				pct[pct::sck::depuncture::Y_N1].bind(chn[chn::sck::add_noise::Y_N]);
		}
		else
			pct[pct::sck::depuncture::Y_N1].bind(mdm[mdm::sck::modulate::X_N2]);
	}

	if (this->params_BFER_std.coset)
	{
		if (this->params_BFER_std.cdc->enc->type != "NO")
			csr[cst::sck::apply::ref].bind(enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_std.crc->type != "NO")
			csr[cst::sck::apply::ref].bind(crc[crc::sck::build::U_K2]);
		else
			csr[cst::sck::apply::ref].bind(src[src::sck::generate::U_K]);

		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
			csr[cst::sck::apply::in].bind(pct[pct::sck::depuncture::Y_N2]);
		else if (this->params_BFER_std.qnt->type != "NO")
			csr[cst::sck::apply::in].bind(qnt[qnt::sck::process::Y_N2]);
		else if (mdm.is_demodulator())
		{
			if (is_rayleigh || is_optical)
				csr[cst::sck::apply::in].bind(mdm[mdm::sck::demodulate_wg::Y_N2]);
			else
				csr[cst::sck::apply::in].bind(mdm[mdm::sck::demodulate::Y_N2]);
		}
		else if (mdm.is_filter())
			csr[cst::sck::apply::in].bind(mdm[mdm::sck::filter::Y_N2]);
		else if (this->params_BFER_std.chn->type != "NO")
		{
			if (is_rayleigh)
				csr[cst::sck::apply::in].bind(chn[chn::sck::add_noise_wg::Y_N]);
			else
				csr[cst::sck::apply::in].bind(chn[chn::sck::add_noise::Y_N]);
		}
		else
			csr[cst::sck::apply::in].bind(mdm[mdm::sck::modulate::X_N2]);

		if (this->params_BFER_std.coded_monitoring)
		{
			dec[dec::sck::decode_siho_cw::Y_N].bind(csr[cst::sck::apply::out]);

			if (this->params_BFER_std.cdc->enc->type != "NO")
				csb[cst::sck::apply::ref].bind(enc[enc::sck::encode::X_N]);
			else if (this->params_BFER_std.crc->type != "NO")
				csb[cst::sck::apply::ref].bind(crc[crc::sck::build::U_K2]);
			else
				csb[cst::sck::apply::ref].bind(src[src::sck::generate::U_K]);

			csb[cst::sck::apply::in].bind(dec[dec::sck::decode_siho_cw::V_N]);
		}
		else
		{
			dec[dec::sck::decode_siho::Y_N].bind(csr[cst::sck::apply::out]);

			if (this->params_BFER_std.crc->type != "NO")
				csb[cst::sck::apply::ref].bind(crc[crc::sck::build::U_K2]);
			else
				csb[cst::sck::apply::ref].bind(src[src::sck::generate::U_K]);

			csb[cst::sck::apply::in].bind(dec[dec::sck::decode_siho::V_K]);

			if (this->params_BFER_std.crc->type != "NO")
				crc[crc::sck::extract::V_K1].bind(csb[cst::sck::apply::out]);
		}
	}
	else
	{
		if (this->params_BFER_std.coded_monitoring)
		{
			if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
				dec[dec::sck::decode_siho_cw::Y_N].bind(pct[pct::sck::depuncture::Y_N2]);
			else if (this->params_BFER_std.qnt->type != "NO")
				dec[dec::sck::decode_siho_cw::Y_N].bind(qnt[qnt::sck::process::Y_N2]);
			else if (mdm.is_demodulator())
			{
				if (is_rayleigh || is_optical)
					dec[dec::sck::decode_siho_cw::Y_N].bind(mdm[mdm::sck::demodulate_wg::Y_N2]);
				else
					dec[dec::sck::decode_siho_cw::Y_N].bind(mdm[mdm::sck::demodulate::Y_N2]);
			}
			else if (mdm.is_filter())
				dec[dec::sck::decode_siho_cw::Y_N].bind(mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
			{
				if (is_rayleigh)
					dec[dec::sck::decode_siho_cw::Y_N].bind(chn[chn::sck::add_noise_wg::Y_N]);
				else
					dec[dec::sck::decode_siho_cw::Y_N].bind(chn[chn::sck::add_noise::Y_N]);
			}
			else
				dec[dec::sck::decode_siho_cw::Y_N].bind(mdm[mdm::sck::modulate::X_N2]);
		}
		else
		{
			if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
				dec[dec::sck::decode_siho::Y_N].bind(pct[pct::sck::depuncture::Y_N2]);
			else if (this->params_BFER_std.qnt->type != "NO")
				dec[dec::sck::decode_siho::Y_N].bind(qnt[qnt::sck::process::Y_N2]);
			else if (mdm.is_demodulator())
			{
				if (is_rayleigh || is_optical)
					dec[dec::sck::decode_siho::Y_N].bind(mdm[mdm::sck::demodulate_wg::Y_N2]);
				else
					dec[dec::sck::decode_siho::Y_N].bind(mdm[mdm::sck::demodulate::Y_N2]);
			}
			else if (mdm.is_filter())
				dec[dec::sck::decode_siho::Y_N].bind(mdm[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_std.chn->type != "NO")
			{
				if (is_rayleigh)
					dec[dec::sck::decode_siho::Y_N].bind(chn[chn::sck::add_noise_wg::Y_N]);
				else
					dec[dec::sck::decode_siho::Y_N].bind(chn[chn::sck::add_noise::Y_N]);
			}
			else
				dec[dec::sck::decode_siho::Y_N].bind(mdm[mdm::sck::modulate::X_N2]);

			if (this->params_BFER_std.crc->type != "NO")
				crc[crc::sck::extract::V_K1].bind(dec[dec::sck::decode_siho::V_K]);
		}
	}

	if (this->params_BFER_std.coded_monitoring)
	{
		if (this->params_BFER_std.cdc->enc->type != "NO")
			mnt[mnt::sck::check_errors::U].bind(enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_std.crc->type != "NO")
			mnt[mnt::sck::check_errors::U].bind(crc[crc::sck::build::U_K2]);
		else
			mnt[mnt::sck::check_errors::U].bind(src[src::sck::generate::U_K]);

		if (this->params_BFER_std.coset)
			mnt[mnt::sck::check_errors::V].bind(csb[cst::sck::apply::out]);
		else
			mnt[mnt::sck::check_errors::V].bind(dec[dec::sck::decode_siho_cw::V_N]);
	}
	else
	{
		mnt[mnt::sck::check_errors::U].bind(src[src::sck::generate::U_K]);
		if (this->params_BFER_std.crc->type != "NO")
			mnt[mnt::sck::check_errors::V].bind(crc[crc::sck::extract::V_K2]);
		else if (this->params_BFER_std.coset)
			mnt[mnt::sck::check_errors::V].bind(csb[cst::sck::apply::out]);
		else
			mnt[mnt::sck::check_errors::V].bind(dec[dec::sck::decode_siho::V_K]);
	}

	if (this->params_BFER_std.mnt_mutinfo)
	{
		if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
			mni[mnt::sck::get_mutual_info::X].bind(pct[pct::sck::puncture::X_N2]);
		else if (this->params_BFER_std.cdc->enc->type != "NO")
			mni[mnt::sck::get_mutual_info::X].bind(enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_std.crc->type != "NO")
			mni[mnt::sck::get_mutual_info::X].bind(crc[crc::sck::build::U_K2]);
		else
			mni[mnt::sck::get_mutual_info::X].bind(src[src::sck::generate::U_K]);

		if (mdm.is_demodulator())
		{
			if (is_rayleigh || is_optical)
				mni[mnt::sck::get_mutual_info::Y].bind(mdm[mdm::sck::demodulate_wg::Y_N2]);
			else
				mni[mnt::sck::get_mutual_info::Y].bind(mdm[mdm::sck::demodulate::Y_N2]);
		}
		else if (mdm.is_filter())
			mni[mnt::sck::get_mutual_info::Y].bind(mdm[mdm::sck::filter::Y_N2]);
		else if (this->params_BFER_std.chn->type != "NO")
			mni[mnt::sck::get_mutual_info::Y].bind(chn[chn::sck::add_noise_wg::Y_N]);
		else
			mni[mnt::sck::get_mutual_info::Y].bind(mdm[mdm::sck::modulate::X_N2]);
	}
}

template <typename B, typename R, typename Q>
void Simulation_chain_BFER_std<B,R,Q>
::create_chain()
{
	const auto is_rayleigh = this->params_BFER_std.chn->type.find("RAYLEIGH") != std::string::npos;
	const auto is_optical = this->params_BFER_std.chn->type == "OPTICAL" && this->params_BFER_std.mdm->rop_est_bits > 0;
	const auto t = this->params_BFER.n_threads;
	if (this->params_BFER_std.src->type != "AZCW")
		this->chain.reset(new tools::Chain((*this->source)[module::src::tsk::generate], t));
	else if (this->params_BFER_std.chn->type != "NO")
	{
		if (is_rayleigh)
			this->chain.reset(new tools::Chain((*this->channel)[module::chn::tsk::add_noise_wg], t));
		else
			this->chain.reset(new tools::Chain((*this->channel)[module::chn::tsk::add_noise], t));
	}
	else if (this->modem->is_demodulator())
	{
		if (is_rayleigh || is_optical)
			this->chain.reset(new tools::Chain((*this->modem)[module::mdm::tsk::demodulate_wg], t));
		else
			this->chain.reset(new tools::Chain((*this->modem)[module::mdm::tsk::demodulate], t));
	}
	else if (this->modem->is_filter())
		this->chain.reset(new tools::Chain((*this->modem)[module::mdm::tsk::filter], t));
	else if (this->params_BFER_std.qnt->type != "NO")
		this->chain.reset(new tools::Chain((*this->quantizer)[module::qnt::tsk::process], t));
	else if (this->params_BFER_std.cdc->pct != nullptr && this->params_BFER_std.cdc->pct->type != "NO")
		this->chain.reset(new tools::Chain(this->codec->get_puncturer()[module::pct::tsk::puncture], t));
	else if (this->params_BFER_std.coset)
		this->chain.reset(new tools::Chain((*this->coset_real)[module::cst::tsk::apply], t));
	else
		this->chain.reset(new tools::Chain(this->codec->get_decoder_siho()[module::dec::tsk::decode_siho], t));

	// set the noise
	this->codec->set_noise(*this->noise);
	for (auto &m : chain->get_modules<tools::Interface_get_set_noise>())
		m->set_noise(*this->noise);

	// registering to noise updates
	this->noise->record_callback_update([this](){ this->codec->notify_noise_update(); });
	for (auto &m : chain->get_modules<tools::Interface_notify_noise_update>())
		this->noise->record_callback_update([m](){ m->notify_noise_update(); });

	// set different seeds in the modules that uses PRNG
	std::mt19937 prng(params_BFER_std.local_seed);
	for (auto &m : chain->get_modules<tools::Interface_set_seed>())
		m->set_seed(prng());

	const auto seed_itl = params_BFER_std.cdc->itl != nullptr ? params_BFER_std.cdc->itl->core->seed +
	                      (params_BFER_std.cdc->itl->core->uniform ? prng() : 0) : 0;
	bool is_interleaver = true;
	try { codec->get_interleaver().set_seed(seed_itl); } catch (...) { is_interleaver = false; }
	if (is_interleaver && codec->get_interleaver().is_uniform())
	{
		std::stringstream message;
		message << "Please use the legacy simulator engine for uniform interleaving (remove the '--sim-chain' "
		        << "argument from the command line).";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->params_BFER_std.err_track_enable)
	{
		auto sources = chain->get_modules<module::Source<B>>();
		for (size_t tid = 0; tid < (size_t)params_BFER.n_threads; tid++)
		{
			auto &source  = sources.size() ? *sources[tid] : *this->source;
			auto src_data = (B*)(source[module::src::sck::generate::U_K].get_dataptr());
			auto src_bytes = source[module::src::sck::generate::U_K].get_databytes();
			auto src_size = (src_bytes / sizeof(B)) / this->params_BFER_std.src->n_frames;
			this->dumper[tid]->register_data(src_data,
			                                 (unsigned int)src_size,
			                                 this->params_BFER_std.err_track_threshold,
			                                 "src",
			                                 false,
			                                 this->params_BFER_std.src->n_frames,
			                                 {});
		}

		auto encoders = chain->get_modules<module::Encoder<B>>();
		for (size_t tid = 0; tid < (size_t)params_BFER.n_threads; tid++)
		{
			auto &encoder = encoders.size() ? *encoders[tid] : this->codec->get_encoder();
			auto enc_data = (B*)(encoder[module::enc::sck::encode::X_N].get_dataptr());
			auto enc_bytes = encoder[module::enc::sck::encode::X_N].get_databytes();
			auto enc_size = (enc_bytes / sizeof(B)) / this->params_BFER_std.src->n_frames;
			this->dumper[tid]->register_data(enc_data,
			                                 (unsigned int)enc_size,
			                                 this->params_BFER_std.err_track_threshold,
			                                 "enc",
			                                 false,
			                                 this->params_BFER_std.src->n_frames,
			                                 {(unsigned)this->params_BFER_std.cdc->enc->K});
		}

		auto channels = chain->get_modules<module::Channel<R>>();
		for (size_t tid = 0; tid < (size_t)params_BFER.n_threads; tid++)
		{
			auto &channel = channels.size() ? *channels[tid] : *this->channel;
			this->dumper[tid]->register_data(channel.get_noised_data(),
			                                 this->params_BFER_std.err_track_threshold,
			                                 "chn",
			                                 true,
			                                 this->params_BFER_std.src->n_frames,
			                                 {});
		}

		auto monitors_er = chain->get_modules<module::Monitor_BFER<B>>();
		for (size_t tid = 0; tid < (size_t)params_BFER.n_threads; tid++)
		{
			monitors_er[tid]->record_callback_fe(std::bind(&tools::Dumper::add,
			                                               this->dumper[tid].get(),
			                                               std::placeholders::_1,
			                                               std::placeholders::_2));
		}
	}
}

template <typename B, typename R, typename Q>
void Simulation_chain_BFER_std<B,R,Q>
::configure_chain_tasks()
{
	for (auto &mod : chain->get_modules<module::Module>())
		for (auto &tsk : mod->tasks)
		{
			if (this->params.statistics)
				tsk->set_stats(true);
			// enable the debug mode in the modules
			if (this->params.debug)
			{
				tsk->set_debug(true);
				tsk->set_debug_hex(this->params.debug_hex);
				if (this->params.debug_limit)
					tsk->set_debug_limit((uint32_t)this->params.debug_limit);
				if (this->params.debug_precision)
					tsk->set_debug_precision((uint8_t)this->params.debug_precision);
				if (params.debug_frame_max)
					tsk->set_debug_frame_max((uint32_t)this->params.debug_frame_max);
			}
			if (!tsk->is_stats() && !tsk->is_debug())
				tsk->set_fast(true);
		}
}

template <typename B, typename R, typename Q>
void Simulation_chain_BFER_std<B,R,Q>
::create_monitors_reduction()
{
	auto monitors_bfer = chain->get_modules<module::Monitor_BFER<B>>();
#ifdef AFF3CT_MPI
	this->monitor_er_red.reset(new tools::Monitor_reduction_MPI<module::Monitor_BFER<B>>(monitors_bfer));
#else
	this->monitor_er_red.reset(new tools::Monitor_reduction<module::Monitor_BFER<B>>(monitors_bfer));
#endif

	if (params_BFER.mnt_mutinfo)
	{
		auto monitors_mi = chain->get_modules<module::Monitor_MI<B,R>>();
#ifdef AFF3CT_MPI
		this->monitor_mi_red.reset(new tools::Monitor_reduction_MPI<module::Monitor_MI<B,R>>(monitors_mi));
#else
		this->monitor_mi_red.reset(new tools::Monitor_reduction<module::Monitor_MI<B,R>>(monitors_mi));
#endif
	}

	tools::Monitor_reduction_static::set_master_thread_id(std::this_thread::get_id());
#ifdef AFF3CT_MPI
	tools::Monitor_reduction_static::set_reduce_frequency(params_BFER.mnt_mpi_comm_freq);
#else
	auto freq = std::chrono::milliseconds(0);
	if (params_BFER.mnt_red_lazy)
	{
		if (params_BFER.mnt_red_lazy_freq.count())
			freq = params_BFER.mnt_red_lazy_freq;
		else
			freq = std::chrono::milliseconds(1000); // default value when lazy reduction and no terminal refresh
	}
	tools::Monitor_reduction_static::set_reduce_frequency(freq);
#endif
	tools::Monitor_reduction_static::reset_all();
	tools::Monitor_reduction_static::check_reducible();
}

template <typename B, typename R, typename Q>
void Simulation_chain_BFER_std<B,R,Q>
::launch()
{
	if (!params_BFER.err_track_revert)
	{
		this->create_modules();
		this->bind_sockets();
		this->create_chain();
		this->configure_chain_tasks();
		this->create_monitors_reduction();

		this->reporters = this->build_reporters(this->noise.get(),
		                                        this->monitor_er_red.get(),
		                                        this->monitor_mi_red.get());
		this->terminal = this->build_terminal(this->reporters);

		if (tools::Terminal::is_over())
			return;
	}

	int noise_begin = 0;
	int noise_end   = (int)params_BFER.noise->range.size();
	int noise_step  = 1;
	if (params_BFER.noise->type == "EP")
	{
		noise_begin = (int)params_BFER.noise->range.size()-1;
		noise_end   = -1;
		noise_step  = -1;
	}

	// for each NOISE to be simulated
	for (auto noise_idx = noise_begin; noise_idx != noise_end; noise_idx += noise_step)
	{
		auto bit_rate = (float)params_BFER.src->K / (float)params_BFER.cdc->N;
		params_BFER.noise->template update<>(*this->noise,
		                                     params_BFER.noise->range[noise_idx],
		                                     bit_rate,
		                                     params_BFER.mdm->bps,
		                                     params_BFER.mdm->cpm_upf);

		if (params_BFER.err_track_revert)
		{
			// dirty hack to override simulation params_BFER
			auto &params_BFER_writable = const_cast<factory::BFER&>(params_BFER);

			std::stringstream s_noise;
			s_noise << std::setprecision(2) << std::fixed << this->noise->get_value();

			params_BFER_writable.src     ->path = params_BFER.err_track_path + "_" + s_noise.str() + ".src";
			params_BFER_writable.cdc->enc->path = params_BFER.err_track_path + "_" + s_noise.str() + ".enc";
			params_BFER_writable.chn     ->path = params_BFER.err_track_path + "_" + s_noise.str() + ".chn";

			std::ifstream file(params_BFER.chn->path, std::ios::binary);
			if (file.is_open())
			{
				unsigned max_fra;
				file.read((char*)&max_fra, sizeof(max_fra));
				file.close();

				*const_cast<unsigned*>(&params_BFER.max_frame) = max_fra;
			}
			else
			{
				std::stringstream message;
				message << "Impossible to read the 'chn' file ('chn' = " << params_BFER.chn->path << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			this->noise->clear_callbacks_changed();

			this->create_modules();
			this->bind_sockets();
			this->create_chain();
			this->configure_chain_tasks();
			this->create_monitors_reduction();

			this->reporters = this->build_reporters(this->noise.get(),
			                                        this->monitor_er_red.get(),
			                                        this->monitor_mi_red.get());
			this->terminal = this->build_terminal(this->reporters);

			if (tools::Terminal::is_over())
				return;
		}

#ifdef AFF3CT_MPI
		if (params_BFER.mpi_rank == 0)
#endif
		if (params_BFER.display_legend)
			if ((!params_BFER.ter->disabled && noise_idx == noise_begin && !params_BFER.debug)
				|| (params_BFER.statistics && !params_BFER.debug))
				terminal->legend(std::cout);

#ifdef AFF3CT_MPI
		if (params_BFER.mpi_rank == 0)
#endif
		// start the terminal to display BER/FER results
		if (!params_BFER.ter->disabled && params_BFER.ter->frequency != std::chrono::nanoseconds(0) &&
		    !params_BFER.debug)
			terminal->start_temp_report(params_BFER.ter->frequency);

		this->t_start_noise_point = std::chrono::steady_clock::now();

		try
		{
			this->chain->exec([this]() { return this->stop_condition(); } );
			tools::Monitor_reduction_static::last_reduce_all(); // final reduction
		}
		catch (std::exception const& e)
		{
			tools::Monitor_reduction_static::last_reduce_all(); // final reduction

			terminal->final_report(std::cout); // display final report to not lost last line overwritten by the error
			                                   // messages
			rang::format_on_each_line(std::cerr, std::string(e.what()) + "\n", rang::tag::error);
			this->simu_error = true;

			tools::Terminal::stop();
		}

#ifdef AFF3CT_MPI
		if (params_BFER.mpi_rank == 0)
#endif
		if (!params_BFER.ter->disabled && terminal != nullptr && !this->simu_error)
		{
			if (params_BFER.debug)
				terminal->legend(std::cout);

			terminal->final_report(std::cout);

			if (params_BFER.statistics)
			{
				std::cout << "#" << std::endl;
				tools::Stats::show(this->chain->get_modules_per_types(), true, std::cout);
				std::cout << "#" << std::endl;
			}
		}

		if (!params_BFER.crit_nostop && !params_BFER.err_track_revert && !tools::Terminal::is_interrupt() &&
		    !this->monitor_er_red->fe_limit_achieved() &&
		    (this->monitor_er_red->frame_limit_achieved() || this->stop_time_reached()))
			tools::Terminal::stop();

		if (params_BFER.mnt_er->err_hist != -1)
		{
			auto err_hist = monitor_er_red->get_err_hist();

			if (err_hist.get_n_values() != 0)
			{
				std::string noise_value;
				switch (this->noise->get_type())
				{
					case tools::Noise_type::SIGMA:
						if (params_BFER.noise->type == "EBN0")
							noise_value = std::to_string(dynamic_cast<tools::Sigma<>*>(this->noise.get())->get_ebn0());
						else //(params_BFER.noise_type == "ESN0")
							noise_value = std::to_string(dynamic_cast<tools::Sigma<>*>(this->noise.get())->get_esn0());
						break;
					case tools::Noise_type::ROP:
					case tools::Noise_type::EP:
						noise_value = std::to_string(this->noise->get_value());
						break;
				}

				std::ofstream file_err_hist(params_BFER.mnt_er->err_hist_path + "_" + noise_value + ".txt");
				file_err_hist << "\"Number of error bits per wrong frame\"; \"Histogram (noise: " << noise_value
				              << this->noise->get_unity() << ", on " << err_hist.get_n_values() << " frames)\""
				              << std::endl;

				int max;
				if (params_BFER.mnt_er->err_hist == 0)
					max = err_hist.get_hist_max();
				else
					max = params_BFER.mnt_er->err_hist;
				err_hist.dump(file_err_hist, 0, max);
			}
		}

		if (this->dumper_red != nullptr && !this->simu_error)
		{
			std::stringstream s_noise;
			s_noise << std::setprecision(2) << std::fixed << this->noise->get_value();

			this->dumper_red->dump(params_BFER.err_track_path + "_" + s_noise.str());
			this->dumper_red->clear();
		}

		if (tools::Terminal::is_over())
			break;

		for (auto &mod : chain->get_modules<module::Module>())
			for (auto &tsk : mod->tasks)
				tsk->reset();

		tools::Monitor_reduction_static::reset_all();
		tools::Terminal::reset();
	}
}

template <typename B, typename R, typename Q>
bool Simulation_chain_BFER_std<B,R,Q>
::stop_time_reached()
{
	return this->params_BFER.stop_time != std::chrono::seconds(0) &&
	       (std::chrono::steady_clock::now() - this->t_start_noise_point) >= this->params_BFER.stop_time;
}

template <typename B, typename R, typename Q>
bool Simulation_chain_BFER_std<B,R,Q>
::stop_condition()
{
	return tools::Terminal::is_interrupt() || tools::Monitor_reduction_static::is_done_all() || stop_time_reached();
}

}
}
