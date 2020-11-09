/*!
 * \file
 * \brief Class module::Decoder_RSC_BCJR_seq_scan.
 */
#ifndef DECODER_RSC_BCJR_SEQ_SCAN_HPP_
#define DECODER_RSC_BCJR_SEQ_SCAN_HPP_

#include <vector>

#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq.hpp"

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
	                          const bool buffered_encoding = true);
	virtual ~Decoder_RSC_BCJR_seq_scan() = default;
	virtual Decoder_RSC_BCJR_seq_scan<B,R,RD>* clone() const;

protected:
	virtual int _decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id);

	virtual void compute_gamma(const R *sys, const R *par);
	virtual void compute_alpha(                          );
	virtual void compute_beta (                          );
	virtual void compute_ext  (const R *sys,       R *ext);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_scan.hxx"
#endif

#endif /* DECODER_RSC_BCJR_SEQ_SCAN_HPP_ */
