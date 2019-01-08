#ifndef CODEC_HIHO_HXX_
#define CODEC_HIHO_HXX_

#include "Codec_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename Q>
Codec_HIHO<B,Q>::
Codec_HIHO(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: Codec<B,Q>(K, N_cw, N, tail_length, n_frames)
{
	const std::string name = "Codec_HIHO";
	this->set_name(name);
}

template <typename B, typename Q>
const std::shared_ptr<Decoder_HIHO<B>>& Codec_HIHO<B,Q>::
get_decoder_hiho()
{
	if (this->decoder_hiho == nullptr)
	{
		std::stringstream message;
		message << "'decoder_hiho' is NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->decoder_hiho;
}

template <typename B, typename Q>
void Codec_HIHO<B,Q>::
reset()
{
	if (this->decoder_hiho == nullptr)
	{
		std::stringstream message;
		message << "'decoder_hiho' is NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->decoder_hiho->reset();
}

template <typename B, typename Q>
void Codec_HIHO<B,Q>::
set_decoder_hiho(std::shared_ptr<Decoder_HIHO<B>> dec)
{
	this->decoder_hiho = dec;
}

template <typename B, typename Q>
void Codec_HIHO<B,Q>::
set_decoder_hiho(Decoder_HIHO<B>* dec)
{
	this->set_decoder_hiho(std::shared_ptr<Decoder_HIHO<B>>(dec));
}

}
}

#endif
