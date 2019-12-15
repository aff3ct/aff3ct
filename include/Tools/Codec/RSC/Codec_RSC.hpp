/*!
 * \file
 * \brief Class tools::Codec_RSC.
 */
#ifndef CODEC_RSC_HPP_
#define CODEC_RSC_HPP_

#include <vector>
#include <memory>

#include "Factory/Module/Encoder/RSC/Encoder_RSC.hpp"
#include "Factory/Module/Decoder/RSC/Decoder_RSC.hpp"
#include "Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_RSC : public Codec_SISO<B,Q>
{
protected:
	const bool buffered_encoding;
	std::shared_ptr<std::vector<std::vector<int>>> trellis;

public:
	Codec_RSC(const factory::Encoder_RSC &enc_params,
	          const factory::Decoder_RSC &dec_params);
	virtual ~Codec_RSC() = default;

	virtual Codec_RSC<B,Q>* clone() const;

	bool is_buffered_encoding() const;
	const std::vector<std::vector<int>>& get_trellis() const;
};
}
}

#endif /* CODEC_RSC_HPP_ */
