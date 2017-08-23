#ifndef CODEC_BCH_HPP_
#define CODEC_BCH_HPP_

#include "Tools/Math/Galois.hpp"

#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"

#include "../Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_BCH : public Codec_SIHO<B,Q>
{
protected:
	const tools::Galois GF;

public:
	Codec_BCH(const factory::Encoder_BCH::parameters &enc_params,
	          const factory::Decoder_BCH::parameters &dec_params,
	          const std::string name = "Codec_BCH");
	virtual ~Codec_BCH();
};
}
}

#endif /* CODEC_BCH_HPP_ */
