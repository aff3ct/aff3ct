#ifndef CODEC_BCH_HPP_
#define CODEC_BCH_HPP_

#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"
#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Module/Codec/Codec_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_BCH : public Codec_SIHO_HIHO<B,Q>
{
protected:
	const tools::BCH_polynomial_generator<B> GF_poly;

public:
	Codec_BCH(const factory::Encoder_BCH::parameters &enc_params,
	          const factory::Decoder_BCH::parameters &dec_params);
	virtual ~Codec_BCH() = default;
};
}
}

#endif /* CODEC_BCH_HPP_ */
