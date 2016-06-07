#include <cassert>

#include "Decoder_RSC_BCJR_intra_std.hpp"

template <typename B, typename R, proto_map_i<R> MAP, proto_hmap_i<R> HMAP>
Decoder_RSC_BCJR_intra_std<B,R,MAP,HMAP>
::Decoder_RSC_BCJR_intra_std(const int &K, const bool buffered_encoding)
: Decoder_RSC_BCJR_intra<B,R>(K, buffered_encoding)
{
	assert(mipp::nElReg<R>() == 8);
	assert(K % 8 == 0);
}

template <typename B, typename R, proto_map_i<R> MAP, proto_hmap_i<R> HMAP>
Decoder_RSC_BCJR_intra_std<B,R,MAP,HMAP>
::~Decoder_RSC_BCJR_intra_std()
{
}

template <typename B, typename R, proto_map_i<R> MAP, proto_hmap_i<R> HMAP>
void Decoder_RSC_BCJR_intra_std<B,R,MAP,HMAP>
::compute_gamma(const mipp::vector<R> &sys, const mipp::vector<R> &par)
{
	// compute gamma values
	for (auto i = 0; i < this->K +3; i += mipp::nElReg<R>())
	{
		const auto r_sys = mipp::load<R>(&sys[i]);
		const auto r_par = mipp::load<R>(&par[i]);

		// there is a big loss of precision here in fixed point
		// compute the two required gamma values
		const auto r_g0 = mipp::div2<R>(mipp::add<R>(r_sys, r_par));
		const auto r_g1 = mipp::div2<R>(mipp::sub<R>(r_sys, r_par));

		const auto r_g0g1 = mipp::interleave<R>(r_g0, r_g1);

		mipp::store<R>(&this->gamma[i*2 + 0*mipp::nElReg<R>()], r_g0g1.val[0]);
		mipp::store<R>(&this->gamma[i*2 + 1*mipp::nElReg<R>()], r_g0g1.val[1]);
	}
}

