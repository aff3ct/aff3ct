/*!
 * \file
 * \brief Class tools::Codec_turbo_product.
 */
#ifndef CODEC_TURBO_PRODUCT_HPP_
#define CODEC_TURBO_PRODUCT_HPP_

#include <memory>

#include "Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"
#include "Factory/Module/Decoder/Turbo_product/Decoder_turbo_product.hpp"
#include "Tools/Codec/Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_turbo_product : public Codec_SISO_SIHO<B,Q>
{
protected:
	const BCH_polynomial_generator<B> GF_poly;

public:
	Codec_turbo_product(const factory::Encoder_turbo_product &enc_params,
	                    const factory::Decoder_turbo_product &dec_params,
	                    const factory::Interleaver           &itl_params);
	virtual ~Codec_turbo_product() = default;

	virtual Codec_turbo_product<B,Q>* clone() const;

	const BCH_polynomial_generator<B>& get_GF_poly() const;
	const module::Encoder_BCH<B>& get_encoder_BCH_rows() const;
	const module::Encoder_BCH<B>& get_encoder_BCH_cols() const;
	const module::Decoder_BCH<B,Q>& get_decoder_BCH_rows() const;
	const module::Decoder_BCH<B,Q>& get_decoder_BCH_cols() const;
	const module::Decoder_chase_pyndiah<B,Q>& get_decoder_chase_pyndiah_rows() const;
	const module::Decoder_chase_pyndiah<B,Q>& get_decoder_chase_pyndiah_cols() const;
};
}
}

#endif /* CODEC_TURBO_PRODUCT_HPP_ */
