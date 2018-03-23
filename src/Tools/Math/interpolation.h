#ifndef INTERPOLATION_H_
#define INTERPOLATION_H_

#include <vector>

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
 * Compute the linear interpolation of xVals array, of length lVals, from the original data yData matching with its abscissa xData.
 * xData, of length lData, must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled array yVals of same size than xVals
 */
template <typename T>
void linear_interpolation(const T* xData, const T* yData, const int lData, // xData is sorted and is strictly monotonic increasing
                          const T* xVals,       T* yVals, const int lVals);

/*
 * Compute the linear interpolation of xVals vector, from the original data yData matching with its abscissa xData.
 * xData, of length lData, must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled vector yVals of same size than xVals.
 */
template <typename T>
void linear_interpolation(const std::vector<T>& xData, const std::vector<T>& yData, // xData is sorted and is strictly monotonic increasing
                          const std::vector<T>& xVals,       std::vector<T>& yVals);

}
}

#include "interpolation.hxx"

#endif // INTERPOLATION_H_