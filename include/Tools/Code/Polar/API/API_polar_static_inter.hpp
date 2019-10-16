#ifndef API_POLAR_STATIC_INTER_HPP_
#define API_POLAR_STATIC_INTER_HPP_

#include <cstdint>
#include <memory>
#include <vector>
#include <mipp.h>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Code/Polar/API/functions_polar_inter_intra.h"
#include "Tools/Code/Polar/API/functions_polar_inter.h"
#include "Tools/Code/Polar/API/API_polar.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int32_t, typename R = float, proto_f_i <  R> FI  = f_LLR_i <  R>,
                                                    proto_g_i <B,R> GI  = g_LLR_i <B,R>,
                                                    proto_g0_i<  R> G0I = g0_LLR_i<  R>,
                                                    proto_h_i <B,R> HI  = h_LLR_i <B,R>,
                                                    proto_xo_i<B  > XOI = xo_STD_i<B  >>
class API_polar_static_inter : public API_polar
{
private:
	static int ol(int off) { return off * mipp::nElReg<R>(); }
	static int os(int off) { return off * mipp::nElReg<B>(); }

public:
	static constexpr int get_n_frames() { return mipp::nElReg<R>(); }

	template <typename T>
	static bool isAligned(const T *ptr)
	{
		return mipp::isAligned(ptr);
	}

	// -------------------------------------------------------------------------------------------------------------- f

	template <int N_ELMTS = 0>
	static void f(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		f_inter_intra<R, FI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<R>>
	static void f(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c, const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + ol(off_l_a);
		const R *__restrict l_b = l.data() + ol(off_l_b);
		      R *__restrict l_c = l.data() + ol(off_l_c);

		f_inter_intra<R, FI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
	}

	// -------------------------------------------------------------------------------------------------------------- g

	template <int N_ELMTS = 0>
	static void g(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	              const int n_elmts = 0)
	{
		g_inter_intra<B, R, GI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void g(const std::vector<B,AB> &s, std::vector<R,AR> &l,
	              const int off_l_a, const int off_l_b, const int off_s_a, const int off_l_c, const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + ol(off_l_a);
		const R *__restrict l_b = l.data() + ol(off_l_b);
		const B *__restrict s_a = s.data() + os(off_s_a);
		      R *__restrict l_c = l.data() + ol(off_l_c);

		g_inter_intra<B, R, GI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- g0

	template <int N_ELMTS = 0>
	static void g0(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		g0_inter_intra<R, G0I, N_ELMTS, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<R>>
	static void g0(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c , const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + ol(off_l_a);
		const R *__restrict l_b = l.data() + ol(off_l_b);
		      R *__restrict l_c = l.data() + ol(off_l_c);

		g0_inter_intra<R, G0I, N_ELMTS, get_n_frames()>::apply(l_a, l_b, l_c, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- gr

	template <int N_ELMTS = 0>
	static void gr(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	               const int n_elmts = 0)
	{
		gr_inter_intra<B, R, GI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void gr(const std::vector<B,AB> &s, std::vector<R,AR> &l,
	               const int off_l_a, const int off_l_b, const int off_s_a, const int off_l_c, const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + ol(off_l_a);
		const R *__restrict l_b = l.data() + ol(off_l_b);
		const B *__restrict s_a = s.data() + os(off_s_a);
		      R *__restrict l_c = l.data() + ol(off_l_c);

		gr_inter_intra<B, R, GI, N_ELMTS, get_n_frames()>::apply(l_a, l_b, s_a, l_c, n_elmts);
	}

	// -------------------------------------------------------------------------------------------------------------- h

	template <int N_ELMTS = 0>
	static void h(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		h_inter_intra<B, R, HI, N_ELMTS, get_n_frames()>::apply(l_a, s_a, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void h(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a,
	              const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + ol(off_l_a);
		      B *__restrict s_a = s.data() + os(off_s_a);

		h_inter_intra<B, R, HI, N_ELMTS, get_n_frames()>::apply(l_a, s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- h0

	template <int N_ELMTS = 0>
	static void h0(B *__restrict s_a, const int n_elmts = 0)
	{
		h0_inter_intra<B, N_ELMTS, get_n_frames()>::apply(s_a, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<B>>
	static void h0(std::vector<B,A> &s, const int off_s_a, const int n_elmts = 0)
	{
		B *__restrict s_a = s.data() + os(off_s_a);

		h0_inter_intra<B, N_ELMTS, get_n_frames()>::apply(s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------ rep

	template <int N_ELMTS = 0>
	static void rep(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		rep_inter<B, R, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void rep(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a,
	                const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + ol(off_l_a);
		      B *__restrict s_a = s.data() + os(off_s_a);

		rep_inter<B, R, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------ spc

	template <int N_ELMTS = 0>
	static void spc(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		spc_inter<B, R, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void spc(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a,
	                const int n_elmts = 0)
	{
		const R *__restrict l_a = l.data() + ol(off_l_a);
		      B *__restrict s_a = s.data() + os(off_s_a);

		spc_inter<B, R, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------- xo

	template <int N_ELMTS = 0>
	static void xo(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	               const int n_elmts = 0)
	{
		xo_inter_intra<B, XOI, N_ELMTS, get_n_frames()>::apply(s_a, s_b, s_c, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<B>>
	static void xo(std::vector<B,A> &s, const int off_s_a, const int off_s_b, const int off_s_c, const int n_elmts = 0)
	{
		const B *__restrict s_a = s.data() + os(off_s_a);
		const B *__restrict s_b = s.data() + os(off_s_b);
		      B *__restrict s_c = s.data() + os(off_s_c);

		xo_inter_intra<B, XOI, N_ELMTS, get_n_frames()>::apply(s_a, s_b, s_c, n_elmts);
	}

	// ------------------------------------------------------------------------------------------------------------ xo0

	template <int N_ELMTS = 0>
	static void xo0(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
		xo0_inter_intra<B, N_ELMTS, get_n_frames()>::apply(s_b, s_c, n_elmts);
	}

	template <int N_ELMTS = 0, class A = std::allocator<B>>
	static void xo0(std::vector<B,A> &s, const int off_s_b, const int off_s_c, const int n_elmts = 0)
	{
		const B *__restrict s_b = s.data() + os(off_s_b);
		      B *__restrict s_c = s.data() + os(off_s_c);

		xo0_inter_intra<B, N_ELMTS, get_n_frames()>::apply(s_b, s_c, n_elmts);
	}
};
}
}

#endif /* API_POLAR_STATIC_INTER_HPP_ */
