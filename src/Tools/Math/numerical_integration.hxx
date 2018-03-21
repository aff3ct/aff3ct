#ifndef NUMERICAL_INTEGRATION_HXX_
#define NUMERICAL_INTEGRATION_HXX_

#include <sstream>

#include "numerical_integration.h"
#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R>
inline R trapz_integral_seq(const R* in, const R step, int size)
{
	if (size <= 1)
		return (R)0;

	R area = div2(in[0]);

	size--;

	for (auto i = 1; i < size; i++)
		area += in[i];

	area += div2(in[size]);

	return area * step;
}

template <typename R>
inline R trapz_integral(const R* in, const R step, int size)
{
	if (size <= 1)
		return (R)0;

	mipp::Reg<R> area = (R)0;

	size--;

	const auto vec_loop_size = (size / mipp::N<R>()) * mipp::N<R>();

	for (auto i = 1; i < vec_loop_size; i += mipp::N<R>())
		area += mipp::Reg<R>(in+i);

	R a = mipp::hadd(area);

	for (auto i = vec_loop_size; i < size; i++)
		a += in[i];

	a += div2(in[size] + in[0]);

	return a * step;
}

template <typename R, typename Function>
inline R trapz_integral_seq(Function f, const R min, const R max, const int number_steps)
{
	R step = (max - min) / number_steps; // width of rectangle
	R area = (R)0;

	R stop = max - step;

	for (R i = min + step ; i < stop ; i += step)
		area += f(i);

	area += div2(f(max) + f(min));

	return area * step;
}

template <typename R>
inline R rect_integral_seq(const R* in, const R step, const int size)
{
	if (size <= 1)
		return (R)0;

	R area = 0;

	for (auto i = 0; i < size; i++)
		area += in[i];

	return area * step;
}


template <typename R>
inline R rect_integral(const R* in, const R step, const int size)
{
	if (size <= 1)
		return (R)0;

	mipp::Reg<R> area = (R)0;

	const auto vec_loop_size = (size / mipp::N<R>()) * mipp::N<R>();

	for (auto i = 0; i < vec_loop_size; i += mipp::N<R>())
		area += mipp::Reg<R>(in+i);

	R a = mipp::hadd(area);

	for (auto i = vec_loop_size; i < size; i++)
		a += in[i];

	return a * step;
}

template <typename R, typename Function>
inline R rect_integral_seq(Function f, const R min, const R max, const int number_steps)
{
	R step = (max - min) / number_steps; // width of rectangle
	R area = (R)0;

	for (auto i = 0; i < number_steps; i++)
		area += f(min + ((R)i + (R)0.5) * step);

	return area * step;
}

template <typename R>
inline R integral_seq(const R* in, const R step, const int size, const NUM_INTEG_APPROX approx)
{
	if (step <= 0)
	{
		std::stringstream message;
		message << "'step' has to be strictly positive ('step' = " << step << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (size < 0)
	{

		std::stringstream message;
		message << "'size' has to be positive ('size' = " << size << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	switch (approx)
	{
		case NUM_INTEG_APPROX::RECT :
			return rect_integral_seq(in, step, size);

		case NUM_INTEG_APPROX::TRAPZ :
			return trapz_integral_seq(in, step, size);

		default:
		{
			std::stringstream message;
			message << "Unknown approximation method ('approx' = " << approx << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

}

template <typename R>
inline R integral(const R* in, const R step, const int size, const NUM_INTEG_APPROX approx)
{
	if (step <= 0)
	{
		std::stringstream message;
		message << "'step' has to be strictly positive ('step' = " << step << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (size < 0)
	{

		std::stringstream message;
		message << "'size' has to be positive ('size' = " << size << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	switch (approx)
	{
		case NUM_INTEG_APPROX::RECT :
			return rect_integral(in, step, size);

		case NUM_INTEG_APPROX::TRAPZ :
			return trapz_integral(in, step, size);

		default:
		{
			std::stringstream message;
			message << "Unknown approximation method ('approx' = " << approx << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}
}

template <typename R, typename Function>
inline R integral_seq(Function f, const R min, const R max, const int number_steps, const NUM_INTEG_APPROX approx)
{
	if (max < min)
	{
		std::stringstream message;
		message << "'max' has to be equal or greater than 'min' ('max' = " << max << ", 'min' = " << min << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (number_steps <= 0)
	{
		std::stringstream message;
		message << "'number_steps' has to be greater than 0 ('number_steps' = " << number_steps << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	switch (approx)
	{
		case NUM_INTEG_APPROX::RECT :
			return rect_integral_seq(f, min, max, number_steps);

		case NUM_INTEG_APPROX::TRAPZ :
			return trapz_integral_seq(f, min, max, number_steps);

		default:
		{
			std::stringstream message;
			message << "Unknown approximation method ('approx' = " << approx << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}
}

}
}

#endif