/*!
 * \file
 * \brief Class module::Decoder_RSC_BCJR_seq_generic.
 */
#ifndef DECODER_RSC_BCJR_SEQ_GENERIC_HPP_
#define DECODER_RSC_BCJR_SEQ_GENERIC_HPP_

#include <vector>
#include <mipp.h>

#include "Module/Decoder/RSC/BCJR/Decoder_RSC_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_BCJR_seq_generic : public Decoder_RSC_BCJR<B,R>
{
protected:
	mipp::vector<mipp::vector<R>> alpha; // node metric (left to right)
	mipp::vector<mipp::vector<R>> beta;  // node metric (right to left)
	mipp::vector<mipp::vector<R>> gamma; // edge metric

	Decoder_RSC_BCJR_seq_generic(const int &K,
	                             const std::vector<std::vector<int>> &trellis,
	                             const bool buffered_encoding = true);
	virtual ~Decoder_RSC_BCJR_seq_generic() = default;
	virtual Decoder_RSC_BCJR_seq_generic<B,R>* clone() const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic.hxx"
#endif

#endif /* DECODER_RSC_BCJR_SEQ_GENERIC_HPP_ */
