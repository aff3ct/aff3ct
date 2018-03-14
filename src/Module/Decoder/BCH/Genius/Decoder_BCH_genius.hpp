#ifndef DECODER_BCH_GENIUS
#define DECODER_BCH_GENIUS

#include <vector>

#include "Module/Encoder/Encoder.hpp"
#include "../Decoder_BCH.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_BCH_genius : public Decoder_BCH<B,R>
{
protected:
	Encoder<B> &encoder;

	std::vector<unsigned> error_pos;

public:
	Decoder_BCH_genius(const int& K, const int& N, const tools::BCH_polynomial_generator &GF, Encoder<B> &encoder, const int n_frames = 1);
	virtual ~Decoder_BCH_genius();

protected:
	virtual void _decode(B *Y_N, const int frame_id);
};
}
}

#endif /* DECODER_BCH_GENIUS */
