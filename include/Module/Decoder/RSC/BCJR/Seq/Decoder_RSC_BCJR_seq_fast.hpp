#ifndef DECODER_RSC_BCJR_SEQ_FAST_HPP_
#define DECODER_RSC_BCJR_SEQ_FAST_HPP_

#include <vector>
#include <mipp.h>

#include "Tools/Math/max.h"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename RD = float,
          tools::proto_max<R> MAX1 = tools::max, tools::proto_max<RD> MAX2 = tools::max>
class Decoder_RSC_BCJR_seq_fast : public Decoder_RSC_BCJR_seq<B,R>
{
public:
	Decoder_RSC_BCJR_seq_fast(const int &K,
	                          const std::vector<std::vector<int>> &trellis,
	                          const bool buffered_encoding = true,
	                          const int n_frames = 1);
	virtual ~Decoder_RSC_BCJR_seq_fast() = default;

protected:
	void _decode_siso(const R *sys, const R *par, R *ext, const int frame_id);

	virtual void compute_gamma(const R *sys, const R *par);
	virtual void compute_alpha(                          );
	virtual void compute_beta (                          );
	virtual void compute_ext  (const R *sys,       R *ext);
};
}
}

#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_fast.hxx"

#endif /* DECODER_RSC_BCJR_SEQ_FAST_HPP_ */
