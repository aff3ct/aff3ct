#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <limits>
#include <algorithm>
#ifdef _MSC_VER
#include <iterator>
#endif
#include <mipp.h>

#include "Tools/Math/utils.h"
#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter.h"
#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter_8bit_bitpacking.h"

namespace aff3ct
{
namespace tools
{
// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI, int N_ELMTS>
void g_inter_8bit_bitpacking<B,R,GI,N_ELMTS>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
        const int init_shift, const int n_elmts)
{
	constexpr auto stride  = mipp::nElmtsPerRegister<R>() * (sizeof(B) * 8);
	constexpr auto _n_elmts = N_ELMTS * mipp::nElReg<R>();

	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	const auto r_mask = mipp::set1<B>(bit_init<B>());
	auto k = 0;
	for (auto i = 0; i < _n_elmts; i += stride)
	{
		const auto r_u_packed = mipp::load<B>(s_a + mipp::nElmtsPerRegister<B>() * k++);

		for (unsigned j = 0; j < (sizeof(B) * 8); j++)
		{
			const auto r_lambda_a   = mipp::load<R>(l_a + i + j * mipp::nElmtsPerRegister<R>()                 );
			const auto r_lambda_b   = mipp::load<R>(l_b + i + j * mipp::nElmtsPerRegister<R>()                 );
			const auto r_u_unpacked = mipp::andb<B>(mipp::lshift<B>(r_u_packed, (sizeof(B) * 8 -1) - j), r_mask);
			const auto r_lambda_c   = GI(r_lambda_a, r_lambda_b, r_u_unpacked                                  );

			mipp::store<R>(l_c + i + j * mipp::nElmtsPerRegister<R>(),
			               API_polar_inter_intra_saturate<R>::perform(r_lambda_c, r_sat));
		}
	}
}

template <typename B, typename R, proto_g_i<B,R> GI>
void g_inter_8bit_bitpacking<B,R,GI,0>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
        const int init_shift, const int n_elmts)
{
	constexpr auto stride  = mipp::nElmtsPerRegister<R>() * (sizeof(B) * 8);
	const     auto _n_elmts = n_elmts * mipp::nElReg<R>();

	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	const auto r_mask = mipp::set1<B>(bit_init<B>());
	auto k = 0;
	for (auto i = 0; i < _n_elmts; i += stride)
	{
		const auto r_u_packed = mipp::load<B>(s_a + mipp::nElmtsPerRegister<B>() * k++);

		for (unsigned j = 0; j < (sizeof(B) * 8); j++)
		{
			const auto r_lambda_a   = mipp::load<R>(l_a + i + j * mipp::nElmtsPerRegister<R>()                 );
			const auto r_lambda_b   = mipp::load<R>(l_b + i + j * mipp::nElmtsPerRegister<R>()                 );
			const auto r_u_unpacked = mipp::andb<B>(mipp::lshift<B>(r_u_packed, (sizeof(B) * 8 -1) - j), r_mask);
			const auto r_lambda_c   = GI(r_lambda_a, r_lambda_b, r_u_unpacked                                  );

			mipp::store<R>(l_c + i + j * mipp::nElmtsPerRegister<R>(),
			               API_polar_inter_intra_saturate<R>::perform(r_lambda_c, r_sat));
		}
	}
}

template <typename B, typename R, proto_g_i<B,R> GI>
void g_inter_8bit_bitpacking<B,R,GI,4>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
        const int init_shift, const int n_elmts)
{
	const auto r_mask     = mipp::set1<B>(bit_init<B>());
	const auto r_u_packed = mipp::load<B>(s_a);

	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	int shift = (sizeof(B) * 8 -1) - init_shift;
	for (auto i = 0; i < 4; i++)
	{
		const auto r_lambda_a   = mipp::load<R>(l_a + i * mipp::nElmtsPerRegister<R>());
		const auto r_lambda_b   = mipp::load<R>(l_b + i * mipp::nElmtsPerRegister<R>());
		const auto r_u_unpacked = mipp::andb<B>(mipp::lshift<B>(r_u_packed, shift--), r_mask);
		const auto r_lambda_c   = GI(r_lambda_a, r_lambda_b, r_u_unpacked);

		mipp::store<R>(l_c + i * mipp::nElmtsPerRegister<R>(),
		               API_polar_inter_intra_saturate<R>::perform(r_lambda_c, r_sat));
	}
}

template <typename B, typename R, proto_g_i<B,R> GI>
void g_inter_8bit_bitpacking<B,R,GI,2>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
        const int init_shift, const int n_elmts)
{
	const auto r_mask     = mipp::set1<B>(bit_init<B>());
	const auto r_u_packed = mipp::load<B>(s_a);

	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	int shift = (sizeof(B) * 8 -1) - init_shift;
	for (auto i = 0; i < 2; i++)
	{
		const auto r_lambda_a   = mipp::load<R>(l_a + i * mipp::nElmtsPerRegister<R>());
		const auto r_lambda_b   = mipp::load<R>(l_b + i * mipp::nElmtsPerRegister<R>());
		const auto r_u_unpacked = mipp::andb<B>(mipp::lshift<B>(r_u_packed, shift--), r_mask);
		const auto r_lambda_c   = GI(r_lambda_a, r_lambda_b, r_u_unpacked);

		mipp::store<R>(l_c + i * mipp::nElmtsPerRegister<R>(),
		               API_polar_inter_intra_saturate<R>::perform(r_lambda_c, r_sat));
	}
}

