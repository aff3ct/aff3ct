#ifndef INTERPOLATION_H_
#define INTERPOLATION_H_

#include <vector>
#include <utility>

#include "Point.hpp"

namespace aff3ct
{
namespace tools
{

/*
 * Compute the linear interpolation of xVal from the original data yData matching with its abscissa xData.
 * xData, of length lData, must be sorted and strictly monotonic increasing
 * xData and yData must have the same length
 * If xVal goes out of xData range, then return the left or right limit value in function of the violated one.
 */
template <typename T>
T linear_interpolation(const T* xData, const T* yData, const int lData, const T xVal);

/*
 * Compute the linear interpolation of xVal from the original data pair (x,y) of length lData.
 * data must be sorted and strictly monotonic increasing according to the first element of its pair, x.
 * If xVal goes out of xData range, then return the left or right limit value in function of the violated one.
 */
template <typename T>
T linear_interpolation(const Point<T>* data, const int lData, const T xVal);

/*
 * Compute the linear interpolation of xVals array, of length lVals, from the original data yData matching with its abscissa xData.
 * xData, of length lData, must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled array yVals of same size than xVals
 */
template <typename T>
void linear_interpolation(const T* xData, const T* yData, const int lData,
                          const T* xVals,       T* yVals, const int lVals);

/*
 * Compute the linear interpolation of xVals vector, from the original data yData matching with its abscissa xData.
 * xData must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled vector yVals of same size than xVals.
 */
template <typename T>
void linear_interpolation(const std::vector<T>& xData, const std::vector<T>& yData,
                          const std::vector<T>& xVals,       std::vector<T>& yVals);

/*
 * Compute the linear interpolation of x absicca of 'vals' vector, from the original data pair (x,y).
 * data must be sorted and strictly monotonic increasing according to the first element of its pair, x.
 * Computed interpolation is returned in the y yield of vector 'vals'.
 */
template <typename T>
void linear_interpolation(const	std::vector<Point<T>>& data,
                                std::vector<Point<T>>& vals);

}
}

#include "interpolation.hxx"

#endif // INTERPOLATION_H_