#ifndef DECODER_LDPC_BP_FLOODING_LOG_SUM_PRODUCT_HPP_
#define DECODER_LDPC_BP_FLOODING_LOG_SUM_PRODUCT_HPP_

#include "../Decoder_LDPC_BP_flooding.hpp"

template <typename B, typename R>
class Decoder_LDPC_BP_flooding_log_sum_product : public Decoder_LDPC_BP_flooding<B,R>
{
public:
	Decoder_LDPC_BP_flooding_log_sum_product(const int &K, const int &N, const int& n_ite,
	                                         const AList_reader &alist_data,
	                                         const int n_frames = 1,
	                                         const std::string name = "Decoder_LDPC_BP_flooding_log_sum_product");
	virtual ~Decoder_LDPC_BP_flooding_log_sum_product();

protected:
	// BP functions for decoding
	virtual bool BP_process();
};

#endif /* DECODER_LDPC_BP_FLOODING_LOG_SUM_PRODUCT_HPP_ */
