#include "Decoder_RSC_BCJR_inter_std.hpp"

#include "../../../../Tools/MIPP/mipp.h"

template <typename B, typename R, proto_map_i<R> MAP>
Decoder_RSC_BCJR_inter_std<B,R,MAP>
::Decoder_RSC_BCJR_inter_std(const int &K, const bool buffered_encoding)
: Decoder_RSC_BCJR_inter<B,R>(K, buffered_encoding)
{
}

template <typename B, typename R, proto_map_i<R> MAP>
Decoder_RSC_BCJR_inter_std<B,R,MAP>
::~Decoder_RSC_BCJR_inter_std()
{
}

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_std<B,R,MAP>
::compute_gamma(const mipp::vector<R> &sys, const mipp::vector<R> &par)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute gamma values (auto-vectorized loop)
	for (auto i = 0; i < (this->K +3) * stride; i += stride)
	{
		const auto r_sys = mipp::load<R>(&sys[i]);
		const auto r_par = mipp::load<R>(&par[i]);

		// there is a big loss of precision here in fixed point
		const auto r_g0 = RSC_BCJR_inter_div_or_not<R>::apply(mipp::add<R>(r_sys, r_par));
		// there is a big loss of precision here in fixed point
		const auto r_g1 = RSC_BCJR_inter_div_or_not<R>::apply(mipp::sub<R>(r_sys, r_par));

		mipp::store<R>(&this->gamma[0][i], r_g0);
		mipp::store<R>(&this->gamma[1][i], r_g1);
	}
}

template <typename R>
struct RSC_BCJR_inter_std_normalize
{
	static void apply(mipp::vector<R> metrics[8], const int &i)
	{
		// no need to do something
	}
};

template <>
struct RSC_BCJR_inter_std_normalize <short>
{
	static void apply(mipp::vector<short> metrics[8], const int &i)
	{
		constexpr auto stride = mipp::nElmtsPerRegister<short>();
		// normalization
		if ((i / stride) % 8 == 0)
		{
			const auto r_norm_val = mipp::load<short>(&metrics[0][i]);
			for (auto j = 0; j < 8; j++)
			{
				const auto r_a = mipp::load<short>(&metrics[j][i]);
				mipp::store<short>(&metrics[j][i], mipp::sub<short>(r_a, r_norm_val));
			}
		}
	}
};

