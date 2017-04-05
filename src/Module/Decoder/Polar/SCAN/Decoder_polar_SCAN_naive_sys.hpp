#ifndef DECODER_POLAR_SCAN_NAIVE_SYS_
#define DECODER_POLAR_SCAN_NAIVE_SYS_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../../SISO.hpp"
#include "Decoder_polar_SCAN_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float,
          proto_i<R> I = tools::init_LLR, proto_f<R> F = f_LLR, proto_v<R> V = v_LLR, proto_h<B,R> H = h_LLR>
class Decoder_polar_SCAN_naive_sys : public Decoder_polar_SCAN_naive<B,R,I,F,V,H>, public SISO<R>
{
public:
	Decoder_polar_SCAN_naive_sys(const int &K, const int &m, const int &max_iter, const mipp::vector<B> &frozen_bits, 
	                             const int n_frames = 1, const std::string name = "Decoder_polar_SCAN_naive_sys");
	virtual ~Decoder_polar_SCAN_naive_sys();

protected:
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
	void _soft_decode_fbf(const R *sys, const R *par, R *ext);
#ifdef __clang__
#pragma clang diagnostic pop
#endif
	void _store(B *V_N) const;
};
}
}

#include "Decoder_polar_SCAN_naive_sys.hxx"

#endif /* DECODER_POLAR_SCAN_NAIVE_SYS_ */
