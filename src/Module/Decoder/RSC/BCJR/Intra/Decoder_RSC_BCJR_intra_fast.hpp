#ifndef DECODER_RSC_BCJR_INTRA_FAST_HPP_
#define DECODER_RSC_BCJR_INTRA_FAST_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/max.h"

#include "Decoder_RSC_BCJR_intra.hpp"

template <typename B, typename R, proto_max_i<R> MAX>
class Decoder_RSC_BCJR_intra_fast : public Decoder_RSC_BCJR_intra<B,R>
{
public:
	Decoder_RSC_BCJR_intra_fast(const int &K,
	                            const std::vector<std::vector<int>> &trellis,
	                            const bool buffered_encoding = true,
	                            const std::string name = "Decoder_RSC_BCJR_intra_fast");
	virtual ~Decoder_RSC_BCJR_intra_fast();

protected:
	void compute_gamma   (const mipp::vector<R> &sys, const mipp::vector<R> &par);
	void compute_alpha   (                                                      );
	void compute_beta_ext(const mipp::vector<R> &sys,       mipp::vector<R> &ext);
};

#include "Decoder_RSC_BCJR_intra_fast.hxx"

#endif /* DECODER_RSC_BCJR_INTRA_FAST_HPP_ */
