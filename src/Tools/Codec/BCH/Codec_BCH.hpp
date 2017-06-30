#ifndef CODEC_BCH_HPP_
#define CODEC_BCH_HPP_

#include "Tools/Math/Galois.hpp"
#include "Tools/Factory/BCH/Factory_encoder_BCH.hpp"
#include "Tools/Factory/BCH/Factory_decoder_BCH.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_BCH : public Codec<B,Q>
{
protected:
	const tools::Galois GF;

public:
	Codec_BCH(const typename Factory_encoder_common<B  >::encoder_parameters     &enc_params,
	          const typename Factory_decoder_BCH   <B,Q>::decoder_parameters_BCH &dec_params);
	virtual ~Codec_BCH();

	module::Encoder<B  >* build_encoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::Decoder<B,Q>* build_decoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                             module::CRC        <B  >* crc = nullptr);
};
}
}

#endif /* CODEC_BCH_HPP_ */
