/*!
 * \file
 * \brief Class module::Decoder_RSC_DB_BCJR_DVB_RCS2.
 */
#ifndef DECODER_RSC_DB_BCJR_DVB_RCS2_HPP_
#define DECODER_RSC_DB_BCJR_DVB_RCS2_HPP_

#include <vector>

#include "Tools/Math/max.h"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_max<R> MAX = tools::max>
class Decoder_RSC_DB_BCJR_DVB_RCS2 : public Decoder_RSC_DB_BCJR<B,R>
{
public:
	Decoder_RSC_DB_BCJR_DVB_RCS2(const int K,
	                             const std::vector<std::vector<int>> &trellis,
	                             const bool buffered_encoding = true);
	virtual ~Decoder_RSC_DB_BCJR_DVB_RCS2() = default;

	virtual Decoder_RSC_DB_BCJR_DVB_RCS2<B,R,MAX>* clone() const;

protected:
	void __fwd_recursion(const R *sys, const R *par        );
	void __bwd_recursion(const R *sys, const R *par, R* ext);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_DVB_RCS2.hxx"
#endif

#endif /* DECODER_RSC_DB_BCJR_DVB_RCS2_HPP_ */
