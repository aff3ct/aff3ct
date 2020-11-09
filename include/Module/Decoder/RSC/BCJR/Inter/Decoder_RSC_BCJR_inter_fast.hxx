#include <limits>
#include <string>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_fast.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_max_i<R> MAX>
Decoder_RSC_BCJR_inter_fast<B,R,MAX>
::Decoder_RSC_BCJR_inter_fast(const int &K,
                              const std::vector<std::vector<int>> &trellis,
                              const bool buffered_encoding)
: Decoder_RSC_BCJR_inter<B,R>(K, trellis, buffered_encoding)
{
	const std::string name = "Decoder_RSC_BCJR_inter_fast";
	this->set_name(name);
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
Decoder_RSC_BCJR_inter_fast<B,R,MAX>* Decoder_RSC_BCJR_inter_fast<B,R,MAX>
::clone() const
{
	auto m = new Decoder_RSC_BCJR_inter_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_inter_fast<B,R,MAX>
::compute_gamma(const R *sys, const R *par)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute gamma values
	for (auto i = 0; i < (this->K +3) * stride; i += stride)
	{
		const auto r_sys = mipp::Reg<R>(&sys[i]);
		const auto r_par = mipp::Reg<R>(&par[i]);

		const auto r_g0 = RSC_BCJR_inter_div_or_not<R>::apply(r_sys + r_par);
		const auto r_g1 = RSC_BCJR_inter_div_or_not<R>::apply(r_sys - r_par);

		r_g0.store(&this->gamma[0][i]);
		r_g1.store(&this->gamma[1][i]);
	}
}

template <typename R>
struct RSC_BCJR_inter_fast_normalize
{
	static void apply(mipp::Reg<R> &r_ab0, mipp::Reg<R> &r_ab1, mipp::Reg<R> &r_ab2, mipp::Reg<R> &r_ab3,
	                  mipp::Reg<R> &r_ab4, mipp::Reg<R> &r_ab5, mipp::Reg<R> &r_ab6, mipp::Reg<R> &r_ab7,
	                  const int &i)
	{
		// no need to normalize
	}
};

template <>
struct RSC_BCJR_inter_fast_normalize <short>
{
	static void apply(mipp::Reg<short> &r_ab0, mipp::Reg<short> &r_ab1, mipp::Reg<short> &r_ab2, mipp::Reg<short> &r_ab3,
	                  mipp::Reg<short> &r_ab4, mipp::Reg<short> &r_ab5, mipp::Reg<short> &r_ab6, mipp::Reg<short> &r_ab7,
	                  const int &i)
	{
		constexpr auto stride = mipp::nElmtsPerRegister<short>();

		// normalization
		if ((i / stride) % 8 == 0)
		{
			const auto r_norm_val = r_ab0;
			r_ab0 -= r_norm_val;
			r_ab1 -= r_norm_val;
			r_ab2 -= r_norm_val;
			r_ab3 -= r_norm_val;
			r_ab4 -= r_norm_val;
			r_ab5 -= r_norm_val;
			r_ab6 -= r_norm_val;
			r_ab7 -= r_norm_val;
		}
	}
};

template <>
struct RSC_BCJR_inter_fast_normalize <signed char>
{
	static void apply(mipp::Reg<signed char> &r_ab0, mipp::Reg<signed char> &r_ab1, mipp::Reg<signed char> &r_ab2,
	                  mipp::Reg<signed char> &r_ab3, mipp::Reg<signed char> &r_ab4, mipp::Reg<signed char> &r_ab5,
	                  mipp::Reg<signed char> &r_ab6, mipp::Reg<signed char> &r_ab7,
	                  const int &i)
	{
		// normalization
		const auto r_norm_val = r_ab0;
		r_ab0 -= r_norm_val;
		r_ab1 -= r_norm_val;
		r_ab2 -= r_norm_val;
		r_ab3 -= r_norm_val;
		r_ab4 -= r_norm_val;
		r_ab5 -= r_norm_val;
		r_ab6 -= r_norm_val;
		r_ab7 -= r_norm_val;
	}
};

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_inter_fast<B,R,MAX>
::compute_alpha()
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute alpha values [trellis forward traversal ->]
	for (auto i = stride; i < (this->K +3) * stride; i += stride)
	{
		const auto r_g0 = mipp::Reg<R>(&this->gamma[0][i -stride]);
		const auto r_g1 = mipp::Reg<R>(&this->gamma[1][i -stride]);

		const auto r_a0_prev = mipp::Reg<R>(&this->alpha[0][i -stride]);
		const auto r_a1_prev = mipp::Reg<R>(&this->alpha[1][i -stride]);
		const auto r_a2_prev = mipp::Reg<R>(&this->alpha[2][i -stride]);
		const auto r_a3_prev = mipp::Reg<R>(&this->alpha[3][i -stride]);
		const auto r_a4_prev = mipp::Reg<R>(&this->alpha[4][i -stride]);
		const auto r_a5_prev = mipp::Reg<R>(&this->alpha[5][i -stride]);
		const auto r_a6_prev = mipp::Reg<R>(&this->alpha[6][i -stride]);
		const auto r_a7_prev = mipp::Reg<R>(&this->alpha[7][i -stride]);

		auto r_a0 = MAX(r_a0_prev + r_g0, r_a1_prev - r_g0);
		auto r_a1 = MAX(r_a3_prev + r_g1, r_a2_prev - r_g1);
		auto r_a2 = MAX(r_a4_prev + r_g1, r_a5_prev - r_g1);
		auto r_a3 = MAX(r_a7_prev + r_g0, r_a6_prev - r_g0);
		auto r_a4 = MAX(r_a1_prev + r_g0, r_a0_prev - r_g0);
		auto r_a5 = MAX(r_a2_prev + r_g1, r_a3_prev - r_g1);
		auto r_a6 = MAX(r_a5_prev + r_g1, r_a4_prev - r_g1);
		auto r_a7 = MAX(r_a6_prev + r_g0, r_a7_prev - r_g0);

		RSC_BCJR_inter_fast_normalize<R>::apply(r_a0, r_a1, r_a2, r_a3, r_a4, r_a5, r_a6, r_a7, i);

		r_a0.store(&this->alpha[0][i]);
		r_a1.store(&this->alpha[1][i]);
		r_a2.store(&this->alpha[2][i]);
		r_a3.store(&this->alpha[3][i]);
		r_a4.store(&this->alpha[4][i]);
		r_a5.store(&this->alpha[5][i]);
		r_a6.store(&this->alpha[6][i]);
		r_a7.store(&this->alpha[7][i]);
	}
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_inter_fast<B,R,MAX>
::compute_beta()
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute beta values [trellis backward traversal <-]
	for (auto i = (this->K +2) * stride; i >= stride; i -= stride)
	{
		const auto r_g0 = mipp::Reg<R>(&this->gamma[0][i]);
		const auto r_g1 = mipp::Reg<R>(&this->gamma[1][i]);

		const auto r_b0_prev = mipp::Reg<R>(&this->beta[0][i +stride]);
		const auto r_b1_prev = mipp::Reg<R>(&this->beta[1][i +stride]);
		const auto r_b2_prev = mipp::Reg<R>(&this->beta[2][i +stride]);
		const auto r_b3_prev = mipp::Reg<R>(&this->beta[3][i +stride]);
		const auto r_b4_prev = mipp::Reg<R>(&this->beta[4][i +stride]);
		const auto r_b5_prev = mipp::Reg<R>(&this->beta[5][i +stride]);
		const auto r_b6_prev = mipp::Reg<R>(&this->beta[6][i +stride]);
		const auto r_b7_prev = mipp::Reg<R>(&this->beta[7][i +stride]);

		auto r_b0 = MAX(r_b0_prev + r_g0, r_b4_prev - r_g0);
		auto r_b1 = MAX(r_b4_prev + r_g0, r_b0_prev - r_g0);
		auto r_b2 = MAX(r_b5_prev + r_g1, r_b1_prev - r_g1);
		auto r_b3 = MAX(r_b1_prev + r_g1, r_b5_prev - r_g1);
		auto r_b4 = MAX(r_b2_prev + r_g1, r_b6_prev - r_g1);
		auto r_b5 = MAX(r_b6_prev + r_g1, r_b2_prev - r_g1);
		auto r_b6 = MAX(r_b7_prev + r_g0, r_b3_prev - r_g0);
		auto r_b7 = MAX(r_b3_prev + r_g0, r_b7_prev - r_g0);

		RSC_BCJR_inter_fast_normalize<R>::apply(r_b0, r_b1, r_b2, r_b3, r_b4, r_b5, r_b6, r_b7, i);

		r_b0.store(&this->beta[0][i]);
		r_b1.store(&this->beta[1][i]);
		r_b2.store(&this->beta[2][i]);
		r_b3.store(&this->beta[3][i]);
		r_b4.store(&this->beta[4][i]);
		r_b5.store(&this->beta[5][i]);
		r_b6.store(&this->beta[6][i]);
		r_b7.store(&this->beta[7][i]);
	}
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_inter_fast<B,R,MAX>
::compute_ext(const R *sys, R *ext)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute extrinsic values
	for (auto i = 0; i < this->K * stride; i += stride)
	{
		auto r_max0 = mipp::Reg<R>(std::numeric_limits<R>::min());
		auto r_max1 = mipp::Reg<R>(std::numeric_limits<R>::min());

		const auto r_a0 = mipp::Reg<R>(&this->alpha[0][i]);
		const auto r_a1 = mipp::Reg<R>(&this->alpha[1][i]);
		const auto r_a2 = mipp::Reg<R>(&this->alpha[2][i]);
		const auto r_a3 = mipp::Reg<R>(&this->alpha[3][i]);
		const auto r_a4 = mipp::Reg<R>(&this->alpha[4][i]);
		const auto r_a5 = mipp::Reg<R>(&this->alpha[5][i]);
		const auto r_a6 = mipp::Reg<R>(&this->alpha[6][i]);
		const auto r_a7 = mipp::Reg<R>(&this->alpha[7][i]);

		const auto r_b0 = mipp::Reg<R>(&this->beta[0][i +stride]);
		const auto r_b1 = mipp::Reg<R>(&this->beta[1][i +stride]);
		const auto r_b2 = mipp::Reg<R>(&this->beta[2][i +stride]);
		const auto r_b3 = mipp::Reg<R>(&this->beta[3][i +stride]);
		const auto r_b4 = mipp::Reg<R>(&this->beta[4][i +stride]);
		const auto r_b5 = mipp::Reg<R>(&this->beta[5][i +stride]);
		const auto r_b6 = mipp::Reg<R>(&this->beta[6][i +stride]);
		const auto r_b7 = mipp::Reg<R>(&this->beta[7][i +stride]);

		const auto r_g0 = mipp::Reg<R>(&this->gamma[0][i]);
		const auto r_g1 = mipp::Reg<R>(&this->gamma[1][i]);

		auto r_sum0 = r_a0 + r_b0 + r_g0; r_max0 = MAX(r_max0, r_sum0);
		auto r_sum1 = r_a1 + r_b4 + r_g0; r_max0 = MAX(r_max0, r_sum1);
		auto r_sum2 = r_a2 + r_b5 + r_g1; r_max0 = MAX(r_max0, r_sum2);
		auto r_sum3 = r_a3 + r_b1 + r_g1; r_max0 = MAX(r_max0, r_sum3);
		auto r_sum4 = r_a4 + r_b2 + r_g1; r_max0 = MAX(r_max0, r_sum4);
		auto r_sum5 = r_a5 + r_b6 + r_g1; r_max0 = MAX(r_max0, r_sum5);
		auto r_sum6 = r_a6 + r_b7 + r_g0; r_max0 = MAX(r_max0, r_sum6);
		auto r_sum7 = r_a7 + r_b3 + r_g0; r_max0 = MAX(r_max0, r_sum7);

		     r_sum0 = r_a0 + r_b4 - r_g0; r_max1 = MAX(r_max1, r_sum0);
		     r_sum1 = r_a1 + r_b0 - r_g0; r_max1 = MAX(r_max1, r_sum1);
		     r_sum2 = r_a2 + r_b1 - r_g1; r_max1 = MAX(r_max1, r_sum2);
		     r_sum3 = r_a3 + r_b5 - r_g1; r_max1 = MAX(r_max1, r_sum3);
		     r_sum4 = r_a4 + r_b6 - r_g1; r_max1 = MAX(r_max1, r_sum4);
		     r_sum5 = r_a5 + r_b2 - r_g1; r_max1 = MAX(r_max1, r_sum5);
		     r_sum6 = r_a6 + r_b3 - r_g0; r_max1 = MAX(r_max1, r_sum6);
		     r_sum7 = r_a7 + r_b7 - r_g0; r_max1 = MAX(r_max1, r_sum7);

		const auto r_post = RSC_BCJR_inter_post<R>::compute(r_max0 - r_max1);
		const auto r_ext  = r_post - &sys[i];
		r_ext.store(&ext[i]);
	}
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
int Decoder_RSC_BCJR_inter_fast<B,R,MAX>
::_decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	if (!mipp::isAligned(sys))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'sys' is misaligned memory.");

	if (!mipp::isAligned(par))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'par' is misaligned memory.");

	if (!mipp::isAligned(ext))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'ext' is misaligned memory.");

	this->compute_gamma(sys, par);
	this->compute_alpha(        );
	this->compute_beta (        );
	this->compute_ext  (sys, ext);

	return 0;
}
}
}
