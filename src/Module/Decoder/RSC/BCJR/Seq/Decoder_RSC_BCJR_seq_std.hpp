#ifndef DECODER_RSC_BCJR_STD_SEQ_HPP_
#define DECODER_RSC_BCJR_STD_SEQ_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../../decoder_RSC_functions.h"

#include "Decoder_RSC_BCJR_seq.hpp"

template <typename B, typename R, typename RD, proto_map<R> MAP1, proto_map<RD> MAP2>
class Decoder_RSC_BCJR_seq_std : public Decoder_RSC_BCJR_seq<B,R>
{

public:
	Decoder_RSC_BCJR_seq_std(const int &K,
	                         const std::vector<std::vector<int>> &trellis,
	                         const bool buffered_encoding = true,
	                         const std::string name = "Decoder_RSC_BCJR_seq_std");
	virtual ~Decoder_RSC_BCJR_seq_std();

	virtual void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

protected:
	virtual void compute_gamma   (const mipp::vector<R> &sys, const mipp::vector<R> &par);
	virtual void compute_alpha   (                                                      );
	virtual void compute_beta    (                                                      );
	virtual void compute_ext     (const mipp::vector<R> &sys,       mipp::vector<R> &ext);
	virtual void compute_beta_ext(const mipp::vector<R> &sys,       mipp::vector<R> &ext);
};

#include "Decoder_RSC_BCJR_seq_std.hxx"

#endif /* DECODER_RSC_BCJR_STD_SEQ_HPP_ */
