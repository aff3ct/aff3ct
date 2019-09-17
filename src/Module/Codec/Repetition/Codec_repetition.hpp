#ifndef CODEC_REPETITION_HPP_
#define CODEC_REPETITION_HPP_

#include "Factory/Module/Encoder/Repetition/Encoder_repetition.hpp"
#include "Factory/Module/Decoder/Repetition/Decoder_repetition.hpp"
#include "Module/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_repetition : public Codec_SIHO<B,Q>
{
public:
	Codec_repetition(const factory::Encoder_repetition::parameters &enc_params,
	                 const factory::Decoder_repetition::parameters &dec_params);
	virtual ~Codec_repetition() = default;
};
}
}

#endif /* CODEC_REPETITION_HPP_ */
