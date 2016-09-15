#include <iostream>
#include <cmath>     // min(), fabs(), copysign()...
#include <algorithm> // min()
#include <chrono>

#include "Tools/Display/bash_tools.h"

#include "decoder_polar_functions.h"

template <typename R>
inline R f_LR(const R& lambda_a, const R& lambda_b)
{
	std::cerr << bold_red("(EE) f_LR work only on 64bit data, exiting.") << std::endl;
	exit(EXIT_FAILURE);
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

	if(abs(lambda_a) > 17 && abs(lambda_b) > 17)
		return (R)sign_lambda_a_b * std::min(abs_lambda_a, abs_lambda_b);
	else
		return (R)2 * atanh(tanh(lambda_a / 2) * tanh(lambda_b / 2));
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
inline short f_LLR(const short& lambda_a, const short& lambda_b)
{
	auto sign_lambda_a   = sgn<short,short>(lambda_a);
	auto sign_lambda_b   = sgn<short,short>(lambda_b);
	auto sign_lambda_a_b = sign_lambda_a * sign_lambda_b;

	auto abs_lambda_a = sign_lambda_a * lambda_a;
	auto abs_lambda_b = sign_lambda_b * lambda_b;

	auto lambda_c = (short)sign_lambda_a_b * std::min(abs_lambda_a, abs_lambda_b);

	return lambda_c;
}

template <>
inline signed char f_LLR(const signed char& lambda_a, const signed char& lambda_b)
{
	auto sign_lambda_a   = sgn<signed char,signed char>(lambda_a);
	auto sign_lambda_b   = sgn<signed char,signed char>(lambda_b);
	auto sign_lambda_a_b = sign_lambda_a * sign_lambda_b;

	auto abs_lambda_a = sign_lambda_a * lambda_a;
	auto abs_lambda_b = sign_lambda_b * lambda_b;

	auto lambda_c = (signed char)sign_lambda_a_b * std::min(abs_lambda_a, abs_lambda_b);

	return lambda_c;
}

template <typename R>
inline mipp::reg f_LLR_i(const mipp::reg& r_lambda_a, const mipp::reg& r_lambda_b)
{
	const auto r_abs_lambda_a   = mipp::abs <R>(r_lambda_a                       ); // abs
	const auto r_abs_lambda_b   = mipp::abs <R>(r_lambda_b                       ); // abs
	const auto r_min_abs_lambda = mipp::min <R>(r_abs_lambda_a,   r_abs_lambda_b ); // min
	      auto r_sign_lambda_c  = mipp::sign<R>(r_lambda_a,       r_lambda_b     ); // sign
	const auto r_lambda_c       = mipp::neg <R>(r_min_abs_lambda, r_sign_lambda_c); // negate

	return r_lambda_c;
}

template <typename B, typename R>
inline R g_LR(const R& lambda_a, const R& lambda_b, const B& u)
{
	std::cerr << bold_red("(EE) f_LR work only on 64bit data, exiting.") << std::endl;
	exit(EXIT_FAILURE);
}

template <>
inline double g_LR(const double& lambda_a, const double& lambda_b, const long long& u)
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
inline short g_LLR(const short& lambda_a, const short& lambda_b, const short& u)
{
	int lambda_c = (int)((u == 0) ? lambda_a : -lambda_a) + (int)lambda_b;
	return (short)saturate<int>(lambda_c, 
	                            (int)sat_vals<short>().first, 
	                            (int)sat_vals<short>().second);
}

template <>
inline signed char g_LLR(const signed char& lambda_a, const signed char& lambda_b, const signed char& u)
{
	int lambda_c = (int)((u == 0) ? lambda_a : -lambda_a) + (int)lambda_b;
	return (signed char)saturate<int>(lambda_c, 
	                                  (int)sat_vals<signed char>().first, 
	                                  (int)sat_vals<signed char>().second);
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
	std::cerr << bold_red("(EE) f_LR work only on 64bit data, exiting.") << std::endl;
	exit(EXIT_FAILURE);
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
inline short g0_LLR(const short& lambda_a, const short& lambda_b)
{
	int lambda_c = (int)lambda_a + (int)lambda_b;
	return (short)saturate<int>(lambda_c, 
	                            (int)sat_vals<short>().first, 
	                            (int)sat_vals<short>().second);
}

template <>
inline signed char g0_LLR(const signed char& lambda_a, const signed char& lambda_b)
{
	int lambda_c = (int)lambda_a + (int)lambda_b;
	return (signed char)saturate<int>(lambda_c, 
	                                  (int)sat_vals<signed char>().first, 
	                                  (int)sat_vals<signed char>().second);
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
	return (lambda_a <= init_LR<R>());
}

template <typename B, typename R>
inline B h_LLR(const R& lambda_a)
{
	return ((lambda_a <= init_LLR<R>()) * bit_init<B>());
}

template <typename B, typename R>
inline mipp::reg h_LLR_i(const mipp::reg& r_lambda_a)
{
	const auto r_u = mipp::sign<R>(r_lambda_a); // sign
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
inline signed char phi(const signed char& mu, const signed char& lambda, const signed char& u)
{
	signed char new_mu;
	
	if (u == 0 && lambda < 0)
		new_mu = mu - lambda;
	else if (u != 0 && lambda > 0)
		new_mu = mu + lambda;
	else
		new_mu = mu;

	return saturate<signed char>(new_mu, -128, sat_vals<signed char>().second);
}

inline int compute_depth(int index, int tree_depth)
{
	auto res = 0;
	if (index == 0) return tree_depth - 1;
	else
	{
		while ((index & 1) != 1 && res <= tree_depth - 1)
		{
			index = index >> 1;
			res++;
		}
	}
	return res;
}
