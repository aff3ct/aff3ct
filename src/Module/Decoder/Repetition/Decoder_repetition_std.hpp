#ifndef DECODER_REPETITION_STD
#define DECODER_REPETITION_STD

#include <vector>
#include <mipp.h>

#include "Decoder_repetition.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_repetition_std : public Decoder_repetition<B,R>
{
public:
	Decoder_repetition_std(const int& K, const int& N, const bool buffered_encoding = true, const int n_frames = 1);
	virtual ~Decoder_repetition_std() = default;

protected:
	void _decode_siso(const R *sys, const R *par, R *ext, const int frame_id);
};
}
}

#endif /* DECODER_REPETITION_STD */
