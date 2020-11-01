#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Tools/Codec/Repetition/Codec_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_repetition<B,Q>
::Codec_repetition(const factory::Encoder_repetition &enc_params,
                   const factory::Decoder_repetition &dec_params)
: Codec_SIHO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw)
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
	this->set_decoder_siho(dec_params.build<B,Q>(&this->get_encoder()));
}

template <typename B, typename Q>
Codec_repetition<B,Q>* Codec_repetition<B,Q>
::clone() const
{
	auto t = new Codec_repetition(*this);
	t->deep_copy(*this);
	return t;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Codec_repetition<B_8,Q_8>;
template class aff3ct::tools::Codec_repetition<B_16,Q_16>;
template class aff3ct::tools::Codec_repetition<B_32,Q_32>;
template class aff3ct::tools::Codec_repetition<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_repetition<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
