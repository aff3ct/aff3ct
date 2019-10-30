#include <mipp.h>

#include "Tools/Math/utils.h"
#include "Tools/Math/numerical_integration.h"

namespace aff3ct
{
namespace tools
{

// =====================================================================================================================
// =============================================================================== MIPP optimized numerical integrations
// =====================================================================================================================

template <typename R>
R trapz_integral(const R* y, const R step, int size)
{
	assert(y    != 0);
	assert(step != 0);
	assert(size != 0);

	if (size <= 1)
		return (R)0;

	mipp::Reg<R> area = (R)0;

	size--;

	const auto vec_loop_size = (size / mipp::N<R>()) * mipp::N<R>();

	for (auto i = 1; i < vec_loop_size; i += mipp::N<R>())
		area += mipp::Reg<R>(y+i);

	R a = mipp::hadd(area);

	for (auto i = vec_loop_size; i < size; i++)
		a += y[i];

	a += div2(y[size] + y[0]);

	return a * step;
}

template <typename R>
R rect_integral(const R* y, const R step, const int size)
{
	assert(y    != 0);
	assert(step != 0);
	assert(size != 0);

	if (size <= 1)
		return (R)0;

	mipp::Reg<R> area = (R)0;

	const auto vec_loop_size = (size / mipp::N<R>()) * mipp::N<R>();

	for (auto i = 0; i < vec_loop_size; i += mipp::N<R>())
		area += mipp::Reg<R>(y+i);

	R a = mipp::hadd(area);

	for (auto i = vec_loop_size; i < size; i++)
		a += y[i];

	return a * step;
}



// =====================================================================================================================
// =================================================================================== Trapezoidal numerical integration
// =====================================================================================================================

template <typename R>
R trapz_integral_seq(const R* y, const R step, int size)
{
	assert(y    != 0);
	assert(step != 0);
	assert(size != 0);

	if (size <= 1)
		return (R)0;

	R area = div2(y[0]);

	size--;

	for (auto i = 1; i < size; i++)
		area += y[i];

	area += div2(y[size]);

	return area * step;
}

template <typename R>
void cumtrapz_integral_seq(const R* y, const R step, R* cumul, int size)
{
	assert(y    != 0);
	assert(cumul!= 0);
	assert(step != 0);
	assert(size != 0);

	if (size == 0)
		return;

	if (size == 1)
	{
		*cumul = 0;
		return;
	}

	size--;

	R area = 0;

	for (auto i = 0; i < size; i++)
	{
		cumul[i] = area;
		area += div2((y[i+1] + y[i])) * step;
	}

	cumul[size] = area;
}

template <typename R>
R trapz_integral_seq(const R* x, const R* y, int size)
{
	assert(x    != 0);
	assert(y    != 0);
	assert(size != 0);

	if (size <= 1)
		return (R)0;

	size--;

	R area = 0;

	for (auto i = 0; i < size; i++)
		area += div2((y[i+1] + y[i])) * (x[i+1] - x[i]);

	return area;
}

template <typename R>
void cumtrapz_integral_seq(const R* x, const R* y, R* cumul, int size)
{
	assert(x    != 0);
	assert(y    != 0);
	assert(cumul!= 0);
	assert(size != 0);

	if (size == 0)
		return;

	if (size == 1)
	{
		*cumul = 0;
		return;
	}

	size--;

	R area = 0;

	for (auto i = 0; i < size; i++)
	{
		cumul[i] = area;
		area += div2((y[i+1] + y[i])) * (x[i+1] - x[i]);
	}

	cumul[size] = area;
}

template <typename R, typename Function>
R trapz_integral_seq(Function f, const R min, const R max, const int number_steps)
{
	assert(number_steps != 0);
	assert(max >= min);

	R step = (max - min) / number_steps; // width of rectangle
	R area = (R)0;

	R stop = max - step;

	for (R i = min + step ; i <= stop ; i += step)
		area += f(i);

	area += div2(f(max) + f(min));

	return area * step;
}

// =====================================================================================================================
// =================================================================================== Rectangular numerical integration
// =====================================================================================================================

template <typename R>
R rect_integral_seq(const R* y, const R step, const int size)
{
	assert(y    != 0);
	assert(step != 0);
	assert(size != 0);

	if (size <= 1)
		return (R)0;

	R area = 0;

	for (auto i = 0; i < size; i++)
		area += y[i];

	return area * step;
}

// ===================================================================== Rectangular numerical integration with function
template <typename R, typename Function>
R mid_rect_integral_seq(Function f, const R min, const R max, const int number_steps)
{
	assert(number_steps != 0);
	assert(max >= min);

	R step = (max - min) / number_steps; // width of rectangle
	R area = (R)0;

	for (R i = min + step * (R)0.5 ; i < max ; i += step)
		area += f(i);

	return area * step;
}

template <typename R, typename Function>
R left_rect_integral_seq(Function f, const R min, const R max, const int number_steps)
{
	assert(number_steps != 0);
	assert(max >= min);

	R step = (max - min) / number_steps; // width of rectangle
	R area = (R)0;

	for (R i = min ; i < max ; i += step)
		area += f(i);

	return area * step;
}

template <typename R, typename Function>
R right_rect_integral_seq(Function f, const R min, const R max, const int number_steps)
{
	assert(number_steps != 0);
	assert(max >= min);

	R step = (max - min) / number_steps; // width of rectangle
	R area = (R)0;

	for (R i = min + step ; i <= max ; i += step)
		area += f(i);

	return area * step;
}

// ======================================================================== Rectangular numerical integration with (x,y)
template <typename R>
R mid_rect_integral_seq(const R* x, const R* y, int size)
{
	assert(x    != 0);
	assert(y    != 0);
	assert(size != 0);

	if (size <= 1)
		return (R)0;

	size--;

	R area = 0;

	for (auto i = 0; i < size; i++)
		area += div2((y[i+1] + y[i])) * (x[i+1] - x[i]);

	return area;
}

template <typename R>
R left_rect_integral_seq(const R* x, const R* y, int size)
{
	assert(x    != 0);
	assert(y    != 0);
	assert(size != 0);

	if (size <= 1)
		return (R)0;

	size--;

	R area = 0;

	for (auto i = 0; i < size; i++)
		area += y[i] * (x[i+1] - x[i]);

	return area;
}

template <typename R>
R right_rect_integral_seq(const R* x, const R* y, int size)
{
	assert(x    != 0);
	assert(y    != 0);
	assert(size != 0);

	if (size <= 1)
		return (R)0;

	R area = 0;

	for (auto i = 1; i < size; i++)
		area += y[i] * (x[i] - x[i-1]);

	return area;
}

// =====================================================================================================================
// ======================================================================================= Simpson numerical integration
// =====================================================================================================================

template <typename R, typename Function>
R simps_integral_seq(Function f, const R min, const R max, const int number_steps)
{
	assert(number_steps != 0);
	assert(max >= min);

	R step = (max - min) / number_steps; // width of rectangle
	R area = (R)0;

	for (R i = min ; i < max ; i += step)
		area += f(i) + 4*f(i+div2(step)) + f(i+step);

	return area * step / (R)6;
}

}
}
