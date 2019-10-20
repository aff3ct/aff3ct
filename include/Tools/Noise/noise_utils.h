/*!
 * \file
 * \brief Functions for noise management.
 */
#ifndef NOISE_UTILS_HPP__
#define NOISE_UTILS_HPP__

#include <mipp.h>

#include "Tools/Noise/Noise.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T, typename R>
inline Noise<T>* cast(const Noise<R>& n);

template <typename R>
constexpr R unknown_symbol_val();

template <typename R>
constexpr R unknown_llr_val();

template <typename R>
inline bool is_unknown_symbol(const R& v);

template <typename R>
inline mipp::Msk<mipp::N<R>()> is_unknown_symbol(const mipp::Reg<R>& q_in);

template <typename R>
inline bool is_unknown_llr(const R& v);

template <typename R>
inline mipp::Msk<mipp::N<R>()> is_unknown_llr(const mipp::Reg<R>& q_in);
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Noise/noise_utils.hxx"
#endif

#endif // NOISE_UTILS_HPP__