#ifndef FUNCTIONS_POLAR_SEQ_H_
#define FUNCTIONS_POLAR_SEQ_H_

#include <algorithm>
#include <limits>
#ifdef _MSC_VER
#include <iterator>
#endif

#include "Tools/Math/utils.h"
#include "Tools/Code/Polar/decoder_polar_functions.h"

namespace aff3ct
{
namespace tools
{
// ================================================================================================================ f()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_f<R> F, int N_ELMTS = 0>
struct f_seq
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		for (auto i = 0; i < N_ELMTS; i++)
			l_c[i] = F(l_a[i], l_b[i]);
	}
};

template <typename R, proto_f<R> F>
struct f_seq <R, F, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		for (auto i = 0; i < n_elmts; i++) // not vectorized with icpc: FLOW and ANTI dependences
			l_c[i] = F(l_a[i], l_b[i]);
	}
};

// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, int N_ELMTS = 0>
struct g_seq
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		for (auto i = 0; i < N_ELMTS; i++)
			l_c[i] = G(l_a[i], l_b[i], s_a[i]);
	}
};

template <typename B, typename R, proto_g<B,R> G>
struct g_seq <B, R, G, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		for (auto i = 0; i < n_elmts; i++)
			l_c[i] = G(l_a[i], l_b[i], s_a[i]);
	}
};

// =============================================================================================================== g0()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_g0<R> G0, int N_ELMTS = 0>
struct g0_seq
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		for (auto i = 0; i < N_ELMTS; i++)
			l_c[i] = G0(l_a[i], l_b[i]);
	}
};

template <typename R, proto_g0<R> G0>
struct g0_seq <R, G0, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts = 0)
	{
		for (auto i = 0; i < n_elmts; i++)
			l_c[i] = G0(l_a[i], l_b[i]);
	}
};

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g<B,R> G, int N_ELMTS = 0>
struct gr_seq
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		const auto u = s_a[0];
		for (auto i = 0; i < N_ELMTS; i++)
			l_c[i] = G(l_a[i], l_b[i], u);
	}
};

template <typename B, typename R, proto_g<B,R> G>
struct gr_seq <B, R, G, 0>
{
	static void apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
	                  const int n_elmts = 0)
	{
		const auto u = s_a[0];
		for (auto i = 0; i < n_elmts; i++)
			l_c[i] = G(l_a[i], l_b[i], u);
	}
};

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, int N_ELMTS = 0>
struct h_seq
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		for (auto i = 0; i < N_ELMTS; i++)
			s_a[i] = H(l_a[i]);
	}
};

template <typename B, typename R, proto_h<B,R> H>
struct h_seq <B, R, H, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		for (auto i = 0; i < n_elmts; i++)
			s_a[i] = H(l_a[i]);
	}
};

// =============================================================================================================== h0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0>
struct h0_seq
{
	static void apply(B *__restrict s_a, const int n_elmts = 0)
	{
		std::fill(s_a, s_a + N_ELMTS, 0);
	}
};

template <typename B>
struct h0_seq <B, 0>
{
	static void apply(B *__restrict s_a, const int n_elmts = 0)
	{
		std::fill(s_a, s_a + n_elmts, 0);
	}
};

// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, int N_ELMTS = 0>
struct rep_seq
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		R sum_l = (R)0;
		for (auto i = 0; i < N_ELMTS; i++)
			sum_l += l_a[i];

		auto r = H(sum_l);
		for (auto i = 0; i < N_ELMTS; i++)
			s_a[i] = r;
	}
};

template <typename B, proto_h<B,signed char> H, int N_ELMTS>
struct rep_seq<B, signed char, H, N_ELMTS>
{
	static void apply(const signed char *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		int sum_l = 0; // accumulate in a integer instead of a char
		for (auto i = 0; i < N_ELMTS; i++)
			sum_l += (int)l_a[i];

		// hardcoded h function
		B r = ((sum_l < 0) * bit_init<B>());
		for (auto i = 0; i < N_ELMTS; i++)
			s_a[i] = r;
	}
};

