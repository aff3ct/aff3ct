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
T linear_interpolation(const T* xData, const T* yData, const unsigned lData, const T xVal)
{
	auto x_above = std::lower_bound(xData, xData + lData, xVal); // find the position of the first x that is above the xVal

	if (x_above == xData)
		return *yData;

	auto x_below = x_above - 1; // get the position of value just below or equal to xVal
	auto y_below = yData + (x_below - xData); // get the position of the matching value y of x_below

	if ((x_above == (xData + lData)) || comp_equal(xVal, *x_below)) // if last or x_below == xVal
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
void linear_interpolation(const T* xData, const T* yData, const unsigned lData,
                          const T* xVals,       T* yVals, const unsigned lVals)
{
	for(unsigned j = 0; j < lVals; j++)
		yVals[j] = linear_interpolation(xData, yData, lData, xVals[j]);
}

template <typename T>
void linear_interpolation(const std::vector<T>& xData, const std::vector<T>& yData,
                          const std::vector<T>& xVals,       std::vector<T>& yVals)
{
	assert(xData.size() == yData.size());
	assert(xVals.size() == yVals.size());

	for(unsigned j = 0; j < xVals.size(); j++)
		yVals[j] = linear_interpolation(xData.data(), yData.data(), xData.size(), xVals[j]);
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