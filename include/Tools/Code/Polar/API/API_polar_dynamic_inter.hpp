/*!
 * \file
 * \brief Class tools::API_polar_dynamic_inter.
 */
#ifndef API_POLAR_DYNAMIC_INTER_HPP_
#define API_POLAR_DYNAMIC_INTER_HPP_

#include <cstdint>
#include <vector>
#include <memory>

#include "Tools/Code/Polar/decoder_polar_functions.h"
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
class API_polar_dynamic_inter : public API_polar
{
private:
	static int ol(int off);
	static int os(int off);

public:
	static constexpr int get_n_frames();

	template <typename T>
	static bool isAligned(const T *ptr);

	template <int N_ELMTS = 0>
	static void f(const R * l_a, const R * l_b, R * l_c, const int n_elmts);

	template <int N_ELMTS = 0, class A = std::allocator<R>>
	static void f(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c, const int n_elmts);

	template <int N_ELMTS = 0>
	static void g(const R * l_a, const R * l_b, const B * s_a, R * l_c, const int n_elmts);

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void g(const std::vector<B,AB> &s, std::vector<R,AR> &l,
	              const int off_l_a, const int off_l_b, const int off_s_a, const int off_l_c, const int n_elmts);

	template <int N_ELMTS = 0>
	static void g0(const R * l_a, const R * l_b, R * l_c, const int n_elmts);

	template <int N_ELMTS = 0, class A = std::allocator<R>>
	static void g0(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c , const int n_elmts);

	template <int N_ELMTS = 0>
	static void gr(const R * l_a, const R * l_b, const B * s_a, R * l_c, const int n_elmts);

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void gr(const std::vector<B,AB> &s, std::vector<R,AR> &l,
	               const int off_l_a, const int off_l_b, const int off_s_a, const int off_l_c, const int n_elmts);

	template <int N_ELMTS = 0>
	static void h(const R * l_a, B * s_a, const int n_elmts);

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void h(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a,
	              const int n_elmts);

	template <int N_ELMTS = 0>
	static void h0(B * s_a, const int n_elmts);

	template <int N_ELMTS = 0, class A = std::allocator<B>>
	static void h0(std::vector<B,A> &s, const int off_s_a, const int n_elmts);

	template <int N_ELMTS = 0>
	static void rep(const R * l_a, B * s_a, const int n_elmts);

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void rep(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a,
	                const int n_elmts);

	template <int N_ELMTS = 0>
	static void spc(const R * l_a, B * s_a, const int n_elmts);

	template <int N_ELMTS = 0, class AB = std::allocator<B>, class AR = std::allocator<R>>
	static void spc(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a,
	                const int n_elmts);

	template <int N_ELMTS = 0>
	static void xo(const B * s_a, const B * s_b, B * s_c, const int n_elmts);

	template <int N_ELMTS = 0, class A = std::allocator<B>>
	static void xo(std::vector<B,A> &s, const int off_s_a, const int off_s_b, const int off_s_c, const int n_elmts);

	template <int N_ELMTS = 0>
	static void xo0(const B * s_b, B * s_c, const int n_elmts);

	template <int N_ELMTS = 0, class A = std::allocator<B>>
	static void xo0(std::vector<B,A> &s, const int off_s_b, const int off_s_c, const int n_elmts);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/API/API_polar_dynamic_inter.hxx"
#endif

#endif /* API_POLAR_DYNAMIC_INTER_HPP_ */
