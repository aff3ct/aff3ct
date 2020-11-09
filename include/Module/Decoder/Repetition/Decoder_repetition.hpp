/*!
 * \file
 * \brief Class module::Decoder_repetition.
 */
#ifndef DECODER_REPETITION
#define DECODER_REPETITION

#include <mipp.h>

#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_repetition : public Decoder_SISO<B,R>
{
protected:
	const int rep_count; // number of repetitions

	const bool buffered_encoding;

	mipp::vector<R> sys;
	mipp::vector<R> par;
	mipp::vector<R> ext;

	Decoder_repetition(const int& K, const int& N, const bool buffered_encoding = true);
	virtual ~Decoder_repetition() = default;
	virtual Decoder_repetition<B,R>* clone() const;

	void _load       (const R *Y_N                               );
	int  _decode_siho(const R *Y_N, B *V_K, const size_t frame_id);
};
}
}

#endif /* DECODER_REPETITION */
