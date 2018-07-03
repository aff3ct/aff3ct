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

  source          (params_BFER_ite.n_threads, nullptr),
  crc             (params_BFER_ite.n_threads, nullptr),
  codec           (params_BFER_ite.n_threads, nullptr),
  modem           (params_BFER_ite.n_threads, nullptr),
  channel         (params_BFER_ite.n_threads, nullptr),
  quantizer       (params_BFER_ite.n_threads, nullptr),
  coset_real      (params_BFER_ite.n_threads, nullptr),
  coset_bit       (params_BFER_ite.n_threads, nullptr),
  interleaver_core(params_BFER_ite.n_threads, nullptr),
  interleaver_bit (params_BFER_ite.n_threads, nullptr),
  interleaver_llr (params_BFER_ite.n_threads, nullptr),

  rd_engine_seed(params_BFER_ite.n_threads)
{
	for (auto tid = 0; tid < params_BFER_ite.n_threads; tid++)
		rd_engine_seed[tid].seed(params_BFER_ite.local_seed + tid);

	this->modules["source"         ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["crc"            ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["codec"          ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["encoder"        ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["modem"          ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["channel"        ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["quantizer"      ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["coset_real"     ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["decoder_siso"   ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["decoder_siho"   ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["coset_bit"      ] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["interleaver_bit"] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
	this->modules["interleaver_llr"] = std::vector<module::Module*>(params_BFER_ite.n_threads, nullptr);
}

template <typename B, typename R, typename Q>
BFER_ite<B,R,Q>
::~BFER_ite()
{
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
	interleaver_bit [tid] = factory::Interleaver::build<B>(*interleaver_core[tid]);
	interleaver_llr [tid] = factory::Interleaver::build<Q>(*interleaver_core[tid]);

	this->modules["source"         ][tid] = source         [tid];
	this->modules["crc"            ][tid] = crc            [tid];
	this->modules["codec"          ][tid] = codec          [tid];
	this->modules["encoder"        ][tid] = codec          [tid]->get_encoder();
	this->modules["modem"          ][tid] = modem          [tid];
	this->modules["channel"        ][tid] = channel        [tid];
	this->modules["quantizer"      ][tid] = quantizer      [tid];
	this->modules["coset_real"     ][tid] = coset_real     [tid];
	this->modules["decoder_siso"   ][tid] = codec          [tid]->get_decoder_siso();
	this->modules["coset_bit"      ][tid] = coset_bit      [tid];
	this->modules["interleaver_bit"][tid] = interleaver_bit[tid];
	this->modules["interleaver_llr"][tid] = interleaver_llr[tid];

	if (dynamic_cast<module::Decoder*>(codec[tid]->get_decoder_siso()) !=
	    dynamic_cast<module::Decoder*>(codec[tid]->get_decoder_siho()))
		this->modules["decoder_siho"][tid] = codec[tid]->get_decoder_siho();

	this->monitor_er[tid]->add_handler_check(std::bind(&module::Codec_SISO_SIHO<B,Q>::reset, codec[tid]));

	interleaver_core[tid]->init();
	if (interleaver_core[tid]->is_uniform())
		this->monitor_er[tid]->add_handler_check(std::bind(&tools::Interleaver_core<>::refresh,
		                                                this->interleaver_core[tid]));

	if (this->params_BFER_ite.err_track_enable)
	{
		using namespace module;

		auto &source      = *this->source    [tid];
		auto &encoder     = *this->codec     [tid]->get_encoder();
		auto &channel     = *this->channel   [tid];
		auto &interleaver = *interleaver_core[tid];

		source[src::tsk::generate].set_autoalloc(true);
		auto src_data = (B*)(source[src::sck::generate::U_K].get_dataptr());
		auto src_size = (source[src::sck::generate::U_K].get_databytes() / sizeof(B)) / this->params_BFER_ite.src->n_frames;
		this->dumper[tid]->register_data(src_data, (unsigned int)src_size, this->params_BFER_ite.err_track_threshold, "src", false, this->params_BFER_ite.src->n_frames, {});

		encoder[enc::tsk::encode].set_autoalloc(true);
		auto enc_data = (B*)(encoder[enc::sck::encode::X_N].get_dataptr());
		auto enc_size = (encoder[enc::sck::encode::X_N].get_databytes() / sizeof(B)) / this->params_BFER_ite.src->n_frames;
		this->dumper[tid]->register_data(enc_data, (unsigned int)enc_size, this->params_BFER_ite.err_track_threshold, "enc", false, this->params_BFER_ite.src->n_frames,
		                                 {(unsigned)this->params_BFER_ite.cdc->enc->K});

		this->dumper[tid]->register_data(channel.get_noise(), this->params_BFER_ite.err_track_threshold, "chn", true, this->params_BFER_ite.src->n_frames, {});

		if (interleaver_core[tid]->is_uniform())
			this->dumper[tid]->register_data(interleaver.get_lut(), this->params_BFER_ite.err_track_threshold, "itl", false, this->params_BFER_ite.src->n_frames, {});
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
void BFER_ite<B,R,Q>
::release_objects()
{
	const auto nthr = params_BFER_ite.n_threads;
	for (auto i = 0; i < nthr; i++) if (source          [i] != nullptr) { delete source          [i]; source          [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (crc             [i] != nullptr) { delete crc             [i]; crc             [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (codec           [i] != nullptr) { delete codec           [i]; codec           [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (interleaver_bit [i] != nullptr) { delete interleaver_bit [i]; interleaver_bit [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (interleaver_llr [i] != nullptr) { delete interleaver_llr [i]; interleaver_llr [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (interleaver_core[i] != nullptr) { delete interleaver_core[i]; interleaver_core[i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (modem           [i] != nullptr) { delete modem           [i]; modem           [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (channel         [i] != nullptr) { delete channel         [i]; channel         [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (quantizer       [i] != nullptr) { delete quantizer       [i]; quantizer       [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_real      [i] != nullptr) { delete coset_real      [i]; coset_real      [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_bit       [i] != nullptr) { delete coset_bit       [i]; coset_bit       [i] = nullptr; }

	BFER<B,R,Q>::release_objects();
}

template <typename B, typename R, typename Q>
module::Source<B>* BFER_ite<B,R,Q>
::build_source(const int tid)
{
	const auto seed_src = rd_engine_seed[tid]();

	auto params_src = params_BFER_ite.src->clone();
	params_src->seed = seed_src;
	auto s = params_src->template build<B>();
	delete params_src;
	return s;
}

template <typename B, typename R, typename Q>
module::CRC<B>* BFER_ite<B,R,Q>
::build_crc(const int tid)
{
	return params_BFER_ite.crc->template build<B>();
}

template <typename B, typename R, typename Q>
module::Codec_SISO_SIHO<B,Q>* BFER_ite<B,R,Q>
::build_codec(const int tid)
{
	const auto seed_enc = rd_engine_seed[tid]();

	auto params_cdc = params_BFER_ite.cdc->clone();
	params_cdc->enc->seed = seed_enc;
	auto crc = this->params_BFER_ite.crc->type == "NO" ? nullptr : this->crc[tid];
	auto c = params_cdc->template build<B,Q>(crc);
	delete params_cdc;
	return c;
}

template <typename B, typename R, typename Q>
tools ::Interleaver_core<>* BFER_ite<B,R,Q>
::build_interleaver(const int tid)
{
	const auto seed_itl = rd_engine_seed[tid]();

	auto params_itl = params_BFER_ite.itl->clone();
	params_itl->core->seed = params_BFER_ite.itl->core->uniform ? seed_itl : params_BFER_ite.itl->core->seed;
	if (params_BFER_ite.err_track_revert && params_BFER_ite.itl->core->uniform)
	{
		std::stringstream s_noise;
		s_noise << std::setprecision(2) << std::fixed << this->noise->get_noise();

		params_itl->core->path = params_BFER_ite.err_track_path + "_" + s_noise.str() + ".itl";
	}

	auto i = params_itl->core->template build<>();
	delete params_itl;
	return i;
}

template <typename B, typename R, typename Q>
module::Modem<B,R,Q>* BFER_ite<B,R,Q>
::build_modem(const int tid)
{
	return params_BFER_ite.mdm->template build<B,R,Q>(this->distributions, this->params_BFER_ite.chn->type);
}

template <typename B, typename R, typename Q>
module::Channel<R>* BFER_ite<B,R,Q>
::build_channel(const int tid)
{
	const auto seed_chn = rd_engine_seed[tid]();

	auto params_chn = params_BFER_ite.chn->clone();
	params_chn->seed = seed_chn;
	auto c = params_chn->template build<R>(this->distributions);
	delete params_chn;
	return c;
}

template <typename B, typename R, typename Q>
module::Quantizer<R,Q>* BFER_ite<B,R,Q>
::build_quantizer(const int tid)
{
	return params_BFER_ite.qnt->template build<R,Q>();
}

template <typename B, typename R, typename Q>
module::Coset<B,Q>* BFER_ite<B,R,Q>
::build_coset_real(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = params_BFER_ite.cdc->N_cw;
	cst_params.n_frames = params_BFER_ite.src->n_frames;
	return cst_params.template build_real<B,Q>();
}

template <typename B, typename R, typename Q>
module::Coset<B,B>* BFER_ite<B,R,Q>
::build_coset_bit(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = params_BFER_ite.coded_monitoring ? params_BFER_ite.cdc->N_cw : params_BFER_ite.cdc->K;
	cst_params.n_frames = params_BFER_ite.src->n_frames;
	return cst_params.template build_bit<B,B>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

