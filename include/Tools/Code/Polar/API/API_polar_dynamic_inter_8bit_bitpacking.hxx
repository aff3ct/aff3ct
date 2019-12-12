#include <mipp.h>

#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter_intra.h"
#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter.h"
#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter_8bit_bitpacking.h"
#include "Tools/Code/Polar/API/API_polar_dynamic_inter_8bit_bitpacking.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
int API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::ol(int off)
{
	return off * mipp::nElReg<R>();
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
int API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::os(int off)
{
	return (off / (sizeof(B) * 8)) * mipp::nElReg<B>();
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
int API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::ishift(int off)
{
	return (off % (sizeof(B) * 8));
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
constexpr int API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::get_n_frames()
{
	return mipp::nElReg<R>();
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <typename T>
bool API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::isAligned(const T *ptr)
{
	return mipp::isAligned(ptr);
}

// ------------------------------------------------------------------------------------------------------------------ f

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::f(const R * l_a, const R * l_b, R * l_c, const int n_elmts)
{
	f_inter_intra<R, FI, 0, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::f(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c, const int n_elmts)
{
	const R * l_a = l.data() + ol(off_l_a);
	const R * l_b = l.data() + ol(off_l_b);
	      R * l_c = l.data() + ol(off_l_c);

	f_inter_intra<R, FI, 0, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
}

// ------------------------------------------------------------------------------------------------------------------ g

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::g(const R * l_a, const R * l_b, const B * s_a, R * l_c, const int init_shift, const int n_elmts)
{
	if      (n_elmts >= 8) g_inter_8bit_bitpacking<B, R, GI   >::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 4) g_inter_8bit_bitpacking<B, R, GI, 4>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 2) g_inter_8bit_bitpacking<B, R, GI, 2>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 1) g_inter_8bit_bitpacking<B, R, GI, 1>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::g(const std::vector<B,AB> &s, std::vector<R,AR> &l, const int off_l_a, const int off_l_b, const int off_s_a,
    const int off_l_c, const int n_elmts)
{
	const R * l_a = l.data() + ol(off_l_a);
	const R * l_b = l.data() + ol(off_l_b);
	const B * s_a = s.data() + os(off_s_a);
	      R * l_c = l.data() + ol(off_l_c);

	const int init_shift = ishift(off_s_a);

	if      (n_elmts >= 8) g_inter_8bit_bitpacking<B, R, GI   >::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 4) g_inter_8bit_bitpacking<B, R, GI, 4>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 2) g_inter_8bit_bitpacking<B, R, GI, 2>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 1) g_inter_8bit_bitpacking<B, R, GI, 1>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- g0

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::g0(const R * l_a, const R * l_b, R * l_c, const int n_elmts)
{
	g0_inter_intra<R, G0I, 0, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::g0(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c , const int n_elmts)
{
	const R * l_a = l.data() + ol(off_l_a);
	const R * l_b = l.data() + ol(off_l_b);
	      R * l_c = l.data() + ol(off_l_c);

	g0_inter_intra<R, G0I, 0, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- gr

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::gr(const R * l_a, const R * l_b, const B * s_a, R * l_c, const int init_shift, const int n_elmts)
{
	if      (n_elmts >= 8) gr_inter_8bit_bitpacking<B, R, GI   >::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 4) gr_inter_8bit_bitpacking<B, R, GI, 4>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 2) gr_inter_8bit_bitpacking<B, R, GI, 2>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 1) gr_inter_8bit_bitpacking<B, R, GI, 1>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::gr(const std::vector<B,AB> &s, std::vector<R,AR> &l, const int off_l_a, const int off_l_b, const int off_s_a,
     const int off_l_c, const int n_elmts)
{
	const R * l_a = l.data() + ol(off_l_a);
	const R * l_b = l.data() + ol(off_l_b);
	const B * s_a = s.data() + os(off_s_a);
	      R * l_c = l.data() + ol(off_l_c);

	const int init_shift = ishift(off_s_a);

	if      (n_elmts >= 8) gr_inter_8bit_bitpacking<B, R, GI   >::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 4) gr_inter_8bit_bitpacking<B, R, GI, 4>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 2) gr_inter_8bit_bitpacking<B, R, GI, 2>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
	else if (n_elmts == 1) gr_inter_8bit_bitpacking<B, R, GI, 1>::apply(l_a, l_b, s_a, l_c, init_shift, n_elmts);
}

