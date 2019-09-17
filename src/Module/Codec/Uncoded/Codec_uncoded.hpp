#ifndef CODEC_UNCODED_HPP_
#define CODEC_UNCODED_HPP_

#include "Factory/Module/Encoder/NO/Encoder_NO.hpp"
#include "Factory/Module/Decoder/NO/Decoder_NO.hpp"
#include "Module/Codec/Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_uncoded : public Codec_SISO_SIHO<B,Q>
{
public:
	Codec_uncoded(const factory::Encoder_NO::parameters &enc_params,
	              const factory::Decoder_NO::parameters &dec_params);
	virtual ~Codec_uncoded() = default;

protected:
	void _extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id);
};
}
}

#endif /* CODEC_UNCODED_HPP_ */
