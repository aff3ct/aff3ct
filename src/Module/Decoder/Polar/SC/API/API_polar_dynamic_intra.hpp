#ifndef API_POLAR_DYNAMIC_INTRA_HPP_
#define API_POLAR_DYNAMIC_INTRA_HPP_

#include <algorithm>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/utils.h"

#include "../../decoder_polar_functions.h"

#include "functions_polar_inter_intra.h"
#include "functions_polar_intra.h"
#include "functions_polar_seq.h"

#include "API_polar.hpp"

namespace aff3ct
{
template <typename B, typename R, 
          proto_f   <R> F,  proto_g   <B,R> G,  proto_g0  <R> G0,  proto_h   <B,R> H,  proto_xo  <B> XO,
          proto_f_i <R> FI, proto_g_i <B,R> GI, proto_g0_i<R> G0I, proto_h_i <B,R> HI, proto_xo_i<B> XOI>
class API_polar_dynamic_intra : public API_polar
{
public:
	static constexpr int get_n_frames() { return 1; }

	// -------------------------------------------------------------------------------------------------------------- f
	
	template <int N_ELMTS = 0>
	static void f(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
	{
		if (n_elmts >= mipp::nElReg<R>()) f_inter_intra    <R, FI>::apply(l_a, l_b, l_c, n_elmts);
		else                              f_intra_unaligned<R, FI>::apply(l_a, l_b, l_c, n_elmts);
	}

	template <int N_ELMTS = 0>
	static void f(mipp::vector<R> &l, const int off_l_a, const int off_l_b, const int off_l_c, const int n_elmts)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		const R *__restrict l_b = l.data() + off_l_b;
		      R *__restrict l_c = l.data() + off_l_c;

		if (n_elmts >= mipp::nElReg<R>()) f_inter_intra    <R, FI>::apply(l_a, l_b, l_c, n_elmts);
		else                              f_intra_unaligned<R, FI>::apply(l_a, l_b, l_c, n_elmts);
	}

	// -------------------------------------------------------------------------------------------------------------- g

