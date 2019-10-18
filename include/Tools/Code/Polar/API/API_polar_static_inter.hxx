#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <mipp.h>

#include "Tools/Code/Polar/API/functions_polar_inter_intra.h"
#include "Tools/Code/Polar/API/functions_polar_inter.h"
#include "Tools/Code/Polar/API/API_polar_static_inter.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
int API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::ol(int off)
{
	return off * mipp::nElReg<R>();
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
int API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::os(int off)
{
	return off * mipp::nElReg<B>();
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
constexpr int API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::get_n_frames()
{
	return mipp::nElReg<R>();
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <typename T>
bool API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::isAligned(const T *ptr)
{
	return mipp::isAligned(ptr);
}

// ------------------------------------------------------------------------------------------------------------------ f

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::f(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	f_inter_intra<R, FI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::f(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c, const int n_elmts)
{
	const R *__restrict l_a = l.data() + ol(off_l_a);
	const R *__restrict l_b = l.data() + ol(off_l_b);
	      R *__restrict l_c = l.data() + ol(off_l_c);

	f_inter_intra<R, FI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
}

// ------------------------------------------------------------------------------------------------------------------ g

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::g(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	g_inter_intra<B, R, GI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::g(const std::vector<B,AB> &s, std::vector<R,AR> &l, const int off_l_a, const int off_l_b, const int off_s_a,
    const int off_l_c, const int n_elmts)
{
	const R *__restrict l_a = l.data() + ol(off_l_a);
	const R *__restrict l_b = l.data() + ol(off_l_b);
	const B *__restrict s_a = s.data() + os(off_s_a);
	      R *__restrict l_c = l.data() + ol(off_l_c);

	g_inter_intra<B, R, GI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- g0

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::g0(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	g0_inter_intra<R, G0I, N_ELMTS, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::g0(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c , const int n_elmts)
{
	const R *__restrict l_a = l.data() + ol(off_l_a);
	const R *__restrict l_b = l.data() + ol(off_l_b);
	      R *__restrict l_c = l.data() + ol(off_l_c);

	g0_inter_intra<R, G0I, N_ELMTS, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- gr

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::gr(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	gr_inter_intra<B, R, GI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::gr(const std::vector<B,AB> &s, std::vector<R,AR> &l, const int off_l_a, const int off_l_b, const int off_s_a,
     const int off_l_c, const int n_elmts)
{
	const R *__restrict l_a = l.data() + ol(off_l_a);
	const R *__restrict l_b = l.data() + ol(off_l_b);
	const B *__restrict s_a = s.data() + os(off_s_a);
	      R *__restrict l_c = l.data() + ol(off_l_c);

	gr_inter_intra<B, R, GI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

// ------------------------------------------------------------------------------------------------------------------ h

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::h(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	h_inter_intra<B, R, HI, N_ELMTS, get_n_frames()>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::h(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R *__restrict l_a = l.data() + ol(off_l_a);
	      B *__restrict s_a = s.data() + os(off_s_a);

	h_inter_intra<B, R, HI, N_ELMTS, get_n_frames()>::apply(l_a, s_a, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- h0

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::h0(B *__restrict s_a, const int n_elmts)
{
	h0_inter_intra<B, N_ELMTS, get_n_frames()>::apply(s_a, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::h0(std::vector<B,A> &s, const int off_s_a, const int n_elmts)
{
	B *__restrict s_a = s.data() + os(off_s_a);

	h0_inter_intra<B, N_ELMTS, get_n_frames()>::apply(s_a, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- rep

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::rep(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	rep_inter<B, R, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::rep(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R *__restrict l_a = l.data() + ol(off_l_a);
	      B *__restrict s_a = s.data() + os(off_s_a);

	rep_inter<B, R, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- spc

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::spc(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	spc_inter<B, R, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::spc(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R *__restrict l_a = l.data() + ol(off_l_a);
	      B *__restrict s_a = s.data() + os(off_s_a);

	spc_inter<B, R, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- xo

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::xo(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo_inter_intra<B, XOI, N_ELMTS, get_n_frames()>::apply(s_a, s_b, s_c, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::xo(std::vector<B,A> &s, const int off_s_a, const int off_s_b, const int off_s_c, const int n_elmts)
{
	const B *__restrict s_a = s.data() + os(off_s_a);
	const B *__restrict s_b = s.data() + os(off_s_b);
	      B *__restrict s_c = s.data() + os(off_s_c);

	xo_inter_intra<B, XOI, N_ELMTS, get_n_frames()>::apply(s_a, s_b, s_c, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- xo0

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::xo0(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo0_inter_intra<B, N_ELMTS, get_n_frames()>::apply(s_b, s_c, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_inter<B,R,FI,GI,G0I,HI,XOI>
::xo0(std::vector<B,A> &s, const int off_s_b, const int off_s_c, const int n_elmts)
{
	const B *__restrict s_b = s.data() + os(off_s_b);
	      B *__restrict s_c = s.data() + os(off_s_c);

	xo0_inter_intra<B, N_ELMTS, get_n_frames()>::apply(s_b, s_c, n_elmts);
}

}
}

#endif