template <typename B, typename R, proto_g_i<B,R> GI>
void g_inter_8bit_bitpacking<B,R,GI,1>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
        const int init_shift, const int n_elmts)
{
	const auto r_mask        = mipp::set1<B>(bit_init<B>());
	const auto r_u_packed    = mipp::load<B>(s_a);
	const auto r_lambda_a0   = mipp::load<R>(l_a + 0 * mipp::nElmtsPerRegister<R>());
	const auto r_lambda_b0   = mipp::load<R>(l_b + 0 * mipp::nElmtsPerRegister<R>());
	const auto r_u_unpacked0 = mipp::andb<B>(mipp::lshift<B>(r_u_packed, (sizeof(B) * 8 -1) - init_shift), r_mask);
	const auto r_lambda_c0   = GI(r_lambda_a0, r_lambda_b0, r_u_unpacked0);

	const auto r_sat = API_polar_inter_intra_saturate<R>::init();
	mipp::store<R>(l_c + 0 * mipp::nElmtsPerRegister<R>(),
	               API_polar_inter_intra_saturate<R>::perform(r_lambda_c0, r_sat));
}

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI, int N_ELMTS>
void gr_inter_8bit_bitpacking<B,R,GI,N_ELMTS>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
        const int init_shift, const int n_elmts)
{
	constexpr auto stride   = mipp::nElmtsPerRegister<R>();
	constexpr auto _n_elmts = N_ELMTS * mipp::nElReg<R>();

	const auto mask         = mipp::set1<B>(bit_init<B>());
	const auto r_u          = mipp::load<B>(s_a);
	const auto r_u_unpacked = mipp::andb<B>(mipp::lshift<B>(r_u, (sizeof(B) * 8 -1) - init_shift), mask);

	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	for (auto i = 0; i < _n_elmts; i += stride)
	{
		const auto r_lambda_a = mipp::load<R>(l_a +i);
		const auto r_lambda_b = mipp::load<R>(l_b +i);
		const auto r_lambda_c = GI(r_lambda_a, r_lambda_b, r_u_unpacked);

		mipp::store<R>(l_c +i, API_polar_inter_intra_saturate<R>::perform(r_lambda_c, r_sat));
	}
}

template <typename B, typename R, proto_g_i<B,R> GI>
void gr_inter_8bit_bitpacking<B,R,GI,0>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c,
        const int init_shift, const int n_elmts)
{
	constexpr auto stride   = mipp::nElmtsPerRegister<R>();
	const     auto _n_elmts = n_elmts * mipp::nElReg<R>();

	const auto mask         = mipp::set1<B>(bit_init<B>());
	const auto r_u          = mipp::load<B>(s_a);
	const auto r_u_unpacked = mipp::andb<B>(mipp::lshift<B>(r_u, (sizeof(B) * 8 -1) - init_shift), mask);

	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	for (auto i = 0; i < _n_elmts; i += stride)
	{
		const auto r_lambda_a = mipp::load<R>(l_a +i);
		const auto r_lambda_b = mipp::load<R>(l_b +i);
		const auto r_lambda_c = GI(r_lambda_a, r_lambda_b, r_u_unpacked);

		mipp::store<R>(l_c +i, API_polar_inter_intra_saturate<R>::perform(r_lambda_c, r_sat));
	}
}

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS>
void h_inter_8bit_bitpacking<B,R,HI,N_ELMTS>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto stride  = mipp::nElmtsPerRegister<R>() * (sizeof(B) * 8);
	constexpr auto _n_elmts = N_ELMTS * mipp::nElReg<R>();

	auto k = 0;
	auto r_u_packed = mipp::set0<B>();

	for (auto i = 0; i < _n_elmts; i += stride)
	{
		for (unsigned j = 0; j < (sizeof(B) * 8); j++)
		{
			const auto r_lambda_a = mipp::load  <R>(l_a + i + j * mipp::nElmtsPerRegister<R>());
			const auto r_u        = mipp::rshift<B>(HI(r_lambda_a), ((sizeof(B) * 8 -1) - j));
			           r_u_packed = mipp::orb   <B>(r_u, r_u_packed);
		}

		mipp::store<B>(s_a + mipp::nElmtsPerRegister<B>() * k++, r_u_packed);
		r_u_packed = mipp::set0<B>();
	}
}

