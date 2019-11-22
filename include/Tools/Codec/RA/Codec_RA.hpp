/*!
 * \file
 * \brief Class tools::Codec_RA.
 */
#ifndef CODEC_RA_HPP_
#define CODEC_RA_HPP_

#include "Factory/Module/Encoder/RA/Encoder_RA.hpp"
#include "Factory/Module/Decoder/RA/Decoder_RA.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_RA : public Codec_SIHO<B,Q>
{
public:
	Codec_RA(const factory::Encoder_RA  &enc_params,
	         const factory::Decoder_RA  &dec_params,
	         const factory::Interleaver &itl_params);
	virtual ~Codec_RA() = default;

	virtual Codec_RA<B,Q>* clone() const;
};
}
}

#endif /* CODEC_RA_HPP_ */
