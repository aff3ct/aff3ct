#ifndef DECODER_POLAR_FUNCTIONS_H
#define DECODER_POLAR_FUNCTIONS_H

#include <limits>
#include <utility> // pair

#include "Tools/MIPP/mipp.h"
#include "Tools/Math/utils.h"

// -------------------------------------------------------------------------- special function prototypes for templates

template <typename R>
using proto_i = R (*)(); // special proto: return init value for a specific domain

template <typename R>
using proto_f = R (*)(const R& lambda_a, const R& lambda_b);

template <typename R>
using proto_f_i = mipp::reg (*)(const mipp::reg& lambda_a, const mipp::reg& lambda_b);

template <typename B, typename R>
using proto_g = R (*)(const R& lambda_a, const R& lambda_b, const B& u);

template <typename B, typename R>
using proto_g_i = mipp::reg (*)(const mipp::reg& lambda_a, const mipp::reg& lambda_b, const mipp::reg& u);

template <typename R>
using proto_g0 = R (*)(const R& lambda_a, const R& lambda_b);

template <typename R>
using proto_g0_i = mipp::reg (*)(const mipp::reg& lambda_a, const mipp::reg& lambda_b);

template <typename B, typename R>
using proto_h = B (*)(const R& lambda_a);

template <typename B, typename R>
using proto_h_i = mipp::reg (*)(const mipp::reg& lambda_a);

template <typename R>
using proto_v = R (*)(const R& lambda_a, const R& lambda_b);

template <typename B>
using proto_xo = B (*)(const B& u_a, const B& u_b);

template <typename B>
using proto_xo_i = mipp::reg (*)(const mipp::reg& u_a, const mipp::reg& u_b);

template <typename B, typename R>
using proto_m = R (*)(const B& u_a, const R& lambda_a);

// ------------------------------------------------------------------------------------------- special function headers

template <typename R>
inline R f_LR(const R& lambda_a, const R& lambda_b) __attribute__((always_inline));

template <typename R>
inline R f_LLR(const R& lambda_a, const R& lambda_b) __attribute__((always_inline));

template <typename R>
inline mipp::reg f_LLR_i(const mipp::reg& r_lambda_a, const mipp::reg& r_lambda_b) __attribute__((always_inline));

template <typename B, typename R>
inline R g_LR(const R& lambda_a, const R& lambda_b, const B& u) __attribute__((always_inline));

template <typename B, typename R>
inline R g_LLR(const R& lambda_a, const R& lambda_b, const B& u) __attribute__((always_inline));

template <typename B, typename R>
inline mipp::reg g_LLR_i(const mipp::reg& r_lambda_a,
                         const mipp::reg& r_lambda_b,
                         const mipp::reg& r_u)  __attribute__((always_inline));

template <typename R>
inline R g0_LR(const R& lambda_a, const R& lambda_b) __attribute__((always_inline));

template <typename R>
inline R g0_LLR(const R& lambda_a, const R& lambda_b) __attribute__((always_inline));

template <typename R>
inline mipp::reg g0_LLR_i(const mipp::reg& r_lambda_a, const mipp::reg& r_lambda_b)  __attribute__((always_inline));

template <typename B, typename R>
inline B h_LR(const R& lambda_a) __attribute__((always_inline));

template <typename B, typename R>
inline B h_LLR(const R& lambda_a) __attribute__((always_inline));

template <typename B, typename R>
inline mipp::reg h_LLR_i(const mipp::reg& r_lambda_a) __attribute__((always_inline));

template <typename R>
inline R v_LR(const R& a, const R& b) __attribute__((always_inline));

template <typename R>
inline R v_LLR(const R& a, const R& b) __attribute__((always_inline));

template <typename B>
inline B xo_STD(const B& u_a, const B& u_b) __attribute__((always_inline));

template <typename B>
inline mipp::reg xo_STD_i(const mipp::reg& r_u_a, const mipp::reg& r_u_b) __attribute__((always_inline));

template <typename B, typename R>
inline R phi(const R& mu, const R& lambda, const B& u);

inline int compute_depth(int index, int tree_depth);

#include "decoder_polar_functions.hxx"

#endif /* DECODER_POLAR_FUNCTIONS_H */