template <typename B, typename R, proto_h_i<B,R> HI>
void h_inter_8bit_bitpacking<B,R,HI,0>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto stride   = mipp::nElmtsPerRegister<R>() * (sizeof(B) * 8);
	const     auto _n_elmts = n_elmts * mipp::nElReg<R>();

	auto k = 0;
	auto r_u_packed = mipp::set0<B>();

	for (auto i = 0; i < _n_elmts; i += stride)
	{
		for (unsigned j = 0; j < (sizeof(B) * 8); j++)
		{
			const auto r_lambda_a = mipp::load  <R>(l_a + i + j * mipp::nElmtsPerRegister<R>());
			const auto r_u        = mipp::rshift<B>(HI(r_lambda_a), ((sizeof(B) * 8 -1) - j));
			           r_u_packed = mipp::orb   <B>(r_u, r_u_packed);
		}

		mipp::store<B>(s_a + mipp::nElmtsPerRegister<B>() * k++, r_u_packed);
		r_u_packed = mipp::set0<B>();
	}
}

template <typename B, typename R, proto_h_i<B,R> HI>
void h_inter_8bit_bitpacking<B,R,HI,4>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	const auto r_init     = mipp::set1  <B>(0x0F);
	const auto r_mask     = mipp::lshift<B>(r_init, init_shift);
	      auto r_u_packed = mipp::load  <B>(s_a);
	           r_u_packed = mipp::andnb <B>(r_mask, r_u_packed);

	for (auto i = 0; i < 4; i++)
	{
		const auto r_lambda_a = mipp::load  <R>(l_a + mipp::nElmtsPerRegister<R>() * i);
		const auto r_u        = mipp::rshift<B>(HI(r_lambda_a), ((sizeof(B) * 8 -1) - (init_shift + i)));
		r_u_packed = mipp::orb<B>(r_u, r_u_packed);
	}

	mipp::store<B>(s_a, r_u_packed);
}

template <typename B, typename R, proto_h_i<B,R> HI>
void h_inter_8bit_bitpacking<B,R,HI,2>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	const auto r_init     = mipp::set1  <B>(0x03);
	const auto r_mask     = mipp::lshift<B>(r_init, init_shift);
	      auto r_u_packed = mipp::load  <B>(s_a);
	           r_u_packed = mipp::andnb <B>(r_mask, r_u_packed);

	for (auto i = 0; i < 2; i++)
	{
		const auto r_lambda_a = mipp::load  <R>(l_a + mipp::nElmtsPerRegister<R>() * i);
		const auto r_u        = mipp::rshift<B>(HI(r_lambda_a), ((sizeof(B) * 8 -1) - (init_shift + i)));
		r_u_packed = mipp::orb<B>(r_u, r_u_packed);
	}

	mipp::store<B>(s_a, r_u_packed);
}

template <typename B, typename R, proto_h_i<B,R> HI>
void h_inter_8bit_bitpacking<B,R,HI,1>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	const auto r_init     = mipp::set1  <B>(0x01);
	const auto r_mask     = mipp::lshift<B>(r_init, init_shift);
	      auto r_u_packed = mipp::load  <B>(s_a);
	           r_u_packed = mipp::andnb <B>(r_mask, r_u_packed);

	const auto r_lambda_a = mipp::load  <R>(l_a);
	const auto r_u        = mipp::rshift<B>(HI(r_lambda_a), ((sizeof(B) * 8 -1) - init_shift));
	           r_u_packed = mipp::orb   <B>(r_u, r_u_packed);

	mipp::store<B>(s_a, r_u_packed);
}

// =============================================================================================================== h0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS>
void h0_inter_8bit_bitpacking<B,N_ELMTS>
::apply(B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto _n_elmts = (N_ELMTS * mipp::nElmtsPerRegister<B>()) / (sizeof(B) * 8);
	std::fill(s_a, s_a + _n_elmts, 0);
}

template <typename B>
void h0_inter_8bit_bitpacking<B,0>
::apply(B *__restrict s_a, const int init_shift, const int n_elmts)
{
	const auto _n_elmts = (n_elmts  * mipp::nElmtsPerRegister<B>()) / (sizeof(B) * 8);
	std::fill(s_a, s_a + _n_elmts, 0);
}

template <typename B>
void h0_inter_8bit_bitpacking<B,4>
::apply(B *__restrict s_a, const int init_shift, const int n_elmts)
{
	const auto r_init     = mipp::set1  <B>(0x0F);
	const auto r_mask     = mipp::lshift<B>(r_init, init_shift);
	      auto r_u_packed = mipp::load  <B>(s_a);
	           r_u_packed = mipp::andnb <B>(r_mask, r_u_packed);

	mipp::store<B>(s_a, r_u_packed);
}

template <typename B>
void h0_inter_8bit_bitpacking<B,2>
::apply(B *__restrict s_a, const int init_shift, const int n_elmts)
{
	const auto r_init     = mipp::set1  <B>(0x03);
	const auto r_mask     = mipp::lshift<B>(r_init, init_shift);
	      auto r_u_packed = mipp::load  <B>(s_a);
	           r_u_packed = mipp::andnb <B>(r_mask, r_u_packed);

	mipp::store<B>(s_a, r_u_packed);
}

