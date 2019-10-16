#ifndef DECODER_RSC_BCJR_STD_SEQ_HPP_
#define DECODER_RSC_BCJR_STD_SEQ_HPP_

#include <vector>

#include "Tools/Math/max.h"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename RD = float,
          tools::proto_max<R> MAX1 = tools::max, tools::proto_max<RD> MAX2 = tools::max>
class Decoder_RSC_BCJR_seq_std : public Decoder_RSC_BCJR_seq<B,R>
{
public:
	Decoder_RSC_BCJR_seq_std(const int &K,
	                         const std::vector<std::vector<int>> &trellis,
	                         const bool buffered_encoding = true,
	                         const int n_frames = 1);
	virtual ~Decoder_RSC_BCJR_seq_std() = default;

protected:
	virtual void _decode_siso(const R *sys, const R *par, R *ext, const int frame_id);

	virtual void compute_gamma   (const R *sys, const R *par);
	virtual void compute_alpha   (                          );
	virtual void compute_beta    (                          );
	virtual void compute_ext     (const R *sys,       R *ext);
	virtual void compute_beta_ext(const R *sys,       R *ext);
};
}
}

#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_std.hxx"

#endif /* DECODER_RSC_BCJR_STD_SEQ_HPP_ */
