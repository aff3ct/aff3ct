/*!
 * \file
 * \brief Functions for interpolations.
 */
#ifndef INTERPOLATION_H_
#define INTERPOLATION_H_

#include <vector>

namespace aff3ct
{
namespace tools
{

enum class Interpolation_type {LINEAR, NEAREST};

//************************************************************************************************* LINEAR INTERPOLATION
/*
 * Compute the linear interpolation of x_val from the original data y_data matching with its abscissa x_data.
 * x_data, of length l_data, must be sorted and strictly monotonic increasing
 * x_data and y_data must have the same length 'l_data'
 * If x_val goes out of x_data range, then return the left or right limit value in function of the violated one.
 */
template <typename T>
T linear_interpolation(const T* x_data, const T* y_data, const unsigned l_data, const T x_val);

/*
 * Compute the linear interpolation of x_vals array, of length l_vals, from the original data y_data matching
 * with its abscissa x_data.
 * x_data, of length l_data, must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled array y_vals of same size than x_vals
 */
template <typename T>
void linear_interpolation(const T* x_data, const T* y_data, const unsigned l_data,
                          const T* x_vals,       T* y_vals, const unsigned l_vals);

/*
 * Compute the linear interpolation of x_vals vector, from the original data y_data matching with its abscissa x_data.
 * x_data must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled vector y_vals of same size than x_vals.
 */
template <typename T>
void linear_interpolation(const std::vector<T>& x_data, const std::vector<T>& y_data,
                          const std::vector<T>& x_vals,       std::vector<T>& y_vals);




//************************************************************************************************ NEAREST INTERPOLATION
/*
 * Compute the nearest interpolation of x_val from the original data y_data matching with its abscissa x_data.
 * x_data, of length l_data, must be sorted and strictly monotonic increasing
 * x_data and y_data must have the same length 'l_data'
 * If x_val goes out of x_data range, then return the left or right limit value in function of the violated one.
 */
template <typename T>
T nearest_interpolation(const T* x_data, const T* y_data, const unsigned l_data, const T x_val);

/*
 * Compute the nearest interpolation of x_vals array, of length l_vals, from the original data y_data matching
 * with its abscissa x_data.
 * x_data, of length l_data, must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled array y_vals of same size than x_vals
 */
template <typename T>
void nearest_interpolation(const T* x_data, const T* y_data, const unsigned l_data,
                           const T* x_vals,       T* y_vals, const unsigned l_vals);

/*
 * Compute the nearest interpolation of x_vals vector, from the original data y_data matching with its abscissa x_data.
 * x_data must be sorted and strictly monotonic increasing
 * Computed interpolation is returned in the filled vector y_vals of same size than x_vals.
 */
template <typename T>
void nearest_interpolation(const std::vector<T>& x_data, const std::vector<T>& y_data,
                           const std::vector<T>& x_vals,       std::vector<T>& y_vals);
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Math/interpolation.hxx"
#endif

#endif // INTERPOLATION_H_