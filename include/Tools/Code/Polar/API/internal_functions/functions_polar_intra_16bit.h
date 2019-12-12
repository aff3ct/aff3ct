#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 * \file
 * \brief Functions for the Polar API in intra-SIMD (16-bit).
 */
#ifndef FUNCTIONS_POLAR_INTRA_16BIT_H_
#define FUNCTIONS_POLAR_INTRA_16BIT_H_

#include "Tools/Code/Polar/decoder_polar_functions.h"

namespace aff3ct
{
namespace tools
{
// ================================================================================================================ f()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_f<R> F, proto_f_i<R> FI, int N_ELMTS = 0>
struct f_intra_16bit
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 16>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 8>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};
#endif

template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 4>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 2>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 1>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI, int N_ELMTS = 0>
struct g_intra_16bit
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 16>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 8>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};
#endif

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 4>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 2>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 1>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

// =============================================================================================================== g0()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I, int N_ELMTS = 0>
struct g0_intra_16bit
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 16>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 8>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};
#endif

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 4>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 2>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 1>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI, int N_ELMTS = 0>
struct gr_intra_16bit
{
	//__attribute__((always_inline))
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct gr_intra_16bit <B, R, G, GI, 16>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct gr_intra_16bit <B, R, G, GI, 8>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};
#endif

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct gr_intra_16bit <B, R, G, GI, 4>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct gr_intra_16bit <B, R, G, GI, 2>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct h_intra_16bit
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 16>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 8>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};
#endif

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 4>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 2>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 1>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct rep_intra_16bit
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct rep_intra_16bit <B, R, H, HI, 16>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct rep_intra_16bit <B, R, H, HI, 8>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};
#endif

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct rep_intra_16bit <B, R, H, HI, 4>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct rep_intra_16bit <B, R, H, HI, 2>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct spc_intra_16bit
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct spc_intra_16bit <B, R, H, HI, 32>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct spc_intra_16bit <B, R, H, HI, 16>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};
#endif

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct spc_intra_16bit <B, R, H, HI, 8>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct spc_intra_16bit <B, R, H, HI, 4>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// =============================================================================================================== xo()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI, int N_ELMTS = 0>
struct xo_intra_16bit
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 16>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 8>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};
#endif

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 4>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 2>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 1>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};

// ============================================================================================================== xo0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0>
struct xo0_intra_16bit
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B>
struct xo0_intra_16bit <B, 16>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B>
struct xo0_intra_16bit <B, 8>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};
#endif

template <typename B>
struct xo0_intra_16bit <B, 4>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};

template <typename B>
struct xo0_intra_16bit <B, 2>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};

template <typename B>
struct xo0_intra_16bit <B, 1>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/API/internal_functions/functions_polar_intra_16bit.hxx"
#endif

#endif /* FUNCTIONS_POLAR_INTRA_16BIT_HPP_ */

#endif
