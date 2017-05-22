#ifndef DECODER_RSC_BCJR_SEQ_SCAN_HPP_
#define DECODER_RSC_BCJR_SEQ_SCAN_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/max.h"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename RD = float>
class Decoder_RSC_BCJR_seq_scan : public Decoder_RSC_BCJR_seq<B,R>
{

public:
	Decoder_RSC_BCJR_seq_scan(const int &K,
	                          const std::vector<std::vector<int>> &trellis,
	                          const bool buffered_encoding = true,
	                          const int n_frames = 1,
	                          const std::string name = "Decoder_RSC_BCJR_seq_scan");
	virtual ~Decoder_RSC_BCJR_seq_scan();

protected:
	virtual void _soft_decode(const R *sys, const R *par, R *ext, const int frame_id);

	virtual void compute_gamma(const R *sys, const R *par);
	virtual void compute_alpha(                          );
	virtual void compute_beta (                          );
	virtual void compute_ext  (const R *sys,       R *ext);
};
}
}

#include "Decoder_RSC_BCJR_seq_scan.hxx"

#endif /* DECODER_RSC_BCJR_SEQ_SCAN_HPP_ */
