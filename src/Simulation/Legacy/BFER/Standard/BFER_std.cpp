#include <functional>
#include <stdexcept>
#include <iomanip>

#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Coset/Coset.hpp"
#include "Simulation/Legacy/BFER/Standard/BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_std<B,R,Q>
::BFER_std(const factory::BFER_std &params_BFER_std)
: BFER<B,R,Q>(params_BFER_std),
  params_BFER_std(params_BFER_std),

  source    (params_BFER_std.n_threads),
  crc       (params_BFER_std.n_threads),
  codec     (params_BFER_std.n_threads),
  modem     (params_BFER_std.n_threads),
  channel   (params_BFER_std.n_threads),
  quantizer (params_BFER_std.n_threads),
  coset_real(params_BFER_std.n_threads),
  coset_bit (params_BFER_std.n_threads),

  rd_engine_seed(params_BFER_std.n_threads)
{
	for (auto tid = 0; tid < params_BFER_std.n_threads; tid++)
		rd_engine_seed[tid].seed(params_BFER_std.local_seed + tid);

	this->add_module("source"    , params_BFER_std.n_threads);
	this->add_module("crc"       , params_BFER_std.n_threads);
	this->add_module("encoder"   , params_BFER_std.n_threads);
	this->add_module("puncturer" , params_BFER_std.n_threads);
	this->add_module("modem"     , params_BFER_std.n_threads);
	this->add_module("channel"   , params_BFER_std.n_threads);
	this->add_module("quantizer" , params_BFER_std.n_threads);
	this->add_module("coset_real", params_BFER_std.n_threads);
	this->add_module("decoder"   , params_BFER_std.n_threads);
	this->add_module("coset_bit" , params_BFER_std.n_threads);
}

