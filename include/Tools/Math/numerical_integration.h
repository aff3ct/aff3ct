/*!
 * \file
 * \brief Functions for numerical integrations.
 */
#ifndef NUMERICAL_INTEGRATION_H_
#define NUMERICAL_INTEGRATION_H_

namespace aff3ct
{
namespace tools
{

// =====================================================================================================================
// =============================================================================== MIPP optimized numerical integrations
// =====================================================================================================================

/*
 * Trapezoidal numerical integration
 * Computes the approximate integral of the array 'y' of length 'size' via the trapezoidal method with 'step' spacing
 * Operations are optimized with MIPP.
  */
template <typename R>
inline R trapz_integral(const R* y, const R step, int size);

/*
 * Rectangular numerical integration
 * Computes the approximate integral of the array 'y' of length 'size' via the rectangular method with 'step' spacing
 * Integral is the y values sum multiplyed by step
 * Operations are optimized with MIPP.
 */
template <typename R>
inline R rect_integral(const R* y, const R step, const int size);



// =====================================================================================================================
// =================================================================================== Trapezoidal numerical integration
// =====================================================================================================================

/*
 * Trapezoidal numerical integration
 * Computes the approximate integral of the array 'y' of length 'size' via the trapezoidal method with 'step' spacing
 */
template <typename R>
inline R trapz_integral_seq(const R* y, const R step, int size);

/*
 * Cumulative Trapezoidal numerical integration
 * Computes the approximate integral of the array 'y' of length 'size' via the trapezoidal method with 'step' spacing
 * Return the cumulative values in the 'cumul' array of length 'size'
 */
template <typename R>
inline void cumtrapz_integral_seq(const R* y, const R step, R* cumul, int size);

/*
 * Trapezoidal numerical integration
 * Computes the approximate integral of the array ('x','y') of length 'size' via the trapezoidal method
 */
template <typename R>
inline R trapz_integral_seq(const R* x, const R* y, int size);

/*
 * Cumulative Trapezoidal numerical integration
 * Computes the approximate cumulative integral of the array ('x','y') of length 'size' via the trapezoidal method
 * Return the cumulative values in the 'cumul' array of length 'size'
 */
template <typename R>
inline void cumtrapz_integral_seq(const R* x, const R* y, R* cumul, int size);

/*
 * Trapezoidal numerical integration
 * Computes the approximate integral of the function 'f' from 'min' to 'max' with 'number_steps' steps via the trapezoidal method
 */
template <typename R, typename Function>
inline R trapz_integral_seq(Function f, const R min, const R max, const int number_steps);




// =====================================================================================================================
// =================================================================================== Rectangular numerical integration
// =====================================================================================================================

/*
 * Rectangular numerical integration
 * Computes the approximate integral of the array 'y' of length 'size' via the rectangular method with 'step' spacing
 * Integral is the y values sum multiplyed by step
 */
template <typename R>
inline R rect_integral_seq(const R* y, const R step, const int size);


// ===================================================================== Rectangular numerical integration with function
/*
 * Middle Rectangular numerical integration
 * Computes the approximate integral of the function 'f' from 'min' to 'max' with 'number_steps' steps via the middle rectangular method
 * the function 'f' must be a "R(*f)(const R x)" callable object
 */
template <typename R, typename Function>
inline R mid_rect_integral_seq(Function f, const R min, const R max, const int number_steps);

/*
 * Left Rectangular numerical integration
 * Computes the approximate integral of the function 'f' from 'min' to 'max' with 'number_steps' steps via the left rectangular method
 * the function 'f' must be a "R(*f)(const R x)" callable object
 */
template <typename R, typename Function>
inline R left_rect_integral_seq(Function f, const R min, const R max, const int number_steps);

/*
 * Right Rectangular numerical integration
 * Computes the approximate integral of the function 'f' from 'min' to 'max' with 'number_steps' steps via the right rectangular method
 * the function 'f' must be a "R(*f)(const R x)" callable object
 */
template <typename R, typename Function>
inline R right_rect_integral_seq(Function f, const R min, const R max, const int number_steps);


// ======================================================================== Rectangular numerical integration with (x,y)
/*
 * Middle Rectangular numerical integration
 * Computes the approximate integral of the array ('x','y') of length 'size' via the middle rectangular method
 */
template <typename R>
inline R mid_rect_integral_seq(const R* x, const R* y, int size);

/*
 * Left Rectangular numerical integration
 * Computes the approximate integral of the array ('x','y') of length 'size' via the left rectangular method
 */
template <typename R>
inline R left_rect_integral_seq(const R* x, const R* y, int size);

/*
 * Right Rectangular numerical integration
 * Computes the approximate integral of the array ('x','y') of length 'size' via the right rectangular method
 */
template <typename R>
inline R right_rect_integral_seq(const R* x, const R* y, int size);


// =====================================================================================================================
// ======================================================================================= Simpson numerical integration
// =====================================================================================================================

/*
 * Simpson numerical integration
 * Computes the approximate integral of the function 'f' from 'min' to 'max' with 'number_steps' steps via the Simpson method
 */
template <typename R, typename Function>
inline R simps_integral_seq(Function f, const R min, const R max, const int number_steps);

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Math/numerical_integration.hxx"
#endif

#endif // NUMERICAL_INTEGRATION_H_