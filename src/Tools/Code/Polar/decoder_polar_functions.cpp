#if !defined(__clang__) && !defined(__llvm__)

#include "decoder_polar_functions.h"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
std::map<std::vector<std::vector<bool>>,
         std::vector<std::function<R(const std::vector<R> &LLRs, const std::vector<B> &bits)>>>
aff3ct::tools::Polar_lambdas<B,R>::functions = {
{
	{{1}},
	{
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			return LLRs[0];
		}
	}
},
{
	{{1,0},
	 {1,1}},
	{
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto sign = std::signbit((float)LLRs[0]) ^ std::signbit((float)LLRs[1]);
			auto abs0 = (R)std::abs(LLRs[0]);
			auto abs1 = (R)std::abs(LLRs[1]);
			auto min = std::min(abs0, abs1);

			return sign ? -min : min;
		},
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			return ((bits[0] == 0) ? LLRs[0] : -LLRs[0]) + LLRs[1];
		}
	}
},
{
	{{1,1,1},
	 {1,0,1},
	 {0,1,1}},
	{
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto sign = std::signbit((float)LLRs[0]) ^ std::signbit((float)LLRs[1]) ^ std::signbit((float)LLRs[2]);
			auto abs0 = (R)std::abs(LLRs[0]);
			auto abs1 = (R)std::abs(LLRs[1]);
			auto abs2 = (R)std::abs(LLRs[2]);
			auto min = std::min(std::min(abs0, abs1), abs2);

			return sign ? -min : min;
		},
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto sign = std::signbit((float)LLRs[1]) ^ std::signbit((float)LLRs[2]);
			auto abs1 = (R)std::abs(LLRs[1]);
			auto abs2 = (R)std::abs(LLRs[2]);
			auto min = std::min(abs1, abs2);

			auto l1_x_l2 = sign ? -min : min;

			return ((bits[0] == 0) ? LLRs[0] : -LLRs[0]) + l1_x_l2;
		},
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			return (( bits[0]            == 0) ? LLRs[1] : -LLRs[1]) +
			       (((bits[0] ^ bits[1]) == 0) ? LLRs[2] : -LLRs[2]);
		}
	}
},
{
	{{1,0,0},
	 {1,1,0},
	 {1,0,1}},
	{
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto sign = std::signbit((float)LLRs[0]) ^ std::signbit((float)LLRs[1]) ^ std::signbit((float)LLRs[2]);
			auto abs0 = (R)std::abs(LLRs[0]);
			auto abs1 = (R)std::abs(LLRs[1]);
			auto abs2 = (R)std::abs(LLRs[2]);
			auto min = std::min(std::min(abs0, abs1), abs2);

			return sign ? -min : min;
		},
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto hl0 = (bits[0] == 0) ? LLRs[0] : -LLRs[0];

			auto sign = std::signbit((float)hl0) ^ std::signbit((float)LLRs[2]);
			auto abs0 = (R)std::abs(hl0);
			auto abs2 = (R)std::abs(LLRs[2]);
			auto min = std::min(abs0, abs2);
			auto hl0_x_l2 = sign ? -min : min;

			return hl0_x_l2 + LLRs[1];
		},
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto hl0 = ((bits[0] ^ bits[1]) == 0) ? LLRs[0] : -LLRs[0];

			return hl0 + LLRs[2];
		}
	}
},
{
	{{1,0,0,0},
	 {1,1,0,0},
	 {1,0,1,0},
	 {1,1,1,1}},
	{
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto sign = std::signbit((float)LLRs[0]) ^
			            std::signbit((float)LLRs[1]) ^
			            std::signbit((float)LLRs[2]) ^
			            std::signbit((float)LLRs[3]);
			auto abs0 = (R)std::abs(LLRs[0]);
			auto abs1 = (R)std::abs(LLRs[1]);
			auto abs2 = (R)std::abs(LLRs[2]);
			auto abs3 = (R)std::abs(LLRs[3]);
			auto min = std::min(std::min(std::min(abs0, abs1), abs2), abs3);

			return sign ? -min : min;
		},
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto hl0 = (bits[0] == 0) ? LLRs[0] : -LLRs[0];

			auto sign = std::signbit((float)hl0) ^ std::signbit((float)LLRs[2]);
			auto abs0 = (R)std::abs(hl0);
			auto abs2 = (R)std::abs(LLRs[2]);
			auto min = std::min(abs0, abs2);
			auto hl0_x_l2 = sign ? -min : min;

			sign = std::signbit((float)LLRs[1]) ^ std::signbit((float)LLRs[3]);
			auto abs1 = (R)std::abs(LLRs[1]);
			auto abs3 = (R)std::abs(LLRs[3]);
			min = std::min(abs1, abs3);
			auto l1_x_l3 = sign ? -min : min;

			return hl0_x_l2 + l1_x_l3;
		},
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto hl0 = ((bits[0] ^ bits[1]) == 0) ? LLRs[0] : -LLRs[0];
			auto hl1 = ((          bits[1]) == 0) ? LLRs[1] : -LLRs[1];

			auto hl0_p_l2 = hl0 + LLRs[2];
			auto hl1_p_l3 = hl1 + LLRs[3];

			auto sign = std::signbit((float)hl0_p_l2) ^ std::signbit((float)hl1_p_l3);
			auto abs02 = (R)std::abs(hl0_p_l2);
			auto abs13 = (R)std::abs(hl1_p_l3);
			auto min = std::min(abs02, abs13);

			return sign ? -min : min;
		},
		[](const std::vector<R> &LLRs, const std::vector<B> &bits) -> R
		{
			auto hl0 = ((bits[0] ^ bits[1] ^ bits[2]) == 0) ? LLRs[0] : -LLRs[0];
			auto hl1 = ((          bits[1]          ) == 0) ? LLRs[1] : -LLRs[1];
			auto hl2 = ((                    bits[2]) == 0) ? LLRs[2] : -LLRs[2];

			return hl0 + hl1 + hl2 + LLRs[3];
		}
	}
}};

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template struct aff3ct::tools::Polar_lambdas<B_8,Q_8>;
template struct aff3ct::tools::Polar_lambdas<B_16,Q_16>;
template struct aff3ct::tools::Polar_lambdas<B_32,Q_32>;
template struct aff3ct::tools::Polar_lambdas<B_64,Q_64>;
#else
template struct aff3ct::tools::Polar_lambdas<B,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif