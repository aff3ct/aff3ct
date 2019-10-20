#include <string>

#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
Decoder_SISO_SIHO<B,R>
::Decoder_SISO_SIHO(const int K, const int N, const int n_frames, const int simd_inter_frame_level)
: Decoder          (K, N, n_frames, simd_inter_frame_level),
  Decoder_SIHO<B,R>(K, N, n_frames, simd_inter_frame_level),
  Decoder_SISO<  R>(K, N, n_frames, simd_inter_frame_level)
{
	const std::string name = "Decoder_SISO_SIHO";
	this->set_name(name);
}

}
}
