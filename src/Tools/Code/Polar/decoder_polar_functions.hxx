#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm> // min()
#include <cstdint>
#include <limits>
#include <cmath>     // fabs(), copysign()...

#include "Tools/Math/utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"

namespace aff3ct
{
namespace tools
{
template <typename R>
inline R f_LR(const R& lambda_a, const R& lambda_b)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "Works only on 64bit data.");
}

template <>
inline double f_LR(const double& lambda_a, const double& lambda_b)
{
	auto res = (double)(((double)1.0 + (lambda_a * lambda_b)) / (lambda_a + lambda_b));
	return saturate<double>(res, -std::sqrt(std::numeric_limits<double>::max()),
	                              std::sqrt(std::numeric_limits<double>::max()));
}

template <typename R>
inline R f_LLR(const R& lambda_a, const R& lambda_b)
{
	auto sign_lambda_a_b = sgn<int,R>(lambda_a * lambda_b);
	auto abs_lambda_a = (lambda_a >= 0) ? lambda_a : -lambda_a;
	auto abs_lambda_b = (lambda_b >= 0) ? lambda_b : -lambda_b;

	return (R)sign_lambda_a_b * std::min(abs_lambda_a, abs_lambda_b);
}

template <typename R>
inline R f_LLR_tanh(const R& lambda_a, const R& lambda_b)
{
	auto sign_lambda_a_b = sgn<int,R>(lambda_a * lambda_b);
	auto abs_lambda_a = (lambda_a >= 0) ? lambda_a : -lambda_a;
	auto abs_lambda_b = (lambda_b >= 0) ? lambda_b : -lambda_b;

	R Le = f_LLR(lambda_a, lambda_b);

	if(abs_lambda_a > 17 || abs_lambda_b > 17)
		return Le;
	else
		return (R)2 * std::atanh(std::tanh(lambda_a / 2) * std::tanh(lambda_b / 2));
}

template <typename R>
inline R f_LLR_tanh_safe(const R& lambda_a, const R& lambda_b)
{
	auto sign_lambda_a_b = sgn<int, R>(lambda_a * lambda_b);
	auto abs_lambda_a = (lambda_a >= 0) ? lambda_a : -lambda_a;
	auto abs_lambda_b = (lambda_b >= 0) ? lambda_b : -lambda_b;

	R le = f_LLR(lambda_a, lambda_b);
	if (abs_lambda_a > 17 || abs_lambda_b > 17)
		return le;
	else
	{
		auto abs_lambda_a_plus_lambda_b = (lambda_a + lambda_b >= 0) ? lambda_b + lambda_a : -(lambda_b + lambda_a);
		auto abs_lambda_a_minus_lambda_b = (lambda_a - lambda_b >= 0) ? lambda_a - lambda_b : -(lambda_a - lambda_b);
		return le + std::log1p(std::exp(-abs_lambda_a_plus_lambda_b)) -
		            std::log1p(std::exp(-abs_lambda_a_minus_lambda_b));
	}
}

template <>
inline float f_LLR(const float& lambda_a, const float& lambda_b)
{
	auto sign_lambda_a_b = sgn<int,float>(lambda_a * lambda_b);

	auto abs_lambda_a = fabsf(lambda_a);
	auto abs_lambda_b = fabsf(lambda_b);

	return (float)sign_lambda_a_b * std::min(abs_lambda_a, abs_lambda_b);
}

template <>
inline int16_t f_LLR(const int16_t& lambda_a, const int16_t& lambda_b)
{
	auto sign_lambda_a   = sgn<int16_t,int16_t>(lambda_a);
	auto sign_lambda_b   = sgn<int16_t,int16_t>(lambda_b);
	auto sign_lambda_a_b = sign_lambda_a * sign_lambda_b;

	auto abs_lambda_a = sign_lambda_a * lambda_a;
	auto abs_lambda_b = sign_lambda_b * lambda_b;

	auto lambda_c = (int16_t)sign_lambda_a_b * std::min(abs_lambda_a, abs_lambda_b);

	return lambda_c;
}

template <>
inline int8_t f_LLR(const int8_t& lambda_a, const int8_t& lambda_b)
{
	auto sign_lambda_a   = sgn<int8_t,int8_t>(lambda_a);
	auto sign_lambda_b   = sgn<int8_t,int8_t>(lambda_b);
	auto sign_lambda_a_b = sign_lambda_a * sign_lambda_b;

	auto abs_lambda_a = sign_lambda_a * lambda_a;
	auto abs_lambda_b = sign_lambda_b * lambda_b;

	auto lambda_c = (int8_t)sign_lambda_a_b * std::min(abs_lambda_a, abs_lambda_b);

	return lambda_c;
}

template <typename R>
inline mipp::reg f_LLR_i(const mipp::reg& r_lambda_a, const mipp::reg& r_lambda_b)
{
	const auto r_abs_lambda_a   = mipp::abs <R>(r_lambda_a                       ); // abs
	const auto r_abs_lambda_b   = mipp::abs <R>(r_lambda_b                       ); // abs
	const auto r_min_abs_lambda = mipp::min <R>(r_abs_lambda_a,   r_abs_lambda_b ); // min
//	      auto r_sign_lambda_c  = mipp::sign<R>(r_lambda_a,       r_lambda_b     ); // sign
	const auto r_sign_lambda_a  = mipp::msb <R>(r_lambda_a);
	const auto r_sign_lambda_b  = mipp::msb <R>(r_lambda_b);
	const auto r_sign_lambda_c  = mipp::xorb<R>(r_sign_lambda_a, r_sign_lambda_b);
	const auto r_lambda_c       = mipp::neg <R>(r_min_abs_lambda, r_sign_lambda_c); // negate

	return r_lambda_c;
}

