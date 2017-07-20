#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Source.hpp"
#include "Factory/Module/CRC.hpp"
#include "Factory/Module/Code/Encoder.hpp"
#include "Factory/Module/Modem.hpp"
#include "Factory/Module/Channel.hpp"
#include "Factory/Module/Quantizer.hpp"
#include "Factory/Module/Coset/Coset_real.hpp"
#include "Factory/Module/Coset/Coset_bit.hpp"
#include "Factory/Module/Interleaver.hpp"

#include "Simulation_BFER_ite.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_ite<B,R,Q>
::Simulation_BFER_ite(const factory::Simulation_BFER_ite::parameters &params, Codec_SISO<B,Q> &codec)
: Simulation_BFER<B,R,Q>(params, codec),
  params(params),

  codec_siso(codec),

  source     (params.n_threads, nullptr),
  crc        (params.n_threads, nullptr),
  encoder    (params.n_threads, nullptr),
  modem      (params.n_threads, nullptr),
  channel    (params.n_threads, nullptr),
  quantizer  (params.n_threads, nullptr),
  interleaver(params.n_threads, nullptr),
  coset_real (params.n_threads, nullptr),
  siso       (params.n_threads, nullptr),
  decoder    (params.n_threads, nullptr),
  coset_bit  (params.n_threads, nullptr),

  rd_engine_seed(params.n_threads)
{
	for (auto tid = 0; tid < params.n_threads; tid++)
		rd_engine_seed[tid].seed(params.seed + tid);
}

template <typename B, typename R, typename Q>
Simulation_BFER_ite<B,R,Q>
::~Simulation_BFER_ite()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite<B,R,Q>
::_build_communication_chain(const int tid)
{
	const auto seed_src = rd_engine_seed[tid]();
	const auto seed_enc = rd_engine_seed[tid]();
	const auto seed_chn = rd_engine_seed[tid]();
//	const auto seed_itl = this->params.interleaver.uniform ? rd_engine_seed[tid]() : this->params.interleaver.seed;
	const auto seed_itl = rd_engine_seed[tid]();

	// build the objects
	source     [tid] = build_source     (tid, seed_src);
	crc        [tid] = build_crc        (tid          );
	encoder    [tid] = build_encoder    (tid, seed_enc);
	modem      [tid] = build_modem      (tid          );
	channel    [tid] = build_channel    (tid, seed_chn);
	quantizer  [tid] = build_quantizer  (tid          );
	coset_real [tid] = build_coset_real (tid          );
	siso       [tid] = build_siso       (tid          );
	decoder    [tid] = build_decoder    (tid          );
	coset_bit  [tid] = build_coset_bit  (tid          );
	interleaver[tid] = build_interleaver(tid, seed_itl);

	interleaver[tid]->init();
	if (interleaver[tid]->is_uniform())
		this->monitor[tid]->add_handler_check(std::bind(&Interleaver<int>::refresh, this->interleaver[tid]));
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite<B,R,Q>
::release_objects()
{
	const auto nthr = params.n_threads;
	for (auto i = 0; i < nthr; i++) if (source     [i] != nullptr) { delete source     [i]; source     [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (crc        [i] != nullptr) { delete crc        [i]; crc        [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (encoder    [i] != nullptr) { delete encoder    [i]; encoder    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (interleaver[i] != nullptr) { delete interleaver[i]; interleaver[i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (modem      [i] != nullptr) { delete modem      [i]; modem      [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (channel    [i] != nullptr) { delete channel    [i]; channel    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (quantizer  [i] != nullptr) { delete quantizer  [i]; quantizer  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_real [i] != nullptr) { delete coset_real [i]; coset_real [i] = nullptr; }
	for (auto i = 0; i < nthr; i++)
		if (siso[i] != nullptr)
		{
			// do not delete the siso if the decoder and the siso are the same pointers
			if (decoder[i] == nullptr || siso[i] != dynamic_cast<SISO<Q>*>(decoder[i]))
				delete siso[i];
			siso[i] = nullptr;
		}
	for (auto i = 0; i < nthr; i++) if (decoder    [i] != nullptr) { delete decoder    [i]; decoder    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_bit  [i] != nullptr) { delete coset_bit  [i]; coset_bit  [i] = nullptr; }

	Simulation_BFER<B,R,Q>::release_objects();
}

template <typename B, typename R, typename Q>
Source<B>* Simulation_BFER_ite<B,R,Q>
::build_source(const int tid, const int seed)
{
	auto src_cpy = *params.src;
	src_cpy.seed = seed;
	return factory::Source::build<B>(src_cpy);
}

template <typename B, typename R, typename Q>
CRC<B>* Simulation_BFER_ite<B,R,Q>
::build_crc(const int tid)
{
	return factory::CRC::build<B>(*params.crc);
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFER_ite<B,R,Q>
::build_encoder(const int tid, const int seed)
{
	try
	{
		return this->codec.build_encoder(tid, interleaver[tid]);
	}
	catch (cannot_allocate const&)
	{
		auto enc_cpy = *params.enc;
		enc_cpy.seed = seed;
		return factory::Encoder::build<B>(enc_cpy);
	}
}

template <typename B, typename R, typename Q>
Interleaver<int>* Simulation_BFER_ite<B,R,Q>
::build_interleaver(const int tid, const int seed)
{
	auto itl_cpy = *params.itl;
	itl_cpy.seed = seed;
	return factory::Interleaver::build<int>(itl_cpy);
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>* Simulation_BFER_ite<B,R,Q>
::build_modem(const int tid)
{
	auto mdm_cpy = *params.mdm;
	if (params.mdm->sigma == -1.f)
		mdm_cpy.sigma = this->sigma;
	return factory::Modem::build<B,R,Q>(mdm_cpy);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_BFER_ite<B,R,Q>
::build_channel(const int tid, const int seed)
{
	auto chn_cpy = *params.chn;
	if (params.chn->sigma == -1.f)
		chn_cpy.sigma = this->sigma;
	chn_cpy.seed = seed;

	return factory::Channel::build<R>(chn_cpy);
}

template <typename B, typename R, typename Q>
Quantizer<R,Q>* Simulation_BFER_ite<B,R,Q>
::build_quantizer(const int tid)
{
	auto qnt_cpy = *params.qnt;
	if (params.qnt->sigma == -1.f)
		qnt_cpy.sigma = this->sigma;

	return factory::Quantizer::build<R,Q>(qnt_cpy);
}

template <typename B, typename R, typename Q>
SISO<Q>* Simulation_BFER_ite<B,R,Q>
::build_siso(const int tid)
{
	return codec_siso.build_siso(tid, interleaver[tid], crc[tid]);
}


template <typename B, typename R, typename Q>
Coset<B,Q>* Simulation_BFER_ite<B,R,Q>
::build_coset_real(const int tid)
{
	return factory::Coset_real::build<B,Q>("STD", params.dec->N_cw, params.src->n_frames);
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFER_ite<B,R,Q>
::build_decoder(const int tid)
{
	return this->codec.build_decoder(tid, interleaver[tid], crc[tid]);
}

template <typename B, typename R, typename Q>
Coset<B,B>* Simulation_BFER_ite<B,R,Q>
::build_coset_bit(const int tid)
{
	return factory::Coset_bit::build<B>("STD", params.dec->K, params.src->n_frames);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