template <typename B>
void h0_inter_8bit_bitpacking<B,1>
::apply(B *__restrict s_a, const int init_shift, const int n_elmts)
{
	const auto r_init     = mipp::set1  <B>(0x01);
	const auto r_mask     = mipp::lshift<B>(r_init, init_shift);
	      auto r_u_packed = mipp::load  <B>(s_a);
	           r_u_packed = mipp::andnb <B>(r_mask, r_u_packed);

	mipp::store<B>(s_a, r_u_packed);
}

// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS>
void rep_inter_8bit_bitpacking<B,R,HI,N_ELMTS>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();
	constexpr auto _n_elmts = N_ELMTS * mipp::nElReg<R>();

#ifndef ENABLE_HARD_REP
	const auto r_sat = API_polar_inter_intra_saturate<R>::init();
#endif
	auto r_sum_l = mipp::set0<R>();
	for (auto i = 0; i < _n_elmts; i += stride)
	{
		const auto r_lambda_a = mipp::load<R>(l_a +i);
#ifndef ENABLE_HARD_REP
		r_sum_l = mipp::add<R>(API_polar_inter_intra_saturate<R>::perform(r_lambda_a, r_sat), r_sum_l);
#else
		r_sum_l = mipp::add<R>(mipp::rshift<R>(r_lambda_a, sizeof(B) * 8 -1), r_sum_l);
#endif
	}

#ifndef ENABLE_HARD_REP
	const auto r_r = HI(r_sum_l);
#else
	const auto r_r = HI(mipp::sub<R>(mipp::set1<R>((R)(N_ELMTS >> 1)), r_sum_l));
#endif
	      auto r_r_packed = mipp::set0<B>();
	for (unsigned i = 0; i < (sizeof(B) * 8); i++)
	{
		r_r_packed = mipp::rshift<B>(r_r_packed,   1);
		r_r_packed = mipp::orb   <B>(r_r, r_r_packed);
	}

	for (unsigned i = 0; i < _n_elmts / (sizeof(B) * 8); i += stride)
	{
		mipp::store<B>(s_a +i, r_r_packed);
	}
}

template <typename B, typename R, proto_h_i<B,R> HI>
void rep_inter_8bit_bitpacking<B,R,HI,0>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto stride   = mipp::nElmtsPerRegister<R>();
	const     auto _n_elmts = n_elmts * mipp::nElReg<R>();

#ifndef ENABLE_HARD_REP
	const auto r_sat = API_polar_inter_intra_saturate<R>::init();
#endif
	auto r_sum_l = mipp::set0<R>();
	for (auto i = 0; i < _n_elmts; i += stride)
	{
		const auto r_lambda_a = mipp::load<R>(l_a +i);
#ifndef ENABLE_HARD_REP
		r_sum_l = mipp::add<R>(API_polar_inter_intra_saturate<R>::perform(r_lambda_a, r_sat), r_sum_l);
#else
		r_sum_l = mipp::add<R>(mipp::rshift<R>(r_lambda_a, sizeof(B) * 8 -1), r_sum_l);
#endif
	}

#ifndef ENABLE_HARD_REP
	const auto r_r = HI(r_sum_l);
#else
	const auto r_r = HI(mipp::sub<R>(mipp::set1<R>((R)(N_ELMTS >> 1)), r_sum_l));
#endif
	      auto r_r_packed = mipp::set0<B>();
	for (unsigned i = 0; i < (sizeof(B) * 8); i++)
	{
		r_r_packed = mipp::rshift<B>(r_r_packed,   1);
		r_r_packed = mipp::orb   <B>(r_r, r_r_packed);
	}

	for (unsigned i = 0; i < _n_elmts / (sizeof(B) * 8); i += stride)
	{
		mipp::store<B>(s_a +i, r_r_packed);
	}
}

template <typename B, typename R, proto_h_i<B,R> HI>
void rep_inter_8bit_bitpacking<B,R,HI,4>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();
	constexpr auto _n_elmts = 4 * mipp::nElReg<R>();

#ifndef ENABLE_HARD_REP4
	const auto r_sat = API_polar_inter_intra_saturate<R>::init();
#endif
	auto r_sum_l = mipp::set0<R>();
	for (auto i = 0; i < _n_elmts; i += stride)
	{
		const auto r_lambda_a = mipp::load<R>(l_a +i);
#ifndef ENABLE_HARD_REP4
		r_sum_l = mipp::add<R>(API_polar_inter_intra_saturate<R>::perform(r_lambda_a, r_sat), r_sum_l);
#else
		r_sum_l = mipp::add<R>(mipp::rshift<R>(r_lambda_a, sizeof(B) * 8 -1), r_sum_l);
#endif
	}

#ifndef ENABLE_HARD_REP4
	const auto r_r = HI(r_sum_l);
#else
	const auto r_r = HI(mipp::sub<R>(mipp::set1<R>(2), r_sum_l));
