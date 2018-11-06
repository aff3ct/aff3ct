#ifndef DECODER_REPETITION
#define DECODER_REPETITION

#include <vector>
#include <mipp.h>

#include "../Decoder_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_repetition : public Decoder_SISO_SIHO<B,R>
{
protected:
	const int rep_count; // number of repetitions

	const bool buffered_encoding;

	mipp::vector<R> sys;
	mipp::vector<R> par;
	mipp::vector<R> ext;

	Decoder_repetition(const int& K, const int& N, const bool buffered_encoding = true, const int n_frames = 1);
	virtual ~Decoder_repetition() = default;

	void _load       (const R *Y_N                            );
	void _decode_siho(const R *Y_N, B *V_K, const int frame_id);
};
}
}

#endif /* DECODER_REPETITION */
