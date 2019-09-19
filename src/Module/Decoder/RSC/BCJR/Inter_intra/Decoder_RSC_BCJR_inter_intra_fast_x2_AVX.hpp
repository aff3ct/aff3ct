#ifndef DECODER_RSC_BCJR_INTER_INTRA_FAST_X2_AVX_HPP_
#define DECODER_RSC_BCJR_INTER_INTRA_FAST_X2_AVX_HPP_

#include <vector>

#include "Tools/Math/max.h"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_max_i<R> MAX = tools::max_i>
class Decoder_RSC_BCJR_inter_intra_fast_x2_AVX : public Decoder_RSC_BCJR_inter_intra<B,R>
{
public:
	Decoder_RSC_BCJR_inter_intra_fast_x2_AVX(const int &K,
	                                         const std::vector<std::vector<int>> &trellis,
	                                         const bool buffered_encoding = true,
	                                         const int n_frames = 1);
	virtual ~Decoder_RSC_BCJR_inter_intra_fast_x2_AVX() = default;

protected:
	void compute_gamma   (const R *sys, const R *par);
	void compute_alpha   (                          );
	void compute_beta_ext(const R *sys,       R *ext);
};
}
}

#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_AVX.hxx"

#endif /* DECODER_RSC_BCJR_INTER_INTRA_FAST_X2_AVX_HPP_ */
