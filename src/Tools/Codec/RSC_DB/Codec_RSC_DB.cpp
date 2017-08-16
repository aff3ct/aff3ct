#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Codec_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_RSC_DB<B,Q>
::Codec_RSC_DB(const factory::Encoder_RSC_DB::parameters &enc_params,
               const factory::Decoder_RSC_DB::parameters &dec_params)
: Codec_SISO<B,Q>(enc_params, dec_params), enc_par(enc_params), dec_par(dec_params)
{
	auto enc_cpy = enc_params;
	enc_cpy.type = "RSC_DB";

	auto encoder_RSC = factory::Encoder_RSC_DB::build<B>(enc_cpy);
	trellis = encoder_RSC->get_trellis();
	delete encoder_RSC;
}

template <typename B, typename Q>
Codec_RSC_DB<B,Q>
::~Codec_RSC_DB()
{
}

template <typename B, typename Q>
module::Encoder_RSC_DB<B>* Codec_RSC_DB<B,Q>
::build_encoder(const int tid, const module::Interleaver<B>* itl)
{
	return factory::Encoder_RSC_DB::build<B>(enc_par);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Codec_RSC_DB<B,Q>
::build_decoder_siso(const int tid, const module::Interleaver<Q>* itl, module::CRC<B>* crc)
{
	return factory::Decoder_RSC_DB::build<B,Q>(dec_par, trellis);
}

template <typename B, typename Q>
module::Decoder_SISO<Q>* Codec_RSC_DB<B,Q>
::build_siso(const int tid, const module::Interleaver<Q>* itl, module::CRC<B>* crc)
{
	return this->build_decoder_siso(tid, itl, crc);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Codec_RSC_DB<B,Q>
::build_decoder(const int tid, const module::Interleaver<Q>* itl, module::CRC<B>* crc)
{
	return this->build_decoder_siso(tid, itl, crc);
}

template <typename B, typename Q>
void Codec_RSC_DB<B,Q>
::extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par)
{
	// TODO: implement this method
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_RSC_DB<B_8,Q_8>;
template class aff3ct::tools::Codec_RSC_DB<B_16,Q_16>;
template class aff3ct::tools::Codec_RSC_DB<B_32,Q_32>;
template class aff3ct::tools::Codec_RSC_DB<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_RSC_DB<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
