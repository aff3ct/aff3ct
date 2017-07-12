#include "Tools/Exception/exception.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/RA/Factory_encoder_RA.hpp"
#include "Tools/Factory/RA/Factory_decoder_RA.hpp"

#include "Codec_RA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_RA<B,Q>
::Codec_RA(const Factory_encoder_common::parameters &enc_params,
           const Factory_decoder_RA    ::parameters &dec_params)
: Codec<B,Q>(enc_params, dec_params), dec_par(dec_params)
{
}

template <typename B, typename Q>
Codec_RA<B,Q>
::~Codec_RA()
{
}

template <typename B, typename Q>
Interleaver<int>* Codec_RA<B,Q>
::build_interleaver(const int tid, const int seed)
{
	return Factory_interleaver::build<int>(dec_par.itl);
}

template <typename B, typename Q>
Encoder<B>* Codec_RA<B,Q>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	if (itl == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'itl' should not be null.");

	return Factory_encoder_RA::build<B>(this->enc_params, *itl);
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_RA<B,Q>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	if (itl == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'itl' should not be null.");

	return Factory_decoder_RA::build<B,Q>(dec_par, *itl);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_RA<B_8,Q_8>;
template class aff3ct::tools::Codec_RA<B_16,Q_16>;
template class aff3ct::tools::Codec_RA<B_32,Q_32>;
template class aff3ct::tools::Codec_RA<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_RA<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
