#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Interleaver.hpp"

#include "Codec_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_RA<B,Q>
::Codec_RA(const factory::Encoder_RA::parameters &enc_params,
           const factory::Decoder_RA::parameters &dec_params)
: Codec<B,Q>(enc_params, dec_params), enc_par(enc_params), dec_par(dec_params)
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
	auto itl_cpy = dec_par.itl;
	itl_cpy.seed = dec_par.itl.uniform ? seed : dec_par.itl.seed;
	return factory::Interleaver::build<int>(itl_cpy);
}

template <typename B, typename Q>
Encoder<B>* Codec_RA<B,Q>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	if (itl == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'itl' should not be null.");

	return factory::Encoder_RA::build<B>(enc_par, *itl);
}

template <typename B, typename Q>
Decoder_SIHO<B,Q>* Codec_RA<B,Q>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	if (itl == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'itl' should not be null.");

	return factory::Decoder_RA::build<B,Q>(dec_par, *itl);
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
