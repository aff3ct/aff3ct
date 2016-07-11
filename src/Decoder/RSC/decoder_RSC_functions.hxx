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
	mipp::Reg<R> zero = (R)0.0, cst1 = (R)0.301, cst2 = (R)0.5;
	return mipp::max(lambda_a, lambda_b) + mipp::max(zero, (cst1 - (mipp::abs(lambda_a - lambda_b) * cst2)));
}

template <typename R>
inline R log_MAP(const R& lambda_a, const R& lambda_b) 
{
	// the two next statements are equivalent !
	// return std::max(lambda_a, lambda_b) + std::log((R)1 + std::exp(-std::abs(lambda_a - lambda_b)));
	return std::max(lambda_a, lambda_b) + std::log1p(std::exp(-std::abs(lambda_a - lambda_b)));
}

template <typename R>
inline mipp::Reg<R> log_MAP_i(const mipp::Reg<R> lambda_a, const mipp::Reg<R> lambda_b)
{
	mipp::Reg<R> zero = (R)0.0, one = (R)1.0;
	return mipp::max(lambda_a, lambda_b) + mipp::log(one + mipp::exp(zero - mipp::abs(lambda_a - lambda_b)));
}