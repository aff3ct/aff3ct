#ifndef CODEC_REPETITION_HPP_
#define CODEC_REPETITION_HPP_

#include "../Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_repetition : public Codec<B,Q>
{
public:
	Codec_repetition(const parameters& params);

	virtual ~Codec_repetition();

	module::Encoder<B  >* build_encoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::Decoder<B,Q>* build_decoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                             module::CRC        <B  >* crc = nullptr);
};
}
}

#endif /* CODEC_REPETITION_HPP_ */
