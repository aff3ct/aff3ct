#include <functional>
#include <iomanip>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Predicate_ite.hpp"
#include "Factory/Module/Coset/Coset.hpp"
#include "Simulation/Legacy/BFER/Iterative/BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_ite<B,R,Q>
::BFER_ite(const factory::BFER_ite &params_BFER_ite)
: BFER<B,R,Q>(params_BFER_ite),
  params_BFER_ite(params_BFER_ite),

  source          (params_BFER_ite.n_threads),
  crc             (params_BFER_ite.n_threads),
  codec1          (params_BFER_ite.n_threads),
  codec2          (params_BFER_ite.n_threads),
  modem1          (params_BFER_ite.n_threads),
  modem2          (params_BFER_ite.n_threads),
  channel         (params_BFER_ite.n_threads),
  quantizer       (params_BFER_ite.n_threads),
  coset_real1     (params_BFER_ite.n_threads),
  coset_real2     (params_BFER_ite.n_threads),
  coset_real3     (params_BFER_ite.n_threads),
  coset_bit       (params_BFER_ite.n_threads),
  interleaver_core(params_BFER_ite.n_threads),
  interleaver_bit (params_BFER_ite.n_threads),
  interleaver_llr1(params_BFER_ite.n_threads),
  interleaver_llr2(params_BFER_ite.n_threads),
  loop_ite        (params_BFER_ite.n_threads),
  loop_crc        (params_BFER_ite.n_threads),

  rd_engine_seed(params_BFER_ite.n_threads)
{
	for (auto tid = 0; tid < params_BFER_ite.n_threads; tid++)
		rd_engine_seed[tid].seed(params_BFER_ite.local_seed + tid);

	this->add_module("source"          , params_BFER_ite.n_threads);
	this->add_module("crc"             , params_BFER_ite.n_threads);
	this->add_module("extractor1"      , params_BFER_ite.n_threads);
	this->add_module("extractor2"      , params_BFER_ite.n_threads);
	this->add_module("encoder"         , params_BFER_ite.n_threads);
	this->add_module("modem1"          , params_BFER_ite.n_threads);
	this->add_module("modem2"          , params_BFER_ite.n_threads);
	this->add_module("channel"         , params_BFER_ite.n_threads);
	this->add_module("quantizer"       , params_BFER_ite.n_threads);
	this->add_module("coset_real1"     , params_BFER_ite.n_threads);
	this->add_module("coset_real2"     , params_BFER_ite.n_threads);
	this->add_module("coset_real3"     , params_BFER_ite.n_threads);
	this->add_module("decoder_siso"    , params_BFER_ite.n_threads);
	this->add_module("decoder_siho"    , params_BFER_ite.n_threads);
	this->add_module("coset_bit"       , params_BFER_ite.n_threads);
	this->add_module("interleaver_bit" , params_BFER_ite.n_threads);
	this->add_module("interleaver_llr1", params_BFER_ite.n_threads);
	this->add_module("interleaver_llr2", params_BFER_ite.n_threads);
	this->add_module("loop_ite"        , params_BFER_ite.n_threads);
	this->add_module("loop_crc"        , params_BFER_ite.n_threads);
}

