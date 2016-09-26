#ifndef DECODER_LDPC_BP_FLOODING_MIN_SUM_HPP_
#define DECODER_LDPC_BP_FLOODING_MIN_SUM_HPP_

#include "Decoder_LDPC_BP_flooding.hpp"

template <typename B, typename R>
class Decoder_LDPC_BP_flooding_min_sum : public Decoder_LDPC_BP_flooding<B,R>
{
public:
	Decoder_LDPC_BP_flooding_min_sum(const int &K, const int &N, const int& n_ite, 
	                                 const std ::vector<unsigned char> &n_variables_per_parity,
	                                 const std ::vector<unsigned char> &n_parities_per_variable,
	                                 const std ::vector<unsigned int > &transpose,
	                                 const mipp::vector<B            > &U_N,
	                                 const bool                         coset = false,
	                                 const std::string name = "Decoder_LDPC_BP_flooding_min_sum");
	virtual ~Decoder_LDPC_BP_flooding_min_sum();

protected:
	// BP functions for decoding
	virtual bool BP_process();
};

#endif /* DECODER_LDPC_BP_FLOODING_MIN_SUM_HPP_ */
