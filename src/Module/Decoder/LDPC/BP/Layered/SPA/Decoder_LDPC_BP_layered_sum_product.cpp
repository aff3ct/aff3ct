#include <limits>
#include <cmath>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered_sum_product.hpp"

template <typename B, typename R>
Decoder_LDPC_BP_layered_sum_product<B,R>
::Decoder_LDPC_BP_layered_sum_product(const int &K, const int &N, const int& n_ite,
                                      const AList_reader &alist_data,
                                      const bool enable_syndrome,
                                      const int n_frames,
                                      const std::string name)
: Decoder_LDPC_BP_layered<B,R>(K, N, n_ite, alist_data, enable_syndrome, n_frames, name),
  contributions(alist_data.get_CN_max_degree()), values(alist_data.get_CN_max_degree())
{
}

template <typename B, typename R>
Decoder_LDPC_BP_layered_sum_product<B,R>
::~Decoder_LDPC_BP_layered_sum_product()
{
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_layered_sum_product<B,R>
::BP_process()
{
	auto kr = 0;
	auto kw = 0;
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		auto sign =    0;
		auto prod = (R)1;

		const auto n_VN = (int)this->CN_to_VN[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			contributions[j]  = this->var_nodes[this->CN_to_VN[i][j]] - this->branches[kr++];
			const auto v_abs  = (R)std::abs(contributions[j]);
			const auto res    = std::tanh(v_abs * (R)0.5);
			const auto c_sign = std::signbit((float)contributions[j]) ? -1 : 0;

			sign ^= c_sign;
			prod  *= res;
			values[j] = res;
		}

		for (auto j = 0; j < n_VN; j++)
		{
			const auto value = contributions[j];
			const auto v_sig = sign ^ (std::signbit((float)value) ? -1 : 0);
			      auto val   = prod / values[j];
			           val   = (val < (R)1.0) ? val : (R)1.0 - std::numeric_limits<R>::epsilon();
			      auto v_res = (R)2.0 * std::atanh(val);
			           v_res = (R)std::copysign(v_res, v_sig);

			this->branches[kw++] = v_res;
			this->var_nodes[this->CN_to_VN[i][j]] = contributions[j] + v_res;
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_LDPC_BP_layered_sum_product<B_8,Q_8>;
template class Decoder_LDPC_BP_layered_sum_product<B_16,Q_16>;
template class Decoder_LDPC_BP_layered_sum_product<B_32,Q_32>;
template class Decoder_LDPC_BP_layered_sum_product<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_layered_sum_product<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
