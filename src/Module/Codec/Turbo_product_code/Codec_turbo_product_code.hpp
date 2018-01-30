#ifndef CODEC_TURBO_PRODUCT_CODE_HPP_
#define CODEC_TURBO_PRODUCT_CODE_HPP_

#include <vector>
#include <fstream>

#include "Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

#include "Factory/Module/Encoder/Turbo_product_code/Encoder_turbo_product_code.hpp"
#include "Factory/Module/Decoder/Turbo_product_code/Decoder_turbo_product_code.hpp"

#include "../Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_turbo_product_code : public Codec_SIHO<B,Q>
{
protected:
	const aff3ct::tools::BCH_polynomial_generator GF_poly;
	aff3ct::module::Encoder_BCH<B,Q>              enc_bch;
	aff3ct::module::Decoder_BCH<B,Q>              dec_bch;

public:
	Codec_turbo_product_code(const factory::Encoder_turbo  ::parameters<> &enc_params,
	                         const factory::Decoder_turbo  ::parameters<> &dec_params);
	virtual ~Codec_turbo_product_code();
};
}
}

#endif /* CODEC_TURBO_PRODUCT_CODE_HPP_ */
