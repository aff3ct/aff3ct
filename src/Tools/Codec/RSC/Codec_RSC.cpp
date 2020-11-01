#include <algorithm>
#include <iostream>
#include <sstream>
#include <memory>

#include "Module/Extractor/RSC/Extractor_RSC.hpp"
#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Tools/Codec/RSC/Codec_RSC.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_RSC<B,Q>
::Codec_RSC(const factory::Encoder_RSC &enc_params,
            const factory::Decoder_RSC &dec_params)
: Codec_SISO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw),
  buffered_encoding(enc_params.buffered),
  trellis(new std::vector<std::vector<int>>())
{
	// ----------------------------------------------------------------------------------------------------- exceptions
	if (enc_params.K != dec_params.K)
	{
		std::stringstream message;
		message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
		        << ", 'dec_params.K' = " << dec_params.K << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.N_cw != dec_params.N_cw)
	{
		std::stringstream message;
		message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
		        << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------------- tools
	auto enc_cpy = enc_params;
	enc_cpy.type = "RSC";

	std::unique_ptr<module::Encoder_RSC_sys<B>> encoder_RSC(enc_cpy.build<B>());
	*trellis = encoder_RSC->get_trellis();

	// ---------------------------------------------------------------------------------------------------- allocations
	factory::Puncturer pct_params;
	pct_params.type = "NO";
	pct_params.K    = enc_params.K;
	pct_params.N    = enc_params.N_cw;
	pct_params.N_cw = enc_params.N_cw;

	this->set_puncturer(pct_params.build<B,Q>());
	try
	{
		this->set_encoder(enc_params.build<B>());
	}
	catch (cannot_allocate const&)
	{
		this->set_encoder(static_cast<const factory::Encoder*>(&enc_params)->build<B>());
	}

	try
	{
		this->set_decoder_siso(dec_params.build_siso<B,Q>(*trellis, std::cout, 1, &this->get_encoder()));
	}
	catch (cannot_allocate const&)
	{
		this->set_decoder_siho(dec_params.build<B,Q>(*trellis, std::cout, 1, &this->get_encoder()));
	}

	this->set_extractor(new module::Extractor_RSC<B,Q>(enc_params.K,
	                                                   enc_params.N_cw,
	                                                   enc_params.tail_length,
	                                                   enc_params.buffered));
}

template <typename B, typename Q>
Codec_RSC<B,Q>* Codec_RSC<B,Q>
::clone() const
{
	auto t = new Codec_RSC(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename Q>
bool Codec_RSC<B,Q>
::is_buffered_encoding() const
{
	return this->buffered_encoding;
}

template <typename B, typename Q>
const std::vector<std::vector<int>>& Codec_RSC<B,Q>
::get_trellis() const
{
	return *this->trellis;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Codec_RSC<B_8,Q_8>;
template class aff3ct::tools::Codec_RSC<B_16,Q_16>;
template class aff3ct::tools::Codec_RSC<B_32,Q_32>;
template class aff3ct::tools::Codec_RSC<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_RSC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