#endif
	      auto r_r_packed     = mipp::load  <B>(s_a);
	// TODO: warning this next line works only for bytes
	const auto r_mask         = mipp::lshift<B>(mipp::set1<B>(0x0F), init_shift);
	           r_r_packed     = mipp::andnb <B>(r_mask, r_r_packed);

	      auto r_r_packed_tmp = mipp::orb   <B>(r_r, mipp::set0<B>());

	           r_r_packed_tmp = mipp::rshift<B>(r_r_packed_tmp, 1);
	           r_r_packed_tmp = mipp::orb   <B>(r_r, r_r_packed_tmp);

	           r_r_packed_tmp = mipp::rshift<B>(r_r_packed_tmp, 1);
	           r_r_packed_tmp = mipp::orb   <B>(r_r, r_r_packed_tmp);

	           r_r_packed_tmp = mipp::rshift<B>(r_r_packed_tmp, 1);
	           r_r_packed_tmp = mipp::orb   <B>(r_r, r_r_packed_tmp);

	           r_r_packed_tmp = mipp::rshift<B>(r_r_packed_tmp, (sizeof(B) * 8 -4) - init_shift);

	           r_r_packed     = mipp::orb   <B>(r_r_packed_tmp, r_r_packed);

	mipp::store<B>(s_a, r_r_packed);
}

template <typename B, typename R, proto_h_i<B,R> HI>
void rep_inter_8bit_bitpacking<B,R,HI,2>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();
	constexpr auto _n_elmts = 2 * mipp::nElReg<R>();

#ifndef ENABLE_HARD_REP2
	const auto r_sat = API_polar_inter_intra_saturate<R>::init();
#endif
	auto r_sum_l = mipp::set0<R>();
	for (auto i = 0; i < _n_elmts; i += stride)
	{
		const auto r_lambda_a = mipp::load<R>(l_a +i);
#ifndef ENABLE_HARD_REP2
		r_sum_l = mipp::add<R>(API_polar_inter_intra_saturate<R>::perform(r_lambda_a, r_sat), r_sum_l);
#else
		r_sum_l = mipp::add<R>(mipp::rshift<R>(r_lambda_a, sizeof(B) * 8 -1), r_sum_l);
#endif
	}

#ifndef ENABLE_HARD_REP2
	const auto r_r = HI(r_sum_l);
#else
	const auto r_r = HI(mipp::sub<R>(mipp::set1<R>(1), r_sum_l));
#endif
	      auto r_r_packed     = mipp::load  <B>(s_a);
	// TODO: warning this next line works only for bytes
	const auto r_mask         = mipp::lshift<B>(mipp::set1<B>(0x03), init_shift);
	           r_r_packed     = mipp::andnb <B>(r_mask, r_r_packed);

	      auto r_r_packed_tmp = mipp::orb   <B>(r_r, mipp::set0<B>());

	           r_r_packed_tmp = mipp::rshift<B>(r_r_packed_tmp, 1);
	           r_r_packed_tmp = mipp::orb   <B>(r_r, r_r_packed_tmp);

	           r_r_packed_tmp = mipp::rshift<B>(r_r_packed_tmp, (sizeof(B) * 8 -2) - init_shift);

	           r_r_packed     = mipp::orb   <B>(r_r_packed_tmp, r_r_packed);

	mipp::store<B>(s_a, r_r_packed);
}

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS>
void spc_inter_8bit_bitpacking<B,R,HI,N_ELMTS>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto stride  = mipp::nElmtsPerRegister<R>() * (sizeof(B) * 8);
	constexpr auto stride2 = mipp::nElmtsPerRegister<R>();
	constexpr auto _n_elmts = N_ELMTS * mipp::nElReg<R>();

	// vectorized part of the SPC
	auto k = 0;
	auto r_s_a_packed  = mipp::set0<B>();
	auto r_cur_min_abs = mipp::set1<R>(std::numeric_limits<R>::max());
	auto r_prod_sign   = mipp::set1<R>((R)1);
	for (auto i = 0; i < _n_elmts; i += stride)
	{
		for (unsigned j = 0; j < (sizeof(B) * 8); j++)
		{
			const auto r_l_a     = mipp::load<R>(l_a + i + j * mipp::nElmtsPerRegister<R>());
			const auto r_s_a     = HI(r_l_a);
			const auto r_sign    = r_s_a;
			const auto r_abs_l_a = mipp::abs<R>(r_l_a);

			r_cur_min_abs = mipp::min <R>(r_cur_min_abs, r_abs_l_a);
			r_prod_sign   = mipp::xorb<R>(r_prod_sign,   r_sign   );

			// mipp::store<R>(l_a + i + j * mipp::nElmtsPerRegister<R>(), r_abs_l_a);

			r_s_a_packed = mipp::rshift<B>(r_s_a_packed, 1    );
			r_s_a_packed = mipp::orb   <B>(r_s_a, r_s_a_packed);
		}

		mipp::store<B>(s_a + mipp::nElmtsPerRegister<B>() * k++, r_s_a_packed);
		r_s_a_packed = mipp::set0<B>();
	}