template <typename B, typename R, typename Q>
void BFER_std<B,R,Q>
::__build_communication_chain(const int tid)
{
	// build the objects
	if (!params_BFER_std.alloc_clone || tid == 0)
	{
		source    [tid] = build_source    (tid);
		crc       [tid] = build_crc       (tid);
		codec     [tid] = build_codec     (tid);
		modem     [tid] = build_modem     (tid);
		channel   [tid] = build_channel   (tid);
		quantizer [tid] = build_quantizer (tid);
		coset_real[tid] = build_coset_real(tid);
		coset_bit [tid] = build_coset_bit (tid);
	}

	if (params_BFER_std.alloc_clone)
	{
		source    [tid].reset(source    [0]->clone());
		crc       [tid].reset(crc       [0]->clone());
		codec     [tid].reset(codec     [0]->clone());
		modem     [tid].reset(modem     [0]->clone());
		channel   [tid].reset(channel   [0]->clone());
		quantizer [tid].reset(quantizer [0]->clone());
		coset_real[tid].reset(coset_real[0]->clone());
		coset_bit [tid].reset(coset_bit [0]->clone());
	}

	// set the noise
	codec  [tid]->set_noise(*this->noise);
	modem  [tid]->set_noise(*this->noise);
	channel[tid]->set_noise(*this->noise);

	// register modules to "noise changed" callback
	auto ptr_cdc = codec  [tid].get();
	auto ptr_mdm = modem  [tid].get();
	auto ptr_chn = channel[tid].get();
	this->noise->record_callback_update([ptr_cdc](){ ptr_cdc->notify_noise_update(); });
	this->noise->record_callback_update([ptr_mdm](){ ptr_mdm->notify_noise_update(); });
	this->noise->record_callback_update([ptr_chn](){ ptr_chn->notify_noise_update(); });

	// set the seeds
	const auto seed_src = rd_engine_seed[tid]();
	const auto seed_enc = rd_engine_seed[tid]();
	const auto seed_dec = rd_engine_seed[tid]();
	const auto seed_itl = params_BFER_std.cdc->itl != nullptr ? params_BFER_std.cdc->itl->core->seed +
	                      (params_BFER_std.cdc->itl->core->uniform ? rd_engine_seed[tid]() : 0) : 0;
	const auto seed_chn = rd_engine_seed[tid]();

	      source [tid]->                   set_seed(seed_src);
	      channel[tid]->                   set_seed(seed_chn);
	try { codec  [tid]->get_encoder     ().set_seed(seed_enc); } catch (...) {}
	try { codec  [tid]->get_decoder_siho().set_seed(seed_dec); } catch (...) {}
	try { codec  [tid]->get_interleaver ().set_seed(seed_itl); } catch (...) {}

	this->set_module("source"    , tid, *source    [tid]);
	this->set_module("crc"       , tid, *crc       [tid]);
	this->set_module("encoder"   , tid,  codec     [tid]->get_encoder());
	this->set_module("puncturer" , tid,  codec     [tid]->get_puncturer());
	this->set_module("modem"     , tid, *modem     [tid]);
	this->set_module("channel"   , tid, *channel   [tid]);
	this->set_module("quantizer" , tid, *quantizer [tid]);
	this->set_module("coset_real", tid, *coset_real[tid]);
	this->set_module("decoder"   , tid,  codec     [tid]->get_decoder_siho());
	this->set_module("coset_bit" , tid, *coset_bit [tid]);

	try
	{
		auto& interleaver = codec[tid]->get_interleaver(); // can raise an exceptions
		if (interleaver.is_uniform())
		{
			this->monitor_er[tid]->record_callback_check(std::bind(&tools::Interleaver_core<>::refresh,
			                                                       &interleaver));
			if (params_BFER_std.err_track_revert)
				interleaver.reinitialize();
		}

		if (this->params_BFER_std.err_track_enable && interleaver.is_uniform())
			this->dumper[tid]->register_data(interleaver.get_lut(),
			                                 this->params_BFER_std.err_track_threshold,
			                                 "itl",
			                                 false,
			                                 this->params_BFER_std.n_frames,
			                                 {});
	}
	catch (...) { /* do nothing if there is no interleaver */ }

	if (this->params_BFER_std.err_track_enable)
	{
		using namespace module;

		auto &source  = *this->source [tid];
		auto &encoder =  this->codec  [tid]->get_encoder();
		auto &channel = *this->channel[tid];

		source[src::tsk::generate].set_autoalloc(true);
		auto src_data = (B*)(source[src::sck::generate::U_K].get_dataptr());
		auto src_bytes = source[src::sck::generate::U_K].get_databytes();
		auto src_size = (src_bytes / sizeof(B)) / this->params_BFER_std.n_frames;
		this->dumper[tid]->register_data(src_data,
		                                 (unsigned int)src_size,
		                                 this->params_BFER_std.err_track_threshold,
		                                 "src",
		                                 false,
		                                 this->params_BFER_std.n_frames,
		                                 {});

		encoder[enc::tsk::encode].set_autoalloc(true);
		auto enc_data = (B*)(encoder[enc::sck::encode::X_N].get_dataptr());
		auto enc_bytes = encoder[enc::sck::encode::X_N].get_databytes();
		auto enc_size = (enc_bytes / sizeof(B)) / this->params_BFER_std.n_frames;
		this->dumper[tid]->register_data(enc_data,
		                                 (unsigned int)enc_size,
		                                 this->params_BFER_std.err_track_threshold,
		                                 "enc",
		                                 false,
		                                 this->params_BFER_std.n_frames,
		                                 {(unsigned)this->params_BFER_std.cdc->enc->K});

		this->dumper[tid]->register_data(channel.get_noised_data(),
		                                 this->params_BFER_std.err_track_threshold,
		                                 "chn",
		                                 true,
		                                 this->params_BFER_std.n_frames,
		                                 {});
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Source<B>> BFER_std<B,R,Q>
::build_source(const int tid)
{
	auto src = std::unique_ptr<module::Source<B>>(params_BFER_std.src->build<B>());
	src->set_n_frames(this->params.n_frames);
	return src;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::CRC<B>> BFER_std<B,R,Q>
::build_crc(const int tid)
{
	auto crc = std::unique_ptr<module::CRC<B>>(params_BFER_std.crc->build<B>());
	crc->set_n_frames(this->params.n_frames);
	return crc;
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools::Codec_SIHO<B,Q>> BFER_std<B,R,Q>
::build_codec(const int tid)
{
	std::unique_ptr<factory::Codec> params_cdc(params_BFER_std.cdc->clone());
	if (params_cdc->itl != nullptr)
	{
		if (params_BFER_std.err_track_revert && params_cdc->itl->core->uniform)
		{
			std::stringstream s_noise;
			s_noise << std::setprecision(2) << std::fixed << this->noise->get_value();

			params_cdc->itl->core->type = "USER";
			params_cdc->itl->core->path = params_BFER_std.err_track_path + "_" + s_noise.str() + ".itl";
		}
	}

	auto crc = this->params_BFER_std.crc->type == "NO" ? nullptr : this->crc[tid].get();
	auto param_siho = dynamic_cast<factory::Codec_SIHO*>(params_cdc.get());
	auto cdc = std::unique_ptr<tools::Codec_SIHO<B,Q>>(param_siho->build<B,Q>(crc));
	cdc->set_n_frames(this->params.n_frames);

	return cdc;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Modem<B,R,R>> BFER_std<B,R,Q>
::build_modem(const int tid)
{
	if (this->distributions != nullptr)
	{
		auto mdm = std::unique_ptr<module::Modem<B,R,R>>(params_BFER_std.mdm->build<B,R,R>(*this->distributions));
		mdm->set_n_frames(this->params.n_frames);
		return mdm;
	}
	else
	{
		auto mdm = std::unique_ptr<module::Modem<B,R,R>>(params_BFER_std.mdm->build<B,R,R>(this->constellation.get()));
		mdm->set_n_frames(this->params.n_frames);
		return mdm;
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Channel<R>> BFER_std<B,R,Q>
::build_channel(const int tid)
{
	if (this->distributions != nullptr)
	{
		auto chn = std::unique_ptr<module::Channel<R>>(params_BFER_std.chn->build<R>(*this->distributions));
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
std::unique_ptr<module::Quantizer<R,Q>> BFER_std<B,R,Q>
::build_quantizer(const int tid)
{
	auto qnt = std::unique_ptr<module::Quantizer<R,Q>>(params_BFER_std.qnt->build<R,Q>());
	qnt->set_n_frames(this->params.n_frames);
	return qnt;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,Q>> BFER_std<B,R,Q>
::build_coset_real(const int tid)
{
	factory::Coset cst_params;
	cst_params.size = params_BFER_std.cdc->N_cw;
	auto cst = std::unique_ptr<module::Coset<B,Q>>(cst_params.build_real<B,Q>());
	cst->set_n_frames(this->params.n_frames);
	return cst;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,B>> BFER_std<B,R,Q>
::build_coset_bit(const int tid)
{
	factory::Coset cst_params;
	cst_params.size = this->params_BFER_std.coded_monitoring ? params_BFER_std.cdc->N_cw : params_BFER_std.cdc->K;
	auto cst = std::unique_ptr<module::Coset<B,B>>(cst_params.build_bit<B,B>());
	cst->set_n_frames(this->params.n_frames);
	return cst;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::simulation::BFER_std<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER_std<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER_std<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER_std<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER_std<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
