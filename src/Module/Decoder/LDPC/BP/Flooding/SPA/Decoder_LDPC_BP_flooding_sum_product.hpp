#ifndef DECODER_LDPC_BP_FLOODING_SUM_PRODUCT_HPP_
#define DECODER_LDPC_BP_FLOODING_SUM_PRODUCT_HPP_

#include "../Decoder_LDPC_BP_flooding.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_flooding_sum_product : public Decoder_LDPC_BP_flooding<B,R>
{
private:
	mipp::vector<R> values;

public:
	Decoder_LDPC_BP_flooding_sum_product(const int &K, const int &N, const int& n_ite, 
	                                     const tools::AList_reader &alist_data,
	                                     const bool enable_syndrome = true,
	                                     const int n_frames = 1,
	                                     const std::string name = "Decoder_LDPC_BP_flooding_sum_product");
	virtual ~Decoder_LDPC_BP_flooding_sum_product();

protected:
	// BP functions for decoding
	virtual bool BP_process(const mipp::vector<R> &Y_N, mipp::vector<R> &V_to_C, mipp::vector<R> &C_to_V);
};
}
}

#endif /* DECODER_LDPC_BP_FLOODING_SUM_PRODUCT_HPP_ */
