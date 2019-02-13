#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Coset/Coset.hpp"

#include "BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_ite<B,R,Q>
::BFER_ite(const factory::BFER_ite::parameters &params_BFER_ite)
: BFER<B,R,Q>(params_BFER_ite),
  params_BFER_ite(params_BFER_ite),

  source          (params_BFER_ite.n_threads),
  crc             (params_BFER_ite.n_threads),
  codec           (params_BFER_ite.n_threads),
  modem           (params_BFER_ite.n_threads),
  channel         (params_BFER_ite.n_threads),
  quantizer       (params_BFER_ite.n_threads),
  coset_real      (params_BFER_ite.n_threads),
  coset_bit       (params_BFER_ite.n_threads),
  interleaver_core(params_BFER_ite.n_threads),
  interleaver_bit (params_BFER_ite.n_threads),
  interleaver_llr (params_BFER_ite.n_threads),

  rd_engine_seed(params_BFER_ite.n_threads)
{
	for (auto tid = 0; tid < params_BFER_ite.n_threads; tid++)
		rd_engine_seed[tid].seed(params_BFER_ite.local_seed + tid);

	this->add_module("source"         , params_BFER_ite.n_threads);
	this->add_module("crc"            , params_BFER_ite.n_threads);
	this->add_module("codec"          , params_BFER_ite.n_threads);
	this->add_module("encoder"        , params_BFER_ite.n_threads);
	this->add_module("modem"          , params_BFER_ite.n_threads);
	this->add_module("channel"        , params_BFER_ite.n_threads);
	this->add_module("quantizer"      , params_BFER_ite.n_threads);
	this->add_module("coset_real"     , params_BFER_ite.n_threads);
	this->add_module("decoder_siso"   , params_BFER_ite.n_threads);
	this->add_module("decoder_siho"   , params_BFER_ite.n_threads);
	this->add_module("coset_bit"      , params_BFER_ite.n_threads);
	this->add_module("interleaver_bit", params_BFER_ite.n_threads);
	this->add_module("interleaver_llr", params_BFER_ite.n_threads);
}

