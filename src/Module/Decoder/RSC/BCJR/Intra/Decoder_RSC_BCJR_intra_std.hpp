#ifndef DECODER_RSC_BCJR_INTRA_STD_HPP_
#define DECODER_RSC_BCJR_INTRA_STD_HPP_

#include <vector>
#include <mipp.h>

#include "Tools/Math/max.h"

#include "Decoder_RSC_BCJR_intra.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_max_i<R> MAX = tools::max_i>
class Decoder_RSC_BCJR_intra_std : public Decoder_RSC_BCJR_intra<B,R>
{
public:
	Decoder_RSC_BCJR_intra_std(const int &K,
	                           const std::vector<std::vector<int>> &trellis,
	                           const bool buffered_encoding = true,
	                           const int n_frames = 1);
	virtual ~Decoder_RSC_BCJR_intra_std() = default;

protected:
	void compute_gamma   (const R *sys, const R *par);
	void compute_alpha   (                          );
	void compute_beta_ext(const R *sys,       R *ext);
};
}
}

#include "Decoder_RSC_BCJR_intra_std.hxx"

#endif /* DECODER_RSC_BCJR_INTRA_STD_HPP_ */
