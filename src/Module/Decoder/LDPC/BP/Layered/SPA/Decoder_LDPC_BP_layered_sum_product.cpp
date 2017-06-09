#include <stdexcept>
#include <typeinfo>
#include <limits>
#include <cmath>

#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered_sum_product.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_LDPC_BP_layered_sum_product<B,R>
::Decoder_LDPC_BP_layered_sum_product(const int &K, const int &N, const int& n_ite,
                                      const Sparse_matrix &H,
                                      const mipp::vector<B> &info_bits_pos,
                                      const bool enable_syndrome,
                                      const int syndrome_depth,
                                      const int n_frames,
                                      const std::string name)
: Decoder_LDPC_BP_layered<B,R>(K, N, n_ite, H, info_bits_pos, enable_syndrome, syndrome_depth, n_frames, name),
  contributions(H.get_cols_max_degree()), values(H.get_cols_max_degree())
{
	if (typeid(R) != typeid(float) && typeid(R) != typeid(double))
		throw std::runtime_error("aff3ct::module::Decoder_LDPC_BP_layered_sum_product: this decoder only supports "
		                         "floating-point numbers for the LLRs.");
}

template <typename B, typename R>
Decoder_LDPC_BP_layered_sum_product<B,R>
::~Decoder_LDPC_BP_layered_sum_product()
{
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_layered_sum_product<B,R>
::BP_process(mipp::vector<R> &var_nodes, mipp::vector<R> &branches)
{
	auto kr = 0;
	auto kw = 0;
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		auto sign =    0;
		auto prod = (R)1;

		const auto n_VN = (int)this->H[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			contributions[j]  = var_nodes[this->H[i][j]] - branches[kr++];
			const auto v_abs  = (R)std::abs(contributions[j]);
			const auto res    = (R)std::tanh(v_abs * (R)0.5);
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
			const auto v_tan = (R)2.0 * std::atanh(val);
			const auto v_res = (R)std::copysign(v_tan, v_sig);

			branches[kw++] = v_res;
			var_nodes[this->H[i][j]] = contributions[j] + v_res;
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_layered_sum_product<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_layered_sum_product<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_layered_sum_product<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_layered_sum_product<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_layered_sum_product<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