#ifndef _MSC_VER
	R cur_min_abs[mipp::nElmtsPerRegister<R>()] __attribute__((aligned(MIPP_REQUIRED_ALIGNMENT)));
#else
	R cur_min_abs[mipp::nElmtsPerRegister<R>()];
#endif
	mipp::store<R>(cur_min_abs, r_cur_min_abs);
#ifndef _MSC_VER
	R prod_sign  [mipp::nElmtsPerRegister<R>()] __attribute__((aligned(MIPP_REQUIRED_ALIGNMENT)));
#else
	R prod_sign  [mipp::nElmtsPerRegister<R>()];
#endif
	mipp::store<R>(prod_sign,   r_prod_sign);

	// sequential part of the SPC
	for (auto i = 0; i < stride2; i++)
	{
		if (prod_sign[i] < 0) // make the correction
		{
			auto j = 0;
			// while (l_a[j * stride2 + i] != cur_min_abs[i]) j++;
			while (std::abs(l_a[j * stride2 + i]) != cur_min_abs[i]) j++;

			// TODO: warning this next line works only for bytes
			unsigned char val = (unsigned char) s_a[(j / (sizeof(B) * 8)) * stride2 +i];
			unsigned char bit_switcher = 1 << (j % (sizeof(B) * 8));
			s_a[(j / (sizeof(B) * 8)) * stride2 +i] = (B) (val ^ bit_switcher);
		}
	}
}

template <typename B, typename R, proto_h_i<B,R> HI>
void spc_inter_8bit_bitpacking<B,R,HI,0>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto stride   = mipp::nElmtsPerRegister<R>() * (sizeof(B) * 8);
	constexpr auto stride2  = mipp::nElmtsPerRegister<R>();
	const     auto _n_elmts = n_elmts * mipp::nElReg<R>();

	// vectorized part of the SPC
	auto k = 0;
	auto r_s_a_packed  = mipp::set0<B>();
	auto r_cur_min_abs = mipp::set1<R>(std::numeric_limits<R>::max());
	auto r_prod_sign   = mipp::set1<R>((R)1);
	for (auto i = 0; i < _n_elmts; i += stride)
	{
		for (unsigned j = 0; j < (sizeof(B) * 8); j++)
		{
			const auto r_l_a     = mipp::load<R>(l_a + i + j * mipp::nElmtsPerRegister<R>());
			const auto r_s_a     = HI(r_l_a);
			const auto r_sign    = r_s_a;
			const auto r_abs_l_a = mipp::abs<R>(r_l_a);

			r_cur_min_abs = mipp::min <R>(r_cur_min_abs, r_abs_l_a);
			r_prod_sign   = mipp::xorb<R>(r_prod_sign,   r_sign   );

			// mipp::store<R>(l_a + i + j * mipp::nElmtsPerRegister<R>(), r_abs_l_a);

			r_s_a_packed = mipp::rshift<B>(r_s_a_packed, 1    );
			r_s_a_packed = mipp::orb   <B>(r_s_a, r_s_a_packed);
		}

		mipp::store<B>(s_a + mipp::nElmtsPerRegister<B>() * k++, r_s_a_packed);
		r_s_a_packed = mipp::set0<B>();
	}

#ifndef _MSC_VER
	R cur_min_abs[mipp::nElmtsPerRegister<R>()] __attribute__((aligned(MIPP_REQUIRED_ALIGNMENT)));
#else
	R cur_min_abs[mipp::nElmtsPerRegister<R>()];
#endif
	mipp::store<R>(cur_min_abs, r_cur_min_abs);
#ifndef _MSC_VER
	R prod_sign  [mipp::nElmtsPerRegister<R>()] __attribute__((aligned(MIPP_REQUIRED_ALIGNMENT)));
#else
	R prod_sign  [mipp::nElmtsPerRegister<R>()];
#endif
	mipp::store<R>(prod_sign,   r_prod_sign);

	// sequential part of the SPC
	for (auto i = 0; i < stride2; i++)
	{
		if (prod_sign[i] < 0) // make the correction
		{
			auto j = 0;
			// while (l_a[j * stride2 + i] != cur_min_abs[i]) j++;
			while (std::abs(l_a[j * stride2 + i]) != cur_min_abs[i]) j++;

			// TODO: warning this next line works only for bytes
			unsigned char val = (unsigned char) s_a[(j / (sizeof(B) * 8)) * stride2 +i];
			unsigned char bit_switcher = 1 << (j % (sizeof(B) * 8));
			s_a[(j / (sizeof(B) * 8)) * stride2 +i] = (B) (val ^ bit_switcher);
		}
	}
}