template <>
struct RSC_BCJR_inter_std_normalize <signed char>
{
	static void apply(mipp::vector<signed char> metrics[8], const int &i)
	{
		// normalization
		const auto r_norm_val = mipp::load<signed char>(&metrics[0][i]);
		for (auto j = 0; j < 8; j++)
		{
			const auto r_a = mipp::load<signed char>(&metrics[j][i]);
			mipp::store<signed char>(&metrics[j][i], mipp::sub<signed char>(r_a, r_norm_val));
		}
	}
};

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_std<B,R,MAP>
::compute_alpha()
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute alpha values [trellis forward traversal ->]
	constexpr int idx_a1[8] = {0, 3, 4, 7, 1, 2, 5, 6};
	constexpr int idx_a2[8] = {1, 2, 5, 6, 0, 3, 4, 7};
	constexpr int idx_g1[8] = {0, 1, 1, 0, 0, 1, 1, 0};
	for (auto i = stride; i < (this->K +3) * stride; i += stride)
	{
		for (auto j = 0; j < 8; j++)
		{
			const auto r_g1 = mipp::load<R>(&this->gamma[idx_g1[j]][i -stride]);
			const auto r_a1 = mipp::load<R>(&this->alpha[idx_a1[j]][i -stride]);
			const auto r_a2 = mipp::load<R>(&this->alpha[idx_a2[j]][i -stride]);

			const auto r_a3 = MAP(mipp::add<R>(r_a1, r_g1), mipp::sub<R>(r_a2, r_g1));

			mipp::store<R>(&this->alpha[j][i], r_a3);
		}

		RSC_BCJR_inter_std_normalize<R>::apply(this->alpha, i);
	}
}

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_std<B,R,MAP>
::compute_beta()
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute beta values [trellis backward traversal <-]
	constexpr int idx_b1[8] = {0, 4, 5, 1, 2, 6, 7, 3};
	constexpr int idx_b2[8] = {4, 0, 1, 5, 6, 2, 3, 7};
	constexpr int idx_g2[8] = {0, 0, 1, 1, 1, 1, 0, 0};
	for (auto i = (this->K +2) * stride; i >= stride; i -= stride)
	{
		for (auto j = 0; j < 8; j++)
		{
			const auto r_g2 = mipp::load<R>(&this->gamma[idx_g2[j]][i        ]);
			const auto r_b1 = mipp::load<R>(&this->beta [idx_b1[j]][i +stride]);
			const auto r_b2 = mipp::load<R>(&this->beta [idx_b2[j]][i +stride]);

			const auto r_b3 = MAP(mipp::add<R>(r_b1, r_g2), mipp::sub<R>(r_b2, r_g2));

			mipp::store<R>(&this->beta[j][i], r_b3);
		}

		RSC_BCJR_inter_std_normalize<R>::apply(this->beta, i);
	}
}

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_std<B,R,MAP>
::compute_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute extrinsic values
	constexpr int idx_b1[8] = {0, 4, 5, 1, 2, 6, 7, 3};
	constexpr int idx_b2[8] = {4, 0, 1, 5, 6, 2, 3, 7};
	constexpr int idx_g2[8] = {0, 0, 1, 1, 1, 1, 0, 0};
	for (auto i = 0; i < this->K * stride; i += stride)
	{
		auto r_a    = mipp::load<R>(&this->alpha[       0 ][i        ]);
		auto r_b    = mipp::load<R>(&this->beta [idx_b1[0]][i +stride]);
		auto r_g    = mipp::load<R>(&this->gamma[idx_g2[0]][i        ]);
		auto r_sum  = mipp::add <R>(r_a,   r_b);
		auto r_max0 = mipp::add <R>(r_sum, r_g);
		for (auto j = 1; j < 8; j++)
		{
			r_a = mipp::load<R>(&this->alpha[       j ][i        ]);
			r_b = mipp::load<R>(&this->beta [idx_b1[j]][i +stride]);
			r_g = mipp::load<R>(&this->gamma[idx_g2[j]][i        ]);

			r_sum = mipp::add<R>(r_a,   r_b);
			r_sum = mipp::add<R>(r_sum, r_g);

			r_max0 = MAP(r_max0, r_sum);
		}

		r_a         = mipp::load<R>(&this->alpha[       0 ][i        ]);
		r_b         = mipp::load<R>(&this->beta [idx_b2[0]][i +stride]);
		r_g         = mipp::load<R>(&this->gamma[idx_g2[0]][i        ]);
		r_sum       = mipp::add <R>(r_a,   r_b);
		auto r_max1 = mipp::sub <R>(r_sum, r_g);
		for (auto j = 1; j < 8; j++)
		{
			r_a = mipp::load<R>(&this->alpha[       j ][i        ]);
			r_b = mipp::load<R>(&this->beta [idx_b2[j]][i +stride]);
			r_g = mipp::load<R>(&this->gamma[idx_g2[j]][i        ]);

			r_sum = mipp::add<R>(r_a,   r_b);
			r_sum = mipp::sub<R>(r_sum, r_g);

			r_max1 = MAP(r_max1, r_sum);
		}

		const auto r_post = RSC_BCJR_inter_post<R>::compute(mipp::sub<R>(r_max0, r_max1));
		const auto r_sys  = mipp::load<R>(&sys[i]);
		mipp::store<R>(&ext[i], mipp::sub<R>(r_post, r_sys));
	}
}

template <typename B, typename R, proto_map_i<R> MAP>
void Decoder_RSC_BCJR_inter_std<B,R,MAP>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	this->compute_gamma(sys, par);
	this->compute_alpha(        );
	this->compute_beta (        );
	this->compute_ext  (sys, ext);
}