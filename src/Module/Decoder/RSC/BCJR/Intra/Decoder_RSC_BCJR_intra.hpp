#ifndef DECODER_RSC_BCJR_INTRA_HPP_
#define DECODER_RSC_BCJR_INTRA_HPP_

#include <vector>
#include <mipp.h>

#include "../Decoder_RSC_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_BCJR_intra : public Decoder_RSC_BCJR<B,R>
{
protected:
	mipp::vector<R> alpha; // node metric (left to right)
	mipp::vector<R> gamma; // edge metric

	Decoder_RSC_BCJR_intra(const int &K,
	                       const std::vector<std::vector<int>> &trellis,
	                       const bool buffered_encoding = true,
	                       const int n_frames = 1);
	virtual ~Decoder_RSC_BCJR_intra() = default;

protected:
	void _decode_siso(const R *sys, const R *par, R *ext, const int frame_id);

	virtual void compute_gamma   (const R *sys, const R *par) = 0;
	virtual void compute_alpha   (                          ) = 0;
	virtual void compute_beta_ext(const R *sys,       R *ext) = 0;
};
}
}

#include "Decoder_RSC_BCJR_intra.hxx"

#endif /* DECODER_RSC_BCJR_INTRA_HPP_ */