template <typename B, typename R, typename Q>
void BFER_ite<B,R,Q>
::__build_communication_chain(const int tid)
{
	// build the objects
	source          [tid] = build_source     (tid);
	crc             [tid] = build_crc        (tid);
	codec           [tid] = build_codec      (tid);
	modem           [tid] = build_modem      (tid);
	channel         [tid] = build_channel    (tid);
	quantizer       [tid] = build_quantizer  (tid);
	coset_real      [tid] = build_coset_real (tid);
	coset_bit       [tid] = build_coset_bit  (tid);
	interleaver_core[tid] = build_interleaver(tid);
	interleaver_bit [tid].reset(factory::Interleaver::build<B>(*interleaver_core[tid]));
	interleaver_llr [tid].reset(factory::Interleaver::build<Q>(*interleaver_core[tid]));

	this->set_module("source"         , tid, source         [tid]);
	this->set_module("crc"            , tid, crc            [tid]);
	this->set_module("codec"          , tid, codec          [tid]);
	this->set_module("encoder"        , tid, codec          [tid]->get_encoder());
	this->set_module("modem"          , tid, modem          [tid]);
	this->set_module("channel"        , tid, channel        [tid]);
	this->set_module("quantizer"      , tid, quantizer      [tid]);
	this->set_module("coset_real"     , tid, coset_real     [tid]);
	this->set_module("decoder_siso"   , tid, codec          [tid]->get_decoder_siso());
	this->set_module("coset_bit"      , tid, coset_bit      [tid]);
	this->set_module("interleaver_bit", tid, interleaver_bit[tid]);
	this->set_module("interleaver_llr", tid, interleaver_llr[tid]);

	if (std::static_pointer_cast<module::Decoder>(codec[tid]->get_decoder_siso()) !=
	    std::static_pointer_cast<module::Decoder>(codec[tid]->get_decoder_siho()))
		this->set_module("decoder_siho", tid, codec[tid]->get_decoder_siho());

	this->monitor_er[tid]->add_handler_check(std::bind(&module::Codec_SISO_SIHO<B,Q>::reset, codec[tid].get()));

	interleaver_core[tid]->init();
	if (interleaver_core[tid]->is_uniform())
		this->monitor_er[tid]->add_handler_check(std::bind(&tools::Interleaver_core<>::refresh,
		                                                   this->interleaver_core[tid].get()));

	if (this->params_BFER_ite.err_track_enable)
	{
		using namespace module;

		auto &source      = *this->source    [tid];
		auto &encoder     = *this->codec     [tid]->get_encoder();
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

		this->dumper[tid]->register_data(channel.get_noise(),
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
void BFER_ite<B,R,Q>
::_launch()
{
	// set current sigma
	for (auto tid = 0; tid < this->params_BFER_ite.n_threads; tid++)
	{
		this->channel[tid]->set_noise(*this->noise);
		this->modem  [tid]->set_noise(*this->noise);
		this->codec  [tid]->set_noise(*this->noise);
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Source<B>> BFER_ite<B,R,Q>
::build_source(const int tid)
{
	const auto seed_src = rd_engine_seed[tid]();

	std::unique_ptr<factory::Source::parameters> params_src(params_BFER_ite.src->clone());
	params_src->seed = seed_src;

	return std::unique_ptr<module::Source<B>>(params_src->template build<B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::CRC<B>> BFER_ite<B,R,Q>
::build_crc(const int tid)
{
	return std::unique_ptr<module::CRC<B>>(params_BFER_ite.crc->template build<B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Codec_SISO_SIHO<B,Q>> BFER_ite<B,R,Q>
::build_codec(const int tid)
{
	const auto seed_enc = rd_engine_seed[tid]();
	const auto seed_dec = rd_engine_seed[tid]();

	std::unique_ptr<factory::Codec::parameters> params_cdc(params_BFER_ite.cdc->clone());
	params_cdc->enc->seed = seed_enc;
	params_cdc->dec->seed = seed_dec;

	auto crc = this->params_BFER_ite.crc->type == "NO" ? nullptr : this->crc[tid].get();

	auto param_siso_siho = dynamic_cast<factory::Codec_SISO_SIHO::parameters*>(params_cdc.get());
	return std::unique_ptr<module::Codec_SISO_SIHO<B,Q>>(param_siso_siho->template build<B,Q>(crc));
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools ::Interleaver_core<>> BFER_ite<B,R,Q>
::build_interleaver(const int tid)
{
	const auto seed_itl = rd_engine_seed[tid]();

	std::unique_ptr<factory::Interleaver::parameters> params_itl(params_BFER_ite.itl->clone());
	params_itl->core->seed = params_BFER_ite.itl->core->uniform ? params_BFER_ite.itl->core->seed + seed_itl :
	                                                              params_BFER_ite.itl->core->seed;

	if (params_BFER_ite.err_track_revert && params_BFER_ite.itl->core->uniform)
	{
		std::stringstream s_noise;
		s_noise << std::setprecision(2) << std::fixed << this->noise->get_noise();

		params_itl->core->path = params_BFER_ite.err_track_path + "_" + s_noise.str() + ".itl";
	}

	return std::unique_ptr<tools ::Interleaver_core<>>(params_itl->core->template build<>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Modem<B,R,Q>> BFER_ite<B,R,Q>
::build_modem(const int tid)
{
	if (this->distributions != nullptr)
		return std::unique_ptr<module::Modem<B,R,Q>>(
			params_BFER_ite.mdm->template build<B,R,Q>(*this->distributions));
	else
		return std::unique_ptr<module::Modem<B,R,Q>>(
			params_BFER_ite.mdm->template build<B,R,Q>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Channel<R>> BFER_ite<B,R,Q>
::build_channel(const int tid)
{
	const auto seed_chn = rd_engine_seed[tid]();

	std::unique_ptr<factory::Channel::parameters> params_chn(params_BFER_ite.chn->clone());
	params_chn->seed = seed_chn;

	if (this->distributions != nullptr)
		return std::unique_ptr<module::Channel<R>>(params_chn->template build<R>(*this->distributions));
	else
		return std::unique_ptr<module::Channel<R>>(params_chn->template build<R>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Quantizer<R,Q>> BFER_ite<B,R,Q>
::build_quantizer(const int tid)
{
	return std::unique_ptr<module::Quantizer<R,Q>>(params_BFER_ite.qnt->template build<R,Q>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,Q>> BFER_ite<B,R,Q>
::build_coset_real(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = params_BFER_ite.cdc->N_cw;
	cst_params.n_frames = params_BFER_ite.src->n_frames;
	return std::unique_ptr<module::Coset<B,Q>>(cst_params.template build_real<B,Q>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,B>> BFER_ite<B,R,Q>
::build_coset_bit(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = params_BFER_ite.coded_monitoring ? params_BFER_ite.cdc->N_cw : params_BFER_ite.cdc->K;
	cst_params.n_frames = params_BFER_ite.src->n_frames;
	return std::unique_ptr<module::Coset<B,B>>(cst_params.template build_bit<B,B>());
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

