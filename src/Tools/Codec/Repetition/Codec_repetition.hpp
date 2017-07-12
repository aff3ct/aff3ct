#ifndef CODEC_REPETITION_HPP_
#define CODEC_REPETITION_HPP_

#include "../Codec.hpp"
#include "Tools/Factory/Repetition/Factory_encoder_repetition.hpp"
#include "Tools/Factory/Repetition/Factory_decoder_repetition.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_repetition : public Codec<B,Q>
{
protected :
	const Factory_encoder_repetition::parameters& enc_par;

public:
	Codec_repetition(const Factory_encoder_repetition::parameters &enc_params,
	                 const Factory_decoder_repetition::parameters &dec_params);

	virtual ~Codec_repetition();

	module::Encoder<B  >* build_encoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::Decoder<B,Q>* build_decoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                             module::CRC        <B  >* crc = nullptr);
};
}
}

#endif /* CODEC_REPETITION_HPP_ */
