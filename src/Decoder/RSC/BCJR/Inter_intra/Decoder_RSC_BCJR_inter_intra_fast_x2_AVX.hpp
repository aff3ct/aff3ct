#ifndef DECODER_RSC_BCJR_INTER_INTRA_FAST_X2_AVX_HPP_
#define DECODER_RSC_BCJR_INTER_INTRA_FAST_X2_AVX_HPP_

#include <vector>
#include "../../../../Tools/MIPP/mipp.h"

#include "../../decoder_RSC_functions.h"

#include "Decoder_RSC_BCJR_inter_intra.hpp"

template <typename B, typename R, proto_map_i<R> MAP>
class Decoder_RSC_BCJR_inter_intra_fast_x2_AVX : public Decoder_RSC_BCJR_inter_intra<B,R>
{
public:
	Decoder_RSC_BCJR_inter_intra_fast_x2_AVX(const int &K,
	                                         const std::vector<std::vector<int>> &trellis,
	                                         const bool buffered_encoding = true,
	                                         const std::string name = "Decoder_RSC_BCJR_inter_intra_fast_x2_AVX");
	virtual ~Decoder_RSC_BCJR_inter_intra_fast_x2_AVX();

protected:
	void compute_gamma   (const mipp::vector<R> &sys, const mipp::vector<R> &par);
	void compute_alpha   (                                                      );
	void compute_beta_ext(const mipp::vector<R> &sys,       mipp::vector<R> &ext);
};

#include "Decoder_RSC_BCJR_inter_intra_fast_x2_AVX.hxx"

#endif /* DECODER_RSC_BCJR_INTER_INTRA_FAST_X2_AVX_HPP_ */