template <typename B, typename R, proto_map_i<R> MAP, proto_hmap_i<R> HMAP>
void Decoder_RSC_BCJR_intra_std<B,R,MAP,HMAP>
::compute_alpha()
{
	constexpr int cmask_a0  [8] = {0, 3, 4, 7, 1, 2, 5, 6}; // alpha trellis transitions 0.
	constexpr int cmask_a1  [8] = {1, 2, 5, 6, 0, 3, 4, 7}; // alpha trellis transitions 1.
	constexpr int cmask_ga0 [8] = {0, 1, 1, 0, 0, 1, 1, 0}; // mask0 to construct the gamma0/1 vector.
	constexpr int cmask_ga1 [8] = {2, 3, 3, 2, 2, 3, 3, 2}; // mask1 to construct the gamma0/1 vector.
	constexpr int cmask_ga2 [8] = {4, 5, 5, 4, 4, 5, 5, 4}; // mask2 to construct the gamma0/1 vector.
	constexpr int cmask_ga3 [8] = {6, 7, 7, 6, 6, 7, 7, 6}; // mask3 to construct the gamma0/1 vector.
	constexpr int cmask_norm[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // mask to broadcast the first alpha value in the 
	                                                        // normalization process.
	const auto r_cmask_a0   = mipp::cmask<R>(cmask_a0  );
	const auto r_cmask_a1   = mipp::cmask<R>(cmask_a1  );
	const auto r_cmask_g0   = mipp::cmask<R>(cmask_ga0 );
	const auto r_cmask_g1   = mipp::cmask<R>(cmask_ga1 );
	const auto r_cmask_g2   = mipp::cmask<R>(cmask_ga2 );
	const auto r_cmask_g3   = mipp::cmask<R>(cmask_ga3 );
	const auto r_cmask_norm = mipp::cmask<R>(cmask_norm);

	mipp::reg r_g4 = mipp::load<R>(&this->gamma[0]);
	mipp::reg r_cmask_g[4] = {r_cmask_g0, r_cmask_g1, r_cmask_g2, r_cmask_g3};
	for (auto i = 1; i < this->K +3; i++)
	{
		// load 4 gamma0 and 4 gamma1
		if ((i -1) % 4 == 0) 
			r_g4 = mipp::load<R>(&this->gamma[(i -1)*2]);

		// compute alpha[0..8] for section i
		const auto r_a_prev = mipp::load <R>(&this->alpha[(i -1)*8]);
		const auto r_g      = mipp::shuff<R>(r_g4,     r_cmask_g[(i -1) % 4]);
		const auto r_a0     = mipp::shuff<R>(r_a_prev, r_cmask_a0);
		const auto r_a1     = mipp::shuff<R>(r_a_prev, r_cmask_a1);
		      auto r_a      = MAP(mipp::add<R>(r_a0, r_g), 
		                          mipp::sub<R>(r_a1, r_g));

		// normalization
		r_a = RSC_BCJR_intra_normalize<R>::apply(r_a, r_cmask_norm, i);

		mipp::store<R>(&this->alpha[i*8], r_a);
	}
}

template <typename R>
struct RSC_BCJR_intra_std_post
{
	static mipp::reg compute(const mipp::reg &r_post)
	{
		return r_post;
	}
};

template <>
struct RSC_BCJR_intra_std_post <signed char>
{
	static mipp::reg compute(const mipp::reg &r_post)
	{
		return mipp::sat<signed char>(r_post, -63, 63);
	}
};

template <typename B, typename R, proto_map_i<R> MAP, proto_hmap_i<R> HMAP>
void Decoder_RSC_BCJR_intra_std<B,R,MAP,HMAP>
::compute_beta_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext)
{
	constexpr int cmask_b0  [8] = {0, 4, 5, 1, 2, 6, 7, 3}; // beta trellis transitions 0.
	constexpr int cmask_b1  [8] = {4, 0, 1, 5, 6, 2, 3, 7}; // beta trellis transitions 1.
	constexpr int cmask_gb0 [8] = {0, 0, 1, 1, 1, 1, 0, 0}; // mask0 to construct the gamma0/1 vector.
	constexpr int cmask_gb1 [8] = {2, 2, 3, 3, 3, 3, 2, 2}; // mask1 to construct the gamma0/1 vector.
	constexpr int cmask_gb2 [8] = {4, 4, 5, 5, 5, 5, 4, 4}; // mask2 to construct the gamma0/1 vector.
	constexpr int cmask_gb3 [8] = {6, 6, 7, 7, 7, 7, 6, 6}; // mask3 to construct the gamma0/1 vector.
	constexpr int cmask_norm[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // mask to broadcast the first alpha value in the 
	                                                        // normalization process.
	const auto r_cmask_b0   = mipp::cmask<R>(cmask_b0  );
	const auto r_cmask_b1   = mipp::cmask<R>(cmask_b1  );
	const auto r_cmask_norm = mipp::cmask<R>(cmask_norm);
	const auto r_cmask_g0   = mipp::cmask<R>(cmask_gb0 );
	const auto r_cmask_g1   = mipp::cmask<R>(cmask_gb1 );
	const auto r_cmask_g2   = mipp::cmask<R>(cmask_gb2 );
	const auto r_cmask_g3   = mipp::cmask<R>(cmask_gb3 );
	mipp::reg r_cmask_g[4] = {r_cmask_g0, r_cmask_g1, r_cmask_g2, r_cmask_g3};

	// compute the first beta values
	auto r_b = mipp::load<R>(&this->alpha[0]);
	auto r_g4_bis = mipp::load<R>(&this->gamma[this->K*2]);
	for (unsigned i = (unsigned)(this->K +2); i >= (unsigned)this->K; i--)
	{
		const auto r_b0 = mipp::shuff<R>(r_b, r_cmask_b0);
		const auto r_b1 = mipp::shuff<R>(r_b, r_cmask_b1);
		const auto r_g  = mipp::shuff<R>(r_g4_bis, r_cmask_g[i % 4]);
		           r_b  = MAP(mipp::add<R>(r_b0, r_g),
		                      mipp::sub<R>(r_b1, r_g));
		// normalization
		r_b = RSC_BCJR_intra_normalize<R>::apply(r_b, r_cmask_norm, i);
	}

	const R m1e[8] = {1, 0, 0, 0, 0, 0, 0, 0};
	const auto r_m1e = mipp::cmpeq<R>(mipp::set1<R>(1), mipp::set<R>(m1e));

	// compute beta values and the extrinsic values [trellis backward traversal <-] (vectorized)
	auto r_g4 = mipp::set0<R>();
	for (auto i = this->K -8; i >= 0; i -= 8)
	{
		auto r_buffer_post = mipp::set0<R>();
		for (auto j = 7; j >= 0; j--)
		{
			if ((i+j+0) % 4 == 3)
				r_g4 = mipp::load<R>(&this->gamma[(i+j-3)*2]);

			// compute the new beta
			const auto r_g  = mipp::shuff<R>(r_g4, r_cmask_g[(i+j+0) % 4]);
			const auto r_b0 = mipp::shuff<R>(r_b, r_cmask_b0);
			const auto r_b1 = mipp::shuff<R>(r_b, r_cmask_b1);
			           r_b  = MAP(mipp::add<R>(r_b0, r_g), 
			                      mipp::sub<R>(r_b1, r_g));

			// normalization
			r_b = RSC_BCJR_intra_normalize<R>::apply(r_b, r_cmask_norm, i+j);

			const auto r_a    = mipp::load<R>(&this->alpha[(i+j+0)*8]);
			const auto r_max0 = HMAP(mipp::add<R>(mipp::add <R>(r_a, r_b0), r_g));
			const auto r_max1 = HMAP(mipp::sub<R>(mipp::add <R>(r_a, r_b1), r_g));
			const auto r_post = mipp::andb<R>(r_m1e, mipp::sub<R>(r_max0, r_max1));

			r_buffer_post = mipp::xorb<R>(mipp::rot<R>(r_buffer_post), r_post);
		}

		// saturate r_buffer_post if the computation are made in 8-bit, do nothing else.
		r_buffer_post = RSC_BCJR_intra_std_post<R>::compute(r_buffer_post);

		// compute extrinsic values
		mipp::store<R>(&ext[i], mipp::sub<R>(r_buffer_post, mipp::load<R>(&sys[i])));
	}
}