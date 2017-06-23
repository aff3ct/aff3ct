#ifndef DECODER_REPETITION_FAST
#define DECODER_REPETITION_FAST

#include <vector>
#include <mipp.h>

#include "Decoder_repetition.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_repetition_fast : public Decoder_repetition<B,R>
{
public:
	Decoder_repetition_fast(const int& K, const int& N, const bool buffered_encoding = true,
	                        const int n_frames = 1, const std::string name = "Decoder_repetition_fast");
	virtual ~Decoder_repetition_fast();

protected:
	void _soft_decode(const R *sys, const R *par, R *ext, const int frame_id);
};
}
}

#endif /* DECODER_REPETITION_FAST */
