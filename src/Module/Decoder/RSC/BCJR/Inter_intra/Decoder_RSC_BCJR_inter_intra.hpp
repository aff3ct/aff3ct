#ifndef DECODER_RSC_BCJR_INTER_INTRA_HPP_
#define DECODER_RSC_BCJR_INTER_INTRA_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Decoder_RSC_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_BCJR_inter_intra : public Decoder_RSC_BCJR<B,R>
{
protected:
	mipp::vector<R> alpha; // node metric (left to right)
	mipp::vector<R> gamma; // edge metric

public:
	Decoder_RSC_BCJR_inter_intra(const int &K,
	                             const std::vector<std::vector<int>> &trellis,
	                             const bool buffered_encoding = true,
	                             const int n_frames = 1,
	                             const std::string name = "Decoder_RSC_BCJR_inter_intra");
	virtual ~Decoder_RSC_BCJR_inter_intra();

	void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

protected:
	void load (const mipp::vector<R>& Y_N);
	void store(      mipp::vector<B>& V_K) const;
	
	virtual void compute_gamma   (const mipp::vector<R> &sys, const mipp::vector<R> &par) = 0;
	virtual void compute_alpha   (                                                      ) = 0;
	virtual void compute_beta_ext(const mipp::vector<R> &sys,       mipp::vector<R> &ext) = 0;
};
}
}

#include "Decoder_RSC_BCJR_inter_intra.hxx"

#endif /* DECODER_RSC_BCJR_INTER_INTRA_HPP_ */