template <typename B, typename R, typename Q>
void BFER_ite<B,R,Q>
::__build_communication_chain(const int tid)
{
	// build the objects
	if (!params_BFER_ite.alloc_clone || tid == 0)
	{
		source          [tid] = build_source     (tid);
		crc             [tid] = build_crc        (tid);
		codec1          [tid] = build_codec      (tid);
		codec2          [tid] = build_codec      (tid);
		modem1          [tid] = build_modem      (tid);
		modem2          [tid] = build_modem      (tid);
		channel         [tid] = build_channel    (tid);
		quantizer       [tid] = build_quantizer  (tid);
		coset_real1     [tid] = build_coset_real (tid);
		coset_real2     [tid] = build_coset_real (tid);
		coset_real3     [tid] = build_coset_real (tid);
		coset_bit       [tid] = build_coset_bit  (tid);
		interleaver_core[tid] = build_interleaver(tid);
		loop_ite        [tid] = build_loop_ite   (tid);
		loop_crc        [tid] = build_loop_crc   (tid);
	}

	if (params_BFER_ite.alloc_clone)
	{
		source          [tid].reset(source          [0]->clone());
		crc             [tid].reset(crc             [0]->clone());
		codec1          [tid].reset(codec1          [0]->clone());
		codec2          [tid].reset(codec2          [0]->clone());
		modem1          [tid].reset(modem1          [0]->clone());
		modem2          [tid].reset(modem2          [0]->clone());
		channel         [tid].reset(channel         [0]->clone());
		quantizer       [tid].reset(quantizer       [0]->clone());
		coset_real1     [tid].reset(coset_real1     [0]->clone());
		coset_real2     [tid].reset(coset_real2     [0]->clone());
		coset_real3     [tid].reset(coset_real3     [0]->clone());
		coset_bit       [tid].reset(coset_bit       [0]->clone());
		interleaver_core[tid].reset(interleaver_core[0]->clone());
		loop_ite        [tid].reset(loop_ite        [0]->clone());
		loop_crc        [tid].reset(loop_crc        [0]->clone());
	}

	interleaver_bit [tid].reset(factory::Interleaver::build<B>(*interleaver_core[tid]));
	interleaver_llr1[tid].reset(factory::Interleaver::build<Q>(*interleaver_core[tid]));
	interleaver_llr2[tid].reset(factory::Interleaver::build<Q>(*interleaver_core[tid]));

	// set the noise
	codec1 [tid]->set_noise(*this->noise);
	codec2 [tid]->set_noise(*this->noise);
	modem1 [tid]->set_noise(*this->noise);
	modem2 [tid]->set_noise(*this->noise);
	channel[tid]->set_noise(*this->noise);

	// register modules to "noise changed" callback
	auto ptr_cdc1 = codec1 [tid].get();
	auto ptr_cdc2 = codec2 [tid].get();
	auto ptr_mdm1 = modem1 [tid].get();
	auto ptr_mdm2 = modem2 [tid].get();
	auto ptr_chn  = channel[tid].get();
	this->noise->record_callback_update([ptr_cdc1](){ ptr_cdc1->notify_noise_update(); });
	this->noise->record_callback_update([ptr_cdc2](){ ptr_cdc2->notify_noise_update(); });
	this->noise->record_callback_update([ptr_mdm1](){ ptr_mdm1->notify_noise_update(); });
	this->noise->record_callback_update([ptr_mdm2](){ ptr_mdm2->notify_noise_update(); });
	this->noise->record_callback_update([ptr_chn ](){ ptr_chn ->notify_noise_update(); });

	// set the seeds
	const auto seed_src = rd_engine_seed[tid]();
	const auto seed_enc = rd_engine_seed[tid]();
	const auto seed_dec = rd_engine_seed[tid]();
	      auto seed_itl = rd_engine_seed[tid]();
	           seed_itl = params_BFER_ite.itl->core->seed + (params_BFER_ite.itl->core->uniform ? seed_itl : 0);
	const auto seed_chn = rd_engine_seed[tid]();

	      source          [tid]->                   set_seed(seed_src);
	      channel         [tid]->                   set_seed(seed_chn);
	      interleaver_core[tid]->                   set_seed(seed_itl);
	try { codec1          [tid]->get_encoder     ().set_seed(seed_enc); } catch (...) {}
	try { codec1          [tid]->get_decoder_siho().set_seed(seed_dec); } catch (...) {}
	try { codec1          [tid]->get_decoder_siso().set_seed(seed_dec); } catch (...) {}
	try { codec2          [tid]->get_encoder     ().set_seed(seed_enc); } catch (...) {}
	try { codec2          [tid]->get_decoder_siho().set_seed(seed_dec); } catch (...) {}
	try { codec2          [tid]->get_decoder_siso().set_seed(seed_dec); } catch (...) {}

	this->set_module("source"          , tid, *source          [tid]);
	this->set_module("crc"             , tid, *crc             [tid]);
	this->set_module("extractor1"      , tid,  codec1          [tid]->get_extractor());
	this->set_module("extractor2"      , tid,  codec2          [tid]->get_extractor());
	this->set_module("encoder"         , tid,  codec1          [tid]->get_encoder());
	this->set_module("modem1"          , tid, *modem1          [tid]);
	this->set_module("modem2"          , tid, *modem2          [tid]);
	this->set_module("channel"         , tid, *channel         [tid]);
	this->set_module("quantizer"       , tid, *quantizer       [tid]);
	this->set_module("coset_real1"     , tid, *coset_real1     [tid]);
	this->set_module("coset_real2"     , tid, *coset_real2     [tid]);
	this->set_module("coset_real3"     , tid, *coset_real3     [tid]);
	this->set_module("decoder_siso"    , tid,  codec1          [tid]->get_decoder_siso());
	this->set_module("coset_bit"       , tid, *coset_bit       [tid]);
	this->set_module("interleaver_bit" , tid, *interleaver_bit [tid]);
	this->set_module("interleaver_llr1", tid, *interleaver_llr1[tid]);
	this->set_module("interleaver_llr2", tid, *interleaver_llr2[tid]);
	this->set_module("loop_ite"        , tid, *loop_ite        [tid]);
	this->set_module("loop_crc"        , tid, *loop_crc        [tid]);

	if (static_cast<module::Decoder*>(&codec1[tid]->get_decoder_siso()) !=
	    static_cast<module::Decoder*>(&codec1[tid]->get_decoder_siho()))
	{
		this->set_module("decoder_siho", tid, codec1[tid]->get_decoder_siho());
		codec1[tid]->get_decoder_siho().set_auto_reset(false);
		this->monitor_er[tid]->record_callback_check([this, tid](){ this->codec1[tid]->get_decoder_siho().reset(); });
	}

	if (static_cast<module::Decoder*>(&codec2[tid]->get_decoder_siso()) !=
	    static_cast<module::Decoder*>(&codec2[tid]->get_decoder_siho()))
	{
		this->set_module("decoder_siho", tid, codec2[tid]->get_decoder_siho());
		codec2[tid]->get_decoder_siho().set_auto_reset(false);
		this->monitor_er[tid]->record_callback_check([this, tid](){ this->codec2[tid]->get_decoder_siho().reset(); });
	}

	codec1[tid]->get_decoder_siso().set_auto_reset(false);
	codec2[tid]->get_decoder_siso().set_auto_reset(false);
	this->monitor_er[tid]->record_callback_check([this, tid](){ this->codec1  [tid]->get_decoder_siso().reset(); });
	this->monitor_er[tid]->record_callback_check([this, tid](){ this->codec2  [tid]->get_decoder_siso().reset(); });
	this->monitor_er[tid]->record_callback_check([this, tid](){ this->loop_ite[tid]                   ->reset(); });
	this->monitor_er[tid]->record_callback_check([this, tid](){ this->loop_crc[tid]                   ->reset(); });

	if (interleaver_core[tid]->is_uniform())
		this->monitor_er[tid]->record_callback_check(std::bind(&tools::Interleaver_core<>::refresh,
		                                                       this->interleaver_core[tid].get()));

	if (this->params_BFER_ite.err_track_enable)
	{
		using namespace module;

		auto &source      = *this->source    [tid];
		auto &encoder     =  this->codec1    [tid]->get_encoder();
		auto &channel     = *this->channel   [tid];
		auto &interleaver = *interleaver_core[tid];

		source[src::tsk::generate].set_autoalloc(true);
		auto src_data = (B*)(source[src::sck::generate::U_K].get_dataptr());
		auto src_bytes = source[src::sck::generate::U_K].get_databytes();
		auto src_size = (src_bytes / sizeof(B)) / this->params_BFER_ite.src->n_frames;
		this->dumper[tid]->register_data(src_data,
		                                 (unsigned int)src_size,
		                                 this->params_BFER_ite.err_track_threshold,
		                                 "src",
		                                 false,
		                                 this->params_BFER_ite.src->n_frames,
		                                 {});

		encoder[enc::tsk::encode].set_autoalloc(true);
		auto enc_data = (B*)(encoder[enc::sck::encode::X_N].get_dataptr());
		auto enc_bytes = encoder[enc::sck::encode::X_N].get_databytes();
		auto enc_size = (enc_bytes / sizeof(B)) / this->params_BFER_ite.src->n_frames;
		this->dumper[tid]->register_data(enc_data,
		                                 (unsigned int)enc_size,
		                                 this->params_BFER_ite.err_track_threshold,
		                                 "enc",
		                                 false,
		                                 this->params_BFER_ite.src->n_frames,
		                                 {(unsigned)this->params_BFER_ite.cdc->enc->K});

		this->dumper[tid]->register_data(channel.get_noised_data(),
		                                 this->params_BFER_ite.err_track_threshold,
		                                 "chn",
		                                 true,
		                                 this->params_BFER_ite.src->n_frames,
		                                 {});

		if (interleaver_core[tid]->is_uniform())
			this->dumper[tid]->register_data(interleaver.get_lut(),
			                                 this->params_BFER_ite.err_track_threshold,
			                                 "itl",
			                                 false,
			                                 this->params_BFER_ite.src->n_frames,
			                                 {});
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Source<B>> BFER_ite<B,R,Q>
::build_source(const int tid)
{
	return std::unique_ptr<module::Source<B>>(params_BFER_ite.src->build<B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::CRC<B>> BFER_ite<B,R,Q>
::build_crc(const int tid)
{
	return std::unique_ptr<module::CRC<B>>(params_BFER_ite.crc->build<B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools::Codec_SISO<B,Q>> BFER_ite<B,R,Q>
::build_codec(const int tid)
{
	auto crc = this->params_BFER_ite.crc->type == "NO" ? nullptr : this->crc[tid].get();
	std::unique_ptr<factory::Codec> params_cdc(params_BFER_ite.cdc->clone());
	auto param_siso_siho = dynamic_cast<factory::Codec_SISO*>(params_cdc.get());
	return std::unique_ptr<tools::Codec_SISO<B,Q>>(param_siso_siho->template build<B,Q>(crc));
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools ::Interleaver_core<>> BFER_ite<B,R,Q>
::build_interleaver(const int tid)
{
	std::unique_ptr<factory::Interleaver> params_itl(params_BFER_ite.itl->clone());
	if (params_BFER_ite.err_track_revert && params_BFER_ite.itl->core->uniform)
	{
		std::stringstream s_noise;
		s_noise << std::setprecision(2) << std::fixed << this->noise->get_value();
		params_itl->core->path = params_BFER_ite.err_track_path + "_" + s_noise.str() + ".itl";
	}
	return std::unique_ptr<tools::Interleaver_core<>>(params_itl->core->build<>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Modem<B,R,Q>> BFER_ite<B,R,Q>
::build_modem(const int tid)
{
	if (this->distributions != nullptr)
		return std::unique_ptr<module::Modem<B,R,Q>>(params_BFER_ite.mdm->build<B,R,Q>(*this->distributions));
	else
		return std::unique_ptr<module::Modem<B,R,Q>>(params_BFER_ite.mdm->build<B,R,Q>(this->constellation.get()));
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Channel<R>> BFER_ite<B,R,Q>
::build_channel(const int tid)
{
	if (this->distributions != nullptr)
		return std::unique_ptr<module::Channel<R>>(params_BFER_ite.chn->build<R>(*this->distributions));
	else
	{
		auto c = std::unique_ptr<module::Channel<R>>(params_BFER_ite.chn->build<R>());
		return c;
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Quantizer<R,Q>> BFER_ite<B,R,Q>
::build_quantizer(const int tid)
{
	return std::unique_ptr<module::Quantizer<R,Q>>(params_BFER_ite.qnt->build<R,Q>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,Q>> BFER_ite<B,R,Q>
::build_coset_real(const int tid)
{
	factory::Coset cst_params;
	cst_params.size = params_BFER_ite.cdc->N_cw;
	cst_params.n_frames = params_BFER_ite.src->n_frames;
	return std::unique_ptr<module::Coset<B,Q>>(cst_params.build_real<B,Q>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,B>> BFER_ite<B,R,Q>
::build_coset_bit(const int tid)
{
	factory::Coset cst_params;
	cst_params.size = params_BFER_ite.coded_monitoring ? params_BFER_ite.cdc->N_cw : params_BFER_ite.cdc->K;
	cst_params.n_frames = params_BFER_ite.src->n_frames;
	return std::unique_ptr<module::Coset<B,B>>(cst_params.build_bit<B,B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Loop_predicate<Q>> BFER_ite<B,R,Q>
::build_loop_ite(const int tid)
{
	tools::Predicate_ite p(params_BFER_ite.n_ite);
	auto loop_ite =  std::unique_ptr<module::Loop_predicate<Q>>(new module::Loop_predicate<Q>(
		p,
		params_BFER_ite.cdc->N_cw,
		params_BFER_ite.src->n_frames));
	return loop_ite;
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Loop_CRC<B,Q>> BFER_ite<B,R,Q>
::build_loop_crc(const int tid)
{
	tools::Predicate_ite p(params_BFER_ite.n_ite);
	auto crc = params_BFER_ite.crc->build<B>();
	auto loop_crc = std::unique_ptr<module::Loop_CRC<B,Q>>(new module::Loop_CRC<B,Q>(
		p,
		*crc,
		(size_t)crc->get_size() + (size_t)crc->get_K(),
		params_BFER_ite.cdc->N_cw,
		params_BFER_ite.crc_start));
	return loop_crc;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::simulation::BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

