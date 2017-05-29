#include "Tools/Factory/LDPC/Factory_encoder_LDPC.hpp"
#include "Tools/Factory/LDPC/Factory_decoder_LDPC.hpp"

#include <numeric>

#include "Codec_LDPC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_LDPC<B,Q>
::Codec_LDPC(const parameters& params)
: Codec_SISO<B,Q>(params),
  alist_data   (params.code.alist_path),
  info_bits_pos(this->params.code.K),
  decoder_siso (params.simulation.n_threads, nullptr)
{
	auto encoder_LDPC = this->build_encoder();
	if (encoder_LDPC != nullptr)
	{
		encoder_LDPC->get_info_bits_pos(info_bits_pos);
		delete encoder_LDPC;
	}
	else
		std::iota(info_bits_pos.begin(), info_bits_pos.end(), 0);
}

template <typename B, typename Q>
Codec_LDPC<B,Q>
::~Codec_LDPC()
{
}

template <typename B, typename Q>
Encoder_LDPC<B>* Codec_LDPC<B,Q>
::build_encoder(const int tid, const module::Interleaver<int>* itl)
{
	return Factory_encoder_LDPC<B>::build(this->params.encoder.type,
	                                      this->params.code.K,
	                                      this->params.code.N_code,
	                                      this->params.encoder.path,
	                                      this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
Decoder_SISO<B,Q>* Codec_LDPC<B,Q>
::build_siso(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	decoder_siso[tid] = Factory_decoder_LDPC<B,Q>::build(this->params.decoder.type,
	                                                     this->params.decoder.implem,
	                                                     this->params.code.K,
	                                                     this->params.code.N_code,
	                                                     this->params.decoder.n_ite,
	                                                     alist_data,
	                                                     info_bits_pos,
	                                                     this->params.decoder.simd_strategy,
	                                                     this->params.decoder.normalize_factor,
	                                                     (Q)this->params.decoder.offset,
	                                                     this->params.decoder.enable_syndrome,
	                                                     this->params.decoder.syndrome_depth,
	                                                     this->params.simulation.inter_frame_level);
	return decoder_siso[tid];
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_LDPC<B,Q>
::build_decoder(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	if (decoder_siso[tid] != nullptr)
	{
		auto ptr = decoder_siso[tid];
		decoder_siso[tid] = nullptr;
		return ptr;
	}
	else
	{
		auto decoder = this->build_siso(tid, itl, crc);
		decoder_siso[tid] = nullptr;
		return decoder;
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_LDPC<B_8,Q_8>;
template class aff3ct::tools::Codec_LDPC<B_16,Q_16>;
template class aff3ct::tools::Codec_LDPC<B_32,Q_32>;
template class aff3ct::tools::Codec_LDPC<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
