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
 * Compute the linear interpolation of xVal from the original data y_data matching with its abscissa x_data.
 * x_data, of length lData, must be sorted and strictly monotonic increasing
 * x_data and y_data must have the same length
 * If xVal goes out of x_data range, then return the left or right limit value in function of the violated one.
 */
template <typename T>
T linear_interpolation(const T* x_data, const T* y_data, const unsigned lData, const T xVal);

/*
 * Compute the linear interpolation of xVal from the original data pair (x,y) of length lData.
 * data must be sorted and strictly monotonic increasing according to the first element of its pair, x.
 * If xVal goes out of x_data range, then return the left or right limit value in function of the violated one.
 */
template <typename T>
T linear_interpolation(const Point<T>* data, const unsigned lData, const T xVal);

/*
 * Compute the linear interpolation of x_vals array, of length lVals, from the original data y_data matching with its abscissa x_data.
 * x_data, of length lData, must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled array y_vals of same size than x_vals
 */
template <typename T>
void linear_interpolation(const T* x_data, const T* y_data, const unsigned lData,
                          const T* x_vals,       T* y_vals, const unsigned lVals);

/*
 * Compute the linear interpolation of x_vals vector, from the original data y_data matching with its abscissa x_data.
 * x_data must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled vector y_vals of same size than x_vals.
 */
template <typename T>
void linear_interpolation(const std::vector<T>& x_data, const std::vector<T>& y_data,
                          const std::vector<T>& x_vals,       std::vector<T>& y_vals);

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