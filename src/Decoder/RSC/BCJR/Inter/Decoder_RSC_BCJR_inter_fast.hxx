#include "Decoder_RSC_BCJR_inter_fast.hpp"

#include "../../../../Tools/MIPP/mipp.h"

template <typename B, typename R, proto_map_i<R> MAP>
Decoder_RSC_BCJR_inter_fast<B,R,MAP>
::Decoder_RSC_BCJR_inter_fast(const int &K, const bool buffered_encoding)
: Decoder_RSC_BCJR_inter<B,R>(K, buffered_encoding)
{
}

template <typename B, typename R, proto_map_i<R> MAP>
Decoder_RSC_BCJR_inter_fast<B,R,MAP>
::~Decoder_RSC_BCJR_inter_fast()
{
}

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_fast<B,R,MAP>
::compute_gamma(const mipp::vector<R> &sys, const mipp::vector<R> &par)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute gamma values
	for (auto i = 0; i < (this->K +3) * stride; i += stride)
	{
		const auto r_sys = mipp::load<R>(&sys[i]);
		const auto r_par = mipp::load<R>(&par[i]);

		const auto r_g0 = RSC_BCJR_inter_div_or_not<R>::apply(mipp::add<R>(r_sys, r_par));
		const auto r_g1 = RSC_BCJR_inter_div_or_not<R>::apply(mipp::sub<R>(r_sys, r_par));

		mipp::store<R>(&this->gamma[0][i], r_g0);
		mipp::store<R>(&this->gamma[1][i], r_g1);
	}
}

template <typename R>
struct RSC_BCJR_inter_fast_normalize
{
	static void apply(mipp::reg &r_ab0, mipp::reg &r_ab1, mipp::reg &r_ab2, mipp::reg &r_ab3, 
	                  mipp::reg &r_ab4, mipp::reg &r_ab5, mipp::reg &r_ab6, mipp::reg &r_ab7,
	                  const int &i)
	{
		// no need to normalize
	}
};

template <>
struct RSC_BCJR_inter_fast_normalize <short>
{
	static void apply(mipp::reg &r_ab0, mipp::reg &r_ab1, mipp::reg &r_ab2, mipp::reg &r_ab3, 
	                  mipp::reg &r_ab4, mipp::reg &r_ab5, mipp::reg &r_ab6, mipp::reg &r_ab7,
	                  const int &i)
	{
		constexpr auto stride = mipp::nElmtsPerRegister<short>();

		// normalization
		if ((i / stride) % 8 == 0)
		{
			const auto r_norm_val = r_ab0;
			r_ab0 = mipp::sub<short>(r_ab0, r_norm_val);
			r_ab1 = mipp::sub<short>(r_ab1, r_norm_val);
			r_ab2 = mipp::sub<short>(r_ab2, r_norm_val);
			r_ab3 = mipp::sub<short>(r_ab3, r_norm_val);
			r_ab4 = mipp::sub<short>(r_ab4, r_norm_val);
			r_ab5 = mipp::sub<short>(r_ab5, r_norm_val);
			r_ab6 = mipp::sub<short>(r_ab6, r_norm_val);
			r_ab7 = mipp::sub<short>(r_ab7, r_norm_val);
		}
	}
};

