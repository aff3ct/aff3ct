#ifndef DECODER_RSC_DB_BCJR_DVB1_HPP_
#define DECODER_RSC_DB_BCJR_DVB1_HPP_

#include <vector>

#include "Tools/Math/max.h"

#include "Decoder_RSC_DB_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_max<R> MAX = tools::max>
class Decoder_RSC_DB_BCJR_DVB1 : public Decoder_RSC_DB_BCJR<B,R>
{
public:
	Decoder_RSC_DB_BCJR_DVB1(const int K,
	                         const std::vector<std::vector<int>> &trellis,
	                         const bool buffered_encoding = true,
	                         const int n_frames = 1,
	                         const std::string name = "Decoder_RSC_DB_BCJR_DVB1");
	virtual ~Decoder_RSC_DB_BCJR_DVB1();

protected:
	void __fwd_recursion(const R *sys, const R *par        );
	void __bwd_recursion(const R *sys, const R *par, R* ext);
};
}
}
#include "Decoder_RSC_DB_BCJR_DVB1.hxx"

#endif /* DECODER_RSC_DB_BCJR_DVB1_HPP_ */
