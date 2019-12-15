/*!
 * \file
 * \brief Class tools::Codec_turbo_product.
 */
#ifndef CODEC_TURBO_PRODUCT_HPP_
#define CODEC_TURBO_PRODUCT_HPP_

#include <memory>

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"
#include "Factory/Module/Decoder/Turbo_product/Decoder_turbo_product.hpp"
#include "Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_turbo_product : public Codec_SISO<B,Q>
{
protected:
	std::shared_ptr<const BCH_polynomial_generator<B>> GF_poly;

public:
	Codec_turbo_product(const factory::Encoder_turbo_product &enc_params,
	                    const factory::Decoder_turbo_product &dec_params,
	                    const factory::Interleaver           &itl_params);
	virtual ~Codec_turbo_product() = default;

	virtual Codec_turbo_product<B,Q>* clone() const;

	const BCH_polynomial_generator<B>& get_GF_poly() const;
};
}
}

#endif /* CODEC_TURBO_PRODUCT_HPP_ */
