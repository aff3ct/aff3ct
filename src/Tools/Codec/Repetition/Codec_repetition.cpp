#include "Codec_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_repetition<B,Q>
::Codec_repetition(const factory::Encoder_repetition::parameters &enc_params,
                   const factory::Decoder_repetition::parameters &dec_params)
: Codec<B,Q>(enc_params, dec_params), enc_par(enc_params), dec_par(dec_params)
{
}

template <typename B, typename Q>
Codec_repetition<B,Q>
::~Codec_repetition()
{
}

template <typename B, typename Q>
Encoder<B>* Codec_repetition<B,Q>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	return factory::Encoder_repetition::build<B>(enc_par);
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_repetition<B,Q>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	return factory::Decoder_repetition::build<B,Q>(dec_par);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_repetition<B_8,Q_8>;
template class aff3ct::tools::Codec_repetition<B_16,Q_16>;
template class aff3ct::tools::Codec_repetition<B_32,Q_32>;
template class aff3ct::tools::Codec_repetition<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_repetition<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
