/*!
 * \file
 * \brief A Decoder_SISO is both a "Soft Input Hard Output" decoder and a "Soft Input Soft Output" decoder.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_SISO_SIHO_HXX_
#define DECODER_SISO_SIHO_HXX_

#include "Decoder_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
Decoder_SISO_SIHO<B,R>::
Decoder_SISO_SIHO(const int K, const int N, const int n_frames, const int simd_inter_frame_level)
: Decoder          (K, N, n_frames, simd_inter_frame_level),
  Decoder_SIHO<B,R>(K, N, n_frames, simd_inter_frame_level),
  Decoder_SISO<  R>(K, N, n_frames, simd_inter_frame_level)
{
	const std::string name = "Decoder_SISO_SIHO";
	this->set_name(name);
}

}
}

#endif /* DECODER_SISO_SIHO_HPP_ */
