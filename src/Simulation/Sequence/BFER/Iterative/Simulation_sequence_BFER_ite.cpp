#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <string>

#include "Tools/Algo/Predicate_ite.hpp"
#include "Tools/Sequence/Sequence.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Interface/Interface_set_seed.hpp"
#include "Tools/Interface/Interface_get_set_noise.hpp"
#include "Tools/Interface/Interface_notify_noise_update.hpp"
#include "Tools/Display/Dumper/Dumper.hpp"
#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Coset/Coset.hpp"
#include "Factory/Tools/Codec/Codec.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"
#include "Simulation/Sequence/BFER/Iterative/Simulation_sequence_BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_sequence_BFER_ite<B,R,Q>
::Simulation_sequence_BFER_ite(const factory::BFER_ite &params_BFER_ite)
: Simulation_sequence_BFER<B,R>(params_BFER_ite),
  params_BFER_ite(params_BFER_ite)
{
	if (this->params_BFER_ite.err_track_revert && this->params_BFER_ite.n_threads != 1)
		std::clog << rang::tag::warning << "Multi-threading detected with error tracking revert feature! "
		                                   "Each thread will play the same frames. Please run with one thread."
		          << std::endl;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Source<B>> Simulation_sequence_BFER_ite<B,R,Q>
::build_source()
{
	auto src = std::unique_ptr<module::Source<B>>(params_BFER_ite.src->build<B>());
	src->set_n_frames(this->params.n_frames);
	return src;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::CRC<B>> Simulation_sequence_BFER_ite<B,R,Q>
::build_crc()
{
	auto crc = std::unique_ptr<module::CRC<B>>(params_BFER_ite.crc->build<B>());
	crc->set_n_frames(this->params.n_frames);
	return crc;
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools::Codec_SISO<B,Q>> Simulation_sequence_BFER_ite<B,R,Q>
::build_codec(const module::CRC<B> *crc)
{
	auto crc_ptr = this->params_BFER_ite.crc->type == "NO" ? nullptr : crc;
	std::unique_ptr<factory::Codec> params_cdc(params_BFER_ite.cdc->clone());
	auto param_siso_siho = dynamic_cast<factory::Codec_SISO*>(params_cdc.get());
	auto cdc = std::unique_ptr<tools::Codec_SISO<B,Q>>(param_siso_siho->template build<B,Q>(crc_ptr));
	cdc->set_n_frames(this->params.n_frames);
	return cdc;
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools ::Interleaver_core<>> Simulation_sequence_BFER_ite<B,R,Q>
::build_interleaver()
{
	std::unique_ptr<factory::Interleaver> params_itl(params_BFER_ite.itl->clone());
	auto itl = std::unique_ptr<tools::Interleaver_core<>>(params_itl->core->build<>());
	itl->set_n_frames(this->params.n_frames);
	return itl;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Modem<B,R,Q>> Simulation_sequence_BFER_ite<B,R,Q>
::build_modem(const tools::Distributions<R> *distributions, const tools::Constellation<R> *constellation)
{
	if (distributions != nullptr)
	{
		auto mdm = std::unique_ptr<module::Modem<B,R,Q>>(params_BFER_ite.mdm->build<B,R,Q>(*distributions));
		mdm->set_n_frames(this->params.n_frames);
		return mdm;
	}
	else
	{
		auto mdm = std::unique_ptr<module::Modem<B,R,Q>>(params_BFER_ite.mdm->build<B,R,Q>(constellation));
		mdm->set_n_frames(this->params.n_frames);
		return mdm;
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Channel<R>> Simulation_sequence_BFER_ite<B,R,Q>
::build_channel(const tools::Distributions<R> *distributions)
{
	if (distributions != nullptr)
	{
		auto chn = std::unique_ptr<module::Channel<R>>(params_BFER_ite.chn->build<R>(*distributions));
		chn->set_n_frames(this->params.n_frames);
		return chn;
	}
	else
	{
		auto chn = std::unique_ptr<module::Channel<R>>(params_BFER_ite.chn->build<R>());
		chn->set_n_frames(this->params.n_frames);
		return chn;
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Quantizer<R,Q>> Simulation_sequence_BFER_ite<B,R,Q>
::build_quantizer()
{
	auto qnt = std::unique_ptr<module::Quantizer<R,Q>>(params_BFER_ite.qnt->build<R,Q>());
	qnt->set_n_frames(this->params.n_frames);
	return qnt;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,Q>> Simulation_sequence_BFER_ite<B,R,Q>
::build_coset_real()
{
	factory::Coset cst_params;
	cst_params.size = params_BFER_ite.cdc->N_cw;
	auto cst = std::unique_ptr<module::Coset<B,Q>>(cst_params.build_real<B,Q>());
	cst->set_n_frames(this->params.n_frames);
	return cst;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,B>> Simulation_sequence_BFER_ite<B,R,Q>
::build_coset_bit()
{
	factory::Coset cst_params;
	cst_params.size = params_BFER_ite.coded_monitoring ? params_BFER_ite.cdc->N_cw : params_BFER_ite.cdc->K;
	auto cst = std::unique_ptr<module::Coset<B,B>>(cst_params.build_bit<B,B>());
	cst->set_n_frames(this->params.n_frames);
	return cst;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Loop_predicate<Q>> Simulation_sequence_BFER_ite<B,R,Q>
::build_loop_ite()
{
	tools::Predicate_ite p(params_BFER_ite.n_ite);
	auto loop_ite =  std::unique_ptr<module::Loop_predicate<Q>>(new module::Loop_predicate<Q>(
		p,
		params_BFER_ite.cdc->N_cw));
	loop_ite->set_n_frames(this->params.n_frames);
	return loop_ite;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Loop_CRC<B,Q>> Simulation_sequence_BFER_ite<B,R,Q>
::build_loop_crc()
{
	tools::Predicate_ite p(params_BFER_ite.n_ite);
	auto crc = std::unique_ptr<module::CRC<B>>(params_BFER_ite.crc->build<B>());
	auto loop_crc = std::unique_ptr<module::Loop_CRC<B,Q>>(new module::Loop_CRC<B,Q>(
		p,
		*crc.get(),
		(size_t)crc->get_size() + (size_t)crc->get_K(),
		params_BFER_ite.cdc->N_cw,
		params_BFER_ite.crc_start));
	loop_crc->set_n_frames(this->params.n_frames);
	return loop_crc;
}

template <typename B, typename R, typename Q>
void Simulation_sequence_BFER_ite<B,R,Q>
::create_modules()
{
	Simulation_sequence_BFER<B,R>::create_modules();

	this->source           = build_source     (                                                    );
	this->crc              = build_crc        (                                                    );
	this->codec1           = build_codec      (this->crc.get()                                     );
	this->codec2           = build_codec      (this->crc.get()                                     );
	this->modem1           = build_modem      (this->distributions.get(), this->constellation.get());
	this->modem2           = build_modem      (this->distributions.get(), this->constellation.get());
	this->channel          = build_channel    (this->distributions.get()                           );
	this->quantizer        = build_quantizer  (                                                    );
	this->coset_real1      = build_coset_real (                                                    );
	this->coset_real2      = build_coset_real (                                                    );
	this->coset_real3      = build_coset_real (                                                    );
	this->coset_bit        = build_coset_bit  (                                                    );
	this->interleaver_core = build_interleaver(                                                    );
	this->loop_ite         = build_loop_ite   (                                                    );
	this->loop_crc         = build_loop_crc   (                                                    );

	this->interleaver_bit .reset(factory::Interleaver::build<B>(*this->interleaver_core));
	this->interleaver_llr1.reset(factory::Interleaver::build<Q>(*this->interleaver_core));
	this->interleaver_llr2.reset(factory::Interleaver::build<Q>(*this->interleaver_core));
}

template <typename B, typename R, typename Q>
void Simulation_sequence_BFER_ite<B,R,Q>
::bind_sockets()
{
	using namespace module;

	auto &src  = *this->source;
	auto &crc  = *this->crc;
	auto &itb  = *this->interleaver_bit;
	auto &mdm1 = *this->modem1;
	auto &mdm2 = *this->modem2;
	auto &chn  = *this->channel;
	auto &qnt  = *this->quantizer;
	auto &itl1 = *this->interleaver_llr1;
	auto &itl2 = *this->interleaver_llr2;
	auto &csr1 = *this->coset_real1;
	auto &csr2 = *this->coset_real2;
	auto &csr3 = *this->coset_real3;
	auto &csb  = *this->coset_bit;
	auto &mnt  = *this->monitor_er;
	auto &lite = *this->loop_ite;
	auto &lcrc = *this->loop_crc;
	auto &enc  =  this->codec1->get_encoder();
	auto &dcs  =  this->codec1->get_decoder_siso();
	auto &dch  =  this->codec1->get_decoder_siho();
	auto &ext1 =  this->codec1->get_extractor();
	auto &ext2 =  this->codec2->get_extractor();

	std::vector<Module*> modules = { &src, &crc, &itb,  &mdm1, &mdm2, &chn, &qnt, &itl1, &itl2, &csr1, &csr2, &csr3,
	                                 &csb, &mnt, &lite, &lcrc, &enc,  &dcs, &dch, &ext1, &ext2 };
	for (auto& mod : modules)
		for (auto& tsk : mod->tasks)
			tsk->set_autoalloc(true);

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

		mdm1[mdm::sck::modulate::X_N1].bind(itb[itl::sck::interleave::itl]);
		mdm1[mdm::tsk::modulate].exec();
		mdm1[mdm::tsk::modulate].reset();
	}
	else
	{
		if (this->params_BFER_ite.crc->type != "NO")
			crc[crc::sck::build::U_K1].bind(src[src::sck::generate::U_K]);

		if (this->params_BFER_ite.cdc->enc->type != "NO")
		{
			if (this->params_BFER_ite.crc->type != "NO")
				enc[enc::sck::encode::U_K].bind(crc[crc::sck::build::U_K2]);
			else
				enc[enc::sck::encode::U_K].bind(src[src::sck::generate::U_K]);
		}

		if (this->params_BFER_ite.cdc->enc->type != "NO")
			itb[itl::sck::interleave::nat].bind(enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_ite.crc->type != "NO")
			itb[itl::sck::interleave::nat].bind(crc[crc::sck::build::U_K2]);
		else
			itb[itl::sck::interleave::nat].bind(src[src::sck::generate::U_K]);

		mdm1[mdm::sck::modulate::X_N1].bind(itb[itl::sck::interleave::itl]);
	}

	if (this->params_BFER_ite.coset)
	{
		if (this->params_BFER_ite.coded_monitoring)
		{
			if (this->params_BFER_ite.cdc->enc->type != "NO")
				csb[cst::sck::apply::ref].bind(enc[enc::sck::encode::X_N]);
			else if (this->params_BFER_ite.crc->type != "NO")
				csb[cst::sck::apply::ref].bind(crc[crc::sck::build::U_K2]);
			else
				csb[cst::sck::apply::ref].bind(src[src::sck::generate::U_K]);
		}
		else
		{
			if (this->params_BFER_ite.crc->type != "NO")
				csb[cst::sck::apply::ref].bind(crc[crc::sck::build::U_K2]);
			else
				csb[cst::sck::apply::ref].bind(src[src::sck::generate::U_K]);
		}

		if (this->params_BFER_ite.cdc->enc->type != "NO")
		{
			csr1[cst::sck::apply::ref].bind(enc[enc::sck::encode::X_N]);
			csr2[cst::sck::apply::ref].bind(enc[enc::sck::encode::X_N]);
			csr3[cst::sck::apply::ref].bind(enc[enc::sck::encode::X_N]);
		}
		else if (this->params_BFER_ite.crc->type != "NO")
		{
			csr1[cst::sck::apply::ref].bind(crc[crc::sck::build::U_K2]);
			csr2[cst::sck::apply::ref].bind(crc[crc::sck::build::U_K2]);
			csr3[cst::sck::apply::ref].bind(crc[crc::sck::build::U_K2]);
		}
		else
		{
			csr1[cst::sck::apply::ref].bind(src[src::sck::generate::U_K]);
			csr2[cst::sck::apply::ref].bind(src[src::sck::generate::U_K]);
			csr3[cst::sck::apply::ref].bind(src[src::sck::generate::U_K]);
		}
	}

	if (this->params_BFER_ite.coded_monitoring)
	{
		if (this->params_BFER_ite.cdc->enc->type != "NO")
			mnt[mnt::sck::check_errors::U].bind(enc[enc::sck::encode::X_N]);
		else if (this->params_BFER_ite.crc->type != "NO")
			mnt[mnt::sck::check_errors::U].bind(crc[crc::sck::build::U_K2]);
		else
			mnt[mnt::sck::check_errors::U].bind(src[src::sck::generate::U_K]);
	}
	else
		mnt[mnt::sck::check_errors::U].bind(src[src::sck::generate::U_K]);

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
		{
			chn[chn::sck::add_noise_wg::CP ].bind(this->channel_params);
			chn[chn::sck::add_noise_wg::X_N].bind(mdm1[mdm::sck::modulate::X_N2]);
		}
		else
		{
			chn[chn::sck::add_noise::CP ].bind(this->channel_params);
			chn[chn::sck::add_noise::X_N].bind(mdm1[mdm::sck::modulate::X_N2]);
		}
	}

	if (mdm1.is_filter())
	{
		mdm1[mdm::sck::filter::CP].bind(this->channel_params);
		if (this->params_BFER_ite.chn->type != "NO")
		{
			if (is_rayleigh)
				mdm1[mdm::sck::filter::Y_N1].bind(chn[chn::sck::add_noise_wg::Y_N]);
			else
				mdm1[mdm::sck::filter::Y_N1].bind(chn[chn::sck::add_noise::Y_N]);
		}
		else
			mdm1[mdm::sck::filter::Y_N1].bind(mdm1[mdm::sck::modulate::X_N2]);
	}

	if (this->params_BFER_ite.qnt->type != "NO")
	{
		if (mdm1.is_filter())
			qnt[qnt::sck::process::Y_N1].bind(mdm1[mdm::sck::filter::Y_N2]);
		else if (this->params_BFER_ite.chn->type != "NO")
		{
			if (is_rayleigh)
				qnt[qnt::sck::process::Y_N1].bind(chn[chn::sck::add_noise_wg::Y_N]);
			else
				qnt[qnt::sck::process::Y_N1].bind(chn[chn::sck::add_noise::Y_N]);
		}
		else
			qnt[qnt::sck::process::Y_N1].bind(mdm1[mdm::sck::modulate::X_N2]);
	}

	if (mdm1.is_demodulator())
	{
		if (is_rayleigh)
		{
			mdm1[mdm::sck::demodulate_wg::CP].bind(this->channel_params);
			if (this->params_BFER_ite.chn->type != "NO")
				mdm1[mdm::sck::demodulate_wg::H_N].bind(chn[chn::sck::add_noise_wg::H_N]);
			else
				mdm1[mdm::sck::demodulate_wg::H_N].bind((uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr()));
		}
		else
			mdm1[mdm::sck::demodulate::CP].bind(this->channel_params);

		if (this->params_BFER_ite.qnt->type != "NO")
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1].bind(qnt[qnt::sck::process::Y_N2]);
			else
				mdm1[mdm::sck::demodulate::Y_N1].bind(qnt[qnt::sck::process::Y_N2]);
		}
		else if (mdm1.is_filter())
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1].bind(mdm1[mdm::sck::filter::Y_N2]);
			else
				mdm1[mdm::sck::demodulate::Y_N1].bind(mdm1[mdm::sck::filter::Y_N2]);
		}
		else if (this->params_BFER_ite.chn->type != "NO")
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1].bind(chn[chn::sck::add_noise_wg::Y_N]);
			else
				mdm1[mdm::sck::demodulate::Y_N1].bind(chn[chn::sck::add_noise::Y_N]);
		}
		else
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1].bind(mdm1[mdm::sck::modulate::X_N2]);
			else
				mdm1[mdm::sck::demodulate::Y_N1].bind(mdm1[mdm::sck::modulate::X_N2]);
		}
	}

	if (mdm1.is_demodulator())
	{
		if (is_rayleigh)
			itl1[itl::sck::deinterleave::itl].bind(mdm1[mdm::sck::demodulate_wg::Y_N2]);
		else
			itl1[itl::sck::deinterleave::itl].bind(mdm1[mdm::sck::demodulate::Y_N2]);
	}
	else if (this->params_BFER_ite.qnt->type != "NO")
		itl1[itl::sck::deinterleave::itl].bind(qnt[qnt::sck::process::Y_N2]);
	else if (mdm1.is_filter())
		itl1[itl::sck::deinterleave::itl].bind(mdm1[mdm::sck::filter::Y_N2]);
	else if (this->params_BFER_ite.chn->type != "NO")
	{
		if (is_rayleigh)
			itl1[itl::sck::deinterleave::itl].bind(chn[chn::sck::add_noise_wg::Y_N]);
		else
			itl1[itl::sck::deinterleave::itl].bind(chn[chn::sck::add_noise::Y_N]);
	}
	else
		itl1[itl::sck::deinterleave::itl].bind(mdm1[mdm::sck::modulate::X_N2]);

	// ----------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------- turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	if (this->params_BFER_ite.crc->type != "NO")
	{
		ext1[ext::sck::get_sys_bit::Y_N    ].bind(itl1[itl::sck::deinterleave::nat]);
		lcrc[lop::sck::stop       ::in1    ].bind(ext1[ext::sck::get_sys_bit ::V_K]);
		lcrc[lop::sck::stop       ::in_out1].bind(itl1[itl::sck::deinterleave::nat]);
	}
	else
		lite[lop::sck::stop::in_out1].bind(itl1[itl::sck::deinterleave::nat]);

	// ------------------------------------------------------------------------------------------------------- decoding
	if (this->params_BFER_ite.coset)
	{
		if (this->params_BFER_ite.crc->type != "NO")
			csr1[cst::sck::apply::in].bind(lcrc[lop::sck::stop::in_out1]);
		else
			csr1[cst::sck::apply::in].bind(lite[lop::sck::stop::in_out1]);

		dcs [dec::sck::decode_siso::Y_N1].bind(csr1[cst::sck::apply      ::out ]);
		csr2[cst::sck::apply      ::in  ].bind(dcs [dec::sck::decode_siso::Y_N2]);
	}
	else
	{
		if (this->params_BFER_ite.crc->type != "NO")
			dcs[dec::sck::decode_siso::Y_N1].bind(lcrc[lop::sck::stop::in_out1]);
		else
			dcs[dec::sck::decode_siso::Y_N1].bind(lite[lop::sck::stop::in_out1]);
	}

	// --------------------------------------------------------------------------------------------------- interleaving
	if (this->params_BFER_ite.coset)
		itl2[itl::sck::interleave::nat].bind(csr2[cst::sck::apply::out]);
	else
		itl2[itl::sck::interleave::nat].bind(dcs[dec::sck::decode_siso::Y_N2]);

	// --------------------------------------------------------------------------------------------------- demodulation
	if (mdm2.is_demodulator())
	{
		if (is_rayleigh)
		{
			mdm2[mdm::sck::tdemodulate_wg::CP].bind(this->channel_params);
			if (this->params_BFER_ite.chn->type != "NO")
				mdm2[mdm::sck::tdemodulate_wg::H_N].bind(chn[chn::sck::add_noise_wg::H_N]);
			else
				mdm2[mdm::sck::tdemodulate_wg::H_N].bind((uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr()));

			if (this->params_BFER_ite.qnt->type != "NO")
				mdm2[mdm::sck::tdemodulate_wg::Y_N1].bind(qnt[qnt::sck::process::Y_N2]);
			else if (mdm1.is_filter())
				mdm2[mdm::sck::tdemodulate_wg::Y_N1].bind(mdm1[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_ite.chn->type != "NO")
				mdm2[mdm::sck::tdemodulate_wg::Y_N1].bind(chn[chn::sck::add_noise_wg::Y_N]);
			else
				mdm2[mdm::sck::tdemodulate_wg::Y_N1].bind(mdm1[mdm::sck::modulate::X_N2]);

			mdm2[mdm::sck::tdemodulate_wg::Y_N2].bind(itl2[itl::sck::interleave::itl]);
		}
		else
		{
			mdm2[mdm::sck::tdemodulate::CP].bind(this->channel_params);
			if (this->params_BFER_ite.qnt->type != "NO")
				mdm2[mdm::sck::tdemodulate::Y_N1].bind(qnt[qnt::sck::process::Y_N2]);
			else if (mdm1.is_filter())
				mdm2[mdm::sck::tdemodulate::Y_N1].bind(mdm1[mdm::sck::filter::Y_N2]);
			else if (this->params_BFER_ite.chn->type != "NO")
				mdm2[mdm::sck::tdemodulate::Y_N1].bind(chn[chn::sck::add_noise::Y_N]);
			else
				mdm2[mdm::sck::tdemodulate::Y_N1].bind(mdm1[mdm::sck::modulate::X_N2]);

			mdm2[mdm::sck::tdemodulate::Y_N2].bind(itl2[itl::sck::interleave::itl ]);
		}
	}

	// ------------------------------------------------------------------------------------------------- deinterleaving
	if (mdm2.is_demodulator())
	{
		if (is_rayleigh)
			itl2[itl::sck::deinterleave::itl].bind(mdm2[mdm::sck::tdemodulate_wg::Y_N3]);
		else
			itl2[itl::sck::deinterleave::itl].bind(mdm2[mdm::sck::tdemodulate::Y_N3]);
	}
	else if (this->params_BFER_ite.qnt->type != "NO")
		itl2[itl::sck::deinterleave::itl].bind(qnt[qnt::sck::process::Y_N2]);
	else if (mdm1.is_filter())
		itl2[itl::sck::deinterleave::itl].bind(mdm1[mdm::sck::filter::Y_N2]);
	else if (this->params_BFER_ite.chn->type != "NO")
	{
		if (is_rayleigh)
			itl2[itl::sck::deinterleave::itl].bind(chn[chn::sck::add_noise_wg::Y_N]);
		else
			itl2[itl::sck::deinterleave::itl].bind(chn[chn::sck::add_noise::Y_N]);
	}
	else
		itl2[itl::sck::deinterleave::itl].bind(mdm1[mdm::sck::modulate::X_N2]);

	if (this->params_BFER_ite.crc->type != "NO")
	{
		if (this->params_BFER_ite.coset)
			ext2[ext::sck::get_sys_bit::Y_N].bind(dcs[dec::sck::decode_siso::Y_N2]);
		else
			ext2[ext::sck::get_sys_bit::Y_N].bind(itl2[itl::sck::deinterleave::nat]);

		lcrc[lop::sck::stop::in2    ].bind(ext2[ext::sck::get_sys_bit ::V_K]);
		lcrc[lop::sck::stop::in_out2].bind(itl2[itl::sck::deinterleave::nat]);
	}
	else
		lite[lop::sck::stop::in_out2].bind(itl2[itl::sck::deinterleave::nat]);

	// ----------------------------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------- end of turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	if (this->params_BFER_ite.coset)
	{
		if (this->params_BFER_ite.crc->type != "NO")
			csr3[cst::sck::apply::in].bind(lcrc[lop::sck::stop::in_out2]);
		else
			csr3[cst::sck::apply::in].bind(lite[lop::sck::stop::in_out2]);

		if (this->params_BFER_ite.coded_monitoring)
		{
			dch[dec::sck::decode_siho_cw::Y_N].bind(csr3[cst::sck::apply         ::out]);
			csb[cst::sck::apply         ::in ].bind(dch [dec::sck::decode_siho_cw::V_N]);
		}
		else
		{
			dch[dec::sck::decode_siho::Y_N].bind(csr3[cst::sck::apply      ::out]);
			csb[cst::sck::apply      ::in ].bind(dch [dec::sck::decode_siho::V_K]);
			if (this->params_BFER_ite.crc->type != "NO")
				crc[crc::sck::extract::V_K1].bind(csb[cst::sck::apply::out]);
		}
	}
	else
	{
		if (this->params_BFER_ite.coded_monitoring)
		{
			if (this->params_BFER_ite.crc->type != "NO")
				dch[dec::sck::decode_siho_cw::Y_N].bind(lcrc[lop::sck::stop::in_out2]);
			else
				dch[dec::sck::decode_siho_cw::Y_N].bind(lite[lop::sck::stop::in_out2]);
		}
		else
		{
			if (this->params_BFER_ite.crc->type != "NO")
				dch[dec::sck::decode_siho::Y_N].bind(lcrc[lop::sck::stop::in_out2]);
			else
				dch[dec::sck::decode_siho::Y_N].bind(lite[lop::sck::stop::in_out2]);

			if (this->params_BFER_ite.crc->type != "NO")
				crc[crc::sck::extract::V_K1].bind(dch[dec::sck::decode_siho::V_K]);
		}
	}

	if (this->params_BFER_ite.coded_monitoring)
	{
		if (this->params_BFER_ite.coset)
			mnt[mnt::sck::check_errors::V].bind(csb[cst::sck::apply::out]);
		else
			mnt[mnt::sck::check_errors::V].bind(dch[dec::sck::decode_siho_cw::V_N]);
	}
	else
	{
		if (this->params_BFER_ite.crc->type != "NO")
			mnt[mnt::sck::check_errors::V].bind(crc[crc::sck::extract::V_K2]);
		else
		{
			if (this->params_BFER_ite.coset)
				mnt[mnt::sck::check_errors::V].bind(csb[cst::sck::apply::out]);
			else
				mnt[mnt::sck::check_errors::V].bind(dch[dec::sck::decode_siho::V_K]);
		}
	}
}

template <typename B, typename R, typename Q>
void Simulation_sequence_BFER_ite<B,R,Q>
::create_sequence()
{
	const auto is_rayleigh = this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos;
	const auto t = this->params_BFER.n_threads;
	if (this->params_BFER_ite.src->type != "AZCW")
		this->sequence.reset(new tools::Sequence((*this->source)[module::src::tsk::generate], t));
	else if (this->params_BFER_ite.chn->type != "NO")
	{
		if (is_rayleigh)
			this->sequence.reset(new tools::Sequence((*this->channel)[module::chn::tsk::add_noise_wg], t));
		else
			this->sequence.reset(new tools::Sequence((*this->channel)[module::chn::tsk::add_noise], t));
	}
	else if (this->modem1->is_filter())
		this->sequence.reset(new tools::Sequence((*this->modem1)[module::mdm::tsk::filter], t));
	else if (this->params_BFER_ite.qnt->type != "NO")
		this->sequence.reset(new tools::Sequence((*this->quantizer)[module::qnt::tsk::process], t));
	else if (this->modem1->is_demodulator())
	{
		if (is_rayleigh)
			this->sequence.reset(new tools::Sequence((*this->modem1)[module::mdm::tsk::demodulate_wg], t));
		else
			this->sequence.reset(new tools::Sequence((*this->modem1)[module::mdm::tsk::demodulate], t));
	}
	else
		this->sequence.reset(new tools::Sequence((*this->interleaver_llr1)[module::itl::tsk::deinterleave], t));

	// set the noise
	this->codec1->set_noise(*this->noise);
	this->codec2->set_noise(*this->noise);
	for (auto &m : this->sequence->template get_modules<tools::Interface_get_set_noise>())
		m->set_noise(*this->noise);

	// registering to noise updates
	this->noise->record_callback_update([this](){ this->codec1->notify_noise_update(); });
	this->noise->record_callback_update([this](){ this->codec2->notify_noise_update(); });
	for (auto &m : this->sequence->template get_modules<tools::Interface_notify_noise_update>())
		this->noise->record_callback_update([m](){ m->notify_noise_update(); });

	// set different seeds in the modules that uses PRNG
	std::mt19937 prng(params_BFER_ite.local_seed);
	for (auto &m : this->sequence->template get_modules<tools::Interface_set_seed>())
		m->set_seed(prng());

	this->interleaver_core->set_seed(params_BFER_ite.itl->core->seed);
	if (this->interleaver_core->is_uniform())
	{
		std::stringstream message;
		message << "Please use the legacy simulator engine for uniform interleaving (remove the '--sim-sequence' "
		        << "argument from the command line).";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto monitors_er   = this->sequence->template get_modules<module::Monitor_BFER<B  >>();
	auto decoders_siso = this->sequence->template get_modules<module::Decoder_SISO<B,Q>>();
	for (size_t m = 0; m < monitors_er.size(); m++)
		if (m < decoders_siso.size())
		{
			auto cur_decoder_siso = decoders_siso[m];
			cur_decoder_siso->set_auto_reset(false);
			monitors_er[m]->record_callback_check([cur_decoder_siso](){ cur_decoder_siso->reset(); });
		}

	if (this->params_BFER_ite.err_track_enable)
	{
		auto sources = this->sequence->template get_modules<module::Source<B>>();
		for (size_t tid = 0; tid < (size_t)this->params_BFER.n_threads; tid++)
		{
			auto &source  = sources.size() ? *sources[tid] : *this->source;
			auto src_data = (B*)(source[module::src::sck::generate::U_K].get_dataptr());
			auto src_bytes = source[module::src::sck::generate::U_K].get_databytes();
			auto src_size = (src_bytes / sizeof(B)) / this->params_BFER_ite.n_frames;
			this->dumper[tid]->register_data(src_data,
			                                 (unsigned int)src_size,
			                                 this->params_BFER_ite.err_track_threshold,
			                                 "src",
			                                 false,
			                                 this->params_BFER_ite.n_frames,
			                                 {});
		}

		auto encoders = this->sequence->template get_modules<module::Encoder<B>>();
		for (size_t tid = 0; tid < (size_t)this->params_BFER.n_threads; tid++)
		{
			auto &encoder = encoders.size() ? *encoders[tid] : this->codec1->get_encoder();
			auto enc_data = (B*)(encoder[module::enc::sck::encode::X_N].get_dataptr());
			auto enc_bytes = encoder[module::enc::sck::encode::X_N].get_databytes();
			auto enc_size = (enc_bytes / sizeof(B)) / this->params_BFER_ite.n_frames;
			this->dumper[tid]->register_data(enc_data,
			                                 (unsigned int)enc_size,
			                                 this->params_BFER_ite.err_track_threshold,
			                                 "enc",
			                                 false,
			                                 this->params_BFER_ite.n_frames,
			                                 {(unsigned)this->params_BFER_ite.cdc->enc->K});
		}

		auto channels = this->sequence->template get_modules<module::Channel<R>>();
		for (size_t tid = 0; tid < (size_t)this->params_BFER.n_threads; tid++)
		{
			auto &channel = channels.size() ? *channels[tid] : *this->channel;
			this->dumper[tid]->register_data(channel.get_noised_data(),
			                                 this->params_BFER_ite.err_track_threshold,
			                                 "chn",
			                                 true,
			                                 this->params_BFER_ite.n_frames,
			                                 {});
		}

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
template class aff3ct::simulation::Simulation_sequence_BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_sequence_BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_sequence_BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_sequence_BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_sequence_BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
