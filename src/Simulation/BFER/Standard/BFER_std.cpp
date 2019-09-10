#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Coset/Coset.hpp"

#include "Simulation/BFER/Standard/BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_std<B,R,Q>
::BFER_std(const factory::BFER_std::parameters &params_BFER_std)
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
	source     [tid] = build_source    (tid);
	crc        [tid] = build_crc       (tid);
	codec      [tid] = build_codec     (tid);
	modem      [tid] = build_modem     (tid);
	channel    [tid] = build_channel   (tid);
	quantizer  [tid] = build_quantizer (tid);
	coset_real [tid] = build_coset_real(tid);
	coset_bit  [tid] = build_coset_bit (tid);

	this->set_module("source"    , tid, source    [tid]);
	this->set_module("crc"       , tid, crc       [tid]);
	this->set_module("encoder"   , tid, codec     [tid]->get_encoder());
	this->set_module("puncturer" , tid, codec     [tid]->get_puncturer());
	this->set_module("modem"     , tid, modem     [tid]);
	this->set_module("channel"   , tid, channel   [tid]);
	this->set_module("quantizer" , tid, quantizer [tid]);
	this->set_module("coset_real", tid, coset_real[tid]);
	this->set_module("decoder"   , tid, codec     [tid]->get_decoder_siho());
	this->set_module("coset_bit" , tid, coset_bit [tid]);

	this->monitor_er[tid]->add_handler_check(std::bind(&module::Codec_SIHO<B,Q>::reset, codec[tid].get()));

	try
	{
		auto& interleaver = codec[tid]->get_interleaver(); // can raise an exceptions
		interleaver->init();
		if (interleaver->is_uniform())
			this->monitor_er[tid]->add_handler_check(std::bind(&tools::Interleaver_core<>::refresh, interleaver.get()));

		if (this->params_BFER_std.err_track_enable && interleaver->is_uniform())
			this->dumper[tid]->register_data(interleaver->get_lut(),
			                                 this->params_BFER_std.err_track_threshold,
			                                 "itl",
			                                 false,
			                                 this->params_BFER_std.src->n_frames,
			                                 {});
	}
	catch (const std::exception&) { /* do nothing if there is no interleaver */ }

	if (this->params_BFER_std.err_track_enable)
	{
		using namespace module;

		auto &source  = *this->source [tid];
		auto &encoder = *this->codec  [tid]->get_encoder();
		auto &channel = *this->channel[tid];

		source[src::tsk::generate].set_autoalloc(true);
		auto src_data = (B*)(source[src::sck::generate::U_K].get_dataptr());
		auto src_bytes = source[src::sck::generate::U_K].get_databytes();
		auto src_size = (src_bytes / sizeof(B)) / this->params_BFER_std.src->n_frames;
		this->dumper[tid]->register_data(src_data,
		                                 (unsigned int)src_size,
		                                 this->params_BFER_std.err_track_threshold,
		                                 "src",
		                                 false,
		                                 this->params_BFER_std.src->n_frames,
		                                 {});

		encoder[enc::tsk::encode].set_autoalloc(true);
		auto enc_data = (B*)(encoder[enc::sck::encode::X_N].get_dataptr());
		auto enc_bytes = encoder[enc::sck::encode::X_N].get_databytes();
		auto enc_size = (enc_bytes / sizeof(B)) / this->params_BFER_std.src->n_frames;
		this->dumper[tid]->register_data(enc_data,
		                                 (unsigned int)enc_size,
		                                 this->params_BFER_std.err_track_threshold,
		                                 "enc",
		                                 false,
		                                 this->params_BFER_std.src->n_frames,
		                                 {(unsigned)this->params_BFER_std.cdc->enc->K});

		this->dumper[tid]->register_data(channel.get_noise(),
		                                 this->params_BFER_std.err_track_threshold,
		                                 "chn",
		                                 true,
		                                 this->params_BFER_std.src->n_frames,
		                                 {});
	}
}

