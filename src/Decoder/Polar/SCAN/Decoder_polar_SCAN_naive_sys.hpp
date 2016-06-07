#ifndef DECODER_POLAR_SCAN_NAIVE_SYS_
#define DECODER_POLAR_SCAN_NAIVE_SYS_

#include <vector>
#include "../../../Tools/MIPP/mipp.h"

#include "../../SISO.hpp"
#include "Decoder_polar_SCAN_naive.hpp"

template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
class Decoder_polar_SCAN_naive_sys : public Decoder_polar_SCAN_naive<B,R,I,F,V,H>, public SISO<R>
{
public:
	Decoder_polar_SCAN_naive_sys(const int &m, const int &max_iter, const mipp::vector<B> &frozen_bits);
	virtual ~Decoder_polar_SCAN_naive_sys();

	void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);
	void store (mipp::vector<B>& V_N) const;
};

#include "Decoder_polar_SCAN_naive_sys.hxx"

#endif /* DECODER_POLAR_SCAN_NAIVE_SYS_ */
