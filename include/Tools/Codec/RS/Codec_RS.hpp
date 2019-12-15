/*!
 * \file
 * \brief Class tools::Codec_RS.
 */
#ifndef CODEC_RS_HPP_
#define CODEC_RS_HPP_

#include <memory>

#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Factory/Module/Encoder/RS/Encoder_RS.hpp"
#include "Factory/Module/Decoder/RS/Decoder_RS.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_RS : public Codec_SIHO<B,Q>
{
protected:
	std::shared_ptr<const RS_polynomial_generator> GF_poly;

public:
	Codec_RS(const factory::Encoder_RS &enc_params,
	         const factory::Decoder_RS &dec_params);
	virtual ~Codec_RS() = default;

	virtual Codec_RS<B,Q>* clone() const;

	const RS_polynomial_generator& get_GF_poly() const;
};
}
}

#endif /* CODEC_RS_HPP_ */
