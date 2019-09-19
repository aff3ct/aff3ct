#ifndef API_POLAR_STATIC_INTRA_8BIT_HPP_
#define API_POLAR_STATIC_INTRA_8BIT_HPP_

#include <cstdint>
#include <memory>
#include <vector>
#include <mipp.h>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Code/Polar/API/functions_polar_inter_intra.h"
#include "Tools/Code/Polar/API/functions_polar_intra_8bit.h"
#include "Tools/Code/Polar/API/functions_polar_seq.h"
#include "Tools/Code/Polar/API/API_polar.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int32_t, typename R = float, proto_f   <  R> F   = f_LLR   <  R>,
                                                    proto_g   <B,R> G   = g_LLR   <B,R>,
                                                    proto_g0  <  R> G0  = g0_LLR  <  R>,
                                                    proto_h   <B,R> H   = h_LLR   <B,R>,
                                                    proto_xo  <B  > XO  = xo_STD  <B  >,
                                                    proto_f_i <  R> FI  = f_LLR_i <  R>,
                                                    proto_g_i <B,R> GI  = g_LLR_i <B,R>,
                                                    proto_g0_i<  R> G0I = g0_LLR_i<  R>,
                                                    proto_h_i <B,R> HI  = h_LLR_i <B,R>,
                                                    proto_xo_i<B  > XOI = xo_STD_i<B  >>
class API_polar_static_intra_8bit : public API_polar
{
public:
	static constexpr int get_n_frames() { return 1; }

	template <typename T>
	static bool isAligned(const T *ptr)
	{
		return mipp::isAligned(ptr);
	}

	// -------------------------------------------------------------------------------------------------------------- f

	template <int N_ELMTS = 0>
	static void f(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		f_intra_8bit<R, F, FI, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<R>>
	static void f(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c, const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		const R *__restrict l_b = l.data() + off_l_b;
		      R *__restrict l_c = l.data() + off_l_c;

		f_intra_8bit<R, F, FI, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
	}

	// -------------------------------------------------------------------------------------------------------------- g

	template <int N_ELMTS = 0>
	static void g(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	              const int n_elmts = 0)
	{
		g_intra_8bit<B, R, G, GI, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void g(const std::vector<B,AB> &s, std::vector<R,AR> &l,
	              const int off_l_a, const int off_l_b, const int off_s_a, const int off_l_c, const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		const R *__restrict l_b = l.data() + off_l_b;
		const B *__restrict s_a = s.data() + off_s_a;
		      R *__restrict l_c = l.data() + off_l_c;

		g_intra_8bit<B, R, G, GI, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- g0

	template <int N_ELMTS = 0>
	static void g0(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		g0_intra_8bit<R, G0, G0I, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<R>>
	static void g0(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c , const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		const R *__restrict l_b = l.data() + off_l_b;
		      R *__restrict l_c = l.data() + off_l_c;

		g0_intra_8bit<R, G0, G0I, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- gr

	template <int N_ELMTS = 0>
	static void gr(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	               const int n_elmts = 0)
	{
		gr_intra_8bit<B, R, G, GI, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void gr(const std::vector<B,AB> &s, std::vector<R,AR> &l,
	               const int off_l_a, const int off_l_b, const int off_s_a, const int off_l_c, const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		const R *__restrict l_b = l.data() + off_l_b;
		const B *__restrict s_a = s.data() + off_s_a;
		      R *__restrict l_c = l.data() + off_l_c;

		gr_intra_8bit<B, R, G, GI, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	// -------------------------------------------------------------------------------------------------------------- h

	template <int N_ELMTS = 0>
	static void h(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		h_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void h(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a,
	              const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		      B *__restrict s_a = s.data() + off_s_a;

		h_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- h0

	template <int N_ELMTS = 0>
	static void h0(B *__restrict s_a, const int n_elmts = 0)
	{
		h0_inter_intra<B, N_ELMTS>::apply(s_a, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<B>>
	static void h0(std::vector<B,A> &s, const int off_s_a, const int n_elmts = 0)
	{
		B *__restrict s_a = s.data() + off_s_a;

		h0_inter_intra<B, N_ELMTS>::apply(s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------ rep

	template <int N_ELMTS = 0>
	static void rep(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		rep_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void rep(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a,
	                const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		      B *__restrict s_a = s.data() + off_s_a;

		rep_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------ spc

	template <int N_ELMTS = 0>
	static bool spc(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		return spc_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static bool spc(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a,
	                const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + off_l_a;
		      B *__restrict s_a = s.data() + off_s_a;

		return spc_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- xo

	template <int N_ELMTS = 0>
	static void xo(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	               const int n_elmts = 0)
	{
		xo_intra_8bit<B, XO, XOI, N_ELMTS>::apply(s_a, s_b, s_c, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<B>>
	static void xo(std::vector<B,A> &s, const int off_s_a, const int off_s_b, const int off_s_c, const int n_elmts = 0)
	{
		const B *__restrict s_a = s.data() + off_s_a;
		const B *__restrict s_b = s.data() + off_s_b;
		      B *__restrict s_c = s.data() + off_s_c;

		xo_intra_8bit<B, XO, XOI, N_ELMTS>::apply(s_a, s_b, s_c, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------ xo0

	template <int N_ELMTS = 0>
	static void xo0(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
		xo0_intra_8bit<B, N_ELMTS>::apply(s_b, s_c, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<B>>
	static void xo0(std::vector<B,A> &s, const int off_s_b, const int off_s_c, const int n_elmts = 0)
	{
		const B *__restrict s_b = s.data() + off_s_b;
		      B *__restrict s_c = s.data() + off_s_c;

		xo0_intra_8bit<B, N_ELMTS>::apply(s_b, s_c, n_elmts);
	}
};
}
}

#endif /* API_POLAR_INTRA_STATIC_8BIT_HPP_ */
