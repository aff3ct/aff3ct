#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>
#include <sstream>
#include "mipp.h"

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_std.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_max_i<R> MAX>
Decoder_RSC_BCJR_intra_std<B,R,MAX>
::Decoder_RSC_BCJR_intra_std(const int &K,
                             const std::vector<std::vector<int>> &trellis,
                             const bool buffered_encoding)
: Decoder_RSC_BCJR_intra<B,R>(K, trellis, buffered_encoding)
{
	const std::string name = "Decoder_RSC_BCJR_intra_std";
	this->set_name(name);

	if (mipp::nElReg<R>() != 8)
	{
		std::stringstream message;
		message << "'mipp::nElReg<R>()' has to be equal to 8 ('mipp::nElReg<R>()' = " << mipp::nElReg<R>() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (K % 8)
	{
		std::stringstream message;
		message << "'K' has to be divisible by 8 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
Decoder_RSC_BCJR_intra_std<B,R,MAX>* Decoder_RSC_BCJR_intra_std<B,R,MAX>
::clone() const
{
	auto m = new Decoder_RSC_BCJR_intra_std(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_intra_std<B,R,MAX>
::compute_gamma(const R *sys, const R *par)
{
	// compute gamma values
	for (auto i = 0; i < this->K +3; i += mipp::nElReg<R>())
	{
		const auto r_sys = mipp::Reg<R>(&sys[i]);
		const auto r_par = mipp::Reg<R>(&par[i]);

		// there is a big loss of precision here in fixed point
		// compute the two required gamma values
		const auto r_g0 = RSC_BCJR_intra_div_or_not<R>::apply(r_sys + r_par);
		const auto r_g1 = RSC_BCJR_intra_div_or_not<R>::apply(r_sys - r_par);

		const auto r_g0g1 = mipp::interleave(r_g0, r_g1);

		r_g0g1.val[0].store(&this->gamma[i*2 + 0*mipp::nElReg<R>()]);
		r_g0g1.val[1].store(&this->gamma[i*2 + 1*mipp::nElReg<R>()]);
	}
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_intra_std<B,R,MAX>
::compute_alpha()
{
	constexpr unsigned cmask_a0  [8] = {0, 3, 4, 7, 1, 2, 5, 6}; // alpha trellis transitions 0.
	constexpr unsigned cmask_a1  [8] = {1, 2, 5, 6, 0, 3, 4, 7}; // alpha trellis transitions 1.
	constexpr unsigned cmask_ga0 [8] = {0, 1, 1, 0, 0, 1, 1, 0}; // mask0 to construct the gamma0/1 vector.
	constexpr unsigned cmask_ga1 [8] = {2, 3, 3, 2, 2, 3, 3, 2}; // mask1 to construct the gamma0/1 vector.
	constexpr unsigned cmask_ga2 [8] = {4, 5, 5, 4, 4, 5, 5, 4}; // mask2 to construct the gamma0/1 vector.
	constexpr unsigned cmask_ga3 [8] = {6, 7, 7, 6, 6, 7, 7, 6}; // mask3 to construct the gamma0/1 vector.
	constexpr unsigned cmask_norm[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // mask to broadcast the first alpha value in the
	                                                             // normalization process.
	const auto r_cmask_a0   = mipp::Reg<R>::cmask(cmask_a0  );
	const auto r_cmask_a1   = mipp::Reg<R>::cmask(cmask_a1  );
	const auto r_cmask_norm = mipp::Reg<R>::cmask(cmask_norm);

	mipp::Reg<R> r_cmask_g[4] = { mipp::Reg<R>::cmask(cmask_ga0),
	                              mipp::Reg<R>::cmask(cmask_ga1),
	                              mipp::Reg<R>::cmask(cmask_ga2),
	                              mipp::Reg<R>::cmask(cmask_ga3) };

	auto r_g4 = mipp::Reg<R>(&this->gamma[0]);
	for (auto i = 1; i < this->K +3; i++)
	{
		// load 4 gamma0 and 4 gamma1
		if ((i -1) % 4 == 0)
			r_g4.load(&this->gamma[(i -1)*2]);

		// compute alpha[0..8] for section i
		const auto r_a_prev = mipp::Reg<R>(&this->alpha[(i -1)*8]);
		const auto r_g      = r_g4.shuff(r_cmask_g[(i -1) % 4]);
		const auto r_a0     = r_a_prev.shuff(r_cmask_a0);
		const auto r_a1     = r_a_prev.shuff(r_cmask_a1);
		      auto r_a      = MAX(r_a0 + r_g, r_a1 - r_g);

		// normalization
		r_a = RSC_BCJR_intra_normalize<R>::apply(r_a, r_cmask_norm, i);

		r_a.store(&this->alpha[i*8]);
	}
}

template <typename B, typename R, tools::proto_max_i<R> MAX>
void Decoder_RSC_BCJR_intra_std<B,R,MAX>
::compute_beta_ext(const R *sys, R *ext)
{
	constexpr unsigned cmask_b0  [8] = {0, 4, 5, 1, 2, 6, 7, 3}; // beta trellis transitions 0.
	constexpr unsigned cmask_b1  [8] = {4, 0, 1, 5, 6, 2, 3, 7}; // beta trellis transitions 1.
	constexpr unsigned cmask_gb0 [8] = {0, 0, 1, 1, 1, 1, 0, 0}; // mask0 to construct the gamma0/1 vector.
	constexpr unsigned cmask_gb1 [8] = {2, 2, 3, 3, 3, 3, 2, 2}; // mask1 to construct the gamma0/1 vector.
	constexpr unsigned cmask_gb2 [8] = {4, 4, 5, 5, 5, 5, 4, 4}; // mask2 to construct the gamma0/1 vector.
	constexpr unsigned cmask_gb3 [8] = {6, 6, 7, 7, 7, 7, 6, 6}; // mask3 to construct the gamma0/1 vector.
	constexpr unsigned cmask_norm[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // mask to broadcast the first alpha value in the
	                                                             // normalization process.
	const auto r_cmask_b0   = mipp::Reg<R>::cmask(cmask_b0  );
	const auto r_cmask_b1   = mipp::Reg<R>::cmask(cmask_b1  );
	const auto r_cmask_norm = mipp::Reg<R>::cmask(cmask_norm);

	mipp::Reg<R> r_cmask_g[4] = { mipp::Reg<R>::cmask(cmask_gb0),
	                              mipp::Reg<R>::cmask(cmask_gb1),
	                              mipp::Reg<R>::cmask(cmask_gb2),
	                              mipp::Reg<R>::cmask(cmask_gb3) };

	// compute the first beta values
	auto r_b = mipp::Reg<R>(&this->alpha[0]);
	auto r_g4_bis = mipp::Reg<R>(&this->gamma[this->K*2]);
	for (unsigned i = (unsigned)(this->K +2); i >= (unsigned)this->K; i--)
	{
		const auto r_b0 = r_b.shuff(r_cmask_b0);
		const auto r_b1 = r_b.shuff(r_cmask_b1);
		const auto r_g  = r_g4_bis.shuff(r_cmask_g[i % 4]);
		           r_b  = MAX(r_b0 + r_g, r_b1 - r_g);
		// normalization
		r_b = RSC_BCJR_intra_normalize<R>::apply(r_b, r_cmask_norm, i);
	}

	const R m1e[8] = {1, 0, 0, 0, 0, 0, 0, 0};
	const auto r_m1e = mipp::toReg<R>(mipp::Reg<R>(1) == mipp::Reg<R>(m1e));

	// compute beta values and the extrinsic values [trellis backward traversal <-] (vectorized)
	auto r_g4 = mipp::Reg<R>((R)0);
	for (auto i = this->K -8; i >= 0; i -= 8)
	{
		auto r_buffer_post = mipp::Reg<R>((R)0);
		for (auto j = 7; j >= 0; j--)
		{
			if ((i+j+0) % 4 == 3)
				r_g4.load(&this->gamma[(i+j-3)*2]);

			// compute the new beta
			const auto r_g  = r_g4.shuff(r_cmask_g[(i+j+0) % 4]);
			const auto r_b0 = r_b.shuff(r_cmask_b0);
			const auto r_b1 = r_b.shuff(r_cmask_b1.r);
			           r_b  = MAX(r_b0 + r_g, r_b1 - r_g);

			// normalization
			r_b = RSC_BCJR_intra_normalize<R>::apply(r_b, r_cmask_norm, i+j);

			const auto r_a    = mipp::Reg<R>(&this->alpha[(i+j+0)*8]);
			const auto r_max0 = mipp::Reduction<R,MAX>::apply(r_a + r_b0 + r_g);
			const auto r_max1 = mipp::Reduction<R,MAX>::apply(r_a + r_b1 - r_g);
			const auto r_post = r_m1e & (r_max0 - r_max1);

			r_buffer_post = r_buffer_post.rrot() ^ r_post;
		}

		// saturate r_buffer_post if the computation are made in 8-bit, do nothing else.
		r_buffer_post = RSC_BCJR_intra_post<R>::compute(r_buffer_post);

		// compute extrinsic values
		const auto r_ext = r_buffer_post - &sys[i];
		r_ext.store(&ext[i]);
	}
}
}
}
