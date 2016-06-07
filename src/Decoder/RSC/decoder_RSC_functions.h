#ifndef DECODER_RSC_FUNCTIONS_H
#define DECODER_RSC_FUNCTIONS_H

#include "../../Tools/MIPP/mipp.h"

#include "../decoder_functions.h"

// -------------------------------------------------------------------------- special function prototypes for templates

template <typename R>
using proto_map = R (*)(const R& lambda_a, const R& lambda_b);

template <typename R>
using proto_map_i = mipp::reg (*)(const mipp::reg& lambda_a, const mipp::reg& lambda_b);

template <typename R>
using proto_hmap_i = mipp::reg (*)(const mipp::reg& lambda_a);

// ------------------------------------------------------------------------------------------------------ tools headers

// ------------------------------------------------------------------------------------------- special function headers

template <typename R>
inline R max_log_MAP(const R& lambda_a, const R& lambda_b) __attribute__((always_inline));

template <typename R>
inline mipp::reg max_log_MAP_i(const mipp::reg& lambda_a, const mipp::reg& lambda_b) __attribute__((always_inline));

template <typename R>
inline R linear_log_MAP(const R& lambda_a, const R& lambda_b) __attribute__((always_inline));

template <typename R>
inline mipp::reg linear_log_MAP_i(const mipp::reg& lambda_a, const mipp::reg& lambda_b) __attribute__((always_inline));

template <typename R>
inline R log_MAP(const R& lambda_a, const R& lambda_b) __attribute__((always_inline));

template <typename R>
inline mipp::reg hmax_log_MAP_i(const mipp::reg& lambda_a) __attribute__((always_inline));

#include "decoder_RSC_functions.hxx"

#endif /* DECODER_RSC_FUNCTIONS_H */
