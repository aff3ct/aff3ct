#ifndef NUMERICAL_INTEGRATION_H_
#define NUMERICAL_INTEGRATION_H_

#include <mipp.h>

namespace aff3ct
{
namespace tools
{

enum NUM_INTEG_APPROX {RECT, TRAPZ};

/*
 * Numerical integration
 * Computes the numerical integral of the array 'in' of length 'size' via the given 'method' with 'step' spacing
 * Possible methods are rectangular integration "RECT", trapezoidal integration "TRAPZ"
 * Operations are optimized with MIPP.
 */
template <typename R>
inline R integral(const R* in, const R step, const int size, const NUM_INTEG_APPROX approx = NUM_INTEG_APPROX::RECT);

/*
 * Numerical integration
 * Computes the numerical integral of the array 'in' of length 'size' via the given 'method' with 'step' spacing
 * Possible methods are rectangular integration "RECT", trapezoidal integration "TRAPZ"
 */
template <typename R>
inline R integral_seq(const R* in, const R step, const int size, const NUM_INTEG_APPROX approx = NUM_INTEG_APPROX::RECT);

/*
 * Numerical integration
 * Computes the numerical integral of the function 'f' from 'min' to 'max' with 'number_steps' steps via the rectangular method
 * Possible methods are rectangular integration "RECT", trapezoidal integration "TRAPZ"
 */
template <typename R, typename Function>
inline R integral_seq(Function f, const R min, const R max, const int number_steps, const NUM_INTEG_APPROX approx = NUM_INTEG_APPROX::RECT);

/*
 * Trapezoidal numerical integration
 * Computes the approximate integral of the array 'in' of length 'size' via the trapezoidal method with 'step' spacing
 * Operations are optimized with MIPP.
  */
template <typename R>
inline R trapz_integral(const R* in, const R step, int size);

/*
 * Trapezoidal numerical integration
 * Computes the approximate integral of the array 'in' of length 'size' via the trapezoidal method with 'step' spacing
 */
template <typename R>
inline R trapz_integral_seq(const R* in, const R step, int size);

/*
 * Trapezoidal numerical integration
 * Computes the approximate integral of the function 'f' from 'min' to 'max' with 'number_steps' steps via the trapezoidal method
 */
template <typename R, typename Function>
inline R trapz_integral_seq(Function f, const R min, const R max, const int number_steps);

/*
 * Rectangular numerical integration
 * Computes the approximate integral of the array 'in' of length 'size' via the rectangular method with 'step' spacing
 * Operations are optimized with MIPP.
 */
template <typename R>
inline R rect_integral(const R* in, const R step, const int size);

/*
 * Rectangular numerical integration
 * Computes the approximate integral of the array 'in' of length 'size' via the rectangular method with 'step' spacing
 */
template <typename R>
inline R rect_integral_seq(const R* in, const R step, const int size);

/*
 * Rectangular numerical integration
 * Computes the approximate integral of the function 'f' from 'min' to 'max' with 'number_steps' steps via the rectangular method
 * the function 'f' must be a "R(*f)(const R x)" callable object
 */
template <typename R, typename Function>
inline R rect_integral_seq(Function f, const R min, const R max, const int number_steps);

}
}

#include "numerical_integration.hxx"

#endif // NUMERICAL_INTEGRATION_H_