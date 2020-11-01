/*!
 * \file
 * \brief Class module::Decoder_RSC_DB_BCJR_generic.
 */
#ifndef DECODER_RSC_DB_BCJR_GENERIC_HPP_
#define DECODER_RSC_DB_BCJR_GENERIC_HPP_

#include <vector>

#include "Tools/Math/max.h"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_max<R> MAX = tools::max>
class Decoder_RSC_DB_BCJR_generic : public Decoder_RSC_DB_BCJR<B,R>
{
public:
	Decoder_RSC_DB_BCJR_generic(const int K,
	                            const std::vector<std::vector<int>> &trellis,
	                            const bool buffered_encoding = true);
	virtual ~Decoder_RSC_DB_BCJR_generic() = default;

	virtual Decoder_RSC_DB_BCJR_generic<B,R,MAX>* clone() const;

protected:
	virtual void __fwd_recursion(const R *sys, const R *par        );
	virtual void __bwd_recursion(const R *sys, const R *par, R* ext);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR_generic.hxx"
#endif

#endif /* DECODER_RSC_DB_BCJR_GENERIC_HPP_ */
