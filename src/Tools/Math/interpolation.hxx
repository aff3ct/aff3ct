#ifndef INTERPOLATION_HXX_
#define INTERPOLATION_HXX_

#include <algorithm>
#include <assert.h>
#include <mipp.h>

#include "interpolation.h"

namespace aff3ct
{
namespace tools
{

template <typename T>
T linear_interpolation(const T* x_data, const T* y_data, const unsigned lData, const T xVal)
{
	auto x_above = std::lower_bound(x_data, x_data + lData, xVal); // find the position of the first x that is above the xVal

	if (x_above == x_data)
		return *y_data;

	auto x_below = x_above - 1; // get the position of value just below or equal to xVal
	auto y_below = y_data + (x_below - x_data); // get the position of the matching value y of x_below

	if ((x_above == (x_data + lData)) || comp_equal(xVal, *x_below)) // if last or x_below == xVal
		return *y_below; // same x so take y directly


	// compute the interpolation	 y = y0 + (y1-y0)*(x-x0)/(x1-x0);
	auto y_above = y_below + 1; // get the position of value just above

	return *y_below + (*y_above - *y_below) * (xVal - *x_below) / (*x_above - *x_below);
}

template <typename T>
T linear_interpolation(const Point<T>* data, const unsigned lData, const T xVal)
{
	// find the position of the first x that is above the xVal
	auto p_above = std::lower_bound(data, data + lData, xVal, [](const Point<T>& p, const T& x){return p.x() < x;});

	if (p_above == data)
		return data->y();

	auto p_below = p_above - 1; // get the position of value just below or equal to xVal

	if ((p_above == (data + lData)) || comp_equal(xVal, p_below->x())) // if last or p_below->x() == xVal
		return p_below->y(); // same x so take y directly


	// compute the interpolation	 y = y0 + (y1-y0)*(x-x0)/(x1-x0);
	return p_below->y() + (p_above->y() - p_below->y()) * (xVal - p_below->x()) / (p_above->x() - p_below->x());
}

template <typename T>
void linear_interpolation(const T* x_data, const T* y_data, const unsigned lData,
                          const T* x_vals,       T* y_vals, const unsigned lVals)
{
	for(unsigned j = 0; j < lVals; j++)
		y_vals[j] = linear_interpolation(x_data, y_data, lData, x_vals[j]);
}

template <typename T>
void linear_interpolation(const std::vector<T>& x_data, const std::vector<T>& y_data,
                          const std::vector<T>& x_vals,       std::vector<T>& y_vals)
{
	assert(x_data.size() == y_data.size());
	assert(x_vals.size() == y_vals.size());

	for(unsigned j = 0; j < x_vals.size(); j++)
		y_vals[j] = linear_interpolation(x_data.data(), y_data.data(), x_data.size(), x_vals[j]);
}

template <typename T>
void linear_interpolation(const	std::vector<Point<T>>& data,
                                std::vector<Point<T>>& vals)
{
	for(unsigned j = 0; j < vals.size(); j++)
		vals[j].y(linear_interpolation(data.data(), data.size(), vals[j].x()));
}

}
}

#endif