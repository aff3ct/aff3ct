#ifndef CODEC_TURBO_PRODUCT_HPP_
#define CODEC_TURBO_PRODUCT_HPP_

#include <vector>
#include <fstream>

#include "Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

#include "Factory/Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"
#include "Factory/Module/Decoder/Turbo_product/Decoder_turbo_product.hpp"

#include "../Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_turbo_product : public Codec_SISO_SIHO<B,Q>
{
protected:
	const aff3ct::tools::BCH_polynomial_generator GF_poly;
	aff3ct::module::Encoder_BCH <B>* enc_bch;
	aff3ct::module::Decoder_HIHO<B>* dec_bch;

public:
	Codec_turbo_product(const factory::Encoder_turbo_product::parameters &enc_params,
	                    const factory::Decoder_turbo_product::parameters &dec_params);
	virtual ~Codec_turbo_product();
};
}
}

#endif /* CODEC_TURBO_PRODUCT_HPP_ */
