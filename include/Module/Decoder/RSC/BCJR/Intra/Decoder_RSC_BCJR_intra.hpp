/*!
 * \file
 * \brief Class module::Decoder_RSC_BCJR_intra.
 */
#ifndef DECODER_RSC_BCJR_INTRA_HPP_
#define DECODER_RSC_BCJR_INTRA_HPP_

#include <vector>
#include <mipp.h>

#include "Module/Decoder/RSC/BCJR/Decoder_RSC_BCJR.hpp"

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
	                       const bool buffered_encoding = true);
	virtual ~Decoder_RSC_BCJR_intra() = default;
	virtual Decoder_RSC_BCJR_intra<B,R>* clone() const;

protected:
	int _decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id);

	virtual void compute_gamma   (const R *sys, const R *par) = 0;
	virtual void compute_alpha   (                          ) = 0;
	virtual void compute_beta_ext(const R *sys,       R *ext) = 0;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra.hxx"
#endif

#endif /* DECODER_RSC_BCJR_INTRA_HPP_ */
