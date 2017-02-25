#ifndef DECODER_LDPC_BP_LAYERED_LOG_SUM_PRODUCT_HPP_
#define DECODER_LDPC_BP_LAYERED_LOG_SUM_PRODUCT_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "../Decoder_LDPC_BP_layered.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
class Decoder_LDPC_BP_layered_log_sum_product : public Decoder_LDPC_BP_layered<B,R>
{
private:
	mipp::vector<R> contributions;
	mipp::vector<R> values;

public:
	Decoder_LDPC_BP_layered_log_sum_product(const int &K, const int &N, const int& n_ite,
	                                        const tools::AList_reader &alist_data,
	                                        const bool enable_syndrome = true,
	                                        const int n_frames = 1,
	                                        const std::string name = "Decoder_LDPC_BP_layered_log_sum_product");
	virtual ~Decoder_LDPC_BP_layered_log_sum_product();

protected:
	void BP_process(mipp::vector<R> &var_nodes, mipp::vector<R> &branches);
};
}
}

#endif /* DECODER_LDPC_BP_LAYERED_LOG_SUM_PRODUCT_HPP_ */