template <>
struct RSC_BCJR_inter_fast_normalize <signed char>
{
	static void apply(mipp::reg &r_ab0, mipp::reg &r_ab1, mipp::reg &r_ab2, mipp::reg &r_ab3, 
	                  mipp::reg &r_ab4, mipp::reg &r_ab5, mipp::reg &r_ab6, mipp::reg &r_ab7,
	                  const int &i)
	{
		// normalization
		const auto r_norm_val = r_ab0;
		r_ab0 = mipp::sub<signed char>(r_ab0, r_norm_val);
		r_ab1 = mipp::sub<signed char>(r_ab1, r_norm_val);
		r_ab2 = mipp::sub<signed char>(r_ab2, r_norm_val);
		r_ab3 = mipp::sub<signed char>(r_ab3, r_norm_val);
		r_ab4 = mipp::sub<signed char>(r_ab4, r_norm_val);
		r_ab5 = mipp::sub<signed char>(r_ab5, r_norm_val);
		r_ab6 = mipp::sub<signed char>(r_ab6, r_norm_val);
		r_ab7 = mipp::sub<signed char>(r_ab7, r_norm_val);
	}
};

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_fast<B,R,MAP>
::compute_alpha()
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute alpha values [trellis forward traversal ->]
	for (auto i = stride; i < (this->K +3) * stride; i += stride)
	{
		const auto r_g0 = mipp::load<R>(&this->gamma[0][i -stride]);
		const auto r_g1 = mipp::load<R>(&this->gamma[1][i -stride]);

		const auto r_a0_prev = mipp::load<R>(&this->alpha[0][i -stride]);
		const auto r_a1_prev = mipp::load<R>(&this->alpha[1][i -stride]);
		const auto r_a2_prev = mipp::load<R>(&this->alpha[2][i -stride]);
		const auto r_a3_prev = mipp::load<R>(&this->alpha[3][i -stride]);
		const auto r_a4_prev = mipp::load<R>(&this->alpha[4][i -stride]);
		const auto r_a5_prev = mipp::load<R>(&this->alpha[5][i -stride]);
		const auto r_a6_prev = mipp::load<R>(&this->alpha[6][i -stride]);
		const auto r_a7_prev = mipp::load<R>(&this->alpha[7][i -stride]);

		auto r_a0 = MAP(mipp::add<R>(r_a0_prev, r_g0), mipp::sub<R>(r_a1_prev, r_g0));
		auto r_a1 = MAP(mipp::add<R>(r_a3_prev, r_g1), mipp::sub<R>(r_a2_prev, r_g1));
		auto r_a2 = MAP(mipp::add<R>(r_a4_prev, r_g1), mipp::sub<R>(r_a5_prev, r_g1));
		auto r_a3 = MAP(mipp::add<R>(r_a7_prev, r_g0), mipp::sub<R>(r_a6_prev, r_g0));
		auto r_a4 = MAP(mipp::add<R>(r_a1_prev, r_g0), mipp::sub<R>(r_a0_prev, r_g0));
		auto r_a5 = MAP(mipp::add<R>(r_a2_prev, r_g1), mipp::sub<R>(r_a3_prev, r_g1));
		auto r_a6 = MAP(mipp::add<R>(r_a5_prev, r_g1), mipp::sub<R>(r_a4_prev, r_g1));
		auto r_a7 = MAP(mipp::add<R>(r_a6_prev, r_g0), mipp::sub<R>(r_a7_prev, r_g0));

		RSC_BCJR_inter_fast_normalize<R>::apply(r_a0, r_a1, r_a2, r_a3, r_a4, r_a5, r_a6, r_a7, i);

		mipp::store<R>(&this->alpha[0][i], r_a0);
		mipp::store<R>(&this->alpha[1][i], r_a1);
		mipp::store<R>(&this->alpha[2][i], r_a2);
		mipp::store<R>(&this->alpha[3][i], r_a3);
		mipp::store<R>(&this->alpha[4][i], r_a4);
		mipp::store<R>(&this->alpha[5][i], r_a5);
		mipp::store<R>(&this->alpha[6][i], r_a6);
		mipp::store<R>(&this->alpha[7][i], r_a7);
	}
}

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_fast<B,R,MAP>
::compute_beta()
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute beta values [trellis backward traversal <-]
	for (auto i = (this->K +2) * stride; i >= stride; i -= stride)
	{
		const auto r_g0 = mipp::load<R>(&this->gamma[0][i]);
		const auto r_g1 = mipp::load<R>(&this->gamma[1][i]);

		const auto r_b0_prev = mipp::load<R>(&this->beta[0][i +stride]);
		const auto r_b1_prev = mipp::load<R>(&this->beta[1][i +stride]);
		const auto r_b2_prev = mipp::load<R>(&this->beta[2][i +stride]);
		const auto r_b3_prev = mipp::load<R>(&this->beta[3][i +stride]);
		const auto r_b4_prev = mipp::load<R>(&this->beta[4][i +stride]);
		const auto r_b5_prev = mipp::load<R>(&this->beta[5][i +stride]);
		const auto r_b6_prev = mipp::load<R>(&this->beta[6][i +stride]);
		const auto r_b7_prev = mipp::load<R>(&this->beta[7][i +stride]);

		auto r_b0 = MAP(mipp::add<R>(r_b0_prev, r_g0), mipp::sub<R>(r_b4_prev, r_g0));
		auto r_b1 = MAP(mipp::add<R>(r_b4_prev, r_g0), mipp::sub<R>(r_b0_prev, r_g0));
		auto r_b2 = MAP(mipp::add<R>(r_b5_prev, r_g1), mipp::sub<R>(r_b1_prev, r_g1));
		auto r_b3 = MAP(mipp::add<R>(r_b1_prev, r_g1), mipp::sub<R>(r_b5_prev, r_g1));
		auto r_b4 = MAP(mipp::add<R>(r_b2_prev, r_g1), mipp::sub<R>(r_b6_prev, r_g1));
		auto r_b5 = MAP(mipp::add<R>(r_b6_prev, r_g1), mipp::sub<R>(r_b2_prev, r_g1));
		auto r_b6 = MAP(mipp::add<R>(r_b7_prev, r_g0), mipp::sub<R>(r_b3_prev, r_g0));
		auto r_b7 = MAP(mipp::add<R>(r_b3_prev, r_g0), mipp::sub<R>(r_b7_prev, r_g0));

		RSC_BCJR_inter_fast_normalize<R>::apply(r_b0, r_b1, r_b2, r_b3, r_b4, r_b5, r_b6, r_b7, i);

		mipp::store<R>(&this->beta[0][i], r_b0);
		mipp::store<R>(&this->beta[1][i], r_b1);
		mipp::store<R>(&this->beta[2][i], r_b2);
		mipp::store<R>(&this->beta[3][i], r_b3);
		mipp::store<R>(&this->beta[4][i], r_b4);
		mipp::store<R>(&this->beta[5][i], r_b5);
		mipp::store<R>(&this->beta[6][i], r_b6);
		mipp::store<R>(&this->beta[7][i], r_b7);
	}
}

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_fast<B,R,MAP>
::compute_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute extrinsic values
	for (auto i = 0; i < this->K * stride; i += stride)
	{
		auto r_max0 = mipp::set1<R>(std::numeric_limits<R>::min());
		auto r_max1 = mipp::set1<R>(std::numeric_limits<R>::min());

		const auto r_a0 = mipp::load<R>(&this->alpha[0][i]);
		const auto r_a1 = mipp::load<R>(&this->alpha[1][i]);
		const auto r_a2 = mipp::load<R>(&this->alpha[2][i]);
		const auto r_a3 = mipp::load<R>(&this->alpha[3][i]);
		const auto r_a4 = mipp::load<R>(&this->alpha[4][i]);
		const auto r_a5 = mipp::load<R>(&this->alpha[5][i]);
		const auto r_a6 = mipp::load<R>(&this->alpha[6][i]);
		const auto r_a7 = mipp::load<R>(&this->alpha[7][i]);

		const auto r_b0 = mipp::load<R>(&this->beta[0][i +stride]);
		const auto r_b1 = mipp::load<R>(&this->beta[1][i +stride]);
		const auto r_b2 = mipp::load<R>(&this->beta[2][i +stride]);
		const auto r_b3 = mipp::load<R>(&this->beta[3][i +stride]);
		const auto r_b4 = mipp::load<R>(&this->beta[4][i +stride]);
		const auto r_b5 = mipp::load<R>(&this->beta[5][i +stride]);
		const auto r_b6 = mipp::load<R>(&this->beta[6][i +stride]);
		const auto r_b7 = mipp::load<R>(&this->beta[7][i +stride]);

		const auto r_g0 = mipp::load<R>(&this->gamma[0][i]);
		const auto r_g1 = mipp::load<R>(&this->gamma[1][i]);

		auto r_sum0 = mipp::add<R>(mipp::add<R>(r_a0, r_b0), r_g0); r_max0 = MAP(r_max0, r_sum0);
		auto r_sum1 = mipp::add<R>(mipp::add<R>(r_a1, r_b4), r_g0); r_max0 = MAP(r_max0, r_sum1);
		auto r_sum2 = mipp::add<R>(mipp::add<R>(r_a2, r_b5), r_g1); r_max0 = MAP(r_max0, r_sum2);
		auto r_sum3 = mipp::add<R>(mipp::add<R>(r_a3, r_b1), r_g1); r_max0 = MAP(r_max0, r_sum3);
		auto r_sum4 = mipp::add<R>(mipp::add<R>(r_a4, r_b2), r_g1); r_max0 = MAP(r_max0, r_sum4);
		auto r_sum5 = mipp::add<R>(mipp::add<R>(r_a5, r_b6), r_g1); r_max0 = MAP(r_max0, r_sum5);
		auto r_sum6 = mipp::add<R>(mipp::add<R>(r_a6, r_b7), r_g0); r_max0 = MAP(r_max0, r_sum6);
		auto r_sum7 = mipp::add<R>(mipp::add<R>(r_a7, r_b3), r_g0); r_max0 = MAP(r_max0, r_sum7);

		     r_sum0 = mipp::sub<R>(mipp::add<R>(r_a0, r_b4), r_g0); r_max1 = MAP(r_max1, r_sum0);
		     r_sum1 = mipp::sub<R>(mipp::add<R>(r_a1, r_b0), r_g0); r_max1 = MAP(r_max1, r_sum1);
		     r_sum2 = mipp::sub<R>(mipp::add<R>(r_a2, r_b1), r_g1); r_max1 = MAP(r_max1, r_sum2);
		     r_sum3 = mipp::sub<R>(mipp::add<R>(r_a3, r_b5), r_g1); r_max1 = MAP(r_max1, r_sum3);
		     r_sum4 = mipp::sub<R>(mipp::add<R>(r_a4, r_b6), r_g1); r_max1 = MAP(r_max1, r_sum4);
		     r_sum5 = mipp::sub<R>(mipp::add<R>(r_a5, r_b2), r_g1); r_max1 = MAP(r_max1, r_sum5);
		     r_sum6 = mipp::sub<R>(mipp::add<R>(r_a6, r_b3), r_g0); r_max1 = MAP(r_max1, r_sum6);
		     r_sum7 = mipp::sub<R>(mipp::add<R>(r_a7, r_b7), r_g0); r_max1 = MAP(r_max1, r_sum7);

		const auto r_post = RSC_BCJR_inter_post<R>::compute(mipp::sub<R>(r_max0, r_max1));
		const auto r_sys  = mipp::load<R>(&sys[i]);
		mipp::store<R>(&ext[i], mipp::sub<R>(r_post, r_sys));
	}
}

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_fast<B,R,MAP>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	this->compute_gamma(sys, par);
	this->compute_alpha(        );
	this->compute_beta (        );
	this->compute_ext  (sys, ext);
}
