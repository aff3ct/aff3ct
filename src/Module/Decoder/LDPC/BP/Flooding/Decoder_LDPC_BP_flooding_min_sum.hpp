#ifndef DECODER_LDPC_BP_FLOODING_MIN_SUM_HPP_
#define DECODER_LDPC_BP_FLOODING_MIN_SUM_HPP_

#include "Decoder_LDPC_BP_flooding.hpp"

template <typename B, typename R>
class Decoder_LDPC_BP_flooding_min_sum : public Decoder_LDPC_BP_flooding<B,R>
{
public:
	Decoder_LDPC_BP_flooding_min_sum(const int &K, const int &N, const int& n_ite, 
	                                 const AList_reader &alist_data,
	                                 const std::string name = "Decoder_LDPC_BP_flooding_min_sum");
	virtual ~Decoder_LDPC_BP_flooding_min_sum();

protected:
	// BP functions for decoding
	virtual bool BP_process();
};

#endif /* DECODER_LDPC_BP_FLOODING_MIN_SUM_HPP_ */