template <typename B, typename R, proto_h<B,R> H>
struct rep_seq <B, R, H, 0>
{
	static void apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		R sum_l = (R)0;
		for (auto i = 0; i < n_elmts; i++)
			sum_l += l_a[i];

		auto r = H(sum_l);
		for (auto i = 0; i < n_elmts; i++)
			s_a[i] = r;
	}
};

template <typename B, proto_h<B,signed char> H>
struct rep_seq <B, signed char, H, 0>
{
	static void apply(const signed char *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		int sum_l = 0; // accumulate in a integer instead of a char
		for (auto i = 0; i < n_elmts; i++)
			sum_l += (int)l_a[i];

		// hardcoded h function
		B r = ((sum_l < 0) * bit_init<B>());
		for (auto i = 0; i < n_elmts; i++)
			s_a[i] = r;
	}
};

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h<B,R> H, int N_ELMTS = 0>
struct spc_seq
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		auto cur_min_abs = std::numeric_limits<R>::max();
		auto cur_min_pos = -1;
		auto prod_sign   =  1;
		for (auto i = 0; i < N_ELMTS; i++)
		{
			s_a[i] = H(l_a[i]);
			auto sign = (s_a[i] == 0) ? 1 : -1;
			auto abs  = (R)sign * l_a[i];

			if (cur_min_abs > abs)
			{
				cur_min_abs = abs;
				cur_min_pos = i;
			}

			prod_sign *= sign;
		}

		if (prod_sign < 0)
			s_a[cur_min_pos] = (s_a[cur_min_pos] == 0) ? bit_init<B>() : 0; // correction

		return (prod_sign < 0);
	}
};

template <typename B, typename R, proto_h<B,R> H>
struct spc_seq <B, R, H, 0>
{
	static bool apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts = 0)
	{
		auto cur_min_abs = std::numeric_limits<R>::max();
		auto cur_min_pos = -1;
		auto prod_sign   =  1;
		for (auto i = 0; i < n_elmts; i++)
		{
			s_a[i] = H(l_a[i]);
			auto sign = (s_a[i] == 0) ? 1 : -1;
			auto abs  = (R)sign * l_a[i];

			if (cur_min_abs > abs)
			{
				cur_min_abs = abs;
				cur_min_pos = i;
			}

			prod_sign *= sign;
		}

		if (prod_sign < 0)
			s_a[cur_min_pos] = (s_a[cur_min_pos] == 0) ? bit_init<B>() : 0; // correction

		return (prod_sign < 0);
	}
};

// ================================================================================================================ x()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, proto_xo<B> X, int N_ELMTS = 0>
struct xo_seq
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0)
	{
		for (auto i = 0; i < N_ELMTS; i += 1)
			s_c[i] = X(s_a[i], s_b[i]);
	}
};

template <typename B, proto_xo<B> X>
struct xo_seq <B, X, 0>
{
	static void apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c,
	                  const int n_elmts = 0)
	{
		for (auto i = 0; i < n_elmts; i += 1)
			s_c[i] = X(s_a[i], s_b[i]);
	}
};

// =============================================================================================================== x0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS = 0>
struct xo0_seq
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
#ifdef _MSC_VER
		std::copy(s_b, s_b + N_ELMTS, stdext::checked_array_iterator<B*>(s_c, N_ELMTS));
#else
		std::copy(s_b, s_b + N_ELMTS, s_c);
#endif
	}
};

template <typename B>
struct xo0_seq <B, 0>
{
	static void apply(const B *__restrict s_b, B *__restrict s_c, const int n_elmts = 0)
	{
#ifdef _MSC_VER
		std::copy(s_b, s_b + n_elmts, stdext::checked_array_iterator<B*>(s_c, n_elmts));
#else
		std::copy(s_b, s_b + n_elmts, s_c);
#endif
	}
};
}
}

#endif /* FUNCTIONS_POLAR_SEQ_H_ */
