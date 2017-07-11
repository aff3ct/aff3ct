#include "Tools/Exception/exception.hpp"
#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_CRC.hpp"
#include "Tools/Factory/Factory_encoder_common.hpp"
#include "Tools/Factory/Factory_modem.hpp"
#include "Tools/Factory/Factory_channel.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Factory_puncturer.hpp"
#include "Tools/Factory/Coset/Factory_coset_real.hpp"
#include "Tools/Factory/Coset/Factory_coset_bit.hpp"

#include "Simulation_BFER_std.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_std<B,R,Q>
::Simulation_BFER_std(const Factory_simulation_BFER_std::chain_parameters_BFER_std<B,R,Q> &chain_params, Codec<B,Q> &codec)
: Simulation_BFER<B,R,Q>(chain_params, codec),
  chain_params(chain_params),
  simu_params(*dynamic_cast<Factory_simulation_BFER_std::simu_parameters_BFER_std*>(chain_params.sim)),

  source     (simu_params.n_threads, nullptr),
  crc        (simu_params.n_threads, nullptr),
  encoder    (simu_params.n_threads, nullptr),
  puncturer  (simu_params.n_threads, nullptr),
  modem      (simu_params.n_threads, nullptr),
  channel    (simu_params.n_threads, nullptr),
  quantizer  (simu_params.n_threads, nullptr),
  coset_real (simu_params.n_threads, nullptr),
  decoder    (simu_params.n_threads, nullptr),
  coset_bit  (simu_params.n_threads, nullptr),
  interleaver(simu_params.n_threads, nullptr),

  rd_engine_seed(simu_params.n_threads)
{
	for (auto tid = 0; tid < simu_params.n_threads; tid++)
		rd_engine_seed[tid].seed(simu_params.seed + tid);
}

template <typename B, typename R, typename Q>
Simulation_BFER_std<B,R,Q>
::~Simulation_BFER_std()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std<B,R,Q>
::_build_communication_chain(const int tid)
{
	const auto seed_src = rd_engine_seed[tid]();
	const auto seed_enc = rd_engine_seed[tid]();
	const auto seed_chn = rd_engine_seed[tid]();

	// build the objects
	source     [tid] = build_source     (tid, seed_src);
	crc        [tid] = build_crc        (tid          );
	encoder    [tid] = build_encoder    (tid, seed_enc);
	puncturer  [tid] = build_puncturer  (tid          );
	modem      [tid] = build_modem      (tid          );
	channel    [tid] = build_channel    (tid, seed_chn);
	quantizer  [tid] = build_quantizer  (tid          );
	coset_real [tid] = build_coset_real (tid          );
	decoder    [tid] = build_decoder    (tid          );
	coset_bit  [tid] = build_coset_bit  (tid          );
	interleaver[tid] = build_interleaver(tid, simu_params.seed, rd_engine_seed[tid]());

	if (interleaver[tid] != nullptr)
	{
		interleaver[tid]->init();
		if (interleaver[tid]->is_uniform())
			this->monitor[tid]->add_handler_check(std::bind(&Interleaver<int>::refresh, this->interleaver[tid]));
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std<B,R,Q>
::release_objects()
{
	const auto nthr = simu_params.n_threads;
	for (auto i = 0; i < nthr; i++) if (source     [i] != nullptr) { delete source     [i]; source     [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (crc        [i] != nullptr) { delete crc        [i]; crc        [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (encoder    [i] != nullptr) { delete encoder    [i]; encoder    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (puncturer  [i] != nullptr) { delete puncturer  [i]; puncturer  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (modem      [i] != nullptr) { delete modem      [i]; modem      [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (channel    [i] != nullptr) { delete channel    [i]; channel    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (quantizer  [i] != nullptr) { delete quantizer  [i]; quantizer  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_real [i] != nullptr) { delete coset_real [i]; coset_real [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (decoder    [i] != nullptr) { delete decoder    [i]; decoder    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_bit  [i] != nullptr) { delete coset_bit  [i]; coset_bit  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (interleaver[i] != nullptr) { delete interleaver[i]; interleaver[i] = nullptr; }

	Simulation_BFER<B,R,Q>::release_objects();
}

template <typename B, typename R, typename Q>
Source<B>* Simulation_BFER_std<B,R,Q>
::build_source(const int tid, const int seed)
{
	return Factory_source<B>::build(chain_params.src, seed);
}

template <typename B, typename R, typename Q>
CRC<B>* Simulation_BFER_std<B,R,Q>
::build_crc(const int tid)
{
	return Factory_CRC<B>::build(chain_params.crc);
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFER_std<B,R,Q>
::build_encoder(const int tid, const int seed)
{
	try
	{
		return this->codec.build_encoder(tid, interleaver[tid]);
	}
	catch (cannot_allocate const&)
	{
		return Factory_encoder_common<B>::build(*chain_params.enc, seed);
	}
}

template <typename B, typename R, typename Q>
Puncturer<B,Q>* Simulation_BFER_std<B,R,Q>
::build_puncturer(const int tid)
{
	try
	{
		return this->codec.build_puncturer(tid);
	}
	catch (cannot_allocate const&)
	{
		typename Factory_puncturer<B,Q>::puncturer_parameters pct;
		pct.K        = simu_params.K;
		pct.N        = simu_params.N;
		pct.N_pct    = simu_params.N;
		pct.n_frames = simu_params.inter_frame_level;

		return Factory_puncturer<B,Q>::build(pct);
	}
}

template <typename B, typename R, typename Q>
Interleaver<int>* Simulation_BFER_std<B,R,Q>
::build_interleaver(const int tid, const int seed, const int rd_seed)
{
	try
	{
		return this->codec.build_interleaver(tid, seed, rd_seed);
	}
	catch (cannot_allocate const&)
	{
		return nullptr;
	}
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>* Simulation_BFER_std<B,R,Q>
::build_modem(const int tid)
{
	return Factory_modem<B,R,Q>::build(chain_params.modem, this->sigma);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_BFER_std<B,R,Q>
::build_channel(const int tid, const int seed)
{
	return Factory_channel<R>::build(chain_params.chn, seed, this->sigma);
}

template <typename B, typename R, typename Q>
Quantizer<R,Q>* Simulation_BFER_std<B,R,Q>
::build_quantizer(const int tid)
{
	return Factory_quantizer<R,Q>::build(chain_params.qua, this->sigma);
}

template <typename B, typename R, typename Q>
Coset<B,Q>* Simulation_BFER_std<B,R,Q>
::build_coset_real(const int tid)
{
	return Factory_coset_real<B,Q>::build("STD", chain_params.dec->N, simu_params.inter_frame_level);
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFER_std<B,R,Q>
::build_decoder(const int tid)
{
	return this->codec.build_decoder(tid, interleaver[tid], crc[tid]);
}

template <typename B, typename R, typename Q>
Coset<B,B>* Simulation_BFER_std<B,R,Q>
::build_coset_bit(const int tid)
{
	return Factory_coset_bit<B>::build("STD", chain_params.dec->K, simu_params.inter_frame_level);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER_std<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_std<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_std<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_std<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_std<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