template <typename B, typename R, typename Q>
void BFER_std<B,R,Q>
::_launch()
{
	// set current sigma
	for (auto tid = 0; tid < this->params_BFER_std.n_threads; tid++)
	{
		this->channel[tid]->set_noise(*this->noise);
		this->modem  [tid]->set_noise(*this->noise);
		this->codec  [tid]->set_noise(*this->noise);
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Source<B>> BFER_std<B,R,Q>
::build_source(const int tid)
{
	const auto seed_src = rd_engine_seed[tid]();

	std::unique_ptr<factory::Source::parameters> params_src(params_BFER_std.src->clone());
	params_src->seed = seed_src;

	return std::unique_ptr<module::Source<B>>(params_src->template build<B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::CRC<B>> BFER_std<B,R,Q>
::build_crc(const int tid)
{
	return std::unique_ptr<module::CRC<B>>(params_BFER_std.crc->template build<B>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Codec_SIHO<B,Q>> BFER_std<B,R,Q>
::build_codec(const int tid)
{
	const auto seed_enc = rd_engine_seed[tid]();
	const auto seed_dec = rd_engine_seed[tid]();

	std::unique_ptr<factory::Codec::parameters> params_cdc(params_BFER_std.cdc->clone());
	params_cdc->enc->seed = seed_enc;
	params_cdc->dec->seed = seed_dec;

	if (params_cdc->itl != nullptr)
	{
		if (params_BFER_std.err_track_revert && params_cdc->itl->core->uniform)
		{
			std::stringstream s_noise;
			s_noise << std::setprecision(2) << std::fixed << this->noise->get_noise();

			params_cdc->itl->core->type = "USER";
			params_cdc->itl->core->path = params_BFER_std.err_track_path + "_" + s_noise.str() + ".itl";
		}
		else if (params_cdc->itl->core->uniform)
		{
			const auto seed_itl = rd_engine_seed[tid]() + params_cdc->itl->core->seed;
			params_cdc->itl->core->seed = seed_itl;
		}
	}

	auto crc = this->params_BFER_std.crc->type == "NO" ? nullptr : this->crc[tid].get();

	auto param_siho = dynamic_cast<factory::Codec_SIHO::parameters*>(params_cdc.get());
	return std::unique_ptr<module::Codec_SIHO<B,Q>>(param_siho->template build<B, Q>(crc));
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Modem<B,R,R>> BFER_std<B,R,Q>
::build_modem(const int tid)
{
	if (this->distributions != nullptr)
		return std::unique_ptr<module::Modem<B,R,R>>(
			params_BFER_std.mdm->template build<B,R,R>(*this->distributions));
	else
		return std::unique_ptr<module::Modem<B,R,R>>(
			params_BFER_std.mdm->template build<B,R,R>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Channel<R>> BFER_std<B,R,Q>
::build_channel(const int tid)
{
	const auto seed_chn = rd_engine_seed[tid]();

	std::unique_ptr<factory::Channel::parameters> params_chn(this->params_BFER_std.chn->clone());
	params_chn->seed = seed_chn;

	if (this->distributions != nullptr)
		return std::unique_ptr<module::Channel<R>>(params_chn->template build<R>(*this->distributions));
	else
		return std::unique_ptr<module::Channel<R>>(params_chn->template build<R>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Quantizer<R,Q>> BFER_std<B,R,Q>
::build_quantizer(const int tid)
{
	return std::unique_ptr<module::Quantizer<R,Q>>(params_BFER_std.qnt->template build<R,Q>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,Q>> BFER_std<B,R,Q>
::build_coset_real(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = params_BFER_std.cdc->N_cw;
	cst_params.n_frames = params_BFER_std.src->n_frames;
	return std::unique_ptr<module::Coset<B,Q>>(cst_params.template build_real<B,Q>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,B>> BFER_std<B,R,Q>
::build_coset_bit(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = this->params_BFER_std.coded_monitoring ? params_BFER_std.cdc->N_cw : params_BFER_std.cdc->K;
	cst_params.n_frames = params_BFER_std.src->n_frames;
	return std::unique_ptr<module::Coset<B,B>>(cst_params.template build_bit<B,B>());
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
