#ifndef DECODER_RSC_BCJR_INTER_HPP_
#define DECODER_RSC_BCJR_INTER_HPP_

#include <vector>
#include <mipp.h>

#include "../Decoder_RSC_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_BCJR_inter : public Decoder_RSC_BCJR<B,R>
{
protected:
	mipp::vector<R> alpha[8]; // node metric (left to right)
	mipp::vector<R> beta [8]; // node metric (right to left)
	mipp::vector<R> gamma[2]; // edge metric

	Decoder_RSC_BCJR_inter(const int &K,
	                       const std::vector<std::vector<int>> &trellis,
	                       const bool buffered_encoding = true,
	                       const int n_frames = 1);
	virtual ~Decoder_RSC_BCJR_inter() = default;

	void _load (const R *Y_N);
	void _store(      B *V_K) const;
};
}
}

#include "Decoder_RSC_BCJR_inter.hxx"

#endif /* DECODER_RSC_BCJR_INTER_HPP_ */
