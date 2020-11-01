/*!
 * \file
 * \brief Class module::Decoder_RSC_BCJR_seq.
 */
#ifndef DECODER_RSC_BCJR_SEQ_HPP_
#define DECODER_RSC_BCJR_SEQ_HPP_

#include <vector>
#include <mipp.h>

#include "Module/Decoder/RSC/BCJR/Decoder_RSC_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_BCJR_seq : public Decoder_RSC_BCJR<B,R>
{
protected:
	mipp::vector<R> alpha[8]; // node metric (left to right)
	mipp::vector<R> beta [8]; // node metric (right to left)
	mipp::vector<R> gamma[2]; // edge metric

	Decoder_RSC_BCJR_seq(const int &K,
	                     const std::vector<std::vector<int>> &trellis,
	                     const bool buffered_encoding = true);
	virtual ~Decoder_RSC_BCJR_seq() = default;
	virtual Decoder_RSC_BCJR_seq<B,R>* clone() const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq.hxx"
#endif

#endif /* DECODER_RSC_BCJR_SEQ_HPP_ */
