#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_CRC.hpp"
#include "Tools/Factory/Factory_encoder_common.hpp"
#include "Tools/Factory/Factory_modulator.hpp"
#include "Tools/Factory/Factory_channel.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Coset/Factory_coset_real.hpp"
#include "Tools/Factory/Coset/Factory_coset_bit.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"

#include "Simulation_BFER_ite.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_ite<B,R,Q>
::Simulation_BFER_ite(const parameters& params, tools::Codec_SISO<B,Q> &codec)
: Simulation_BFER<B,R,Q>(params, codec),

  codec_siso(codec),

  source     (params.simulation.n_threads, nullptr),
  crc        (params.simulation.n_threads, nullptr),
  encoder    (params.simulation.n_threads, nullptr),
  modulator  (params.simulation.n_threads, nullptr),
  channel    (params.simulation.n_threads, nullptr),
  quantizer  (params.simulation.n_threads, nullptr),
  interleaver(params.simulation.n_threads, nullptr),
  coset_real (params.simulation.n_threads, nullptr),
  siso       (params.simulation.n_threads, nullptr),
  decoder    (params.simulation.n_threads, nullptr),
  coset_bit  (params.simulation.n_threads, nullptr),

  rd_engine_seed(params.simulation.n_threads)
{
	for (auto tid = 0; tid < params.simulation.n_threads; tid++)
		rd_engine_seed[tid].seed(params.simulation.seed + tid);
}

template <typename B, typename R, typename Q>
Simulation_BFER_ite<B,R,Q>
::~Simulation_BFER_ite()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite<B,R,Q>
::build_communication_chain(const int tid)
{
	const auto seed_src =                                    rd_engine_seed[tid]();
	const auto seed_itl = this->params.interleaver.uniform ? rd_engine_seed[tid]() : this->params.interleaver.seed;
	const auto seed_enc =                                    rd_engine_seed[tid]();
	const auto seed_chn =                                    rd_engine_seed[tid]();

	// build the objects
	source     [tid] = build_source     (tid, seed_src);
	crc        [tid] = build_crc        (tid          );
	encoder    [tid] = build_encoder    (tid, seed_enc);
	interleaver[tid] = build_interleaver(tid, seed_itl);
	modulator  [tid] = build_modulator  (tid          );
	channel    [tid] = build_channel    (tid, seed_chn);
	quantizer  [tid] = build_quantizer  (tid          );
	coset_real [tid] = build_coset_real (tid          );
	siso       [tid] = build_siso       (tid          );
	decoder    [tid] = build_decoder    (tid          );
	coset_bit  [tid] = build_coset_bit  (tid          );

	interleaver[tid]->init();
	if (interleaver[tid]->is_uniform())
		this->monitor[tid]->add_handler_check(std::bind(&Interleaver<int>::refresh, this->interleaver[tid]));

	Simulation_BFER<B,R,Q>::build_communication_chain(tid);
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite<B,R,Q>
::release_objects()
{
	const auto nthr = this->params.simulation.n_threads;
	for (auto i = 0; i < nthr; i++) if (source     [i] != nullptr) { delete source     [i]; source     [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (crc        [i] != nullptr) { delete crc        [i]; crc        [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (encoder    [i] != nullptr) { delete encoder    [i]; encoder    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (interleaver[i] != nullptr) { delete interleaver[i]; interleaver[i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (modulator  [i] != nullptr) { delete modulator  [i]; modulator  [i] = nullptr; }
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
	return Factory_source<B>::build(this->params.source.type,
	                                this->params.code.K_info,
	                                this->params.source.path,
	                                seed,
	                                this->params.simulation.inter_frame_level);
}

template <typename B, typename R, typename Q>
CRC<B>* Simulation_BFER_ite<B,R,Q>
::build_crc(const int tid)
{
	return Factory_CRC<B>::build(this->params.crc.poly.empty() ? "NO" : this->params.crc.type,
	                             this->params.code.K_info,
	                             this->params.crc.size,
	                             this->params.crc.poly,
	                             this->params.simulation.inter_frame_level);
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFER_ite<B,R,Q>
::build_encoder(const int tid, const int seed)
{
	try
	{
		return this->codec.build_encoder(tid, interleaver[tid]);
	}
	catch (std::exception const&)
	{
		return Factory_encoder_common<B>::build(this->params.encoder.type,
		                                        this->params.code.K,
		                                        this->params.code.N_code,
		                                        this->params.encoder.path,
		                                        seed,
		                                        this->params.simulation.inter_frame_level);
	}
}

template <typename B, typename R, typename Q>
Interleaver<int>* Simulation_BFER_ite<B,R,Q>
::build_interleaver(const int tid, const int seed)
{
	return Factory_interleaver<int>::build(this->params.interleaver.type,
	                                       this->params.code.N_code,
	                                       this->params.interleaver.path,
	                                       this->params.interleaver.uniform,
	                                       this->params.interleaver.n_cols,
	                                       seed,
	                                       this->params.simulation.inter_frame_level);
}

template <typename B, typename R, typename Q>
Modulator<B,R,Q>* Simulation_BFER_ite<B,R,Q>
::build_modulator(const int tid)
{
	return Factory_modulator<B,R,Q>::build(this->params.modulator.type,
	                                       this->params.code.N,
	                                       this->sigma,
	                                       this->params.demodulator.max,
	                                       this->params.demodulator.psi,
	                                       this->params.modulator.bits_per_symbol,
	                                       this->params.modulator.const_path,
	                                       this->params.modulator.upsample_factor,
	                                       this->params.modulator.cpm_L,
	                                       this->params.modulator.cpm_k,
	                                       this->params.modulator.cpm_p,
	                                       this->params.modulator.mapping,
	                                       this->params.modulator.wave_shape,
	                                       this->params.demodulator.no_sig2,
	                                       this->params.demodulator.n_ite,
	                                       this->params.simulation.inter_frame_level);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_BFER_ite<B,R,Q>
::build_channel(const int tid, const int seed)
{
	const auto add_users = this->params.modulator.type == "SCMA";
	return Factory_channel<R>::build(this->params.channel.type,
	                                 this->params.code.N_mod,
	                                 this->sigma,
	                                 this->params.modulator.complex,
	                                 add_users,
	                                 this->params.channel.path,
	                                 seed,
	                                 this->params.simulation.inter_frame_level);
}

template <typename B, typename R, typename Q>
Quantizer<R,Q>* Simulation_BFER_ite<B,R,Q>
::build_quantizer(const int tid)
{
	return Factory_quantizer<R,Q>::build((typeid(R) == typeid(Q)) ? "NO" : this->params.quantizer.type,
	                                     this->params.code.N_fil,
	                                     this->params.quantizer.n_decimals,
	                                     this->params.quantizer.n_bits,
	                                     this->sigma,
	                                     this->params.quantizer.range,
	                                     this->params.simulation.inter_frame_level);
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
	return Factory_coset_real<B,Q>::build("STD",
	                                      this->params.code.N_code,
	                                      this->params.simulation.inter_frame_level);
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
	return Factory_coset_bit<B>::build("STD",
	                                   this->params.code.K,
	                                   this->params.simulation.inter_frame_level);
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
