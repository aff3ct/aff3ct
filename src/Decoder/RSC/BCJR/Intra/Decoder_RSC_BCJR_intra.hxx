#include <limits>

#include "Decoder_RSC_BCJR_intra.hpp"

template <typename R>
struct RSC_BCJR_intra_init
{
	static void apply(mipp::vector<R> &alpha)
	{
		// init alpha values
		alpha[0*8 +0] = (R)0;
		for (auto i = 1; i < 8; i++)
			alpha[0*8 +i] = -std::numeric_limits<R>::max();
	}
};

template <>
struct RSC_BCJR_intra_init<short>
{
	static void apply(mipp::vector<short> &alpha)
	{
		// init alpha values
		alpha[0*8 +0] = (short)0;
		for (auto i = 1; i < 8; i++)
			alpha[0*8 +i] = -(1 << (sizeof(short) * 8 -2));
	}
};

template <>
struct RSC_BCJR_intra_init<signed char>
{
	static void apply(mipp::vector<signed char> &alpha)
	{
		// init alpha values
		alpha[0*8 +0] = (signed char)0;
		for (auto i = 1; i < 8; i++)
			alpha[0*8 +i] = -63;
	}
};

template <typename B, typename R>
Decoder_RSC_BCJR_intra<B,R>
::Decoder_RSC_BCJR_intra(const int &K, const bool buffered_encoding)
: Decoder_RSC_BCJR<B,R>(K, buffered_encoding),
  alpha(8 * (K +4) + 1 * mipp::nElReg<R>()),
  gamma(2 * (K +3) + 2 * mipp::nElReg<R>())
{
	// init alpha values
	RSC_BCJR_intra_init<R>::apply(alpha);
}

template <typename B, typename R>
Decoder_RSC_BCJR_intra<B,R>
::~Decoder_RSC_BCJR_intra()
{
}

template <typename B, typename R>
void Decoder_RSC_BCJR_intra<B,R>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	this->compute_gamma   (sys, par);
	this->compute_alpha   (        );
	this->compute_beta_ext(sys, ext);
}

// =================================================================================================== sys/par division
template <typename R>
struct RSC_BCJR_intra_div_or_not
{
	static mipp::reg apply(mipp::reg r)
	{
		return r;
	}
};

template <>
struct RSC_BCJR_intra_div_or_not <signed char>
{
	static mipp::reg apply(mipp::reg r)
	{
		return mipp::div2<signed char>(r);
	}
};

// ====================================================================================================== post division
template <typename R>
struct RSC_BCJR_intra_post
{
	static mipp::reg compute(const mipp::reg &r_post)
	{
		return mipp::div2<R>(r_post);
	}
};

template <>
struct RSC_BCJR_intra_post <signed char>
{
	static mipp::reg compute(const mipp::reg &r_post)
	{
		return mipp::sat<signed char>(r_post, -63, 63);
	}
};

// ====================================================================================================== normalization

template <typename R>
struct RSC_BCJR_intra_normalize_core
{
	static mipp::reg apply(const mipp::reg &r_metrics, const mipp::reg &r_cmask_norm)
	{
		const auto r_norm         = mipp::shuff<R>(r_metrics, r_cmask_norm);
		const auto r_metrics_norm = mipp::sub  <R>(r_metrics, r_norm);

		return r_metrics_norm;
	}
};

template <typename R, int I = -1>
struct RSC_BCJR_intra_normalize
{
	static mipp::reg apply(const mipp::reg &r_metrics, const mipp::reg &r_cmask_norm, const int &i = 0)
	{
		return r_metrics;
	}
};

template <>
struct RSC_BCJR_intra_normalize <short, -1>
{
	static mipp::reg apply(const mipp::reg &r_metrics, const mipp::reg &r_cmask_norm, const int &i = 0)
	{  
		if (i % 4 == 0)
			return RSC_BCJR_intra_normalize_core<short>::apply(r_metrics, r_cmask_norm);
		else
			return r_metrics;
	}
};

template <>
struct RSC_BCJR_intra_normalize <short, 3>
{
	static mipp::reg apply(const mipp::reg &r_metrics, const mipp::reg &r_cmask_norm, const int &i = 0)
	{
		return RSC_BCJR_intra_normalize_core<short>::apply(r_metrics, r_cmask_norm);
	}
};

// template <typename R>
// struct RSC_BCJR_intra_normalize_legacy
// {
// 	static void apply(R *metrics, const int &i = 0)
// 	{
// 		// no need to do something
// 	}
// };

// template <>
// struct RSC_BCJR_intra_normalize_legacy <short>
// {
// 	static void apply(short *metrics, const int &i = 0)
// 	{
// 		// normalization
// 		if (i % 4 == 0)
// 		{
// 			auto norm_val = metrics[i*8 +0];
// 			for (auto j = 0; j < 8; j++)
// 				metrics[i*8 +j] -= norm_val;
// 		}
// 	}
// };

// template <>
// struct RSC_BCJR_intra_normalize_legacy <signed char>
// {
// 	static void apply(signed char *metrics, const int &i = 0)
// 	{
// 		// normalization & saturation
// 		auto norm_val = metrics[i*8 +0];
// 		for (auto j = 0; j < 8; j++)
// 			metrics[i*8 +j] = saturate<signed char>(metrics[i*8 +j] - norm_val, -63, +63);
// 	}
// };

