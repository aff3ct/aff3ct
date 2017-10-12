#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Coset/Coset.hpp"

#include "BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_std<B,R,Q>
::BFER_std(const factory::BFER_std::parameters &params)
: BFER<B,R,Q>(params),
  params(params),

  source    (params.n_threads, nullptr),
  crc       (params.n_threads, nullptr),
  codec     (params.n_threads, nullptr),
  modem     (params.n_threads, nullptr),
  channel   (params.n_threads, nullptr),
  quantizer (params.n_threads, nullptr),
  coset_real(params.n_threads, nullptr),
  coset_bit (params.n_threads, nullptr),

  rd_engine_seed(params.n_threads)
{
	for (auto tid = 0; tid < params.n_threads; tid++)
		rd_engine_seed[tid].seed(params.local_seed + tid);

	this->modules["source"    ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["crc"       ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["encoder"   ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["puncturer" ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["modem"     ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["channel"   ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["quantizer" ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["coset_real"] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["decoder"   ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["coset_bit" ] = std::vector<module::Module*>(params.n_threads, nullptr);
}

template <typename B, typename R, typename Q>
BFER_std<B,R,Q>
::~BFER_std()
{
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

	this->modules["source"    ][tid] = source    [tid];
	this->modules["crc"       ][tid] = crc       [tid];
	this->modules["encoder"   ][tid] = codec     [tid]->get_encoder();
	this->modules["puncturer" ][tid] = codec     [tid]->get_puncturer();
	this->modules["modem"     ][tid] = modem     [tid];
	this->modules["channel"   ][tid] = channel   [tid];
	this->modules["quantizer" ][tid] = quantizer [tid];
	this->modules["coset_real"][tid] = coset_real[tid];
	this->modules["decoder"   ][tid] = codec     [tid]->get_decoder_siho();
	this->modules["coset_bit" ][tid] = coset_bit [tid];

	this->monitor[tid]->add_handler_check(std::bind(&module::Codec_SIHO<B,Q>::reset, codec[tid]));

	try
	{
		auto *interleaver = codec[tid]->get_interleaver(); // can raise an exceptions
		interleaver->init();
		if (interleaver->is_uniform())
			this->monitor[tid]->add_handler_check(std::bind(&tools::Interleaver_core<>::refresh, interleaver));

		if (this->params.err_track_enable && interleaver->is_uniform())
			this->dumper[tid]->register_data(interleaver->get_lut(), "itl", false, this->params.src->n_frames, {});
	}
	catch (const std::exception&) { /* do nothing if the is no interleaver */ }

	if (this->params.err_track_enable)
	{
		using namespace module;

		auto &source  = *this->source [tid];
		auto &encoder = *this->codec  [tid]->get_encoder();
		auto &channel = *this->channel[tid];

		source[src::tsk::generate].set_autoalloc(true);
		auto src_data = (B*)(source[src::tsk::generate][src::sck::generate::U_K].get_dataptr());
		auto src_size = (source[src::tsk::generate][src::sck::generate::U_K].get_databytes() / sizeof(B)) / this->params.src->n_frames;
		this->dumper[tid]->register_data(src_data, src_size, "src", false, this->params.src->n_frames, {});

		encoder[enc::tsk::encode].set_autoalloc(true);
		auto enc_data = (B*)(encoder[enc::tsk::encode][enc::sck::encode::X_N].get_dataptr());
		auto enc_size = (encoder[enc::tsk::encode][enc::sck::encode::X_N].get_databytes() / sizeof(B)) / this->params.src->n_frames;
		this->dumper[tid]->register_data(enc_data, enc_size, "enc", false, this->params.src->n_frames,
		                                 {(unsigned)this->params.cdc->enc->K});

		this->dumper[tid]->register_data(channel.get_noise(), "chn", true, this->params.src->n_frames, {});
	}
}

template <typename B, typename R, typename Q>
void BFER_std<B,R,Q>
::_launch()
{
	// set current sigma
	for (auto tid = 0; tid < this->params.n_threads; tid++)
	{
		this->channel[tid]->set_sigma(                                                           this->sigma);
		this->modem  [tid]->set_sigma(this->params.mdm->complex ? this->sigma * std::sqrt(2.f) : this->sigma);
		this->codec  [tid]->set_sigma(                                                           this->sigma);
	}
}

template <typename B, typename R, typename Q>
void BFER_std<B,R,Q>
::release_objects()
{
	const auto nthr = params.n_threads;
	for (auto i = 0; i < nthr; i++) if (source    [i] != nullptr) { delete source    [i]; source    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (crc       [i] != nullptr) { delete crc       [i]; crc       [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (codec     [i] != nullptr) { delete codec     [i]; codec     [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (modem     [i] != nullptr) { delete modem     [i]; modem     [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (channel   [i] != nullptr) { delete channel   [i]; channel   [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (quantizer [i] != nullptr) { delete quantizer [i]; quantizer [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_real[i] != nullptr) { delete coset_real[i]; coset_real[i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_bit [i] != nullptr) { delete coset_bit [i]; coset_bit [i] = nullptr; }

	BFER<B,R,Q>::release_objects();
}

template <typename B, typename R, typename Q>
module::Source<B>* BFER_std<B,R,Q>
::build_source(const int tid)
{
	const auto seed_src = rd_engine_seed[tid]();

	auto params_src = params.src->clone();
	params_src->seed = seed_src;
	auto s = params_src->template build<B>();
	delete params_src;
	return s;
}

template <typename B, typename R, typename Q>
module::CRC<B>* BFER_std<B,R,Q>
::build_crc(const int tid)
{
	return params.crc->template build<B>();
}

template <typename B, typename R, typename Q>
module::Codec_SIHO<B,Q>* BFER_std<B,R,Q>
::build_codec(const int tid)
{
	const auto seed_enc = rd_engine_seed[tid]();

	auto params_cdc = params.cdc->clone();
	params_cdc->enc->seed = seed_enc;

	if (params_cdc->itl != nullptr)
	{
		if (params.err_track_revert && params_cdc->itl->core->uniform)
		{
			std::stringstream s_snr_b;
			s_snr_b << std::setprecision(2) << std::fixed << this->snr_b;

			params_cdc->itl->core->type = "USER";
			params_cdc->itl->core->path = params.err_track_path + "_" + s_snr_b.str() + ".itl";
		}
		else if (params_cdc->itl->core->uniform)
		{
			const auto seed_itl = rd_engine_seed[tid]();
			params_cdc->itl->core->seed = seed_itl;
		}
	}

	auto crc = this->params.crc->type == "NO" ? nullptr : this->crc[tid];
	auto c = params_cdc->template build<B,Q>(crc);
	delete params_cdc;
	return c;
}

template <typename B, typename R, typename Q>
module::Modem<B,R,R>* BFER_std<B,R,Q>
::build_modem(const int tid)
{
	return params.mdm->template build<B,R,R>();
}

template <typename B, typename R, typename Q>
module::Channel<R>* BFER_std<B,R,Q>
::build_channel(const int tid)
{
	const auto seed_chn = rd_engine_seed[tid]();

	auto params_chn = this->params.chn->clone();
	params_chn->seed = seed_chn;
	auto c = params_chn->template build<R>();
	delete params_chn;
	return c;
}

template <typename B, typename R, typename Q>
module::Quantizer<R,Q>* BFER_std<B,R,Q>
::build_quantizer(const int tid)
{
	return params.qnt->template build<R,Q>();
}

template <typename B, typename R, typename Q>
module::Coset<B,Q>* BFER_std<B,R,Q>
::build_coset_real(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = params.cdc->N_cw;
	cst_params.n_frames = params.src->n_frames;
	return cst_params.template build_real<B,Q>();
}

template <typename B, typename R, typename Q>
module::Coset<B,B>* BFER_std<B,R,Q>
::build_coset_bit(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = this->params.coded_monitoring ? params.cdc->N_cw : params.cdc->K;
	cst_params.n_frames = params.src->n_frames;
	return cst_params.template build_bit<B,B>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::BFER_std<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER_std<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER_std<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER_std<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER_std<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

