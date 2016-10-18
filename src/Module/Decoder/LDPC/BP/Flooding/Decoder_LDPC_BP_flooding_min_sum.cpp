#include <limits>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding_min_sum.hpp"

template <typename B, typename R>
Decoder_LDPC_BP_flooding_min_sum<B,R>
::Decoder_LDPC_BP_flooding_min_sum(const int &K, const int &N, const int& n_ite,
                                   const mipp::vector<unsigned char> &n_variables_per_parity,
                                   const mipp::vector<unsigned char> &n_parities_per_variable,
                                   const mipp::vector<unsigned int > &transpose,
                                   const std::string name)
: Decoder_LDPC_BP_flooding<B,R>(K, N, n_ite, n_variables_per_parity, n_parities_per_variable, transpose, name)
{
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_min_sum<B,R>
::~Decoder_LDPC_BP_flooding_min_sum()
{
}

// normalized offest min-sum implementation
template <typename B, typename R>
bool Decoder_LDPC_BP_flooding_min_sum<B,R>
::BP_process()
{
	auto syndrome = 0;	
	auto transpose_ptr = this->transpose.data();

	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		const auto length = this->n_variables_per_parity[i];

		auto sign = 0;
		auto min1 = std::numeric_limits<R>::max();
		auto min2 = std::numeric_limits<R>::max();

		// accumulate the incoming information in CN
		for (auto j = 0; j < length; j++)
		{
			const auto value  = this->V_to_C[transpose_ptr[j]];
			const auto c_sign = std::signbit((float)value) ? -1 : 0;
			const auto v_abs  = (R)std::abs(value);
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

		// regenerate the CN outcoming values
		for (auto j = 0; j < length; j++)
		{
			const auto value   = this->V_to_C[transpose_ptr[j]];
			const auto v_abs   = (R)std::abs(value);
			const auto v_res   = ((v_abs == min1) ? cste1 : cste2);            // cmov
			const auto v_sig   = sign ^ (std::signbit((float)value) ? -1 : 0); // xor bit
			const auto v_to_st = (R)std::copysign(v_res, v_sig);               // magnitude of v_res, sign of v_sig

			this->C_to_V[transpose_ptr[j]] = v_to_st;
		}

		transpose_ptr += length;
		syndrome = syndrome || sign;
	}

	return (syndrome == 0);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_LDPC_BP_flooding_min_sum<B_8,Q_8>;
template class Decoder_LDPC_BP_flooding_min_sum<B_16,Q_16>;
template class Decoder_LDPC_BP_flooding_min_sum<B_32,Q_32>;
template class Decoder_LDPC_BP_flooding_min_sum<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_flooding_min_sum<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