// == DEBUG ================================================================================================== DEBUG ==
// template <typename B, typename R>
// void Decoder_RSC_BCJR_intra<B,R>
// ::compute_gamma(const mipp::vector<R> &sys, const mipp::vector<R> &par)
// {
// 	// compute gamma values (auto-vectorized loop)
// 	for (auto i = 0; i < K +3; i++)
// 	{
// 		gamma[i*2 +0] = div2<R>(sys[i] + par[i]); // there is a big loss of precision here in fixed point
// 		gamma[i*2 +1] = div2<R>(sys[i] - par[i]); // there is a big loss of precision here in fixed point
// 	}
// }

// template <typename B, typename R>
// void Decoder_RSC_BCJR_intra<B,R>
// ::compute_alpha()
// {
// 	// compute alpha values [trellis forward traversal ->]
// 	constexpr int idx_a1[8] = {0, 3, 4, 7, 1, 2, 5, 6};
// 	constexpr int idx_a2[8] = {1, 2, 5, 6, 0, 3, 4, 7};
// 	constexpr int idx_g1[8] = {0, 1, 1, 0, 0, 1, 1, 0};

// 	for (auto i = 1; i < this->K +3; i++)
// 	{
// 		for (auto j = 0; j < 8; j++)
// 			this->alpha[i*8 +j] = std::max(this->alpha[(i -1)*8 + idx_a1[j]] + this->gamma[(i -1)*2 + idx_g1[j]],
// 			                               this->alpha[(i -1)*8 + idx_a2[j]] - this->gamma[(i -1)*2 + idx_g1[j]]);

// 		RSC_BCJR_intra_normalize_legacy<R>::apply(this->alpha.data(), i);
// 	}
// }

// template <typename R, typename RD>
// struct RSC_BCJR_intra_std_post
// {
// 	static R compute(const RD &max0, const RD &max1)
// 	{
// 		return max0 - max1;
// 	}
// };

// template <typename RD>
// struct RSC_BCJR_intra_std_post <signed char, RD>
// {
// 	static signed char compute(const RD &max0, const RD &max1)
// 	{
// 		return (signed char)saturate<RD>(max0 - max1, -63, 63);
// 	}
// };

// template <typename B, typename R>
// void Decoder_RSC_BCJR_intra<B,R>
// ::compute_beta_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext)
// {
// 	constexpr int cmask_b0[8] = {0, 4, 5, 1, 2, 6, 7, 3}; // beta trellis transitions 0.
// 	constexpr int cmask_b1[8] = {4, 0, 1, 5, 6, 2, 3, 7}; // beta trellis transitions 1.
// 	constexpr int cmask_g [8] = {0, 0, 1, 1, 1, 1, 0, 0}; // mask  to construct the gamma0/1 vector (seq).

// 	// compute beta values [trellis backward traversal <-] (sequential)
// 	R beta_tmp_prev[8] = {this->alpha[0], this->alpha[1], this->alpha[2], this->alpha[3], 
// 	                      this->alpha[4], this->alpha[5], this->alpha[6], this->alpha[7]};
// 	for (auto i = this->K +2; i >= this->K; i--)
// 	{
// 		R beta_tmp[8];
// 		for (auto j = 0; j < 8; j++)
// 			beta_tmp[j] = std::max(beta_tmp_prev[cmask_b0[j]] + this->gamma[i*2 + cmask_g[j]],
// 			                       beta_tmp_prev[cmask_b1[j]] - this->gamma[i*2 + cmask_g[j]]);

// 		RSC_BCJR_intra_normalize_legacy<R>::apply(beta_tmp);

// 		for (auto j = 0; j < 8; j++) 
// 			beta_tmp_prev[j] = beta_tmp[j];
// 	}

// 	using RD = R;

// 	// compute extrinsic values
// 	for (auto i = this->K -1; i >= 0; i--)
// 	{
// 		R beta_tmp[8];
// 		for (auto j = 0; j < 8; j++)
// 			beta_tmp[j] = std::max(beta_tmp_prev[cmask_b0[j]] + this->gamma[i*2 + cmask_g[j]],
// 			                       beta_tmp_prev[cmask_b1[j]] - this->gamma[i*2 + cmask_g[j]]);
// 		RSC_BCJR_intra_normalize_legacy<R>::apply(beta_tmp);

// 		RD max0 = (RD)this->alpha[i*8 +0] + (RD)beta_tmp_prev[cmask_b0[0]] + (RD)this->gamma[i*2 + cmask_g[0]];
// 		for (auto j = 1; j < 8; j++)
// 			max0 = std::max(max0, (RD)(this->alpha  [i*8           +j ] + 
// 			                           beta_tmp_prev[      cmask_b0[j]] + 
// 			                           this->gamma  [i*2 + cmask_g [j]]));

// 		RD max1 = (RD)this->alpha[i*8 +0] + (RD)beta_tmp_prev[cmask_b1[0]] - (RD)this->gamma[i*2 + cmask_g[0]];
// 		for (auto j = 1; j < 8; j++)
// 			max1 = std::max(max1, (RD)(this->alpha  [i*8           +j ] + 
// 			                           beta_tmp_prev[      cmask_b1[j]] - 
// 			                           this->gamma  [i*2 + cmask_g [j]]));

// 		ext[i] = RSC_BCJR_intra_std_post<R,RD>::compute(max0, max1) - sys[i];

// 		for (auto j = 0; j < 8; j++)
// 			beta_tmp_prev[j] = beta_tmp[j];
// 	}
// }
// == DEBUG ================================================================================================== DEBUG ==