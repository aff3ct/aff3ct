#include "Codec_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_BCH<B,Q>
::Codec_BCH(const Factory_encoder    ::parameters &enc_params,
            const Factory_decoder_BCH::parameters &dec_params)
: Codec<B,Q>(enc_params, dec_params), dec_par(dec_params),
  GF(dec_params.K, dec_params.N, dec_params.t)
{
	// assertion are made in the Galois Field (GF)
}

template <typename B, typename Q>
Codec_BCH<B,Q>
::~Codec_BCH()
{
}

template <typename B, typename Q>
Encoder<B>* Codec_BCH<B,Q>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	return Factory_encoder_BCH::build<B>(this->enc_params, GF);
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_BCH<B,Q>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	return Factory_decoder_BCH::build<B,Q>(dec_par, GF);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_BCH<B_8,Q_8>;
template class aff3ct::tools::Codec_BCH<B_16,Q_16>;
template class aff3ct::tools::Codec_BCH<B_32,Q_32>;
template class aff3ct::tools::Codec_BCH<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_BCH<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
