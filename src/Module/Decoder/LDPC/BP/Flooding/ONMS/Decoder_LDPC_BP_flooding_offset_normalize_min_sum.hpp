#ifndef DECODER_LDPC_BP_FLOODING_MIN_SUM_OFFSET_HPP_
#define DECODER_LDPC_BP_FLOODING_MIN_SUM_OFFSET_HPP_

#include "../Decoder_LDPC_BP_flooding.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
class Decoder_LDPC_BP_flooding_offset_normalize_min_sum : public Decoder_LDPC_BP_flooding<B,R>
{
private:
	const R normalize_factor;
	const R offset;

public:
	Decoder_LDPC_BP_flooding_offset_normalize_min_sum(const int &K, const int &N, const int& n_ite,
	                                                  const tools::AList_reader &alist_data,
	                                                  const float normalize_factor = 1.f,
	                                                  const float offset = 0.f,
	                                                  const bool enable_syndrome = true,
	                                                  const int n_frames = 1,
	                                                  const std::string name = "Decoder_LDPC_BP_flooding_offset_normalize_min_sum");
	virtual ~Decoder_LDPC_BP_flooding_offset_normalize_min_sum();

protected:
	// BP functions for decoding
	virtual bool BP_process(const mipp::vector<R> &Y_N, mipp::vector<R> &V_to_C, mipp::vector<R> &C_to_V);
};
}
}

#endif /* DECODER_LDPC_BP_FLOODING_MIN_SUM_OFFSET_HPP_ */
