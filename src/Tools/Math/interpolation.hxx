#ifndef INTERPOLATION_HXX_
#define INTERPOLATION_HXX_

#include <algorithm>
#include <assert.h>
#include <mipp.h>

#include "interpolation.h"

#define PRECISION 1e-5

namespace aff3ct
{
namespace tools
{

template <typename T>
T linear_interpolation(const T* xData, const T* yData, const int lData, const T xVal) // xData is sorted and is strictly monotonic increasing
{
	auto x_above = std::lower_bound(xData, xData + lData, xVal); // find the position of the first x that is above the xVal

	if (x_above == xData)
		return *yData;

	auto x_below = x_above - 1; // get the position of value just below or equal to xVal[j]
	auto y_below = yData + (x_below - xData); // get the position of the matching value y of x_below

	if ((xVal - *x_below) < (T)PRECISION) // x_below <= xVal[j]
		return *y_below; // same x so take y directly


	// compute the interpolation	 y = y0 + (y1-y0)*(x-x0)/(x1-x0);
	auto y_above = y_below + 1; // get the position of value just above

	return *y_below + (*y_above - *y_below) * (xVal - *x_below) / (*x_above - *x_below);
}

template <typename T>
void linear_interpolation(const T* xData, const T* yData, const int lData, // xData is sorted and is strictly monotonic increasing
                          const T* xVals,       T* yVals, const int lVals)
{
	for(int j = 0; j < lVals; j++)
		yVals[j] = linear_interpolation(xData, yData, lData, xVals[j]);
}

template <typename T>
void linear_interpolation(const std::vector<T>& xData, const std::vector<T>& yData, // xData is sorted and is strictly monotonic increasing
                          const std::vector<T>& xVals,       std::vector<T>& yVals)
{
	assert(xData.size() == yData.size());
	assert(xVals.size() == yVals.size());

	for(int j = 0; j < xVals.size(); j++)
		yVals[j] = linear_interpolation(xData.data(), yData.data(), xData.size(), xVals[j]);
}

}
}

#endif