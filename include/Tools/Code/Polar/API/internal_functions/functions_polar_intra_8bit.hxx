#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter_intra.h"
#include "Tools/Code/Polar/API/internal_functions/functions_polar_intra.h"
#include "Tools/Code/Polar/API/internal_functions/functions_polar_seq.h"
#include "Tools/Code/Polar/API/internal_functions/functions_polar_intra_8bit.h"

namespace aff3ct
{
namespace tools
{
// ================================================================================================================ f()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_f<R> F, proto_f_i<R> FI, int N_ELMTS>
void f_intra_8bit<R,F,FI,N_ELMTS>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	f_inter_intra<R,FI,N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_f<R> F, proto_f_i<R> FI>
void f_intra_8bit<R,F,FI,32>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
}
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_f<R> F, proto_f_i<R> FI>
void f_intra_8bit<R,F,FI,16>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
}
#endif

template <typename R, proto_f<R> F, proto_f_i<R> FI>
void f_intra_8bit<R,F,FI,8>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename R, proto_f<R> F, proto_f_i<R> FI>
void f_intra_8bit<R,F,FI,4>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename R, proto_f<R> F, proto_f_i<R> FI>
void f_intra_8bit<R,F,FI,2>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename R, proto_f<R> F, proto_f_i<R> FI>
void f_intra_8bit<R,F,FI,1>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	f_intra_unaligned<R,FI>::apply(l_a, l_b, l_c, n_elmts);
}

// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI, int N_ELMTS>
void g_intra_8bit<B,R,G,GI,N_ELMTS>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	g_inter_intra<B,R,GI,N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void g_intra_8bit<B,R,G,GI,32>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void g_intra_8bit<B,R,G,GI,16>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}
#endif

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void g_intra_8bit<B,R,G,GI,8>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void g_intra_8bit<B,R,G,GI,4>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void g_intra_8bit<B,R,G,GI,2>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void g_intra_8bit<B,R,G,GI,1>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	g_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

// =============================================================================================================== g0()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I, int N_ELMTS>
void g0_intra_8bit<R,G0,G0I,N_ELMTS>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	g0_inter_intra<R,G0I,N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
void g0_intra_8bit<R,G0,G0I,32>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
}
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
void g0_intra_8bit<R,G0,G0I,16>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
}
#endif

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
void g0_intra_8bit<R,G0,G0I,8>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
void g0_intra_8bit<R,G0,G0I,4>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
void g0_intra_8bit<R,G0,G0I,2>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename R, proto_g0<R> G0, proto_g0_i<R> G0I>
void g0_intra_8bit<R,G0,G0I,1>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	g0_intra_unaligned<R,G0I>::apply(l_a, l_b, l_c, n_elmts);
}

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI, int N_ELMTS>
void gr_intra_8bit<B,R,G,GI,N_ELMTS>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	gr_inter_intra<B,R,GI,N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void gr_intra_8bit<B,R,G,GI,32>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	gr_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void gr_intra_8bit<B,R,G,GI,16>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	gr_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}
#endif

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void gr_intra_8bit<B,R,G,GI,8>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	gr_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void gr_intra_8bit<B,R,G,GI,4>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	gr_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_g<B,R> G, proto_g_i<B,R> GI>
void gr_intra_8bit<B,R,G,GI,2>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	gr_intra_unaligned<B,R,GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI, int N_ELMTS>
void h_intra_8bit<B,R,H,HI,N_ELMTS>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	h_inter_intra<B,R,HI,N_ELMTS>::apply(l_a, s_a, n_elmts);
}

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void h_intra_8bit<B,R,H,HI,32>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
}
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void h_intra_8bit<B,R,H,HI,16>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
}
#endif

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void h_intra_8bit<B,R,H,HI,8>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void h_intra_8bit<B,R,H,HI,4>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void h_intra_8bit<B,R,H,HI,2>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void h_intra_8bit<B,R,H,HI,1>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	h_intra_unaligned<B,R,HI>::apply(l_a, s_a, n_elmts);
}

// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI, int N_ELMTS>
void rep_intra_8bit<B,R,H,HI,N_ELMTS>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	rep_intra<B,R,HI,N_ELMTS>::apply(l_a, s_a, n_elmts);
}

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void rep_intra_8bit<B,R,H,HI,32>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	rep_seq<B,R,H,32>::apply(l_a, s_a, n_elmts);
}
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void rep_intra_8bit<B,R,H,HI,16>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	rep_seq<B,R,H,16>::apply(l_a, s_a, n_elmts);
}
#endif

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void rep_intra_8bit<B,R,H,HI,8>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	rep_seq<B,R,H,8>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void rep_intra_8bit<B,R,H,HI,4>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	rep_seq<B,R,H,4>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
void rep_intra_8bit<B,R,H,HI,2>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	rep_seq<B,R,H,2>::apply(l_a, s_a, n_elmts);
}

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI, int N_ELMTS>
bool spc_intra_8bit<B,R,H,HI,N_ELMTS>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	return spc_intra<B,R,HI,N_ELMTS>::apply(l_a, s_a, n_elmts);
}

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
bool spc_intra_8bit<B,R,H,HI,64>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	return spc_seq<B,R,H,64>::apply(l_a, s_a, n_elmts);
}
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
bool spc_intra_8bit<B,R,H,HI,32>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	return spc_seq<B,R,H,32>::apply(l_a, s_a, n_elmts);
}
#endif

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
bool spc_intra_8bit<B,R,H,HI,16>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	return spc_seq<B,R,H,16>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
bool spc_intra_8bit<B,R,H,HI,8>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	return spc_seq<B,R,H,8>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_h<B,R> H, proto_h_i<B,R> HI>
bool spc_intra_8bit<B,R,H,HI,4>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	return spc_seq<B,R,H,4>::apply(l_a, s_a, n_elmts);
}

// =============================================================================================================== xo()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI, int N_ELMTS>
void xo_intra_8bit<B,XO,XOI,N_ELMTS>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo_inter_intra<B,XOI,N_ELMTS>::apply(s_a, s_b, s_c, n_elmts);
}

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
void xo_intra_8bit<B,XO,XOI,32>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo_seq<B,XO,32>::apply(s_a, s_b, s_c, n_elmts);
}
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
void xo_intra_8bit<B,XO,XOI,16>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo_seq<B,XO,16>::apply(s_a, s_b, s_c, n_elmts);
}
#endif

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
void xo_intra_8bit<B,XO,XOI,8>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo_seq<B,XO,8>::apply(s_a, s_b, s_c, n_elmts);
}

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
void xo_intra_8bit<B,XO,XOI,4>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo_seq<B,XO,4>::apply(s_a, s_b, s_c, n_elmts);
}

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
void xo_intra_8bit<B,XO,XOI,2>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo_seq<B,XO,2>::apply(s_a, s_b, s_c, n_elmts);
}

template <typename B, proto_xo<B> XO, proto_xo_i<B> XOI>
void xo_intra_8bit<B,XO,XOI,1>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo_seq<B,XO,1>::apply(s_a, s_b, s_c, n_elmts);
}

// ============================================================================================================== xo0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS>
void xo0_intra_8bit<B,N_ELMTS>
::apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo0_inter_intra<B,N_ELMTS>::apply(s_b, s_c, n_elmts);
}

#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B>
void xo0_intra_8bit<B,32>
::apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo0_seq<B,32>::apply(s_b, s_c, n_elmts);
}
#endif

#if defined(__AVX__) || defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
template <typename B>
void xo0_intra_8bit<B,16>
::apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo0_seq<B,16>::apply(s_b, s_c, n_elmts);
}
#endif

template <typename B>
void xo0_intra_8bit<B,8>
::apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo0_seq<B,8>::apply(s_b, s_c, n_elmts);
}

template <typename B>
void xo0_intra_8bit<B,4>
::apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo0_seq<B,4>::apply(s_b, s_c, n_elmts);
}

template <typename B>
void xo0_intra_8bit<B,2>
::apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo0_seq<B,2>::apply(s_b, s_c, n_elmts);
}

template <typename B>
void xo0_intra_8bit<B,1>
::apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo0_seq<B,1>::apply(s_b, s_c, n_elmts);
}

}
}
