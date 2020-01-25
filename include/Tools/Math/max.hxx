#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>     // fabs(), copysign()...
#include <algorithm> // min()

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/max.h"

namespace aff3ct
{
namespace tools
{
template <typename R>
inline R max(const R a, const R b)
{
	return std::max(a, b);
}

template <typename R>
inline R max_linear(const R a, const R b)
{
	//return std::max(a, b) + std::max((R)0, (R)((R)0.25 * ((R)2.77 - std::abs(a - b))));
	return std::max(a, b) + std::max((R)0, (R)((R)0.301 - ((R)0.5 * std::abs(a - b))));
}

template <typename R>
inline R max_star(const R a, const R b)
{
	// the two next statements are equivalent !
	// return std::max(a, b) + std::log((R)1 + std::exp(-std::abs(a - b)));
	return std::max(a, b) + (R)std::log1p(std::exp(-std::abs(a - b)));
}

template <typename R>
inline R max_star_safe(const R a, const R b)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "This method is not defined in fixed-point arithmetic.");

	return (R)0;
}

template <>
inline float max_star_safe(const float a, const float b)
{
	float d = std::abs(a - b);

	if (d >= 37.f || std::isnan(d))
		d = 0.f; // exp(-d);
	else if (d < 37.f && d >= 9.f)
		d = std::exp(-d);
	else
		d = (float)std::log1p(std::exp(-d));

	return std::max(a, b) + d;
}

template <>
inline double max_star_safe(const double a, const double b)
{
	double d = std::abs(a - b);

	if (d >= 37.0 || std::isnan(d))
		d = 0.0; // exp(-d);
	else if (d < 37.0 && d >= 9.0)
		d = std::exp(-d);
	else
		d = std::log1p(std::exp(-d));

	return std::max(a, b) + d;
}

template <typename R>
inline mipp::Reg<R> max_i(const mipp::Reg<R> a, const mipp::Reg<R> b)
{
	return mipp::max(a, b);
}

template <typename R>
inline mipp::Reg<R> max_linear_i(const mipp::Reg<R> a, const mipp::Reg<R> b)
{
	mipp::Reg<R> zero = (R)0.0, cst1 = (R)0.301, cst2 = (R)0.5;
	return mipp::max(a, b) + mipp::max(zero, (cst1 - (mipp::abs(a - b) * cst2)));
}

template <typename R>
inline mipp::Reg<R> max_star_i(const mipp::Reg<R> a, const mipp::Reg<R> b)
{
	mipp::Reg<R> zero = (R)0.0, one = (R)1.0;
	return mipp::max(a, b) + mipp::log(one + mipp::exp(zero - mipp::abs(a - b)));
}

template <typename R>
inline mipp::Reg<R> max_star_safe_i(const mipp::Reg<R> a, const mipp::Reg<R> b)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename R>
inline R min(const R a, const R b)
{
	return std::min(a, b);
}

template <typename R>
inline R correction_linear2(const R x)
{
	if      (x > 2.625) return (R)0;
	else if (x < 1.000) return (R)-0.3750 * x + (R)0.6825;
	else                return (R)-0.1875 * x + (R)0.5;
}

template <typename R>
inline R min_star_linear2(const R a, const R b)
{
	return (R)(std::min(a, b) + correction_linear2(a + b) - correction_linear2(std::abs(a - b)));
}

template <typename R>
inline R min_star(const R a, const R b)
{
	return std::min(a, b) + (R)std::log1p(std::exp(-(a + b))) - (R)std::log1p(std::exp(-std::abs(a - b)));
}

template <typename R>
inline mipp::Reg<R> min_i(const mipp::Reg<R> a, const mipp::Reg<R> b)
{
	return mipp::min(a, b);
}

template <typename R>
inline mipp::Reg<R> correction_linear2_i(const mipp::Reg<R> x)
{
	auto res =             x * (R)-0.1875 +              (R)0.5000;
	     res = mipp::blend(                 mipp::Reg<R>((R)0.0000), res, x > mipp::Reg<R>((R)2.625));
	     res = mipp::blend(x * (R)-0.3750 +              (R)0.6825,  res, x < mipp::Reg<R>((R)1.000));

	return res;
}

template <typename R>
inline mipp::Reg<R> min_star_linear2_i(const mipp::Reg<R> a, const mipp::Reg<R> b)
{
	return mipp::min(a, b) + correction_linear2_i(a + b) - correction_linear2_i(mipp::abs(a - b));
}

template <typename R>
inline mipp::Reg<R> min_star_i(const mipp::Reg<R> a, const mipp::Reg<R> b)
{
	auto zero = mipp::Reg<R>((R)0);
	auto one = mipp::Reg<R>((R)1);

	auto log1pp = mipp::log(one + mipp::exp(zero -          (a + b)));
	auto log1pm = mipp::log(one + mipp::exp(zero - mipp::abs(a - b)));

	return mipp::min(a, b) + log1pp - log1pm;
}
}
}
