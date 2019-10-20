#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 * \file
 * \brief Functions for the Polar API in inter-SIMD.
 */
#ifndef FUNCTIONS_POLAR_INTER_H_
#define FUNCTIONS_POLAR_INTER_H_

#include "Tools/Code/Polar/decoder_polar_functions.h"

namespace aff3ct
{
namespace tools
{
// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct rep_inter
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};


template <typename B, typename R, proto_h_i<B,R> HI>
struct rep_inter <B, R, HI, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct spc_inter
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};

template <typename B, typename R, proto_h_i<B,R> HI>
struct spc_inter <B, R, HI, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter.hxx"
#endif

#endif /* FUNCTIONS_POLAR_INTER_H_ */

#endif
