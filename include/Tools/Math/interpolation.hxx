#include <algorithm>
#include <cassert>

#include "Tools/Math/utils.h"
#include "Tools/general_utils.h"
#include "Tools/Math/interpolation.h"

namespace aff3ct
{
namespace tools
{

//************************************************************************************************* LINEAR INTERPOLATION

template <typename T>
T linear_interpolation(const T* x_data, const T* y_data, const unsigned l_data, const T x_val)
{
	auto x_above = std::lower_bound(x_data, x_data + l_data, x_val); // find the position of the first x that is above
	                                                                 // or equal to the x_val

	auto y_above = y_data + (x_above - x_data); // get the position of the matching value y of x_above

	if (x_above == x_data || comp_equal(x_val, *x_above)) // if first or x_above == x_val
		return *y_above; // good x so take y directly

	auto x_below = x_above - 1; // get the position of value just below or equal to x_val
	auto y_below = y_above - 1; // get the position of the matching value y of x_below

	if ((x_above == (x_data + l_data)) || comp_equal(x_val, *x_below)) // if last or x_below == x_val
		return *y_below; // good x so take y directly

	// compute the interpolation	 y = y0 + (y1-y0)*(x-x0)/(x1-x0);
	return *y_below + (*y_above - *y_below) * (x_val - *x_below) / (*x_above - *x_below);
}

template <typename T>
void linear_interpolation(const T* x_data, const T* y_data, const unsigned l_data,
                          const T* x_vals,       T* y_vals, const unsigned l_vals)
{
	for(unsigned j = 0; j < l_vals; j++)
		y_vals[j] = linear_interpolation(x_data, y_data, l_data, x_vals[j]);
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

//************************************************************************************************ NEAREST INTERPOLATION
template <typename T>
T nearest_interpolation(const T* x_data, const T* y_data, const unsigned l_data, const T x_val)
{
	auto pos = get_closest_index(x_data, x_data + l_data, x_val);
	return y_data[pos];
}

template <typename T>
void nearest_interpolation(const T* x_data, const T* y_data, const unsigned l_data,
                           const T* x_vals,       T* y_vals, const unsigned l_vals)
{
	for(unsigned j = 0; j < l_vals; j++)
		y_vals[j] = nearest_interpolation(x_data, y_data, l_data, x_vals[j]);
}

template <typename T>
void nearest_interpolation(const std::vector<T>& x_data, const std::vector<T>& y_data,
                           const std::vector<T>& x_vals,       std::vector<T>& y_vals)
{
	assert(x_data.size() == y_data.size());
	assert(x_vals.size() == y_vals.size());

	for(unsigned j = 0; j < x_vals.size(); j++)
		y_vals[j] = nearest_interpolation(x_data.data(), y_data.data(), x_data.size(), x_vals[j]);
}

}
}
