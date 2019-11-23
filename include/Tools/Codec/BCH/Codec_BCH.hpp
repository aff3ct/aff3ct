/*!
 * \file
 * \brief Class tools::Codec_BCH.
 */
#ifndef CODEC_BCH_HPP_
#define CODEC_BCH_HPP_

#include <memory>

#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"
#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_BCH : public Codec_SIHO<B,Q>
{
protected:
	std::shared_ptr<const BCH_polynomial_generator<B>> GF_poly;

public:
	Codec_BCH(const factory::Encoder_BCH &enc_params,
	          const factory::Decoder_BCH &dec_params);
	virtual ~Codec_BCH() = default;

	virtual Codec_BCH<B,Q>* clone() const;

	const BCH_polynomial_generator<B>& get_GF_poly() const;
};
}
}

#endif /* CODEC_BCH_HPP_ */
