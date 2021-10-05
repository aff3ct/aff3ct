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
#include "Simulation/BFER/Iterative/Simulation_BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_ite<B,R,Q>
::Simulation_BFER_ite(const factory::BFER_ite &params_BFER_ite)
: Simulation_BFER<B,R>(params_BFER_ite),
  params_BFER_ite(params_BFER_ite)
{
	if (this->params_BFER_ite.err_track_revert && this->params_BFER_ite.n_threads != 1)
		std::clog << rang::tag::warning << "Multi-threading detected with error tracking revert feature! "
		                                   "Each thread will play the same frames. Please run with one thread."
		          << std::endl;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Source<B>> Simulation_BFER_ite<B,R,Q>
::build_source()
{
	auto src = std::unique_ptr<module::Source<B>>(params_BFER_ite.src->build<B>());
	src->set_n_frames(this->params.n_frames);
	return src;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::CRC<B>> Simulation_BFER_ite<B,R,Q>
::build_crc()
{
	auto crc = std::unique_ptr<module::CRC<B>>(params_BFER_ite.crc->build<B>());
	crc->set_n_frames(this->params.n_frames);
	return crc;
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools::Codec_SISO<B,Q>> Simulation_BFER_ite<B,R,Q>
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
std::unique_ptr<tools ::Interleaver_core<>> Simulation_BFER_ite<B,R,Q>
::build_interleaver()
{
	std::unique_ptr<factory::Interleaver> params_itl(params_BFER_ite.itl->clone());
	auto itl = std::unique_ptr<tools::Interleaver_core<>>(params_itl->core->build<>());
	itl->set_n_frames(this->params.n_frames);
	return itl;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Modem<B,R,Q>> Simulation_BFER_ite<B,R,Q>
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
std::unique_ptr<module::Channel<R>> Simulation_BFER_ite<B,R,Q>
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
std::unique_ptr<module::Quantizer<R,Q>> Simulation_BFER_ite<B,R,Q>
::build_quantizer()
{
	auto qnt = std::unique_ptr<module::Quantizer<R,Q>>(params_BFER_ite.qnt->build<R,Q>());
	qnt->set_n_frames(this->params.n_frames);
	return qnt;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,Q>> Simulation_BFER_ite<B,R,Q>
::build_coset_real()
{
	factory::Coset cst_params;
	cst_params.size = params_BFER_ite.cdc->N_cw;
	auto cst = std::unique_ptr<module::Coset<B,Q>>(cst_params.build_real<B,Q>());
	cst->set_n_frames(this->params.n_frames);
	return cst;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,B>> Simulation_BFER_ite<B,R,Q>
::build_coset_bit()
{
	factory::Coset cst_params;
	cst_params.size = params_BFER_ite.coded_monitoring ? params_BFER_ite.cdc->N_cw : params_BFER_ite.cdc->K;
	auto cst = std::unique_ptr<module::Coset<B,B>>(cst_params.build_bit<B,B>());
	cst->set_n_frames(this->params.n_frames);
	return cst;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Switcher> Simulation_BFER_ite<B,R,Q>
::build_switcher()
{
	auto switcher = std::unique_ptr<module::Switcher>(
		new module::Switcher(2, params_BFER_ite.cdc->N_cw, typeid(Q)));
	switcher->set_n_frames(this->params.n_frames);
	return switcher;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Iterator> Simulation_BFER_ite<B,R,Q>
::build_iterator()
{
	auto iterator = std::unique_ptr<module::Iterator>(new module::Iterator(params_BFER_ite.n_ite));
	iterator->set_n_frames(this->params.n_frames);
	return iterator;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Unaryop_not_abs<int32_t>> Simulation_BFER_ite<B,R,Q>
::build_unaryop()
{
	auto unaryop = std::unique_ptr<module::Unaryop_not_abs<int32_t>>(new module::Unaryop_not_abs<int32_t>(1));
	unaryop->set_n_frames(this->params.n_frames);
	return unaryop;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Reducer_and<int32_t,int8_t>> Simulation_BFER_ite<B,R,Q>
::build_reducer()
{
	auto reducer = std::unique_ptr<module::Reducer_and<int32_t,int8_t>>(new module::Reducer_and<int32_t,int8_t>(1));
	reducer->set_n_frames(this->params.n_frames);
	return reducer;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Binaryop_or<int8_t>> Simulation_BFER_ite<B,R,Q>
::build_binaryop()
{
	auto binaryop = std::unique_ptr<module::Binaryop_or<int8_t>>(new module::Binaryop_or<int8_t>(1));
	binaryop->set_n_frames(this->params.n_frames);
	return binaryop;
}


template <typename B, typename R, typename Q>
void Simulation_BFER_ite<B,R,Q>
::create_modules()
{
	Simulation_BFER<B,R>::create_modules();

	this->source           = build_source     (                                                    );
	this->crc              = build_crc        (                                                    );
	this->codec            = build_codec      (this->crc.get()                                     );
	this->modem1           = build_modem      (this->distributions.get(), this->constellation.get());
	this->modem2           = build_modem      (this->distributions.get(), this->constellation.get());
	this->channel          = build_channel    (this->distributions.get()                           );
	this->quantizer        = build_quantizer  (                                                    );
	this->coset_real1      = build_coset_real (                                                    );
	this->coset_real2      = build_coset_real (                                                    );
	this->coset_real3      = build_coset_real (                                                    );
	this->coset_bit        = build_coset_bit  (                                                    );
	this->interleaver_core = build_interleaver(                                                    );
	this->switcher         = build_switcher   (                                                    );
	this->iterator         = build_iterator   (                                                    );
	this->unaryop          = build_unaryop    (                                                    );
	this->reducer          = build_reducer    (                                                    );
	this->binaryop         = build_binaryop   (                                                    );

	this->interleaver_bit .reset(factory::Interleaver::build<B>(*this->interleaver_core));
	this->interleaver_llr1.reset(factory::Interleaver::build<Q>(*this->interleaver_core));
	this->interleaver_llr2.reset(factory::Interleaver::build<Q>(*this->interleaver_core));
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite<B,R,Q>
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
	auto &swi  = *this->switcher;
	auto &ite  = *this->iterator;
	auto &enc  =  this->codec->get_encoder();
	auto &dcs  =  this->codec->get_decoder_siso();
	auto &dch  =  this->codec->get_decoder_siho();
	auto &ext  =  this->codec->get_extractor();
	auto &uop  = *this->unaryop;
	auto &red  = *this->reducer;
	auto &bop  = *this->binaryop;

	std::vector<Module*> modules = { &src, &crc, &itb,  &mdm1, &mdm2, &chn, &qnt, &itl1, &itl2, &csr1, &csr2, &csr3,
	                                 &csb, &mnt, &swi, &ite, &enc, &dcs, &dch, &ext, &uop, &red, &bop };

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

		mdm1[mdm::sck::modulate::X_N1] = itb[itl::sck::interleave::itl];
		mdm1[mdm::tsk::modulate].exec();
		mdm1[mdm::tsk::modulate].reset();
	}
	else
	{
		if (this->params_BFER_ite.crc->type != "NO")
			crc[crc::sck::build::U_K1] = src[src::sck::generate::U_K];

		if (this->params_BFER_ite.cdc->enc->type != "NO")
		{
			if (this->params_BFER_ite.crc->type != "NO")
				enc[enc::sck::encode::U_K] = crc[crc::sck::build::U_K2];
			else
				enc[enc::sck::encode::U_K] = src[src::sck::generate::U_K];
		}

		if (this->params_BFER_ite.cdc->enc->type != "NO")
			itb[itl::sck::interleave::nat] = enc[enc::sck::encode::X_N];
		else if (this->params_BFER_ite.crc->type != "NO")
			itb[itl::sck::interleave::nat] = crc[crc::sck::build::U_K2];
		else
			itb[itl::sck::interleave::nat] = src[src::sck::generate::U_K];

		mdm1[mdm::sck::modulate::X_N1] = itb[itl::sck::interleave::itl];
	}

	if (this->params_BFER_ite.coset)
	{
		if (this->params_BFER_ite.src->type == "AZCW")
			csb[cst::sck::apply::ref] = enc[enc::sck::encode::X_N].get_dataptr();
		else if (this->params_BFER_ite.coded_monitoring)
		{
			if (this->params_BFER_ite.cdc->enc->type != "NO")
				csb[cst::sck::apply::ref] = enc[enc::sck::encode::X_N];
			else if (this->params_BFER_ite.crc->type != "NO")
				csb[cst::sck::apply::ref] = crc[crc::sck::build::U_K2];
			else
				csb[cst::sck::apply::ref] = src[src::sck::generate::U_K];
		}
		else
		{
			if (this->params_BFER_ite.crc->type != "NO")
				csb[cst::sck::apply::ref] = crc[crc::sck::build::U_K2];
			else
				csb[cst::sck::apply::ref] = src[src::sck::generate::U_K];
		}

		if (this->params_BFER_ite.src->type == "AZCW")
		{
			csr1[cst::sck::apply::ref] = enc[enc::sck::encode::X_N].get_dataptr();
			csr2[cst::sck::apply::ref] = enc[enc::sck::encode::X_N].get_dataptr();
			csr3[cst::sck::apply::ref] = enc[enc::sck::encode::X_N].get_dataptr();
		}
		else if (this->params_BFER_ite.cdc->enc->type != "NO")
		{
			csr1[cst::sck::apply::ref] = enc[enc::sck::encode::X_N];
			csr2[cst::sck::apply::ref] = enc[enc::sck::encode::X_N];
			csr3[cst::sck::apply::ref] = enc[enc::sck::encode::X_N];
		}
		else if (this->params_BFER_ite.crc->type != "NO")
		{
			csr1[cst::sck::apply::ref] = crc[crc::sck::build::U_K2];
			csr2[cst::sck::apply::ref] = crc[crc::sck::build::U_K2];
			csr3[cst::sck::apply::ref] = crc[crc::sck::build::U_K2];
		}
		else
		{
			csr1[cst::sck::apply::ref] = src[src::sck::generate::U_K];
			csr2[cst::sck::apply::ref] = src[src::sck::generate::U_K];
			csr3[cst::sck::apply::ref] = src[src::sck::generate::U_K];
		}
	}

	if (this->params_BFER_ite.src->type == "AZCW")
		mnt[mnt::sck::check_errors::U] = enc[enc::sck::encode::X_N].get_dataptr();
	else if (this->params_BFER_ite.coded_monitoring)
	{
		if (this->params_BFER_ite.cdc->enc->type != "NO")
			mnt[mnt::sck::check_errors::U] = enc[enc::sck::encode::X_N];
		else if (this->params_BFER_ite.crc->type != "NO")
			mnt[mnt::sck::check_errors::U] = crc[crc::sck::build::U_K2];
		else
			mnt[mnt::sck::check_errors::U] = src[src::sck::generate::U_K];
	}
	else
		mnt[mnt::sck::check_errors::U] = src[src::sck::generate::U_K];

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
			chn[chn::sck::add_noise_wg::CP ] = this->channel_params;
			chn[chn::sck::add_noise_wg::X_N] = mdm1[mdm::sck::modulate::X_N2];
		}
		else
		{
			chn[chn::sck::add_noise::CP ] = this->channel_params;
			chn[chn::sck::add_noise::X_N] = mdm1[mdm::sck::modulate::X_N2];
		}
	}

	if (mdm1.is_filter())
	{
		mdm1[mdm::sck::filter::CP] = this->channel_params;
		if (this->params_BFER_ite.chn->type != "NO")
		{
			if (is_rayleigh)
				mdm1[mdm::sck::filter::Y_N1] = chn[chn::sck::add_noise_wg::Y_N];
			else
				mdm1[mdm::sck::filter::Y_N1] = chn[chn::sck::add_noise::Y_N];
		}
		else
			mdm1[mdm::sck::filter::Y_N1] = mdm1[mdm::sck::modulate::X_N2];
	}

	if (this->params_BFER_ite.qnt->type != "NO")
	{
		if (mdm1.is_filter())
			qnt[qnt::sck::process::Y_N1] = mdm1[mdm::sck::filter::Y_N2];
		else if (this->params_BFER_ite.chn->type != "NO")
		{
			if (is_rayleigh)
				qnt[qnt::sck::process::Y_N1] = chn[chn::sck::add_noise_wg::Y_N];
			else
				qnt[qnt::sck::process::Y_N1] = chn[chn::sck::add_noise::Y_N];
		}
		else
			qnt[qnt::sck::process::Y_N1] = mdm1[mdm::sck::modulate::X_N2];
	}

	if (mdm1.is_demodulator())
	{
		if (is_rayleigh)
		{
			mdm1[mdm::sck::demodulate_wg::CP] = this->channel_params;
			if (this->params_BFER_ite.chn->type != "NO")
				mdm1[mdm::sck::demodulate_wg::H_N] = chn[chn::sck::add_noise_wg::H_N];
			else
				mdm1[mdm::sck::demodulate_wg::H_N] = (uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr());
		}
		else
			mdm1[mdm::sck::demodulate::CP] = this->channel_params;

		if (this->params_BFER_ite.qnt->type != "NO")
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1] = qnt[qnt::sck::process::Y_N2];
			else
				mdm1[mdm::sck::demodulate::Y_N1] = qnt[qnt::sck::process::Y_N2];
		}
		else if (mdm1.is_filter())
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1] = mdm1[mdm::sck::filter::Y_N2];
			else
				mdm1[mdm::sck::demodulate::Y_N1] = mdm1[mdm::sck::filter::Y_N2];
		}
		else if (this->params_BFER_ite.chn->type != "NO")
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1] = chn[chn::sck::add_noise_wg::Y_N];
			else
				mdm1[mdm::sck::demodulate::Y_N1] = chn[chn::sck::add_noise::Y_N];
		}
		else
		{
			if (is_rayleigh)
				mdm1[mdm::sck::demodulate_wg::Y_N1] = mdm1[mdm::sck::modulate::X_N2];
			else
				mdm1[mdm::sck::demodulate::Y_N1] = mdm1[mdm::sck::modulate::X_N2];
		}
	}

	if (mdm1.is_demodulator())
	{
		if (is_rayleigh)
			itl1[itl::sck::deinterleave::itl] = mdm1[mdm::sck::demodulate_wg::Y_N2];
		else
			itl1[itl::sck::deinterleave::itl] = mdm1[mdm::sck::demodulate::Y_N2];
	}
	else if (this->params_BFER_ite.qnt->type != "NO")
		itl1[itl::sck::deinterleave::itl] = qnt[qnt::sck::process::Y_N2];
	else if (mdm1.is_filter())
		itl1[itl::sck::deinterleave::itl] = mdm1[mdm::sck::filter::Y_N2];
	else if (this->params_BFER_ite.chn->type != "NO")
	{
		if (is_rayleigh)
			itl1[itl::sck::deinterleave::itl] = chn[chn::sck::add_noise_wg::Y_N];
		else
			itl1[itl::sck::deinterleave::itl] = chn[chn::sck::add_noise::Y_N];
	}
	else
		itl1[itl::sck::deinterleave::itl] = mdm1[mdm::sck::modulate::X_N2];

	// ----------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------- turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	swi[swi::tsk::select ][1] = itl1[itl::sck::deinterleave::nat];
	ite[ite::tsk::iterate   ] = swi [swi::tsk::select      ][2  ];
	swi[swi::tsk::commute][0] = swi [swi::tsk::select      ][2  ];

	if (this->params_BFER_ite.crc->type != "NO" && this->params_BFER_ite.crc_early_termination)
	{
		ext[ext::sck::get_sys_bit::Y_N] = swi[swi::tsk::select     ][2     ];
		crc[crc::sck::check      ::V_K] = ext[ext::sck::get_sys_bit::V_K   ];
		uop[uop::sck::perform    ::in ] = crc[crc::sck::check      ::status];
		red[red::sck::reduce     ::in ] = uop[uop::sck::perform    ::out   ];
		bop[bop::sck::perform    ::in1] = ite[ite::sck::iterate    ::out   ];
		bop[bop::sck::perform    ::in2] = red[red::sck::reduce     ::out   ];
		swi[swi::tsk::commute    ][1  ] = bop[bop::sck::perform    ::out   ];
	}
	else
		swi[swi::tsk::commute][1] = ite[ite::sck::iterate::out];

	// ------------------------------------------------------------------------------------------------------- decoding
	if (this->params_BFER_ite.coset)
	{
		csr1[cst::sck::apply      ::in  ] = swi [swi::tsk::commute    ][2   ];
		dcs [dec::sck::decode_siso::Y_N1] = csr1[cst::sck::apply      ::out ];
		csr2[cst::sck::apply      ::in  ] = dcs [dec::sck::decode_siso::Y_N2];
	}
	else
	{
		dcs[dec::sck::decode_siso::Y_N1] = swi[swi::tsk::commute][2];
	}

	// --------------------------------------------------------------------------------------------------- interleaving
	if (this->params_BFER_ite.coset)
		itl2[itl::sck::interleave::nat] = csr2[cst::sck::apply::out];
	else
		itl2[itl::sck::interleave::nat] = dcs[dec::sck::decode_siso::Y_N2];

	// --------------------------------------------------------------------------------------------------- demodulation
	if (mdm2.is_demodulator())
	{
		if (is_rayleigh)
		{
			mdm2[mdm::sck::tdemodulate_wg::CP] = this->channel_params;
			if (this->params_BFER_ite.chn->type != "NO")
				mdm2[mdm::sck::tdemodulate_wg::H_N] = chn[chn::sck::add_noise_wg::H_N];
			else
				mdm2[mdm::sck::tdemodulate_wg::H_N] = (uint8_t*)(chn[chn::sck::add_noise_wg::H_N].get_dataptr());

			if (this->params_BFER_ite.qnt->type != "NO")
				mdm2[mdm::sck::tdemodulate_wg::Y_N1] = qnt[qnt::sck::process::Y_N2];
			else if (mdm1.is_filter())
				mdm2[mdm::sck::tdemodulate_wg::Y_N1] = mdm1[mdm::sck::filter::Y_N2];
			else if (this->params_BFER_ite.chn->type != "NO")
				mdm2[mdm::sck::tdemodulate_wg::Y_N1] = chn[chn::sck::add_noise_wg::Y_N];
			else
				mdm2[mdm::sck::tdemodulate_wg::Y_N1] = mdm1[mdm::sck::modulate::X_N2];

			mdm2[mdm::sck::tdemodulate_wg::Y_N2] = itl2[itl::sck::interleave::itl];
		}
		else
		{
			mdm2[mdm::sck::tdemodulate::CP] = this->channel_params;
			if (this->params_BFER_ite.qnt->type != "NO")
				mdm2[mdm::sck::tdemodulate::Y_N1] = qnt[qnt::sck::process::Y_N2];
			else if (mdm1.is_filter())
				mdm2[mdm::sck::tdemodulate::Y_N1] = mdm1[mdm::sck::filter::Y_N2];
			else if (this->params_BFER_ite.chn->type != "NO")
				mdm2[mdm::sck::tdemodulate::Y_N1] = chn[chn::sck::add_noise::Y_N];
			else
				mdm2[mdm::sck::tdemodulate::Y_N1] = mdm1[mdm::sck::modulate::X_N2];

			mdm2[mdm::sck::tdemodulate::Y_N2] = itl2[itl::sck::interleave::itl ];
		}
	}

	// ------------------------------------------------------------------------------------------------- deinterleaving
	if (mdm2.is_demodulator())
	{
		if (is_rayleigh)
			itl2[itl::sck::deinterleave::itl] = mdm2[mdm::sck::tdemodulate_wg::Y_N3];
		else
			itl2[itl::sck::deinterleave::itl] = mdm2[mdm::sck::tdemodulate::Y_N3];
	}
	else if (this->params_BFER_ite.qnt->type != "NO")
		itl2[itl::sck::deinterleave::itl] = qnt[qnt::sck::process::Y_N2];
	else if (mdm1.is_filter())
		itl2[itl::sck::deinterleave::itl] = mdm1[mdm::sck::filter::Y_N2];
	else if (this->params_BFER_ite.chn->type != "NO")
	{
		if (is_rayleigh)
			itl2[itl::sck::deinterleave::itl] = chn[chn::sck::add_noise_wg::Y_N];
		else
			itl2[itl::sck::deinterleave::itl] = chn[chn::sck::add_noise::Y_N];
	}
	else
		itl2[itl::sck::deinterleave::itl] = mdm1[mdm::sck::modulate::X_N2];

	swi[swi::tsk::select][0] = itl2[itl::sck::deinterleave::nat];

	// ----------------------------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------- end of turbo demodulation loop
	// ----------------------------------------------------------------------------------------------------------------

	if (this->params_BFER_ite.crc->type != "NO" && this->params_BFER_ite.crc_early_termination)
		ite[ite::tsk::reset] = swi[swi::tsk::commute][3];

	if (this->params_BFER_ite.coset)
	{
		csr3[cst::sck::apply::in] = swi[swi::tsk::commute][3];

		if (this->params_BFER_ite.coded_monitoring)
		{
			dch[dec::sck::decode_siho_cw::Y_N] = csr3[cst::sck::apply         ::out];
			csb[cst::sck::apply         ::in ] = dch [dec::sck::decode_siho_cw::V_N];
		}
		else
		{
			dch[dec::sck::decode_siho::Y_N] = csr3[cst::sck::apply      ::out];
			csb[cst::sck::apply      ::in ] = dch [dec::sck::decode_siho::V_K];
			if (this->params_BFER_ite.crc->type != "NO")
				crc[crc::sck::extract::V_K1] = csb[cst::sck::apply::out];
		}
	}
	else
	{
		if (this->params_BFER_ite.coded_monitoring)
		{
			dch[dec::sck::decode_siho_cw::Y_N] = swi[swi::tsk::commute][3];
		}
		else
		{
			dch[dec::sck::decode_siho::Y_N] = swi[swi::tsk::commute][3];

			if (this->params_BFER_ite.crc->type != "NO")
				crc[crc::sck::extract::V_K1] = dch[dec::sck::decode_siho::V_K];
		}
	}

	if (this->params_BFER_ite.coded_monitoring)
	{
		if (this->params_BFER_ite.coset)
			mnt[mnt::sck::check_errors::V] = csb[cst::sck::apply::out];
		else
			mnt[mnt::sck::check_errors::V] = dch[dec::sck::decode_siho_cw::V_N];
	}
	else
	{
		if (this->params_BFER_ite.crc->type != "NO")
			mnt[mnt::sck::check_errors::V] = crc[crc::sck::extract::V_K2];
		else
		{
			if (this->params_BFER_ite.coset)
				mnt[mnt::sck::check_errors::V] = csb[cst::sck::apply::out];
			else
				mnt[mnt::sck::check_errors::V] = dch[dec::sck::decode_siho::V_K];
		}
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite<B,R,Q>
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
	this->codec->set_noise(*this->noise);
	for (auto &m : this->sequence->template get_modules<tools::Interface_get_set_noise>())
		m->set_noise(*this->noise);

	// registering to noise updates
	this->noise->record_callback_update([this](){ this->codec->notify_noise_update(); });
	for (auto &m : this->sequence->template get_modules<tools::Interface_notify_noise_update>())
		this->noise->record_callback_update([m](){ m->notify_noise_update(); });

	// set different seeds in the modules that uses PRNG
	std::mt19937 prng(params_BFER_ite.local_seed);
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

	this->interleaver_core->set_seed(params_BFER_ite.itl->core->seed);
	if (this->interleaver_core->is_uniform())
	{
		std::stringstream message;
		message << "Uniform interleaver is not supported at this time in the simulations.";
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
			auto &encoder = encoders.size() ? *encoders[tid] : this->codec->get_encoder();
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
template class aff3ct::simulation::Simulation_BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
