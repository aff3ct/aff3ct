#include <sstream>
#include <typeinfo>
#include <limits>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered_offset_normalize_min_sum.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
inline R normalize(const R val, const float factor)
{
	     if (factor == 0.125f) return tools::div8<R>(val);
	else if (factor == 0.250f) return tools::div4<R>(val);
	else if (factor == 0.375f) return tools::div4<R>(val) + tools::div8<R>(val);
	else if (factor == 0.500f) return tools::div2<R>(val);
	else if (factor == 0.625f) return tools::div2<R>(val) + tools::div8<R>(val);
	else if (factor == 0.750f) return tools::div2<R>(val) + tools::div4<R>(val);
	else if (factor == 0.875f) return tools::div2<R>(val) + tools::div4<R>(val) + tools::div8<R>(val);
	else if (factor == 1.000f) return val;
	else
	{
		std::stringstream message;
		message << "'factor' can only be 0.125f, 0.250f, 0.375f, 0.500f, 0.625f, 0.750f, 0.875f or 1.000f ('factor' = "
		        << factor << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <>
inline float normalize(const float val, const float factor)
{
	return val * factor;
}

template <>
inline double normalize(const double val, const float factor)
{
	return val * (double)factor;
}

template <typename B, typename R>
Decoder_LDPC_BP_layered_offset_normalize_min_sum<B,R>
::Decoder_LDPC_BP_layered_offset_normalize_min_sum(const int K, const int N, const int n_ite,
                                                   const tools::Sparse_matrix &H,
                                                   const std::vector<unsigned> &info_bits_pos,
                                                   const float normalize_factor,
                                                   const R offset,
                                                   const bool enable_syndrome,
                                                   const int syndrome_depth,
                                                   const int n_frames,
                                                   const std::string name)
: Decoder(K, N, n_frames, 1, name),
  Decoder_LDPC_BP_layered<B,R>(K, N, n_ite, H, info_bits_pos, enable_syndrome, syndrome_depth, n_frames, name),
  normalize_factor(normalize_factor), offset(offset), contributions(H.get_cols_max_degree())
{
	if (typeid(R) == typeid(signed char))
	{
		std::stringstream message;
		message << "This decoder does not work in 8-bit fixed-point (try in 16-bit).";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_LDPC_BP_layered_offset_normalize_min_sum<B,R>
::~Decoder_LDPC_BP_layered_offset_normalize_min_sum()
{
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_layered_offset_normalize_min_sum<B,R>
::BP_process(std::vector<R> &var_nodes, std::vector<R> &branches)
{
	auto kr = 0;
	auto kw = 0;
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		auto sign = 0;
		auto min1 = std::numeric_limits<R>::max();
		auto min2 = std::numeric_limits<R>::max();

		const auto n_VN = (int)this->H[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			contributions[j]  = var_nodes[this->H[i][j]] - branches[kr++];
			const auto v_abs  = (R)std::abs(contributions[j]);
			const auto c_sign = std::signbit((float)contributions[j]) ? -1 : 0;
			const auto v_temp = min1;

			sign ^= c_sign;
			min1  = std::min(min1,          v_abs         ); // 1st min
			min2  = std::min(min2, std::max(v_abs, v_temp)); // 2nd min
		}

		auto cste1 = normalize<R>(min2 - offset, normalize_factor);
		auto cste2 = normalize<R>(min1 - offset, normalize_factor);
		cste1 = (cste1 < 0) ? 0 : cste1;
		cste2 = (cste2 < 0) ? 0 : cste2;

		for (auto j = 0; j < n_VN; j++)
		{
			const auto value = contributions[j];
			const auto v_abs = (R)std::abs(value);
			      auto v_res = ((v_abs == min1) ? cste1 : cste2);            // cmov
			const auto v_sig = sign ^ (std::signbit((float)value) ? -1 : 0); // xor bit
			           v_res = (R)std::copysign(v_res, v_sig);               // magnitude of v_res, sign of v_sig

			branches[kw++] = v_res;
			var_nodes[this->H[i][j]] = contributions[j] + v_res;
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_layered_offset_normalize_min_sum<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_layered_offset_normalize_min_sum<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_layered_offset_normalize_min_sum<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_layered_offset_normalize_min_sum<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_layered_offset_normalize_min_sum<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
