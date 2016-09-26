#ifndef DECODER_RSC_BCJR_INTER_STD_HPP_
#define DECODER_RSC_BCJR_INTER_STD_HPP_

#include <vector>
#include "Tools/MIPP/mipp.h"

#include "Decoder_RSC_BCJR_inter.hpp"

template <typename B, typename R, proto_map_i<R> MAP>
class Decoder_RSC_BCJR_inter_std : public Decoder_RSC_BCJR_inter<B,R>
{
public:
	Decoder_RSC_BCJR_inter_std(const int &K,
	                           const std::vector<std::vector<int>> &trellis, 
	                           const bool buffered_encoding = true,
	                           const std::string name = "Decoder_RSC_BCJR_inter_std");
	virtual ~Decoder_RSC_BCJR_inter_std();

	void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

protected:
	virtual void compute_gamma(const mipp::vector<R> &sys, const mipp::vector<R> &par);
	virtual void compute_alpha(                                                      );
	virtual void compute_beta (                                                      );
	virtual void compute_ext  (const mipp::vector<R> &sys,       mipp::vector<R> &ext);
};

#include "Decoder_RSC_BCJR_inter_std.hxx"

#endif /* DECODER_RSC_BCJR_INTER_STD_HPP_ */
