#ifndef DECODER_LDPC_BP_FLOODING_MIN_SUM_OFFSET_HPP_
#define DECODER_LDPC_BP_FLOODING_MIN_SUM_OFFSET_HPP_

#include "../Decoder_LDPC_BP_flooding.hpp"

template <typename B, typename R>
class Decoder_LDPC_BP_flooding_min_sum_offset : public Decoder_LDPC_BP_flooding<B,R>
{
public:
	Decoder_LDPC_BP_flooding_min_sum_offset(const int &K, const int &N, const int& n_ite,
	                                        const AList_reader &alist_data,
	                                        const bool enable_syndrome = true,
	                                        const int n_frames = 1,
	                                        const std::string name = "Decoder_LDPC_BP_flooding_min_sum_offset");
	virtual ~Decoder_LDPC_BP_flooding_min_sum_offset();

protected:
	// BP functions for decoding
	virtual bool BP_process();
};

#endif /* DECODER_LDPC_BP_FLOODING_MIN_SUM_OFFSET_HPP_ */
