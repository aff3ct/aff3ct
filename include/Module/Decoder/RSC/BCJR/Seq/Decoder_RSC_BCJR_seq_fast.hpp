/*!
 * \file
 * \brief Class module::Decoder_RSC_BCJR_seq_fast.
 */
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
	                          const bool buffered_encoding = true);
	virtual ~Decoder_RSC_BCJR_seq_fast() = default;

	virtual Decoder_RSC_BCJR_seq_fast<B,R,RD,MAX1,MAX2>* clone() const;

protected:
	int _decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id);

	virtual void compute_gamma(const R *sys, const R *par);
	virtual void compute_alpha(                          );
	virtual void compute_beta (                          );
	virtual void compute_ext  (const R *sys,       R *ext);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_fast.hxx"
#endif

#endif /* DECODER_RSC_BCJR_SEQ_FAST_HPP_ */
