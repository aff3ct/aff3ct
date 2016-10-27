#include <limits>
#include <cmath>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered_min_sum_offset.hpp"

template <typename B, typename R>
Decoder_LDPC_BP_layered_min_sum_offset<B,R>
::Decoder_LDPC_BP_layered_min_sum_offset(const int &K, const int &N, const int& n_ite,
                                         const AList_reader &alist_data,
                                         const bool enable_syndrome,
                                         const int n_frames,
                                         const std::string name)
: Decoder_LDPC_BP_layered<B,R>(K, N, n_ite, alist_data, enable_syndrome, n_frames, name),
  contributions(alist_data.get_CN_max_degree())
{
}

template <typename B, typename R>
Decoder_LDPC_BP_layered_min_sum_offset<B,R>
::~Decoder_LDPC_BP_layered_min_sum_offset()
{
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_layered_min_sum_offset<B,R>
::BP_process()
{
	auto kr = 0;
	auto kw = 0;
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		auto sign = 0;
		auto min1 = std::numeric_limits<R>::max();
		auto min2 = std::numeric_limits<R>::max();

		const auto n_VN = (int)this->CN_to_VN[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			contributions[j]  = this->var_nodes[this->CN_to_VN[i][j]] - this->branches[kr++];
			const auto v_abs  = (R)std::abs(contributions[j]);
			const auto c_sign = std::signbit((float)contributions[j]) ? -1 : 0;
			const auto v_temp = min1;

			sign ^= c_sign;
			min1  = std::min(min1,          v_abs         ); // 1st min
			min2  = std::min(min2, std::max(v_abs, v_temp)); // 2nd min
		}

		// compute CN inter values
		constexpr float offset     = 0.000f; // 0.00f // 0.15f
		constexpr float normalize1 = 0.825f; // 1.00f // 1.25f // 0.825f
		constexpr float normalize2 = 0.825f; // 1.00f // 1.25f // 0.825f

		auto cste1 = (min2 - offset) * normalize1;
		auto cste2 = (min1 - offset) * normalize2;
		cste1 = (cste1 < 0) ? 0 : cste1;
		cste2 = (cste2 < 0) ? 0 : cste2;

		for (auto j = 0; j < n_VN; j++)
		{
			const auto value = contributions[j];
			const auto v_abs = (R)std::abs(value);
			      auto v_res = ((v_abs == min1) ? cste1 : cste2);            // cmov
			const auto v_sig = sign ^ (std::signbit((float)value) ? -1 : 0); // xor bit
			           v_res = (R)std::copysign(v_res, v_sig);               // magnitude of v_res, sign of v_sig

			this->branches[kw++] = v_res;
			this->var_nodes[this->CN_to_VN[i][j]] = contributions[j] + v_res;
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_LDPC_BP_layered_min_sum_offset<B_8,Q_8>;
template class Decoder_LDPC_BP_layered_min_sum_offset<B_16,Q_16>;
template class Decoder_LDPC_BP_layered_min_sum_offset<B_32,Q_32>;
template class Decoder_LDPC_BP_layered_min_sum_offset<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_layered_min_sum_offset<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
