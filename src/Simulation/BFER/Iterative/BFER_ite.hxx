#include "Tools/Exception/exception.hpp"

#include "Factory/Tools/Interleaver/Interleaver_core.hpp"
#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/CRC/CRC.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Coset/Coset.hpp"

#include "BFER_ite.hpp"

namespace aff3ct
{
namespace simulation
{
template <class C, typename B, typename R, typename Q, int CRC>
BFER_ite<C,B,R,Q,CRC>
::BFER_ite(const factory::BFER_ite::parameters<C> &params)
: BFER<C,B,R,Q>(params),
  params(params),

  source          (params.n_threads, nullptr),
  crc             (params.n_threads, nullptr),
  codec           (params.n_threads, nullptr),
  modem           (params.n_threads, nullptr),
  channel         (params.n_threads, nullptr),
  quantizer       (params.n_threads, nullptr),
  coset_real      (params.n_threads, nullptr),
  coset_bit       (params.n_threads, nullptr),
  interleaver_core(params.n_threads, nullptr),
  interleaver_bit (params.n_threads, nullptr),
  interleaver_llr (params.n_threads, nullptr),

  rd_engine_seed(params.n_threads)
{
	for (auto tid = 0; tid < params.n_threads; tid++)
		rd_engine_seed[tid].seed(params.local_seed + tid);

	this->modules["source"         ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["crc"            ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["codec"          ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["encoder"        ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["modem"          ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["channel"        ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["quantizer"      ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["coset_real"     ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["decoder_siso"   ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["decoder_siho"   ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["coset_bit"      ] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["interleaver_bit"] = std::vector<module::Module*>(params.n_threads, nullptr);
	this->modules["interleaver_llr"] = std::vector<module::Module*>(params.n_threads, nullptr);
}

template <class C, typename B, typename R, typename Q, int CRC>
BFER_ite<C,B,R,Q,CRC>
::~BFER_ite()
{
}

template <class C, typename B, typename R, typename Q, int CRC>
void BFER_ite<C,B,R,Q,CRC>
::_build_communication_chain(const int tid)
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
	this->modules["decoder_siho"   ][tid] = codec          [tid]->get_decoder_siho();
	this->modules["coset_bit"      ][tid] = coset_bit      [tid];
	this->modules["interleaver_bit"][tid] = interleaver_bit[tid];
	this->modules["interleaver_llr"][tid] = interleaver_llr[tid];

	interleaver_core[tid]->init();
	if (interleaver_core[tid]->is_uniform())
		this->monitor[tid]->add_handler_check(std::bind(&tools::Interleaver_core<>::refresh,
		                                                this->interleaver_core[tid]));

	if (this->params.err_track_enable && interleaver_core[tid]->is_uniform())
		this->dumper[tid]->register_data(interleaver_core[tid]->get_lut(), "itl", false, {});

	if (this->params.err_track_enable)
		this->dumper[tid]->register_data(this->channel[tid]->get_noise(), "chn", true, {});
}

template <class C, typename B, typename R, typename Q, int CRC>
void BFER_ite<C,B,R,Q,CRC>
::_launch()
{
	// set current sigma
	for (auto tid = 0; tid < this->params.n_threads; tid++)
	{
		this->channel[tid]->set_sigma(this->sigma);
		this->modem  [tid]->set_sigma(this->sigma);
		this->codec  [tid]->set_sigma(this->sigma);
	}
}

template <class C, typename B, typename R, typename Q, int CRC>
void BFER_ite<C,B,R,Q,CRC>
::release_objects()
{
	const auto nthr = params.n_threads;
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

	BFER<C,B,R,Q>::release_objects();
}

template <class C, typename B, typename R, typename Q, int CRC>
module::Source<B>* BFER_ite<C,B,R,Q,CRC>
::build_source(const int tid)
{
	const auto seed_src = rd_engine_seed[tid]();

	auto params_src = params.src;
	params_src.seed = seed_src;
	return factory::Source::build<B>(params_src);
}

template <class C, typename B, typename R, typename Q, int CRC>
module::CRC<B>* BFER_ite<C,B,R,Q,CRC>
::build_crc(const int tid)
{
	return factory::CRC::build<B>(params.crc);
}

template <class C, typename B, typename Q, int CRC>
struct Codec_SISO_SIHO
{
	static module::Codec_SISO_SIHO<B,Q>* build(const factory::BFER_ite::parameters<C> &params, const int seed_enc,
	                                           module::CRC<B> *crc = nullptr)
	{
		auto params_cdc = params.cdc;
		params_cdc.enc.seed = seed_enc;
		return params_cdc.template build<B,Q>();
	}
};

template <class C, typename B, typename Q>
struct Codec_SISO_SIHO<C,B,Q,1>
{
	static module::Codec_SISO_SIHO<B,Q>* build(const factory::BFER_ite::parameters<C> &params, const int seed_enc,
	                                           module::CRC<B> *crc = nullptr)
	{
		auto params_cdc = params.cdc;
		params_cdc.enc.seed = seed_enc;
		return params.crc.type == "NO" || crc == nullptr ? params_cdc.template build<B,Q>(   ) :
		                                                   params_cdc.template build<B,Q>(crc);
	}
};

template <class C, typename B, typename R, typename Q, int CRC>
module::Codec_SISO_SIHO<B,Q>* BFER_ite<C,B,R,Q,CRC>
::build_codec(const int tid)
{
	const auto seed_enc = rd_engine_seed[tid]();
	return Codec_SISO_SIHO<C,B,Q,CRC>::build(params, seed_enc, this->crc[tid]);
}

template <class C, typename B, typename R, typename Q, int CRC>
tools ::Interleaver_core<>* BFER_ite<C,B,R,Q,CRC>
::build_interleaver(const int tid)
{
	const auto seed_itl = rd_engine_seed[tid]();

	auto params_itl = params.itl;
	params_itl.core.seed = params.itl.core.uniform ? seed_itl : params.itl.core.seed;
	if (params.err_track_revert)
		params_itl.core.path = params.err_track_path + "_" + std::to_string(this->snr_b) + ".itl";
	return factory::Interleaver_core::build<>(params_itl.core);
}

template <class C, typename B, typename R, typename Q, int CRC>
module::Modem<B,R,Q>* BFER_ite<C,B,R,Q,CRC>
::build_modem(const int tid)
{
	return factory::Modem::build<B,R,Q>(params.mdm);
}

template <class C, typename B, typename R, typename Q, int CRC>
module::Channel<R>* BFER_ite<C,B,R,Q,CRC>
::build_channel(const int tid)
{
	const auto seed_chn = rd_engine_seed[tid]();

	auto params_chn = params.chn;
	params_chn.seed = seed_chn;
	return factory::Channel::build<R>(params_chn);
}

template <class C, typename B, typename R, typename Q, int CRC>
module::Quantizer<R,Q>* BFER_ite<C,B,R,Q,CRC>
::build_quantizer(const int tid)
{
	return factory::Quantizer::build<R,Q>(params.qnt);
}

template <class C, typename B, typename R, typename Q, int CRC>
module::Coset<B,Q>* BFER_ite<C,B,R,Q,CRC>
::build_coset_real(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = params.cdc.N_cw;
	cst_params.n_frames = params.src.n_frames;
	return factory::Coset::build_real<B,Q>(cst_params);
}

template <class C, typename B, typename R, typename Q, int CRC>
module::Coset<B,B>* BFER_ite<C,B,R,Q,CRC>
::build_coset_bit(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = params.coded_monitoring ? params.cdc.N_cw : params.cdc.K;
	cst_params.n_frames = params.src.n_frames;
	return factory::Coset::build_bit<B>(cst_params);
}
}
}
