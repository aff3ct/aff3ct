#ifndef FUNCTIONS_POLAR_INTRA_16BIT_H_
#define FUNCTIONS_POLAR_INTRA_16BIT_H_

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Code/Polar/API/functions_polar_intra.h"
#include "Tools/Code/Polar/API/functions_polar_seq.h"

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
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		f_inter_intra<R,FI,N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
	}
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 16>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
	}
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 8>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
	}
};
#endif

template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 4>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
	}
};

template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 2>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
	}
};

template <typename R, proto_f<R> F, proto_f_i<R> FI>
struct f_intra_16bit <R, F, FI, 1>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
	}
};

// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI, int N_ELMTS = 0>
struct g_intra_16bit
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		g_inter_intra<B,R,GI,N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 16>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 8>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};
#endif

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 4>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 2>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct g_intra_16bit <B, R, G, GI, 1>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};

// =============================================================================================================== g0()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I, int N_ELMTS = 0>
struct g0_intra_16bit
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		g0_inter_intra<R,G0I,N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
	}
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 16>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
	}
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 8>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
	}
};
#endif

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 4>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
	}
};

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 2>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
	}
};

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
struct g0_intra_16bit <R, G0, G0I, 1>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
	}
};

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI, int N_ELMTS = 0>
struct gr_intra_16bit
{
	//__attribute__((always_inline))
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		gr_inter_intra<B,R,GI,N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct gr_intra_16bit <B, R, G, GI, 16>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		gr_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct gr_intra_16bit <B, R, G, GI, 8>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		gr_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};
#endif

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct gr_intra_16bit <B, R, G, GI, 4>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		gr_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
struct gr_intra_16bit <B, R, G, GI, 2>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		gr_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}
};

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct h_intra_16bit
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		h_inter_intra<B,R,HI,N_ELMTS>::apply(l_a, s_a, n_elmts);
	}
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 16>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
	}
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 8>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
	}
};
#endif

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 4>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
	}
};

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 2>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
	}
};

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct h_intra_16bit <B, R, H, HI, 1>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
	}
};

// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct rep_intra_16bit
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		rep_intra<B,R,HI,N_ELMTS>::apply(l_a, s_a, n_elmts);
	}
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct rep_intra_16bit <B, R, H, HI, 16>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		rep_seq<B,R,H,16>::apply(l_a, s_a, n_elmts);
	}
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct rep_intra_16bit <B, R, H, HI, 8>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		rep_seq<B,R,H,8>::apply(l_a, s_a, n_elmts);
	}
};
#endif

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct rep_intra_16bit <B, R, H, HI, 4>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		rep_seq<B,R,H,4>::apply(l_a, s_a, n_elmts);
	}
};

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct rep_intra_16bit <B, R, H, HI, 2>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		rep_seq<B,R,H,2>::apply(l_a, s_a, n_elmts);
	}
};

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI, int N_ELMTS = 0>
struct spc_intra_16bit
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		return spc_intra<B,R,HI,N_ELMTS>::apply(l_a, s_a, n_elmts);
	}
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct spc_intra_16bit <B, R, H, HI, 32>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		return spc_seq<B,R,H,32>::apply(l_a, s_a, n_elmts);
	}
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct spc_intra_16bit <B, R, H, HI, 16>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		return spc_seq<B,R,H,16>::apply(l_a, s_a, n_elmts);
	}
};
#endif

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct spc_intra_16bit <B, R, H, HI, 8>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		return spc_seq<B,R,H,8>::apply(l_a, s_a, n_elmts);
	}
};

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
struct spc_intra_16bit <B, R, H, HI, 4>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		return spc_seq<B,R,H,4>::apply(l_a, s_a, n_elmts);
	}
};

// =============================================================================================================== xo()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI, int N_ELMTS = 0>
struct xo_intra_16bit
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0)
	{
		xo_inter_intra<B,XOI,N_ELMTS>::apply(s_a, s_b, s_c, n_elmts);
	}
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 16>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0)
	{
		xo_seq<B,XO,16>::apply(s_a, s_b, s_c, n_elmts);
	}
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 8>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0)
	{
		xo_seq<B,XO,8>::apply(s_a, s_b, s_c, n_elmts);
	}
};
#endif

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 4>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0)
	{
		xo_seq<B,XO,4>::apply(s_a, s_b, s_c, n_elmts);
	}
};

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 2>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0)
	{
		xo_seq<B,XO,2>::apply(s_a, s_b, s_c, n_elmts);
	}
};

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
struct xo_intra_16bit <B, XO, XOI, 1>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0)
	{
		xo_seq<B,XO,1>::apply(s_a, s_b, s_c, n_elmts);
	}
};

// ============================================================================================================== xo0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0>
struct xo0_intra_16bit
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
		xo0_inter_intra<B,N_ELMTS>::apply(s_b, s_c, n_elmts);
	}
};

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B>
struct xo0_intra_16bit <B, 16>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
		xo0_seq<B,16>::apply(s_b, s_c, n_elmts);
	}
};
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B>
struct xo0_intra_16bit <B, 8>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
		xo0_seq<B,8>::apply(s_b, s_c, n_elmts);
	}
};
#endif

template <typename B>
struct xo0_intra_16bit <B, 4>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
		xo0_seq<B,4>::apply(s_b, s_c, n_elmts);
	}
};

template <typename B>
struct xo0_intra_16bit <B, 2>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
		xo0_seq<B,2>::apply(s_b, s_c, n_elmts);
	}
};

template <typename B>
struct xo0_intra_16bit <B, 1>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
		xo0_seq<B,1>::apply(s_b, s_c, n_elmts);
	}
};
}
}

#endif /* FUNCTIONS_POLAR_INTRA_16BIT_HPP_ */
