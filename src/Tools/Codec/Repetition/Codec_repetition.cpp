#include "Tools/Factory/Repetition/Factory_encoder_repetition.hpp"
#include "Tools/Factory/Repetition/Factory_decoder_repetition.hpp"

#include "Codec_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_repetition<B,Q>
::Codec_repetition(const typename Factory_encoder_repetition<B  >::encoder_parameters_repetition &enc_params,
                   const typename Factory_decoder_repetition<B,Q>::decoder_parameters            &dec_params)
: Codec<B,Q>(enc_params, dec_params), enc_par(enc_params)
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
	return Factory_encoder_repetition<B>::build(enc_par);
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_repetition<B,Q>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	return Factory_decoder_repetition<B,Q>::build(this->dec_params, enc_par.buffered);
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
