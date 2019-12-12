#include "Tools/Code/Polar/API/internal_functions/functions_polar_seq.h"
#include "Tools/Code/Polar/API/API_polar_static_seq.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
constexpr int API_polar_static_seq<B,R,F,G,G0,H,XO>
::get_n_frames()
{
	return 1;
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <typename T>
bool API_polar_static_seq<B,R,F,G,G0,H,XO>
::isAligned(const T *ptr)
{
	return true;
}

// ------------------------------------------------------------------------------------------------------------------ f

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::f(const R * l_a, const R * l_b, R * l_c, const int n_elmts)
{
	f_seq<R, F, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class A>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::f(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c, const int n_elmts)
{
	const R * l_a = l.data() + off_l_a;
	const R * l_b = l.data() + off_l_b;
	      R * l_c = l.data() + off_l_c;

	f_seq<R, F, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

// ------------------------------------------------------------------------------------------------------------------ g

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::g(const R * l_a, const R * l_b, const B * s_a, R * l_c, const int n_elmts)
{
	g_seq<B, R, G, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::g(const std::vector<B,AB> &s, std::vector<R,AR> &l, const int off_l_a, const int off_l_b, const int off_s_a,
    const int off_l_c, const int n_elmts)
{
	const R * l_a = l.data() + off_l_a;
	const R * l_b = l.data() + off_l_b;
	const B * s_a = s.data() + off_s_a;
	      R * l_c = l.data() + off_l_c;

	g_seq<B, R, G, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- g0

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::g0(const R * l_a, const R * l_b, R * l_c, const int n_elmts)
{
	g0_seq<R, G0, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class A>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::g0(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c , const int n_elmts)
{
	const R * l_a = l.data() + off_l_a;
	const R * l_b = l.data() + off_l_b;
	      R * l_c = l.data() + off_l_c;

	g0_seq<R, G0, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- gr

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::gr(const R * l_a, const R * l_b, const B * s_a, R * l_c,
     const int n_elmts)
{
	gr_seq<B, R, G, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::gr(const std::vector<B,AB> &s, std::vector<R,AR> &l, const int off_l_a, const int off_l_b, const int off_s_a,
     const int off_l_c, const int n_elmts)
{
	const R * l_a = l.data() + off_l_a;
	const R * l_b = l.data() + off_l_b;
	const B * s_a = s.data() + off_s_a;
	      R * l_c = l.data() + off_l_c;

	gr_seq<B, R, G, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

// ------------------------------------------------------------------------------------------------------------------ h

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::h(const R * l_a, B * s_a, const int n_elmts)
{
	h_seq<B, R, H, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::h(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R * l_a = l.data() + off_l_a;
	      B * s_a = s.data() + off_s_a;

	h_seq<B, R, H, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- h0

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::h0(B * s_a, const int n_elmts)
{
	h0_seq<B, N_ELMTS>::apply(s_a, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class A>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::h0(std::vector<B,A> &s, const int off_s_a, const int n_elmts)
{
	B * s_a = s.data() + off_s_a;

	h0_seq<B, N_ELMTS>::apply(s_a, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- rep

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::rep(const R * l_a, B * s_a, const int n_elmts)
{
	rep_seq<B, R, H, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::rep(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R * l_a = l.data() + off_l_a;
	      B * s_a = s.data() + off_s_a;

	rep_seq<B, R, H, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- spc

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
bool API_polar_static_seq<B,R,F,G,G0,H,XO>
::spc(const R * l_a, B * s_a, const int n_elmts)
{
	return spc_seq<B, R, H, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class AB, class AR>
bool API_polar_static_seq<B,R,F,G,G0,H,XO>
::spc(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R * l_a = l.data() + off_l_a;
	      B * s_a = s.data() + off_s_a;

	return spc_seq<B, R, H, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- xo

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::xo(const B * s_a, const B * s_b, B * s_c, const int n_elmts)
{
	xo_seq<B, XO, N_ELMTS>::apply(s_a, s_b, s_c, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class A>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::xo(std::vector<B,A> &s, const int off_s_a, const int off_s_b, const int off_s_c, const int n_elmts)
{
	const B * s_a = s.data() + off_s_a;
	const B * s_b = s.data() + off_s_b;
	      B * s_c = s.data() + off_s_c;

	xo_seq<B, XO, N_ELMTS>::apply(s_a, s_b, s_c, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- xo0

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::xo0(const B * s_b, B * s_c, const int n_elmts)
{
	xo0_seq<B, N_ELMTS>::apply(s_b, s_c, n_elmts);
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_g0<R> G0, proto_h<B,R> H, proto_xo<B> XO>
template <int N_ELMTS, class A>
void API_polar_static_seq<B,R,F,G,G0,H,XO>
::xo0(std::vector<B,A> &s, const int off_s_b, const int off_s_c, const int n_elmts)
{
	const B * s_b = s.data() + off_s_b;
	      B * s_c = s.data() + off_s_c;

	xo0_seq<B, N_ELMTS>::apply(s_b, s_c, n_elmts);
}

}
}
