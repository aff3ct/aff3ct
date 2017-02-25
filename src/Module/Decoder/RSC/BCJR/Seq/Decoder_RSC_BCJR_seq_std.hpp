#ifndef DECODER_RSC_BCJR_STD_SEQ_HPP_
#define DECODER_RSC_BCJR_STD_SEQ_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/max.h"

#include "Decoder_RSC_BCJR_seq.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
class Decoder_RSC_BCJR_seq_std : public Decoder_RSC_BCJR_seq<B,R>
{

public:
	Decoder_RSC_BCJR_seq_std(const int &K,
	                         const std::vector<std::vector<int>> &trellis,
	                         const bool buffered_encoding = true,
	                         const int n_frames = 1,
	                         const std::string name = "Decoder_RSC_BCJR_seq_std");
	virtual ~Decoder_RSC_BCJR_seq_std();

	virtual void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

protected:
	virtual void compute_gamma   (const mipp::vector<R> &sys, const mipp::vector<R> &par);
	virtual void compute_alpha   (                                                      );
	virtual void compute_beta    (                                                      );
	virtual void compute_ext     (const mipp::vector<R> &sys,       mipp::vector<R> &ext);
	virtual void compute_beta_ext(const mipp::vector<R> &sys,       mipp::vector<R> &ext);
};
}
}

#include "Decoder_RSC_BCJR_seq_std.hxx"

#endif /* DECODER_RSC_BCJR_STD_SEQ_HPP_ */