// ------------------------------------------------------------------------------------------------------------------ h

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::h(const R * l_a, B * s_a, const int init_shift, const int n_elmts)
{
	if      (n_elmts >= 8) h_inter_8bit_bitpacking<B, R, HI   >::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 4) h_inter_8bit_bitpacking<B, R, HI, 4>::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 2) h_inter_8bit_bitpacking<B, R, HI, 2>::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 1) h_inter_8bit_bitpacking<B, R, HI, 1>::apply(l_a, s_a, init_shift, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::h(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R * l_a = l.data() + ol(off_l_a);
	      B * s_a = s.data() + os(off_s_a);

	const int init_shift = ishift(off_s_a);

	if      (n_elmts >= 8) h_inter_8bit_bitpacking<B, R, HI   >::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 4) h_inter_8bit_bitpacking<B, R, HI, 4>::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 2) h_inter_8bit_bitpacking<B, R, HI, 2>::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 1) h_inter_8bit_bitpacking<B, R, HI, 1>::apply(l_a, s_a, init_shift, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- h0

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::h0(B * s_a, const int init_shift, const int n_elmts)
{
	if      (n_elmts >= 8) h0_inter_8bit_bitpacking<B   >::apply(s_a, init_shift, n_elmts);
	else if (n_elmts == 4) h0_inter_8bit_bitpacking<B, 4>::apply(s_a, init_shift, n_elmts);
	else if (n_elmts == 2) h0_inter_8bit_bitpacking<B, 2>::apply(s_a, init_shift, n_elmts);
	else if (n_elmts == 1) h0_inter_8bit_bitpacking<B, 1>::apply(s_a, init_shift, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::h0(std::vector<B,A> &s, const int off_s_a, const int n_elmts)
{
	B * s_a = s.data() + os(off_s_a);

	const int init_shift = ishift(off_s_a);

	if      (n_elmts >= 8) h0_inter_8bit_bitpacking<B   >::apply(s_a, init_shift, n_elmts);
	else if (n_elmts == 4) h0_inter_8bit_bitpacking<B, 4>::apply(s_a, init_shift, n_elmts);
	else if (n_elmts == 2) h0_inter_8bit_bitpacking<B, 2>::apply(s_a, init_shift, n_elmts);
	else if (n_elmts == 1) h0_inter_8bit_bitpacking<B, 1>::apply(s_a, init_shift, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- rep

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::rep(const R * l_a, B * s_a, const int init_shift, const int n_elmts)
{
	if      (n_elmts >= 8) rep_inter_8bit_bitpacking<B, R, HI   >::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 4) rep_inter_8bit_bitpacking<B, R, HI, 4>::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 2) rep_inter_8bit_bitpacking<B, R, HI, 2>::apply(l_a, s_a, init_shift, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::rep(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R * l_a = l.data() + ol(off_l_a);
	      B * s_a = s.data() + os(off_s_a);

	const int init_shift = ishift(off_s_a);

	if      (n_elmts >= 8) rep_inter_8bit_bitpacking<B, R, HI   >::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 4) rep_inter_8bit_bitpacking<B, R, HI, 4>::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 2) rep_inter_8bit_bitpacking<B, R, HI, 2>::apply(l_a, s_a, init_shift, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- spc

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::spc(const R * l_a, B * s_a, const int init_shift, const int n_elmts)
{
	if      (n_elmts >= 8) spc_inter_8bit_bitpacking<B, R, HI   >::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 4) spc_inter_8bit_bitpacking<B, R, HI, 4>::apply(l_a, s_a, init_shift, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::spc(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R * l_a = l.data() + ol(off_l_a);
	      B * s_a = s.data() + os(off_s_a);

	const int init_shift = ishift(off_s_a);

	if      (n_elmts >= 8) spc_inter_8bit_bitpacking<B, R, HI   >::apply(l_a, s_a, init_shift, n_elmts);
	else if (n_elmts == 4) spc_inter_8bit_bitpacking<B, R, HI, 4>::apply(l_a, s_a, init_shift, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- xo

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::xo(const B * s_a, const B * s_b, B * s_c, const int init_shift, const int n_elmts)
{
	if      (n_elmts >= 8) xo_inter_8bit_bitpacking<B, XOI   >::apply(s_a, s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 4) xo_inter_8bit_bitpacking<B, XOI, 4>::apply(s_a, s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 2) xo_inter_8bit_bitpacking<B, XOI, 2>::apply(s_a, s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 1) xo_inter_8bit_bitpacking<B, XOI, 1>::apply(s_a, s_b, s_c, init_shift, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::xo(std::vector<B,A> &s, const int off_s_a, const int off_s_b, const int off_s_c, const int n_elmts)
{
	const B * s_a = s.data() + os(off_s_a);
	const B * s_b = s.data() + os(off_s_b);
	      B * s_c = s.data() + os(off_s_c);

	const int init_shift = ishift(off_s_c);

	if      (n_elmts >= 8) xo_inter_8bit_bitpacking<B, XOI   >::apply(s_a, s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 4) xo_inter_8bit_bitpacking<B, XOI, 4>::apply(s_a, s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 2) xo_inter_8bit_bitpacking<B, XOI, 2>::apply(s_a, s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 1) xo_inter_8bit_bitpacking<B, XOI, 1>::apply(s_a, s_b, s_c, init_shift, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- xo0

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::xo0(const B * s_b, B * s_c, const int init_shift, const int n_elmts)
{
	if      (n_elmts >= 8) xo0_inter_8bit_bitpacking<B   >::apply(s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 4) xo0_inter_8bit_bitpacking<B, 4>::apply(s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 2) xo0_inter_8bit_bitpacking<B, 2>::apply(s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 1) xo0_inter_8bit_bitpacking<B, 1>::apply(s_b, s_c, init_shift, n_elmts);
}

template <typename B, typename R, proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_dynamic_inter_8bit_bitpacking<B,R,FI,GI,G0I,HI,XOI>
::xo0(std::vector<B,A> &s, const int off_s_b, const int off_s_c, const int n_elmts)
{
	const B * s_b = s.data() + os(off_s_b);
	      B * s_c = s.data() + os(off_s_c);

	const int init_shift = ishift(off_s_c);

	if      (n_elmts >= 8) xo0_inter_8bit_bitpacking<B   >::apply(s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 4) xo0_inter_8bit_bitpacking<B, 4>::apply(s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 2) xo0_inter_8bit_bitpacking<B, 2>::apply(s_b, s_c, init_shift, n_elmts);
	else if (n_elmts == 1) xo0_inter_8bit_bitpacking<B, 1>::apply(s_b, s_c, init_shift, n_elmts);
}
}
}
