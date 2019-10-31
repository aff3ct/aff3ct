#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 * \file
 * \brief Functions for the Polar API in inter-SIMD (8-bit).
 */
#ifndef FUNCTIONS_POLAR_INTER_8BIT_BITPACKING_H_
#define FUNCTIONS_POLAR_INTER_8BIT_BITPACKING_H_

#include "Tools/Code/Polar/decoder_polar_functions.h"

namespace aff3ct
{
namespace tools
{
// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI, int N_ELMTS = 0>
struct g_inter_8bit_bitpacking
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_g_i<B,R> GI>
struct g_inter_8bit_bitpacking <B, R, GI, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_g_i<B,R> GI>
struct g_inter_8bit_bitpacking <B, R, GI, 4>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_g_i<B,R> GI>
struct g_inter_8bit_bitpacking <B, R, GI, 2>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_g_i<B,R> GI>
struct g_inter_8bit_bitpacking <B, R, GI, 1>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int init_shift, const int n_elmts = 0);
};

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI, int N_ELMTS = 0>
struct gr_inter_8bit_bitpacking
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_g_i<B,R> GI>
struct gr_inter_8bit_bitpacking <B, R, GI, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int init_shift, const int n_elmts = 0);
};

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct h_inter_8bit_bitpacking
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct h_inter_8bit_bitpacking <B, R, HI, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct h_inter_8bit_bitpacking <B, R, HI, 4>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct h_inter_8bit_bitpacking <B, R, HI, 2>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct h_inter_8bit_bitpacking <B, R, HI, 1>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

// =============================================================================================================== h0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0>
struct h0_inter_8bit_bitpacking
{
	static void apply(B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B>
struct h0_inter_8bit_bitpacking <B, 0>
{
	static void apply(B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B>
struct h0_inter_8bit_bitpacking <B, 4>
{
	static void apply(B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B>
struct h0_inter_8bit_bitpacking <B, 2>
{
	static void apply(B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B>
struct h0_inter_8bit_bitpacking <B, 1>
{
	static void apply(B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct rep_inter_8bit_bitpacking
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct rep_inter_8bit_bitpacking <B, R, HI, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct rep_inter_8bit_bitpacking <B, R, HI, 4>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct rep_inter_8bit_bitpacking <B, R, HI, 2>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct spc_inter_8bit_bitpacking
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct spc_inter_8bit_bitpacking <B, R, HI, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct spc_inter_8bit_bitpacking <B, R, HI, 4>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts = 0);
};

// =============================================================================================================== xo()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, proto_xo_i<B> XOI, int N_ELMTS = 0>
struct xo_inter_8bit_bitpacking
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int init_shift, const int n_elmts = 0);
};

template <typename B, proto_xo_i<B> XOI>
struct xo_inter_8bit_bitpacking <B, XOI, 0>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int init_shift, const int n_elmts = 0);
};

template <typename B, proto_xo_i<B> XOI>
struct xo_inter_8bit_bitpacking <B, XOI, 4>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int init_shift, const int n_elmts = 0);
};

template <typename B, proto_xo_i<B> XOI>
struct xo_inter_8bit_bitpacking <B, XOI, 2>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int init_shift, const int n_elmts = 0);
};

template <typename B, proto_xo_i<B> XOI>
struct xo_inter_8bit_bitpacking <B, XOI, 1>
{
	//__attribute__((always_inline))
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int init_shift, const int n_elmts = 0);
};

// =============================================================================================================== x0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0>
struct xo0_inter_8bit_bitpacking
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts = 0);
};

template <typename B>
struct xo0_inter_8bit_bitpacking <B, 0>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts = 0);
};

template <typename B>
struct xo0_inter_8bit_bitpacking <B, 4>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts = 0);
};

template <typename B>
struct xo0_inter_8bit_bitpacking <B, 2>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts = 0);
};

template <typename B>
struct xo0_inter_8bit_bitpacking <B, 1>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts = 0);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter_8bit_bitpacking.hxx"
#endif

#endif /* FUNCTIONS_POLAR_INTER_8BIT_BITPACKING_HPP_ */

#endif
