/*!
 * \file
 * \brief Functions for min/max.
 */
#ifndef MAX_H
#define MAX_H

#include <mipp.h>

namespace aff3ct
{
namespace tools
{
// -------------------------------------------------------------------------- special function prototypes for templates

template <typename R>
using proto_max = R (*)(const R a, const R b);

template <typename R>
using proto_min = R (*)(const R a, const R b);

template <typename R>
using proto_max_i = mipp::Reg<R> (*)(const mipp::Reg<R> a, const mipp::Reg<R> b);

template <typename R>
using proto_min_i = mipp::Reg<R> (*)(const mipp::Reg<R> a, const mipp::Reg<R> b);

// ------------------------------------------------------------------------------------------- special function headers

template <typename R> inline R max          (const R a, const R b);
template <typename R> inline R max_linear   (const R a, const R b);
template <typename R> inline R max_star     (const R a, const R b);
template <typename R> inline R max_star_safe(const R a, const R b);

template <typename R> inline R min             (const R a, const R b);
template <typename R> inline R min_star_linear2(const R a, const R b);
template <typename R> inline R min_star        (const R a, const R b);

template <typename R> inline mipp::Reg<R> max_i          (const mipp::Reg<R> a, const mipp::Reg<R> b);
template <typename R> inline mipp::Reg<R> max_linear_i   (const mipp::Reg<R> a, const mipp::Reg<R> b);
template <typename R> inline mipp::Reg<R> max_star_i     (const mipp::Reg<R> a, const mipp::Reg<R> b);
template <typename R> inline mipp::Reg<R> max_star_safe_i(const mipp::Reg<R> a, const mipp::Reg<R> b);

template <typename R> inline mipp::Reg<R> min_i             (const mipp::Reg<R> a, const mipp::Reg<R> b);
template <typename R> inline mipp::Reg<R> min_star_linear2_i(const mipp::Reg<R> a, const mipp::Reg<R> b);
template <typename R> inline mipp::Reg<R> min_star_i        (const mipp::Reg<R> a, const mipp::Reg<R> b);
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Math/max.hxx"
#endif

#endif /* MAX_H */
