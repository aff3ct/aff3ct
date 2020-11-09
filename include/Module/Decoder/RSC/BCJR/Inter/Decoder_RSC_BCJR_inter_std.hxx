#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <string>
#include <cmath>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_std.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_max_i<R> MAX>
Decoder_RSC_BCJR_inter_std<B,R,MAX>
::Decoder_RSC_BCJR_inter_std(const int &K,
                             const std::vector<std::vector<int>> &trellis,
                             const bool buffered_encoding)
: Decoder_RSC_BCJR_inter<B,R>(K, trellis, buffered_encoding)
{
	const std::string name = "Decoder_RSC_BCJR_inter_std";
	this->set_name(name);
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
Decoder_RSC_BCJR_inter_std<B,R,MAX>* Decoder_RSC_BCJR_inter_std<B,R,MAX>
::clone() const
{
	auto m = new Decoder_RSC_BCJR_inter_std(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_inter_std<B,R,MAX>
::compute_gamma(const R *sys, const R *par)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	for (auto i = 0; i < (this->K +3) * stride; i += stride)
	{
		const auto r_sys = mipp::Reg<R>(&sys[i]);
		const auto r_par = mipp::Reg<R>(&par[i]);

		// there is a big loss of precision here in fixed point
		const auto r_g0 = RSC_BCJR_inter_div_or_not<R>::apply(r_sys + r_par);
		const auto r_g1 = RSC_BCJR_inter_div_or_not<R>::apply(r_sys - r_par);

		r_g0.store(&this->gamma[0][i]);
		r_g1.store(&this->gamma[1][i]);
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
			const auto r_norm_val = mipp::Reg<short>(&metrics[0][i]);
			for (auto j = 0; j < 8; j++)
			{
				auto r_a = mipp::Reg<short>(&metrics[j][i]);
				r_a -= r_norm_val;
				r_a.store(&metrics[j][i]);
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
		const auto r_norm_val = mipp::Reg<signed char>(&metrics[0][i]);
		for (auto j = 0; j < 8; j++)
		{
			auto r_a = mipp::Reg<signed char>(&metrics[j][i]);
			r_a -= r_norm_val;
			r_a.store(&metrics[j][i]);
		}
	}
};

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_inter_std<B,R,MAX>
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
			const auto r_g1 = mipp::Reg<R>(&this->gamma[idx_g1[j]][i -stride]);
			const auto r_a1 = mipp::Reg<R>(&this->alpha[idx_a1[j]][i -stride]);
			const auto r_a2 = mipp::Reg<R>(&this->alpha[idx_a2[j]][i -stride]);

			const auto r_a3 = MAX(r_a1 + r_g1, r_a2 - r_g1);

			r_a3.store(&this->alpha[j][i]);
		}

		RSC_BCJR_inter_std_normalize<R>::apply(this->alpha, i);
	}
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_inter_std<B,R,MAX>
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
			const auto r_g2 = mipp::Reg<R>(&this->gamma[idx_g2[j]][i        ]);
			const auto r_b1 = mipp::Reg<R>(&this->beta [idx_b1[j]][i +stride]);
			const auto r_b2 = mipp::Reg<R>(&this->beta [idx_b2[j]][i +stride]);

			const auto r_b3 = MAX(r_b1 + r_g2, r_b2 - r_g2);

			r_b3.store(&this->beta[j][i]);
		}

		RSC_BCJR_inter_std_normalize<R>::apply(this->beta, i);
	}
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_inter_std<B,R,MAX>
::compute_ext(const R *sys, R *ext)
{
	constexpr auto stride = mipp::nElmtsPerRegister<R>();

	// compute extrinsic values
	constexpr int idx_b1[8] = {0, 4, 5, 1, 2, 6, 7, 3};
	constexpr int idx_b2[8] = {4, 0, 1, 5, 6, 2, 3, 7};
	constexpr int idx_g2[8] = {0, 0, 1, 1, 1, 1, 0, 0};
	for (auto i = 0; i < this->K * stride; i += stride)
	{
		auto r_a    = mipp::Reg<R>(&this->alpha[       0 ][i        ]);
		auto r_b    = mipp::Reg<R>(&this->beta [idx_b1[0]][i +stride]);
		auto r_g    = mipp::Reg<R>(&this->gamma[idx_g2[0]][i        ]);
		auto r_max0 = r_a + r_b + r_g;
		for (auto j = 1; j < 8; j++)
		{
			r_a = mipp::Reg<R>(&this->alpha[       j ][i        ]);
			r_b = mipp::Reg<R>(&this->beta [idx_b1[j]][i +stride]);
			r_g = mipp::Reg<R>(&this->gamma[idx_g2[j]][i        ]);

			auto r_sum = r_a + r_b + r_g;
			r_max0 = MAX(r_max0, r_sum);
		}

		r_a         = mipp::Reg<R>(&this->alpha[       0 ][i        ]);
		r_b         = mipp::Reg<R>(&this->beta [idx_b2[0]][i +stride]);
		r_g         = mipp::Reg<R>(&this->gamma[idx_g2[0]][i        ]);
		auto r_max1 = r_a + r_b - r_g;
		for (auto j = 1; j < 8; j++)
		{
			r_a = mipp::Reg<R>(&this->alpha[       j ][i        ]);
			r_b = mipp::Reg<R>(&this->beta [idx_b2[j]][i +stride]);
			r_g = mipp::Reg<R>(&this->gamma[idx_g2[j]][i        ]);

			auto r_sum = r_a + r_b - r_g;
			r_max1 = MAX(r_max1, r_sum);
		}

		const auto r_post = RSC_BCJR_inter_post<R>::compute(r_max0 - r_max1);
		const auto r_ext  = r_post - &sys[i];
		r_ext.store(&ext[i]);
	}
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
int Decoder_RSC_BCJR_inter_std<B,R,MAX>
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
