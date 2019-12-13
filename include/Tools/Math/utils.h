/*!
 * \file
 * \brief Functions for generic math utils.
 */
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <vector>
#include <memory>
#include <utility>

namespace aff3ct
{
namespace tools
{
template <typename R> inline R div2      (R val         );
template <typename R> inline R div4      (R val         );
template <typename R> inline R div8      (R val         );
template <typename R> inline R comp_equal(R val1, R val2);

// init value depending on the domain
template <typename R>
constexpr inline R init_LR();

template <typename R>
constexpr inline R init_LLR();

// saturate values in function of the domain
template <typename R>
constexpr inline R sat_val();

template <typename R>
constexpr inline std::pair<R, R> sat_vals();

// return the initial value for a bit
template <typename B>
constexpr inline B bit_init();

// make a saturation on a the value "val"
template <typename T>
inline T saturate(const T val, const T min, const T max);

// make a saturation on a full vector
template <typename T, class A = std::allocator<T>>
inline void saturate(std::vector<T,A> &array, const T min, const T max);

template <typename B, typename R>
inline B sgn(R val);

template <typename T>
constexpr inline bool is_power_of_2(T x);

template <typename T>
inline T next_power_of_2(T x);

/* Returns 'true' if 'y' is a power of 'x' */
template <typename T>
bool is_power(T x, T y)
{
    // The only power of 1 is 1 itself
    if (x == 1)
        return (y == 1);

    // Repeatedly comput power of x
    T pow = 1;
    while (pow < y)
        pow *= x;

    // Check if power of x becomes y
    return (pow == y);
}

template <typename T>
inline T greatest_common_divisor(T a, T b);

template <typename T>
inline T least_common_multiple(T a, T b);

template <typename T>
inline bool signbit(T arg);
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Math/utils.hxx"
#endif

#endif /* MATH_UTILS_H */