template <typename B, typename R, proto_h_i<B,R> HI>
void spc_inter_8bit_bitpacking<B,R,HI,4>
::apply(const R *__restrict l_a, B *__restrict s_a, const int init_shift, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// vectorized part of the SPC
	// TODO: warning this next line works only for bytes
	auto r_mask           = mipp::lshift<B>(mipp::set1<B>(0x0F), init_shift);
	auto r_s_a_packed     = mipp::andnb <B>(r_mask, mipp::load<B>(s_a));
	auto r_s_a_packed_tmp = mipp::set0  <B>();

	auto r_cur_min_abs = mipp::set1<R>(std::numeric_limits<R>::max());
	auto r_prod_sign   = mipp::set1<R>((R)1                         );

	for (auto j = 0; j < 4; j++)
	{
		const auto r_l_a            = mipp::load  <R>(l_a + j * mipp::nElmtsPerRegister<R>());
		const auto r_s_a            = HI(r_l_a);
		const auto r_sign           = r_s_a;
		const auto r_abs_l_a        = mipp::abs   <R>(r_l_a);
		           r_cur_min_abs    = mipp::min   <R>(r_cur_min_abs, r_abs_l_a);
		           r_prod_sign      = mipp::xorb  <R>(r_prod_sign, r_sign);

		           r_s_a_packed_tmp = mipp::rshift<B>(r_s_a_packed_tmp, 1);
		           r_s_a_packed_tmp = mipp::orb   <B>(r_s_a, r_s_a_packed_tmp);

		// mipp::store<R>(l_a + j * mipp::nElmtsPerRegister<R>(), r_abs_l_a);
	}

	r_s_a_packed_tmp = mipp::rshift<B>(r_s_a_packed_tmp, (sizeof(B) * 8 -4) - init_shift);
	r_s_a_packed     = mipp::orb   <B>(r_s_a_packed, r_s_a_packed_tmp);

	mipp::store<B>(s_a, r_s_a_packed);

#ifndef _MSC_VER
	R cur_min_abs[mipp::nElmtsPerRegister<R>()] __attribute__((aligned(MIPP_REQUIRED_ALIGNMENT)));
#else
	R cur_min_abs[mipp::nElmtsPerRegister<R>()];
#endif
	mipp::store<R>(cur_min_abs, r_cur_min_abs);
#ifndef _MSC_VER
	R prod_sign  [mipp::nElmtsPerRegister<R>()] __attribute__((aligned(MIPP_REQUIRED_ALIGNMENT)));
#else
	R prod_sign  [mipp::nElmtsPerRegister<R>()];
#endif
	mipp::store<R>(prod_sign,   r_prod_sign);

	// sequential part of the SPC
	for (auto i = 0; i < stride; i++)
	{
		if (prod_sign[i] < 0) // make the correction
		{
			auto j = 0;
			// while (l_a[j * stride + i] != cur_min_abs[i]) j++;
			while (std::abs(l_a[j * stride + i]) != cur_min_abs[i]) j++;

			// TODO: warning this next line works only for bytes
			unsigned char val = (unsigned char) s_a[(j / (sizeof(B) * 8)) * stride +i];
			unsigned char bit_switcher = 1 << ((j % (sizeof(B) * 8)) + init_shift);
			s_a[(j / (sizeof(B) * 8)) * stride +i] = (B) (val ^ bit_switcher);
		}
	}
}

// =============================================================================================================== xo()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, proto_xo_i<B> XOI, int N_ELMTS>
void xo_inter_8bit_bitpacking<B,XOI,N_ELMTS>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<B>();
	constexpr auto _n_elmts = (unsigned) ((N_ELMTS * mipp::nElReg<B>()) / (sizeof(B) * 8));

	for (unsigned i = 0; i < _n_elmts; i += stride)
	{
		const auto r_u_a_packed = mipp::load<B>(s_a +i);
		const auto r_u_b_packed = mipp::load<B>(s_b +i);
		const auto r_u_c_packed = XOI(r_u_a_packed, r_u_b_packed);
		mipp::store<B>(s_c +i, r_u_c_packed);
	}
}

template <typename B, proto_xo_i<B> XOI>
void xo_inter_8bit_bitpacking<B,XOI,0>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<B>();
	const     auto _n_elmts = (unsigned) ((n_elmts * mipp::nElReg<B>()) / (sizeof(B) * 8));

	for (unsigned i = 0; i < _n_elmts; i += stride)
	{
		const auto r_u_a_packed = mipp::load<B>(s_a +i);
		const auto r_u_b_packed = mipp::load<B>(s_b +i);
		const auto r_u_c_packed = XOI(r_u_a_packed, r_u_b_packed);
		mipp::store<B>(s_c +i, r_u_c_packed);
	}
}

template <typename B, proto_xo_i<B> XOI>
void xo_inter_8bit_bitpacking<B,XOI,4>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	// TODO: warning this next line works only for bytes
	const auto r_init       = mipp::set1  <B>(0x0F);
	const auto r_mask       = r_init;
	const auto r_u_a_packed = mipp::load  <B>(s_a);
	      auto r_u_b_packed = mipp::rshift<B>(r_u_a_packed, 4);
	           r_u_b_packed = mipp::andb  <B>(r_u_b_packed, r_mask);
	const auto r_u_c_packed = XOI(r_u_a_packed, r_u_b_packed);
	mipp::store<B>(s_c, r_u_c_packed);
}

