#ifndef DECODER_LDPC_BP_LAYERED_LOG_SUM_PRODUCT_HPP_
#define DECODER_LDPC_BP_LAYERED_LOG_SUM_PRODUCT_HPP_

#include "../Decoder_LDPC_BP_layered.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_layered_log_sum_product : public Decoder_LDPC_BP_layered<B,R>
{
private:
	std::vector<R> contributions;
	std::vector<R> values;

public:
	Decoder_LDPC_BP_layered_log_sum_product(const int &K, const int &N, const int& n_ite,
	                                        const tools::Sparse_matrix &H,
	                                        const std::vector<unsigned> &info_bits_pos,
	                                        const bool enable_syndrome = true,
	                                        const int syndrome_depth = 1,
	                                        const int n_frames = 1,
	                                        const std::string name = "Decoder_LDPC_BP_layered_log_sum_product");
	virtual ~Decoder_LDPC_BP_layered_log_sum_product();

protected:
	void BP_process(std::vector<R> &var_nodes, std::vector<R> &branches);
};

template <typename B = int, typename R = float>
using Decoder_LDPC_BP_layered_LSPA = Decoder_LDPC_BP_layered_log_sum_product<B,R>;
}
}

#endif /* DECODER_LDPC_BP_LAYERED_LOG_SUM_PRODUCT_HPP_ */
