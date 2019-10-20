#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 * \file
 * \brief Functions for the Polar API in intra-SIMD.
 */
#ifndef FUNCTIONS_POLAR_INTRA_H_
#define FUNCTIONS_POLAR_INTRA_H_

#include "Tools/Code/Polar/decoder_polar_functions.h"

namespace aff3ct
{
namespace tools
{
// ================================================================================================================ f()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_f_i<R> FI>
struct f_intra_unaligned
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI>
struct g_intra_unaligned
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

// =============================================================================================================== g0()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_g0_i<R> G0I, int N_ELMTS = 0>
struct g0_intra_unaligned
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0);
};

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI>
struct gr_intra_unaligned
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0);
};

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI>
struct h_intra_unaligned
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct rep_intra
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct rep_intra <B, R, HI, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct spc_intra
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct spc_intra <B, R, HI, 0>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/API/internal_functions/functions_polar_intra.hxx"
#endif

#endif /* FUNCTIONS_POLAR_INTRA_H_ */

#endif
