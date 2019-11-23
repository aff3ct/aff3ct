/*!
 * \file
 * \brief Class tools::Codec_uncoded.
 */
#ifndef CODEC_UNCODED_HPP_
#define CODEC_UNCODED_HPP_

#include "Factory/Module/Encoder/NO/Encoder_NO.hpp"
#include "Factory/Module/Decoder/NO/Decoder_NO.hpp"
#include "Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_uncoded : public Codec_SISO<B,Q>
{
public:
	Codec_uncoded(const factory::Encoder_NO &enc_params,
	              const factory::Decoder_NO &dec_params);
	virtual ~Codec_uncoded() = default;

	virtual Codec_uncoded<B,Q>* clone() const;
};
}
}

#endif /* CODEC_UNCODED_HPP_ */
