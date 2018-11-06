#ifndef DECODER_RSC_BCJR_SEQ_GENERIC_HPP_
#define DECODER_RSC_BCJR_SEQ_GENERIC_HPP_

#include <vector>
#include <mipp.h>

#include "../Decoder_RSC_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_BCJR_seq_generic : public Decoder_RSC_BCJR<B,R>
{
protected:
	mipp::vector<mipp::vector<R>> alpha; // node metric (left to right)
	mipp::vector<mipp::vector<R>> beta;  // node metric (right to left)
	mipp::vector<mipp::vector<R>> gamma; // edge metric

	Decoder_RSC_BCJR_seq_generic(const int &K,
	                             const std::vector<std::vector<int>> &trellis,
	                             const bool buffered_encoding = true,
	                             const int n_frames = 1);
	virtual ~Decoder_RSC_BCJR_seq_generic() = default;
};
}
}

#include "Decoder_RSC_BCJR_seq_generic.hxx"

#endif /* DECODER_RSC_BCJR_SEQ_GENERIC_HPP_ */
