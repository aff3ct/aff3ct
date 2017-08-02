#ifndef CODEC_REPETITION_HPP_
#define CODEC_REPETITION_HPP_

#include "Factory/Module/Code/Repetition/Encoder_repetition.hpp"
#include "Factory/Module/Code/Repetition/Decoder_repetition.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_repetition : public Codec<B,Q>
{
protected:
	const factory::Encoder_repetition::parameters& enc_par;
	const factory::Decoder_repetition::parameters& dec_par;

public:
	Codec_repetition(const factory::Encoder_repetition::parameters &enc_params,
	                 const factory::Decoder_repetition::parameters &dec_params);

	virtual ~Codec_repetition();

	module::Encoder     <B  >* build_encoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::Decoder_SIHO<B,Q>* build_decoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                  module::CRC        <B  >* crc = nullptr);
};
}
}

#endif /* CODEC_REPETITION_HPP_ */
