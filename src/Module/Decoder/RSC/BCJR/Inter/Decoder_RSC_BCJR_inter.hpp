#ifndef DECODER_RSC_BCJR_INTER_HPP_
#define DECODER_RSC_BCJR_INTER_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Decoder_RSC_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
class Decoder_RSC_BCJR_inter : public Decoder_RSC_BCJR<B,R>
{
protected:
	mipp::vector<R> alpha[8]; // node metric (left to right)
	mipp::vector<R> beta [8]; // node metric (right to left)
	mipp::vector<R> gamma[2]; // edge metric

public:
	Decoder_RSC_BCJR_inter(const int &K, 
	                       const std::vector<std::vector<int>> &trellis, 
	                       const bool buffered_encoding = true,
	                       const int n_frames = 1,
	                       const std::string name = "Decoder_RSC_BCJR_inter");
	virtual ~Decoder_RSC_BCJR_inter();

	virtual void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;

protected:
	void load (const mipp::vector<R>& Y_N);
	void store(      mipp::vector<B>& V_K) const;
};
}
}

#include "Decoder_RSC_BCJR_inter.hxx"

#endif /* DECODER_RSC_BCJR_INTER_HPP_ */