	template <int N_ELMTS = 0>
	static void g(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	              const int n_elmts)
	{
		if (n_elmts >= mipp::nElReg<R>()) g_inter_intra    <B, R, GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
		else                              g_intra_unaligned<B, R, GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	template <int N_ELMTS = 0>
	static void g(const mipp::vector<B> &s, mipp::vector<R> &l, 
	              const int off_l_a, const int off_l_b, const int off_s_a, const int off_l_c, const int n_elmts)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		const R *__restrict l_b = l.data() + off_l_b;
		const B *__restrict s_a = s.data() + off_s_a;
		      R *__restrict l_c = l.data() + off_l_c;

		if (n_elmts >= mipp::nElReg<R>()) g_inter_intra    <B, R, GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
		else                              g_intra_unaligned<B, R, GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- g0

	template <int N_ELMTS = 0>
	static void g0(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
	{
		if (n_elmts >= mipp::nElReg<R>()) g0_inter_intra    <R, G0I>::apply(l_a, l_b, l_c, n_elmts);
		else                              g0_intra_unaligned<R, G0I>::apply(l_a, l_b, l_c, n_elmts);
	}

	template <int N_ELMTS = 0>
	static void g0(mipp::vector<R> &l, const int off_l_a, const int off_l_b, const int off_l_c , const int n_elmts)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		const R *__restrict l_b = l.data() + off_l_b;
		      R *__restrict l_c = l.data() + off_l_c;

		if (n_elmts >= mipp::nElReg<R>()) g0_inter_intra    <R, G0I>::apply(l_a, l_b, l_c, n_elmts);
		else                              g0_intra_unaligned<R, G0I>::apply(l_a, l_b, l_c, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- gr

	template <int N_ELMTS = 0>
	static void gr(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	               const int n_elmts)
	{
		if (n_elmts >= mipp::nElReg<R>()) gr_inter_intra    <B, R, GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
		else                              gr_intra_unaligned<B, R, GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	template <int N_ELMTS = 0>
	static void gr(const mipp::vector<B> &s, mipp::vector<R> &l, 
	               const int off_l_a, const int off_l_b, const int off_s_a, const int off_l_c, const int n_elmts)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		const R *__restrict l_b = l.data() + off_l_b;
		const B *__restrict s_a = s.data() + off_s_a;
		      R *__restrict l_c = l.data() + off_l_c;

		if (n_elmts >= mipp::nElReg<R>()) gr_inter_intra    <B, R, GI>::apply(l_a, l_b, s_a, l_c, n_elmts);
		else                              gr_intra_unaligned<B, R, GI>::apply(l_a, l_b, s_a, l_c, n_elmts);

	}

	// -------------------------------------------------------------------------------------------------------------- h

	template <int N_ELMTS = 0>
	static void h(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
	{
		if (n_elmts >= mipp::nElReg<R>()) h_inter_intra    <B, R, HI>::apply(l_a, s_a, n_elmts);
		else                              h_intra_unaligned<B, R, HI>::apply(l_a, s_a, n_elmts);
	}

	template <int N_ELMTS = 0>
	static void h(mipp::vector<B> &s, const mipp::vector<R> &l, const int off_l_a, const int off_s_a, 
	              const int n_elmts)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		      B *__restrict s_a = s.data() + off_s_a;

		if (n_elmts >= mipp::nElReg<R>()) h_inter_intra    <B, R, HI>::apply(l_a, s_a, n_elmts);
		else                              h_intra_unaligned<B, R, HI>::apply(l_a, s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- h0

	// template <int N_ELMTS = 0>
	// static void h0(B *__restrict s_a, const int n_elmts)
	// {
	// 	if (n_elmts >= mipp::nElReg<B>()) h0_inter_intra    <B>::apply(s_a, n_elmts);
	// 	else                              h0_intra_unaligned<B>::apply(s_a, n_elmts);
	// }

	// template <int N_ELMTS = 0>
	// static void h0(mipp::vector<B> &s, const int off_s_a, const int n_elmts)
	// {
	// 	B *__restrict s_a = s.data() + off_s_a;

	// 	if (n_elmts >= mipp::nElReg<B>()) h0_inter_intra    <B>::apply(s_a, n_elmts);
	// 	else                              h0_intra_unaligned<B>::apply(s_a, n_elmts);
	// }

	// ------------------------------------------------------------------------------------------------------------ rep

	template <int N_ELMTS = 0>
	static void rep(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
	{
		if (n_elmts >= mipp::nElReg<R>()) rep_intra<B, R, HI>::apply(l_a, s_a, n_elmts);
		else                              rep_seq  <B, R, H >::apply(l_a, s_a, n_elmts);
	}

	template <int N_ELMTS = 0>
	static void rep(mipp::vector<B> &s, const mipp::vector<R> &l, const int off_l_a, const int off_s_a, 
	                const int n_elmts)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		      B *__restrict s_a = s.data() + off_s_a;

		if (n_elmts >= mipp::nElReg<R>()) rep_intra<B, R, HI>::apply(l_a, s_a, n_elmts);
		else                              rep_seq  <B, R, H >::apply(l_a, s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------ spc

	template <int N_ELMTS = 0>
	static bool spc(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
	{
		if (n_elmts >= mipp::nElReg<R>() * 2) return spc_intra<B, R, HI>::apply(l_a, s_a, n_elmts);
		else                                  return spc_seq  <B, R, H >::apply(l_a, s_a, n_elmts);
	}

	template <int N_ELMTS = 0>
	static bool spc(mipp::vector<B> &s, const mipp::vector<R> &l, const int off_l_a, const int off_s_a, 
	                const int n_elmts)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		      B *__restrict s_a = s.data() + off_s_a;

		if (n_elmts >= mipp::nElReg<R>() * 2) return spc_intra<B, R, HI>::apply(l_a, s_a, n_elmts);
		else                                  return spc_seq  <B, R, H >::apply(l_a, s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- xo

	template <int N_ELMTS = 0>
	static void xo(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	               const int n_elmts)
	{
		if (n_elmts >= mipp::nElReg<B>()) xo_inter_intra<B, XOI>::apply(s_a, s_b, s_c, n_elmts);
		else                              xo_seq        <B, XO >::apply(s_a, s_b, s_c, n_elmts);
	}

	template <int N_ELMTS = 0>
	static void xo(mipp::vector<B> &s, const int off_s_a, const int off_s_b, const int off_s_c, const int n_elmts)
	{
		const B *__restrict s_a = s.data() + off_s_a;
		const B *__restrict s_b = s.data() + off_s_b;
		      B *__restrict s_c = s.data() + off_s_c;

		if (n_elmts >= mipp::nElReg<B>()) xo_inter_intra<B, XOI>::apply(s_a, s_b, s_c, n_elmts);
		else                              xo_seq        <B, XO >::apply(s_a, s_b, s_c, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------ xo0

	template <int N_ELMTS = 0>
	static void xo0(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
	{
		if (n_elmts >= mipp::nElReg<B>()) xo0_inter_intra<B>::apply(s_b, s_c, n_elmts);
		else                              xo0_seq        <B>::apply(s_b, s_c, n_elmts);
	}

	template <int N_ELMTS = 0>
	static void xo0(mipp::vector<B> &s, const int off_s_b, const int off_s_c, const int n_elmts)
	{
		const B *__restrict s_b = s.data() + off_s_b;
		      B *__restrict s_c = s.data() + off_s_c;

		if (n_elmts >= mipp::nElReg<B>()) xo0_inter_intra<B>::apply(s_b, s_c, n_elmts);
		else                              xo0_seq        <B>::apply(s_b, s_c, n_elmts);
	}
};
}

#endif /* API_POLAR_INTRA_DYNAMIC_HPP_ */
