#ifndef CODEC_SISO_HXX_
#define CODEC_SISO_HXX_

#include "Codec_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename Q>
Codec_SISO<B,Q>::
Codec_SISO(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: Codec<B,Q>(K, N_cw, N, tail_length, n_frames), decoder_siso(nullptr)
{
	const std::string name = "Codec_SISO";
	this->set_name(name);
}

template <typename B, typename Q>
const std::shared_ptr<Decoder_SISO<Q>>& Codec_SISO<B,Q>::
get_decoder_siso()
{
	if (this->decoder_siso == nullptr)
	{
		std::stringstream message;
		message << "'decoder_siso' is NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->decoder_siso;
}

template <typename B, typename Q>
void Codec_SISO<B,Q>::
reset()
{
	if (this->decoder_siso == nullptr)
	{
		std::stringstream message;
		message << "'decoder_siso' is NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->decoder_siso->reset();
}

template <typename B, typename Q>
void Codec_SISO<B,Q>::
set_decoder_siso(std::shared_ptr<Decoder_SISO<Q>> dec)
{
	this->decoder_siso = dec;
}

template <typename B, typename Q>
void Codec_SISO<B,Q>::
set_decoder_siso(Decoder_SISO<Q>* dec)
{
	this->set_decoder_siso(std::shared_ptr<Decoder_SISO<Q>>(dec));
}

}
}

#endif /* CODEC_SISO_HXX_ */
