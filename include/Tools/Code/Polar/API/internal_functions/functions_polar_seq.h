#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 * \file
 * \brief Functions for the Polar API in sequential.
 */
#ifndef FUNCTIONS_POLAR_SEQ_H_
#define FUNCTIONS_POLAR_SEQ_H_

#include "Tools/Code/Polar/decoder_polar_functions.h"

namespace aff3ct
{
namespace tools
{
// ================================================================================================================ f()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_f<R> F, int N_ELMTS = 0>
struct f_seq
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

template <typename R, proto_f<R> F>
struct f_seq <R, F, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, int N_ELMTS = 0>
struct g_seq
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

template <typename B, typename R, proto_g<B,R> G>
struct g_seq <B, R, G, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

// =============================================================================================================== g0()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_g0<R> G0, int N_ELMTS = 0>
struct g0_seq
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

template <typename R, proto_g0<R> G0>
struct g0_seq <R, G0, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, int N_ELMTS = 0>
struct gr_seq
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

template <typename B, typename R, proto_g<B,R> G>
struct gr_seq <B, R, G, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, int N_ELMTS = 0>
struct h_seq
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h<B,R> H>
struct h_seq <B, R, H, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// =============================================================================================================== h0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0>
struct h0_seq
{
	static void apply(B *__restrict s_a, const int n_elmts = 0);
};

template <typename B>
struct h0_seq <B, 0>
{
	static void apply(B *__restrict s_a, const int n_elmts = 0);
};

// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, int N_ELMTS = 0>
struct rep_seq
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, proto_h<B,signed char> H, int N_ELMTS>
struct rep_seq<B, signed char, H, N_ELMTS>
{
	static void apply(const signed char *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h<B,R> H>
struct rep_seq <B, R, H, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, proto_h<B,signed char> H>
struct rep_seq <B, signed char, H, 0>
{
	static void apply(const signed char *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, int N_ELMTS = 0>
struct spc_seq
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h<B,R> H>
struct spc_seq <B, R, H, 0>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// ================================================================================================================ x()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, proto_xo<B> X, int N_ELMTS = 0>
struct xo_seq
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};

template <typename B, proto_xo<B> X>
struct xo_seq <B, X, 0>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};

// =============================================================================================================== x0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0>
struct xo0_seq
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};

template <typename B>
struct xo0_seq <B, 0>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/API/internal_functions/functions_polar_seq.hxx"
#endif

#endif /* FUNCTIONS_POLAR_SEQ_H_ */

#endif
