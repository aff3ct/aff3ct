#ifndef DECODER_REPETITION
#define DECODER_REPETITION

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Decoder.hpp"
#include "../SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_repetition : public Decoder<B,R>, public SISO<R>
{
protected:
	const int rep_count; // number of repetitions

	const bool buffered_encoding;

	mipp::vector<R> sys;
	mipp::vector<R> par;
	mipp::vector<R> ext;

public:
	Decoder_repetition(const int& K, const int& N, const bool buffered_encoding = true, 
	                   const int n_frames = 1, const std::string name = "Decoder_repetition");
	virtual ~Decoder_repetition();

protected:
	void _load           (const R *Y_N);
	void _hard_decode_fbf(const R *Y_N, B *V_K);
};
}
}

#endif /* DECODER_REPETITION */
