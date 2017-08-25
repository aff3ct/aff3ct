#include "Tools/Exception/exception.hpp"

#include "Factory/Tools/Interleaver/Interleaver_core.hpp"
#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/CRC/CRC.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Coset/Coset.hpp"

#include "BFER_std.hpp"

namespace aff3ct
{
namespace simulation
{
template <class C, typename B, typename R, typename Q, int CRC, int ITL>
BFER_std<C,B,R,Q,CRC,ITL>
::BFER_std(const factory::BFER_std::parameters<C> &params)
: BFER<C,B,R,Q>(params),
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

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
BFER_std<C,B,R,Q,CRC,ITL>
::~BFER_std()
{
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
void BFER_std<C,B,R,Q,CRC,ITL>
::_build_communication_chain(const int tid)
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
			this->dumper[tid]->register_data(interleaver->get_lut(), "itl", false, {});
	}
	catch (const std::exception&) { /* do nothing if the is no interleaver */ }

	if (this->params.err_track_enable)
		this->dumper[tid]->register_data(this->channel[tid]->get_noise(), "chn", true, {});
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
void BFER_std<C,B,R,Q,CRC,ITL>
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

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
void BFER_std<C,B,R,Q,CRC,ITL>
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

	BFER<C,B,R,Q>::release_objects();
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
module::Source<B>* BFER_std<C,B,R,Q,CRC,ITL>
::build_source(const int tid)
{
	const auto seed_src = rd_engine_seed[tid]();

	auto params_src = this->params.src;
	params_src.seed = seed_src;
	return factory::Source::build<B>(params_src);
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
module::CRC<B>* BFER_std<C,B,R,Q,CRC,ITL>
::build_crc(const int tid)
{
	return factory::CRC::build<B>(params.crc);
}

template <class C, typename B, typename Q, int CRC, int ITL>
struct Codec_SIHO
{
	static module::Codec_SIHO<B,Q>* build(const factory::BFER_std::parameters<C> &params, const float snr,
	                                      std::mt19937 &rd_engine_seed, module::CRC<B> *crc = nullptr)
	{
		const auto seed_enc = rd_engine_seed();

		auto params_cdc = params.cdc;
		params_cdc.enc.seed = seed_enc;
		return params_cdc.template build<B,Q>();
	}
};

template <class C, typename B, typename Q>
struct Codec_SIHO<C,B,Q,1,0>
{
	static module::Codec_SIHO<B,Q>* build(const factory::BFER_std::parameters<C> &params, const float snr,
	                                      std::mt19937 &rd_engine_seed, module::CRC<B> *crc = nullptr)
	{
		const auto seed_enc = rd_engine_seed();

		auto params_cdc = params.cdc;
		params_cdc.enc.seed = seed_enc;
		return params.crc.type == "NO" || crc == nullptr ? params_cdc.template build<B,Q>(   ) :
		                                                   params_cdc.template build<B,Q>(crc);
	}
};

template <class C, typename B, typename Q>
struct Codec_SIHO<C,B,Q,0,1>
{
	static module::Codec_SIHO<B,Q>* build(const factory::BFER_std::parameters<C> &params, const float snr,
	                                      std::mt19937 &rd_engine_seed, module::CRC<B> *crc = nullptr)
	{
		const auto seed_enc = rd_engine_seed();
		const auto seed_itl = rd_engine_seed();

		auto params_cdc = params.cdc;
		if (params_cdc.enc.itl.core.uniform)
		{
			params_cdc.enc.itl.core.seed = seed_itl;
			params_cdc.dec.itl.core.seed = seed_itl;
		}

		if (params.err_track_revert)
		{
			params_cdc.enc.itl.core.type = "USER";
			params_cdc.dec.itl.core.type = "USER";

			params_cdc.enc.itl.core.path = params.err_track_path + "_" + std::to_string(snr) + ".itl";
			params_cdc.dec.itl.core.path = params.err_track_path + "_" + std::to_string(snr) + ".itl";
		}

		params_cdc.enc.seed = seed_enc;
		return params_cdc.template build<B,Q>();
	}
};

template <class C, typename B, typename Q>
struct Codec_SIHO<C,B,Q,1,1>
{
	static module::Codec_SIHO<B,Q>* build(const factory::BFER_std::parameters<C> &params, const float snr,
	                                      std::mt19937 &rd_engine_seed, module::CRC<B> *crc = nullptr)
	{
		const auto seed_enc = rd_engine_seed();
		const auto seed_itl = rd_engine_seed();

		auto params_cdc = params.cdc;
		if (params_cdc.enc.itl.core.uniform)
		{
			params_cdc.enc.itl.core.seed = seed_itl;
			params_cdc.dec.itl.core.seed = seed_itl;
		}

		if (params.err_track_revert)
		{
			params_cdc.enc.itl.core.type = "USER";
			params_cdc.dec.itl.core.type = "USER";

			params_cdc.enc.itl.core.path = params.err_track_path + "_" + std::to_string(snr) + ".itl";
			params_cdc.dec.itl.core.path = params.err_track_path + "_" + std::to_string(snr) + ".itl";
		}

		params_cdc.enc.seed = seed_enc;
		return params.crc.type == "NO" || crc == nullptr ? params_cdc.template build<B,Q>(   ) :
		                                                   params_cdc.template build<B,Q>(crc);
	}
};

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
module::Codec_SIHO<B,Q>* BFER_std<C,B,R,Q,CRC,ITL>
::build_codec(const int tid)
{
	return Codec_SIHO<C,B,Q,CRC,ITL>::build(params, this->snr_b, rd_engine_seed[tid], this->crc[tid]);
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
module::Modem<B,R,R>* BFER_std<C,B,R,Q,CRC,ITL>
::build_modem(const int tid)
{
	return factory::Modem::build<B,R,R>(params.mdm);
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
module::Channel<R>* BFER_std<C,B,R,Q,CRC,ITL>
::build_channel(const int tid)
{
	const auto seed_chn = rd_engine_seed[tid]();

	auto params_chn = this->params.chn;
	params_chn.seed = seed_chn;
	return factory::Channel::build<R>(params_chn);
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
module::Quantizer<R,Q>* BFER_std<C,B,R,Q,CRC,ITL>
::build_quantizer(const int tid)
{
	return factory::Quantizer::build<R,Q>(params.qnt);
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
module::Coset<B,Q>* BFER_std<C,B,R,Q,CRC,ITL>
::build_coset_real(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = params.cdc.dec.N_cw;
	cst_params.n_frames = params.src.n_frames;
	return factory::Coset::build_real<B,Q>(cst_params);
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
module::Coset<B,B>* BFER_std<C,B,R,Q,CRC,ITL>
::build_coset_bit(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = this->params.coded_monitoring ? params.cdc.dec.N_cw : params.cdc.dec.K;
	cst_params.n_frames = params.src.n_frames;
	return factory::Coset::build_bit<B,B>(cst_params);
}
}
}
