#include "Codec_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_BCH<B,Q>
::Codec_BCH(const factory::Encoder    ::parameters &enc_params,
            const factory::Decoder_BCH::parameters &dec_params)
: Codec<B,Q>(enc_params, dec_params), dec_par(dec_params),
  GF(dec_params.K, dec_params.N_cw, dec_params.t)
{
	// assertion are made in the Galois Field (GF)
}

template <typename B, typename Q>
Codec_BCH<B,Q>
::~Codec_BCH()
{
}

template <typename B, typename Q>
module::Encoder<B>* Codec_BCH<B,Q>
::build_encoder(const int tid, const module::Interleaver<B>* itl)
{
	return factory::Encoder_BCH::build<B>(this->enc_params, GF);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Codec_BCH<B,Q>
::build_decoder(const int tid, const module::Interleaver<Q>* itl, module::CRC<B>* crc)
{
	return factory::Decoder_BCH::build<B,Q>(dec_par, GF);
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
