#include <cmath>     // min(), fabs(), copysign()...
#include <algorithm> // min()

#include "decoder_RSC_functions.h"

template <typename R>
inline R max_log_MAP(const R& lambda_a, const R& lambda_b)
{
	return std::max(lambda_a, lambda_b);
}

template <typename R>
inline mipp::reg max_log_MAP_i(const mipp::reg& lambda_a, const mipp::reg& lambda_b)
{
	return mipp::max<R>(lambda_a, lambda_b);
}

template <typename R>
inline R linear_log_MAP(const R& lambda_a, const R& lambda_b)
{
	//return std::max(lambda_a, lambda_b) + std::max((R)0, (R)((R)0.25 * ((R)2.77 - std::abs(lambda_a - lambda_b))));
	return std::max(lambda_a, lambda_b) + std::max((R)0, (R)((R)0.301 - ((R)0.5 * std::abs(lambda_a - lambda_b))));
}

template <typename R>
inline mipp::reg linear_log_MAP_i(const mipp::reg& lambda_a, const mipp::reg& lambda_b)
{
	const auto magic_num       = mipp::set1<R>(0.301);
	const auto max             = mipp::max <R>(lambda_a, lambda_b);
	const auto llr             = mipp::sat <R>(mipp::sub<R>(lambda_a, lambda_b), -127, 127);
	const auto llr_abs         = mipp::abs <R>(llr);
	const auto correction_term = mipp::sub <R>(magic_num, mipp::div2<R>(llr_abs));

	return mipp::add<R>(max, mipp::max<R>(mipp::set0<R>(), correction_term));
}

template <typename R>
inline R log_MAP(const R& lambda_a, const R& lambda_b) 
{
	// the two next statements are equivalent !
	// return std::max(lambda_a, lambda_b) + std::log((R)1 + std::exp(-std::abs(lambda_a - lambda_b)));
	return std::max(lambda_a, lambda_b) + std::log1p(std::exp(-std::abs(lambda_a - lambda_b)));
}

template <typename R>
inline mipp::reg hmax_log_MAP_i(const mipp::reg& lambda_a)
{
	return mipp::hmax<R>(lambda_a);
}