template <typename B, typename R>
inline R g_LR(const R& lambda_a, const R& lambda_b, const B& u)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "Works only on 64bit data.");
}

template <>
inline double g_LR(const double& lambda_a, const double& lambda_b, const int64_t& u)
{
	auto res = (u) ? (double)(lambda_b / lambda_a) : (double)(lambda_a * lambda_b);
	return saturate<double>(res, -std::sqrt(std::numeric_limits<double>::max()),
	                              std::sqrt(std::numeric_limits<double>::max()));
}

template <typename B, typename R>
inline R g_LLR(const R& lambda_a, const R& lambda_b, const B& u)
{
	return ((u == 0) ? lambda_a : -lambda_a) + lambda_b;
}

template <>
inline int16_t g_LLR(const int16_t& lambda_a, const int16_t& lambda_b, const int16_t& u)
{
	int32_t lambda_c = (int32_t)((u == 0) ? lambda_a : -lambda_a) + (int32_t)lambda_b;
	return (int16_t)saturate<int32_t>(lambda_c, (int32_t)sat_vals<int16_t>().first,
	                                            (int32_t)sat_vals<int16_t>().second);
}

template <>
inline int8_t g_LLR(const int8_t& lambda_a, const int8_t& lambda_b, const int8_t& u)
{
	int32_t lambda_c = (int32_t)((u == 0) ? lambda_a : -lambda_a) + (int32_t)lambda_b;
	return (int8_t)saturate<int32_t>(lambda_c, (int32_t)sat_vals<int8_t>().first,
	                                           (int32_t)sat_vals<int8_t>().second);
}

template <typename B, typename R>
inline mipp::reg g_LLR_i(const mipp::reg& r_lambda_a, const mipp::reg& r_lambda_b, const mipp::reg& r_u)
{
	auto r_lambda_c = mipp::neg<R>(r_lambda_a, r_u       ); // negate
	     r_lambda_c = mipp::add<R>(r_lambda_c, r_lambda_b); // add

	return r_lambda_c;
}

template <typename R>
inline R g0_LR(const R& lambda_a, const R& lambda_b)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "Works only on 64bit data.");
}

template <>
inline double g0_LR(const double& lambda_a, const double& lambda_b)
{
	auto res = lambda_a * lambda_b;
	return saturate<double>(res, -std::sqrt(std::numeric_limits<double>::max()),
	                              std::sqrt(std::numeric_limits<double>::max()));
}

template <typename R>
inline R g0_LLR(const R& lambda_a, const R& lambda_b)
{
	return lambda_a + lambda_b;
}

template <>
inline int16_t g0_LLR(const int16_t& lambda_a, const int16_t& lambda_b)
{
	int32_t lambda_c = (int32_t)lambda_a + (int32_t)lambda_b;
	return (int16_t)saturate<int32_t>(lambda_c, (int32_t)sat_vals<int16_t>().first,
	                                            (int32_t)sat_vals<int16_t>().second);
}

template <>
inline int8_t g0_LLR(const int8_t& lambda_a, const int8_t& lambda_b)
{
	int32_t lambda_c = (int32_t)lambda_a + (int32_t)lambda_b;
	return (int8_t)saturate<int32_t>(lambda_c, (int32_t)sat_vals<int8_t>().first,
	                                           (int32_t)sat_vals<int8_t>().second);
}

template <typename R>
inline mipp::reg g0_LLR_i(const mipp::reg& r_lambda_a, const mipp::reg& r_lambda_b)
{
	const auto r_lambda_c = mipp::add<R>(r_lambda_a, r_lambda_b); // add
	return r_lambda_c;
}

template <typename B, typename R>
inline B h_LR(const R& lambda_a)
{
	return (lambda_a < init_LR<R>());
}

template <typename B, typename R>
inline B h_LLR(const R& lambda_a)
{
	return ((lambda_a < init_LLR<R>()) * bit_init<B>());
}

template <typename B, typename R>
inline mipp::reg h_LLR_i(const mipp::reg& r_lambda_a)
{
	const auto r_u = mipp::msb<R>(r_lambda_a); // sign
	return r_u;
}

template <typename R>
inline R v_LR(const R& a, const R& b)
{
	return g0_LR<R>(a, b);
}

template <typename R>
inline R v_LLR(const R& a, const R& b)
{
	return g0_LLR<R>(a, b);
}

template <typename B>
inline B xo_STD(const B& r_u_a, const B& r_u_b)
{
	return r_u_a ^ r_u_b;
}

template <typename B>
inline mipp::reg xo_STD_i(const mipp::reg& r_u_a, const mipp::reg& r_u_b)
{
	const auto r_u_c = mipp::xorb<B>(r_u_a, r_u_b);
	return r_u_c;
}

template <typename B, typename R>
inline R phi(const R& mu, const R& lambda, const B& u)
{
	R new_mu;

	if (u == 0 && lambda < 0)
		new_mu = mu - lambda;
	else if (u != 0 && lambda > 0)
		new_mu = mu + lambda;
	else
		new_mu = mu;

	return new_mu;
}

template <>
inline int8_t phi(const int8_t& mu, const int8_t& lambda, const int8_t& u)
{
	int8_t new_mu;

	if (u == 0 && lambda < 0)
		new_mu = mu - lambda;
	else if (u != 0 && lambda > 0)
		new_mu = mu + lambda;
	else
		new_mu = mu;

	return saturate<int8_t>(new_mu, -128, sat_vals<int8_t>().second);
}

inline int compute_depth(int index, int tree_depth)
{
	if (index == 0) return tree_depth - 1;
	else
	{
		auto res = 0;
		while ((index & 1) != 1 && res <= tree_depth - 1)
		{
			index = index >> 1;
			res++;
		}
		return res;
	}
}
}
}
