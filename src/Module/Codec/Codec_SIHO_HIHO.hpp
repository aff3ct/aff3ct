#ifndef CODEC_SIHO_HIHO_HPP_
#define CODEC_SIHO_HIHO_HPP_

#include <memory>

#include "Module/Decoder/Decoder_SIHO_HIHO.hpp"
#include "Module/Codec/Codec_HIHO.hpp"
#include "Module/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_SIHO_HIHO : public Codec_SIHO<B,Q>, public Codec_HIHO<B,Q>
{
public:
	Codec_SIHO_HIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec_SIHO_HIHO() = default;

	virtual void reset();

	virtual void set_decoder_siho_hiho(std::shared_ptr<Decoder_SIHO_HIHO<B,Q>> dec);
	virtual void set_decoder_siho_hiho(Decoder_SIHO_HIHO<B,Q>* dec);
};
}
}

#include "Module/Codec/Codec_SIHO_HIHO.hxx"

#endif /* CODEC_SIHO_HIHO_HPP_ */
