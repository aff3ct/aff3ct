#ifndef MAX_H
#define MAX_H

#include "Tools/Perf/MIPP/mipp.h"

// -------------------------------------------------------------------------- special function prototypes for templates

template <typename R>
using proto_max = R (*)(const R& a, const R& b);

template <typename R>
using proto_max_i = mipp::Reg<R> (*)(const mipp::Reg<R> a, const mipp::Reg<R> b);

// ------------------------------------------------------------------------------------------------------ tools headers

// ------------------------------------------------------------------------------------------- special function headers

template <typename R> inline R max          (const R& a, const R& b) __attribute__((always_inline));
template <typename R> inline R max_linear   (const R& a, const R& b) __attribute__((always_inline));
template <typename R> inline R max_star     (const R& a, const R& b) __attribute__((always_inline));
template <typename R> inline R max_star_safe(const R& a, const R& b) __attribute__((always_inline));

template <typename R> inline mipp::Reg<R> max_i       (const mipp::Reg<R> a, const mipp::Reg<R> b) __attribute__((always_inline));
template <typename R> inline mipp::Reg<R> max_linear_i(const mipp::Reg<R> a, const mipp::Reg<R> b) __attribute__((always_inline));
template <typename R> inline mipp::Reg<R> max_star_i  (const mipp::Reg<R> a, const mipp::Reg<R> b) __attribute__((always_inline));

#include "max.hxx"

#endif /* MAX_H */
