#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <limits>
#include <cmath>
#include <mipp.h>

#include "Tools/Math/utils.h"
#include "Tools/Code/Polar/API/internal_functions/functions_polar_inter_intra.h"
#include "Tools/Code/Polar/API/internal_functions/functions_polar_intra.h"

namespace aff3ct
{
namespace tools
{
// ================================================================================================================ f()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_f_i<R> FI>
void f_intra_unaligned<R,FI>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	const auto r_lambda_a = mipp::loadu<R>(l_a);
	const auto r_lambda_b = mipp::loadu<R>(l_b);
	const auto r_lambda_c = FI(r_lambda_a, r_lambda_b);
	mipp::storeu<R>(l_c, r_lambda_c);
}

// ================================================================================================================ g()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI>
void g_intra_unaligned<B,R,GI>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	const auto r_lambda_a = mipp::loadu<R>(l_a);
	const auto r_lambda_b = mipp::loadu<R>(l_b);
	const auto r_u        = mipp::loadu<B>(s_a);
	const auto r_lambda_c = GI(r_lambda_a, r_lambda_b, r_u);

	mipp::storeu<R>(l_c, API_polar_inter_intra_saturate<R>::perform(r_lambda_c, r_sat));
}

// =============================================================================================================== g0()
// ====================================================================================================================
// ====================================================================================================================

template <typename R, proto_g0_i<R> G0I, int N_ELMTS>
void g0_intra_unaligned<R,G0I,N_ELMTS>
::apply(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	const auto r_lambda_a = mipp::loadu<R>(l_a);
	const auto r_lambda_b = mipp::loadu<R>(l_b);
	const auto r_lambda_c = G0I(r_lambda_a, r_lambda_b);

	mipp::storeu<R>(l_c, API_polar_inter_intra_saturate<R>::perform(r_lambda_c, r_sat));
}

// =============================================================================================================== gr()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_g_i<B,R> GI>
void gr_intra_unaligned<B,R,GI>
::apply(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	const auto r_lambda_a = mipp::loadu<R>(l_a);
	const auto r_lambda_b = mipp::loadu<R>(l_b);
	const auto r_u        = mipp::loadu<B>(s_a);
	const auto r_lambda_c = GI(r_lambda_a, r_lambda_b, r_u);

	mipp::storeu<R>(l_c, API_polar_inter_intra_saturate<R>::perform(r_lambda_c, r_sat));
}

// ================================================================================================================ h()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI>
void h_intra_unaligned<B,R,HI>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	const auto r_lambda_a = mipp::loadu<R>(l_a);
	const auto r_u        = HI(r_lambda_a);
	mipp::storeu<B>(s_a, r_u);
}

// ============================================================================================================== rep()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS>
void rep_intra<B,R,HI,N_ELMTS>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	auto r_sum_l = mipp::set0<R>();
	for (auto i = 0; i < N_ELMTS; i += stride)
	{
		const auto r_lambda_a = mipp::load<R>(l_a +i);
		r_sum_l = mipp::add<R>(API_polar_inter_intra_saturate<R>::perform(r_lambda_a, r_sat), r_sum_l);
	}

//	const auto r_r = HI(mipp::sum<R>(r_sum_l));
	const auto r_r = HI(mipp::reduction<R,mipp::add<R>>::apply(r_sum_l));
	for (auto i = 0; i < N_ELMTS; i += stride)
		mipp::store<B>(s_a +i, r_r);
}

template <typename B, typename R, proto_h_i<B,R> HI>
void rep_intra<B,R,HI,0>::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	const auto r_sat = API_polar_inter_intra_saturate<R>::init();

	auto r_sum_l = mipp::set0<R>();
	for (auto i = 0; i < n_elmts; i += stride)
	{
		const auto r_lambda_a = mipp::load<R>(l_a +i);
		r_sum_l = mipp::add<R>(API_polar_inter_intra_saturate<R>::perform(r_lambda_a, r_sat), r_sum_l);
	}

//	const auto r_r = HI(mipp::sum<R>(r_sum_l));
	const auto r_r = HI(mipp::reduction<R,mipp::add<R>>::apply(r_sum_l));
	for (auto i = 0; i < n_elmts; i += stride)
		mipp::store<B>(s_a +i, r_r);
}

// ============================================================================================================== spc()
// ====================================================================================================================
// ====================================================================================================================

template <typename B, typename R, proto_h_i<B,R> HI, int N_ELMTS>
bool spc_intra<B,R,HI,N_ELMTS>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// vectorized part of the SPC
	auto r_cur_min_abs = mipp::set1<R>(std::numeric_limits<R>::max());
	auto r_prod_sign   = mipp::set1<R>((R)1);
	for (auto i = 0; i < N_ELMTS; i += stride)
	{
		const auto r_l_a     = mipp::load<R>(l_a +i);
		const auto r_s_a     = HI(r_l_a);
		const auto r_sign    = r_s_a;
		const auto r_abs_l_a = mipp::abs<R>(r_l_a);

		r_cur_min_abs = mipp::min<R>(r_cur_min_abs, r_abs_l_a);
		r_prod_sign   = mipp::xorb<R>(r_prod_sign, r_sign);

		mipp::store<B>(s_a +i, r_s_a);
		//mipp::store<R>(l_a +i, r_abs_l_a);
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
	R s_cur_min_abs = std::numeric_limits<R>::max();
	R s_prod_sign = (R)1;
	for (auto i = 0; i < mipp::nElmtsPerRegister<R>(); i += 1)
	{
		s_cur_min_abs = std::min(s_cur_min_abs, cur_min_abs[i]);
		s_prod_sign *= prod_sign[i];
	}

	if (s_prod_sign < 0) // make the correction
	{
		auto i = 0;
		// while (l_a[i] != s_cur_min_abs) i++;
		while (std::abs(l_a[i]) != s_cur_min_abs) i++;
		s_a[i] = (s_a[i] == 0) ? bit_init<B>() : 0;
	}

	return (s_prod_sign < 0);
}

template <typename B, typename R, proto_h_i<B,R> HI>
bool spc_intra<B,R,HI,0>
::apply(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// vectorized part of the SPC
	auto r_cur_min_abs = mipp::set1<R>(std::numeric_limits<R>::max());
	auto r_prod_sign   = mipp::set1<R>((R)1);
	for (auto i = 0; i < n_elmts; i += stride)
	{
		const auto r_l_a     = mipp::load<R>(l_a +i);
		const auto r_s_a     = HI(r_l_a);
		const auto r_sign    = r_s_a;
		const auto r_abs_l_a = mipp::abs<R>(r_l_a);

		r_cur_min_abs = mipp::min<R>(r_cur_min_abs, r_abs_l_a);
		r_prod_sign   = mipp::xorb<R>(r_prod_sign, r_sign);

		mipp::store<B>(s_a +i, r_s_a);
		// mipp::store<R>(l_a +i, r_abs_l_a);
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
	R s_cur_min_abs = std::numeric_limits<R>::max();
	R s_prod_sign = (R)1;
	for (auto i = 0; i < mipp::nElmtsPerRegister<R>(); i += 1)
	{
		s_cur_min_abs = std::min(s_cur_min_abs, cur_min_abs[i]);
		s_prod_sign *= prod_sign[i];
	}

	if (s_prod_sign < 0) // make the correction
	{
		auto i = 0;
		// while (l_a[i] != s_cur_min_abs) i++;
		while (std::abs(l_a[i]) != s_cur_min_abs) i++;
		s_a[i] = (s_a[i] == 0) ? bit_init<B>() : 0;
	}
	return (s_prod_sign < 0);
}

}
}
