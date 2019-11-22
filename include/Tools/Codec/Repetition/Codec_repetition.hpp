/*!
 * \file
 * \brief Class tools::Codec_repetition.
 */
#ifndef CODEC_REPETITION_HPP_
#define CODEC_REPETITION_HPP_

#include "Factory/Module/Encoder/Repetition/Encoder_repetition.hpp"
#include "Factory/Module/Decoder/Repetition/Decoder_repetition.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_repetition : public Codec_SIHO<B,Q>
{
public:
	Codec_repetition(const factory::Encoder_repetition &enc_params,
	                 const factory::Decoder_repetition &dec_params);
	virtual ~Codec_repetition() = default;

	virtual Codec_repetition<B,Q>* clone() const;
};
}
}

#endif /* CODEC_REPETITION_HPP_ */
