#ifndef CODEC_SISO_SIHO_HPP_
#define CODEC_SISO_SIHO_HPP_

#include "Module/Decoder/Decoder_SISO.hpp"

#include "Codec_SISO.hpp"
#include "Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_SISO_SIHO : public Codec_SISO<B,Q>, public Codec_SIHO<B,Q>
{
public:
	Codec_SISO_SIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec_SISO_SIHO();

	virtual void reset();
};
}
}
#include "Codec_SISO_SIHO.hxx"

#endif /* CODEC_SISO_SIHO_HPP_ */
