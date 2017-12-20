#ifndef DECODER_LDPC_BP_FLOODING_AMS_HPP_
#define DECODER_LDPC_BP_FLOODING_AMS_HPP_

#include "Tools/Math/max.h"

#include "../Decoder_LDPC_BP_flooding.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_min<R> MIN = tools::min_star_linear2>
class Decoder_LDPC_BP_flooding_approximate_min_star : public Decoder_LDPC_BP_flooding<B,R>
{
public:
	Decoder_LDPC_BP_flooding_approximate_min_star(const int K, const int N, const int n_ite,
	                                              const tools::Sparse_matrix &H,
	                                              const std::vector<unsigned> &info_bits_pos,
	                                              const bool enable_syndrome = true,
	                                              const int syndrome_depth = 1,
	                                              const int n_frames = 1,
	                                              const std::string name = "Decoder_LDPC_BP_flooding_approximate_min_star");
	virtual ~Decoder_LDPC_BP_flooding_approximate_min_star();

protected:
	// BP functions for decoding
	virtual void BP_process(const R *Y_N, std::vector<R> &V_to_C, std::vector<R> &C_to_V);
};

template <typename B = int, typename R = float, tools::proto_min<R> MIN = tools::min_star_linear2>
using Decoder_LDPC_BP_flooding_AMS = Decoder_LDPC_BP_flooding_approximate_min_star<B,R,MIN>;
}
}

#include "Decoder_LDPC_BP_flooding_approximate_min_star.hxx"

#endif /* DECODER_LDPC_BP_FLOODING_AMS_HPP_ */
