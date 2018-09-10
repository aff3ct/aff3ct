#ifndef CODEC_SIHO_HIHO_HXX_
#define CODEC_SIHO_HIHO_HXX_

#include "Codec_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename Q>
Codec_SIHO_HIHO<B,Q>::
Codec_SIHO_HIHO(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: Codec     <B,Q>(K, N_cw, N, tail_length, n_frames),
  Codec_SIHO<B,Q>(K, N_cw, N, tail_length, n_frames),
  Codec_HIHO<B,Q>(K, N_cw, N, tail_length, n_frames)
{
	const std::string name = "Codec_SIHO_HIHO";
	this->set_name(name);
}

template <typename B, typename Q>
void Codec_SIHO_HIHO<B,Q>::
reset()
{
	this->get_decoder_siho()->reset();
}

}
}

#endif /* CODEC_SIHO_HIHO_HXX_ */
