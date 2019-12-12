#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 * \file
 * \brief Functions for the Polar API in inter/intra-SIMD.
 */
#ifndef FUNCTIONS_POLAR_INTER_INTRA_H_
#define FUNCTIONS_POLAR_INTER_INTRA_H_

#include <mipp.h>

#include "Tools/Code/Polar/decoder_polar_functions.h"

namespace aff3ct
{
namespace tools
{
// ========================================================================================================= saturate()
// ====================================================================================================================
// ====================================================================================================================

template <typename T>
struct API_polar_inter_intra_saturate
{
	static inline mipp::reg init();

	static inline mipp::reg perform(const mipp::reg r_lambda, const mipp::reg r_sat);
};

// ================================================================================================================ f()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_f_i<R> FI, int N_ELMTS = 0, int N_FRAMES = 1>
struct f_inter_intra
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

template <typename R, proto_f_i<R> FI, int N_FRAMES>
struct f_inter_intra <R, FI, 0, N_FRAMES>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI, int N_ELMTS = 0, int N_FRAMES = 1>
struct g_inter_intra
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

template <typename B, typename R, proto_g_i<B,R> GI, int N_FRAMES>
struct g_inter_intra <B, R, GI, 0, N_FRAMES>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

// =============================================================================================================== g0()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_g0_i<R> G0I, int N_ELMTS = 0, int N_FRAMES = 1>
struct g0_inter_intra
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

template <typename R, proto_g0_i<R> G0I, int N_FRAMES>
struct g0_inter_intra <R,G0I,0,N_FRAMES>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI, int N_ELMTS = 0, int N_FRAMES = 1>
struct gr_inter_intra
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

template <typename B, typename R, proto_g_i<B,R> GI, int N_FRAMES>
struct gr_inter_intra <B,R,GI,0,N_FRAMES>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS = 0, int N_FRAMES = 1>
struct h_inter_intra
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI, int N_FRAMES>
struct h_inter_intra <B,R,HI,0,N_FRAMES>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// =============================================================================================================== h0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0, int N_FRAMES = 1>
struct h0_inter_intra
{
	static void apply(B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, int N_FRAMES>
struct h0_inter_intra <B,0,N_FRAMES>
{
	static void apply(B *__restrict s_a, const int n_elmts = 0);
};

// ================================================================================================================ x()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, proto_xo_i<B> XOI, int N_ELMTS = 0, int N_FRAMES = 1>
struct xo_inter_intra
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};

template <typename B, proto_xo_i<B> XOI, int N_FRAMES>
struct xo_inter_intra <B,XOI,0,N_FRAMES>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0);
};

// =============================================================================================================== x0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0, int N_FRAMES = 1>
struct xo0_inter_intra
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};

template <typename B, int N_FRAMES>
struct xo0_inter_intra <B,0,N_FRAMES>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter_intra.hxx"
#endif

#endif /* FUNCTIONS_POLAR_INTER_INTRA_H_ */

#endif
