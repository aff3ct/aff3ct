#ifndef CODEC_SIHO_HIHO_HPP_
#define CODEC_SIHO_HIHO_HPP_

#include "Module/Decoder/Decoder_SISO.hpp"

#include "Codec_HIHO.hpp"
#include "Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_SIHO_HIHO : public Codec_SIHO<B,Q>, public Codec_HIHO<B,Q>
{
public:
	Codec_SIHO_HIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec_SIHO_HIHO();

	virtual void reset();
};
}
}

#include "Codec_SIHO_HIHO.hxx"

#endif /* CODEC_SIHO_HIHO_HPP_ */