template <typename B, proto_xo_i<B> XOI>
void xo_inter_8bit_bitpacking<B,XOI,2>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	// TODO: warning this next line is hard coded for bytes
	const auto r_init       = mipp::set1  <B>(0x03);
	const auto r_mask       = mipp::lshift<B>(r_init, init_shift);
	const auto r_u_a_packed = mipp::load  <B>(s_a);
	      auto r_u_b_packed = mipp::rshift<B>(r_u_a_packed, 2);
	           r_u_b_packed = mipp::andb  <B>(r_u_b_packed, r_mask);
	const auto r_u_c_packed = XOI(r_u_a_packed, r_u_b_packed);
	mipp::store<B>(s_c, r_u_c_packed);
}

template <typename B, proto_xo_i<B> XOI>
void xo_inter_8bit_bitpacking<B,XOI,1>
::apply(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	// TODO: warning this next line is hard coded for bytes
	const auto r_init       = mipp::set1  <B>(0x01);
	const auto r_mask       = mipp::lshift<B>(r_init, init_shift);
	const auto r_u_a_packed = mipp::load  <B>(s_a);
	      auto r_u_b_packed = mipp::rshift<B>(r_u_a_packed, 1);
	           r_u_b_packed = mipp::andb  <B>(r_u_b_packed, r_mask);
	const auto r_u_c_packed = XOI(r_u_a_packed, r_u_b_packed);
	mipp::store<B>(s_c, r_u_c_packed);
}

// =============================================================================================================== x0()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, int N_ELMTS>
void xo0_inter_8bit_bitpacking<B,N_ELMTS>
::apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	constexpr auto _n_elmts = (N_ELMTS * mipp::nElReg<B>()) / (sizeof(B) * 8);
#ifdef _MSC_VER
	std::copy(s_b, s_b + _n_elmts, stdext::checked_array_iterator<B*>(s_c, _n_elmts));
#else
	std::copy(s_b, s_b + _n_elmts, s_c);
#endif
}

template <typename B>
void xo0_inter_8bit_bitpacking<B,0>
::apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	const auto _n_elmts = (n_elmts * mipp::nElReg<B>()) / (sizeof(B) * 8);
#ifdef _MSC_VER
	std::copy(s_b, s_b + _n_elmts, stdext::checked_array_iterator<B*>(s_c, _n_elmts));
#else
	std::copy(s_b, s_b + _n_elmts, s_c);
#endif
}

template <typename B>
void xo0_inter_8bit_bitpacking<B,4>
::apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	// TODO: warning this next line works only for bytes
	const auto r_init         = mipp::set1  <B>(0x0F);
	const auto r_mask         = r_init;
	const auto r_u_b_c_packed = mipp::load  <B>(s_b);
	      auto r_u_b_packed   = mipp::rshift<B>(r_u_b_c_packed, 4);
	           r_u_b_packed   = mipp::andb  <B>(r_mask, r_u_b_packed);
	      auto r_u_c_packed   = mipp::andnb <B>(r_mask, r_u_b_c_packed);
	           r_u_c_packed   = mipp::orb   <B>(r_u_b_packed, r_u_c_packed);
	mipp::store<B>(s_c, r_u_c_packed);
}

template <typename B>
void xo0_inter_8bit_bitpacking<B,2>
::apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	// TODO: warning this next line works only for bytes
	const auto r_init         = mipp::set1  <B>(0x03);
	const auto r_mask         = mipp::lshift<B>(r_init, init_shift);
	const auto r_u_b_c_packed = mipp::load  <B>(s_b);
	      auto r_u_b_packed   = mipp::rshift<B>(r_u_b_c_packed, 2);
	           r_u_b_packed   = mipp::andb  <B>(r_mask, r_u_b_packed);
	      auto r_u_c_packed   = mipp::andnb <B>(r_mask, r_u_b_c_packed);
	           r_u_c_packed   = mipp::orb   <B>(r_u_b_packed, r_u_c_packed);
	mipp::store<B>(s_c, r_u_c_packed);
}

template <typename B>
void xo0_inter_8bit_bitpacking <B,1>
::apply(const B *__restrict s_b, B *__restrict s_c, const int init_shift, const int n_elmts)
{
	// TODO: warning this next line works only for bytes
	const auto r_init         = mipp::set1  <B>(0x01);
	const auto r_mask         = mipp::lshift<B>(r_init, init_shift);
	const auto r_u_b_c_packed = mipp::load  <B>(s_b);
	      auto r_u_b_packed   = mipp::rshift<B>(r_u_b_c_packed, 1);
	           r_u_b_packed   = mipp::andb  <B>(r_mask, r_u_b_packed);
	      auto r_u_c_packed   = mipp::andnb <B>(r_mask, r_u_b_c_packed);
	           r_u_c_packed   = mipp::orb   <B>(r_u_b_packed, r_u_c_packed);
	mipp::store<B>(s_c, r_u_c_packed);
}

}
}
