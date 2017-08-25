#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Decoder/NO/Decoder_NO.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"

#include "Codec_uncoded.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_uncoded<B,Q>
::Codec_uncoded(const factory::Encoder   ::parameters &enc_params,
                const factory::Decoder_NO::parameters &dec_params,
                const std::string name)
: Codec          <B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, enc_params.tail_length, enc_params.n_frames, name),
  Codec_SISO_SIHO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, enc_params.tail_length, enc_params.n_frames, name)
{
	// ----------------------------------------------------------------------------------------------------- exceptions
	if (enc_params.K != dec_params.K)
	{
		std::stringstream message;
		message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
		        << ", 'dec_params.K' = " << dec_params.K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.N_cw != dec_params.N_cw)
	{
		std::stringstream message;
		message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
		        << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.K != enc_params.N_cw)
	{
		std::stringstream message;
		message << "'K' has to be equal to 'N' ('K' = " << enc_params.K
		        << ", 'N' = " << enc_params.N_cw << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.n_frames != dec_params.n_frames)
	{
		std::stringstream message;
		message << "'enc_params.n_frames' has to be equal to 'dec_params.n_frames' ('enc_params.n_frames' = "
		        << enc_params.n_frames << ", 'dec_params.n_frames' = " << dec_params.n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------- allocations
	factory::Puncturer::parameters pct_params;
	pct_params.type     = "NO";
	pct_params.K        = enc_params.K;
	pct_params.N        = enc_params.N_cw;
	pct_params.N_cw     = enc_params.N_cw;
	pct_params.n_frames = enc_params.n_frames;

	this->set_puncturer     (factory::Puncturer ::build<B,Q>(pct_params));
	this->set_encoder       (factory::Encoder   ::build<B>  (enc_params));
	auto decoder_siso_siho = factory::Decoder_NO::build<B,Q>(dec_params);
	this->set_decoder_siso(decoder_siso_siho);
	this->set_decoder_siho(decoder_siso_siho);
}

template <typename B, typename Q>
Codec_uncoded<B,Q>
::~Codec_uncoded()
{
	this->set_decoder_siho(nullptr);
}

template <typename B, typename Q>
void Codec_uncoded<B,Q>
::_extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id)
{
	auto K = this->K;
	std::copy(Y_N, Y_N + K, sys);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Codec_uncoded<B_8,Q_8>;
template class aff3ct::module::Codec_uncoded<B_16,Q_16>;
template class aff3ct::module::Codec_uncoded<B_32,Q_32>;
template class aff3ct::module::Codec_uncoded<B_64,Q_64>;
#else
template class aff3ct::module::Codec_uncoded<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
