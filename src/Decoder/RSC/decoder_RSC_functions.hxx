#include <cmath>     // min(), fabs(), copysign()...
#include <algorithm> // min()

#include "decoder_RSC_functions.h"

template <typename R>
inline R max_log_MAP(const R& lambda_a, const R& lambda_b)
{
	return std::max(lambda_a, lambda_b);
}

template <typename R>
inline mipp::Reg<R> max_log_MAP_i(const mipp::Reg<R> lambda_a, const mipp::Reg<R> lambda_b)
{
	return mipp::max(lambda_a, lambda_b);
}

template <typename R>
inline R linear_log_MAP(const R& lambda_a, const R& lambda_b)
{
	//return std::max(lambda_a, lambda_b) + std::max((R)0, (R)((R)0.25 * ((R)2.77 - std::abs(lambda_a - lambda_b))));
	return std::max(lambda_a, lambda_b) + std::max((R)0, (R)((R)0.301 - ((R)0.5 * std::abs(lambda_a - lambda_b))));
}

template <typename R>
inline mipp::Reg<R> linear_log_MAP_i(const mipp::Reg<R> lambda_a, const mipp::Reg<R> lambda_b)
{
	const auto magic_num       = mipp::Reg<R>(0.301);
	const auto max             = mipp::max(lambda_a, lambda_b);
	const auto llr             = lambda_a - lambda_b;
	const auto llr_abs         = mipp::abs(llr.sat(-127, 127));
	const auto correction_term = magic_num - llr_abs.div2();

	return max + mipp::max(mipp::Reg<R>((R)0), correction_term);
}

template <typename R>
inline R log_MAP(const R& lambda_a, const R& lambda_b) 
{
	// the two next statements are equivalent !
	// return std::max(lambda_a, lambda_b) + std::log((R)1 + std::exp(-std::abs(lambda_a - lambda_b)));
	return std::max(lambda_a, lambda_b) + std::log1p(std::exp(-std::abs(lambda_a - lambda_b)));
}