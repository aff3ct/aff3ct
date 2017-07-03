#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Module/Decoder/NO/Decoder_NO.hpp"

#include "Codec_uncoded.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_uncoded<B,Q>
::Codec_uncoded(const typename Factory_encoder_common<B  >::encoder_parameters &enc_params,
                const typename Factory_decoder_NO    <B,Q>::decoder_parameters &dec_params)
: Codec_SISO<B,Q>(enc_params, dec_params)
{
	if (enc_params.K != enc_params.N)
	{
		std::stringstream message;
		message << "'K' has to be equal to 'N' ('K' = " << enc_params.K
		        << ", 'N' = " << enc_params.N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename Q>
Codec_uncoded<B,Q>
::~Codec_uncoded()
{
}

template <typename B, typename Q>
Encoder<B>* Codec_uncoded<B,Q>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	return new Encoder_NO<B>(this->enc_params.K, this->enc_params.n_frames);
}

template <typename B, typename Q>
SISO<Q>* Codec_uncoded<B,Q>
::build_siso(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	return new Decoder_NO<B,Q>(this->dec_params.K, this->dec_params.n_frames);
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_uncoded<B,Q>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	return new Decoder_NO<B,Q>(this->dec_params.K, this->dec_params.n_frames);
}

template <typename B, typename Q>
void Codec_uncoded<B,Q>
::extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par)
{
	const auto K = this->dec_params.K;

	if ((int)Y_N.size() != K * this->dec_params.n_frames)
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'K' * 'inter_frame_level' ('Y_N.size()' = " << Y_N.size()
		        << ", 'K' = " << K << ", 'inter_frame_level' = " << this->dec_params.n_frames << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)sys.size() != K * this->dec_params.n_frames)
	{
		std::stringstream message;
		message << "'sys.size()' has to be equal to 'K' * 'inter_frame_level' ('sys.size()' = " << sys.size()
		        << ", 'K' = " << K << ", 'inter_frame_level' = " << this->dec_params.n_frames << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)par.size() != 0)
	{
		std::stringstream message;
		message << "'par.size()' has to be equal to 0 ('par.size()' = " << par.size() << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::copy(Y_N.begin(), Y_N.end(), sys.begin());
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_uncoded<B_8,Q_8>;
template class aff3ct::tools::Codec_uncoded<B_16,Q_16>;
template class aff3ct::tools::Codec_uncoded<B_32,Q_32>;
template class aff3ct::tools::Codec_uncoded<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_uncoded<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
