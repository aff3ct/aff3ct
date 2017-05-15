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
	auto encoder_LDPC = Factory_encoder_LDPC<B>::build(this->params);
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
Encoder<B>* Codec_LDPC<B,Q>
::build_encoder(const int tid, const module::Interleaver<int>* itl)
{
	return Factory_encoder_LDPC<B>::build(this->params);
}

template <typename B, typename Q>
SISO<Q>* Codec_LDPC<B,Q>
::build_siso(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	decoder_siso[tid] = Factory_decoder_LDPC<B,Q>::build(this->params, alist_data, info_bits_pos);
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
		return Factory_decoder_LDPC<B,Q>::build(this->params, alist_data, info_bits_pos);
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
