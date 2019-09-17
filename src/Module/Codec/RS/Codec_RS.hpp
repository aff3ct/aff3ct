#ifndef CODEC_RS_HPP_
#define CODEC_RS_HPP_

#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Factory/Module/Encoder/RS/Encoder_RS.hpp"
#include "Factory/Module/Decoder/RS/Decoder_RS.hpp"
#include "Module/Codec/Codec_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_RS : public Codec_SIHO_HIHO<B,Q>
{
protected:
	const tools::RS_polynomial_generator GF_poly;

public:
	Codec_RS(const factory::Encoder_RS::parameters &enc_params,
	         const factory::Decoder_RS::parameters &dec_params);
	virtual ~Codec_RS() = default;
};
}
}

#endif /* CODEC_RS_HPP_ */
