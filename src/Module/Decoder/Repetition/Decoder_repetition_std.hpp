#ifndef DECODER_REPETITION_STD
#define DECODER_REPETITION_STD

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Decoder_repetition.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_repetition_std : public Decoder_repetition<B,R>
{
public:
	Decoder_repetition_std(const int& K, const int& N, const bool buffered_encoding = true, 
	                       const int n_frames = 1, const std::string name = "Decoder_repetition_std");
	virtual ~Decoder_repetition_std();

protected:
	void _soft_decode_fbf(const R *sys, const R *par, R *ext);
};
}
}

#endif /* DECODER_REPETITION_STD */
