#ifndef DECODER_LDPC_BP_LAYERED_MIN_SUM_OFFSET_HPP_
#define DECODER_LDPC_BP_LAYERED_MIN_SUM_OFFSET_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "../Decoder_LDPC_BP_layered.hpp"

namespace aff3ct
{
template <typename B, typename R>
class Decoder_LDPC_BP_layered_offset_normalize_min_sum : public Decoder_LDPC_BP_layered<B,R>
{
private:
	const R normalize_factor;
	const R offset;
	mipp::vector<R> contributions;

public:
	Decoder_LDPC_BP_layered_offset_normalize_min_sum(const int &K, const int &N, const int& n_ite,
	                                                 const AList_reader &alist_data,
	                                                 const float normalize_factor = 1.f,
	                                                 const float offset = 0.f,
	                                                 const bool enable_syndrome = true,
	                                                 const int n_frames = 1,
	                                                 const std::string name = "Decoder_LDPC_BP_layered_offset_normalize_min_sum");
	virtual ~Decoder_LDPC_BP_layered_offset_normalize_min_sum();

protected:
	void BP_process(mipp::vector<R> &var_nodes, mipp::vector<R> &branches);
};
}

#endif /* DECODER_LDPC_BP_LAYERED_MIN_SUM_OFFSET_HPP_ */
