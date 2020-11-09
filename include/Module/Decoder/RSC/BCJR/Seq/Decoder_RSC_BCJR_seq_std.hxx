#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>

#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_std.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_std<B,R,RD,MAX1,MAX2>
::Decoder_RSC_BCJR_seq_std(const int &K,
                           const std::vector<std::vector<int>> &trellis,
                           const bool buffered_encoding)
: Decoder_RSC_BCJR_seq<B,R>(K, trellis, buffered_encoding)
{
	const std::string name = "Decoder_RSC_BCJR_seq_std";
	this->set_name(name);
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_std<B,R,RD,MAX1,MAX2>* Decoder_RSC_BCJR_seq_std<B,R,RD,MAX1,MAX2>
::clone() const
{
	auto m = new Decoder_RSC_BCJR_seq_std(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_std<B,R,RD,MAX1,MAX2>
::compute_gamma(const R *sys, const R *par)
{
	// compute gamma values (auto-vectorized loop)
	for (auto i = 0; i < this->K +3; i++)
	{
		// there is a big loss of precision here in fixed point
		this->gamma[0][i] = RSC_BCJR_seq_div_or_not<R>::apply(sys[i] + par[i]);
		// there is a big loss of precision here in fixed point
		this->gamma[1][i] = RSC_BCJR_seq_div_or_not<R>::apply(sys[i] - par[i]);
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_std<B,R,RD,MAX1,MAX2>
::compute_alpha()
{
	// compute alpha values [trellis forward traversal ->]
	constexpr int idx_a1[8] = {0, 3, 4, 7, 1, 2, 5, 6};
	constexpr int idx_a2[8] = {1, 2, 5, 6, 0, 3, 4, 7};
	constexpr int idx_g1[8] = {0, 1, 1, 0, 0, 1, 1, 0};
	for (auto i = 1; i < this->K +3; i++)
	{
		for (auto j = 0; j < 8; j++)
			this->alpha[j][i] = MAX1(this->alpha[idx_a1[j]][i -1] + this->gamma[idx_g1[j]][(i -1)],
			                         this->alpha[idx_a2[j]][i -1] - this->gamma[idx_g1[j]][(i -1)]);

		RSC_BCJR_seq_normalize<R>::apply(this->alpha, i);
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_std<B,R,RD,MAX1,MAX2>
::compute_beta()
{
	// compute beta values [trellis backward traversal <-]
	constexpr int idx_b1[8] = {0, 4, 5, 1, 2, 6, 7, 3};
	constexpr int idx_b2[8] = {4, 0, 1, 5, 6, 2, 3, 7};
	constexpr int idx_g2[8] = {0, 0, 1, 1, 1, 1, 0, 0};
	for (auto i = this->K +2; i >= 1; i--)
	{
		for (auto j = 0; j < 8; j++)
			this->beta[j][i] = MAX1(this->beta[idx_b1[j]][i +1] + this->gamma[idx_g2[j]][i],
			                        this->beta[idx_b2[j]][i +1] - this->gamma[idx_g2[j]][i]);

		RSC_BCJR_seq_normalize<R>::apply(this->beta, i);
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_std<B,R,RD,MAX1,MAX2>
::compute_ext(const R *sys, R *ext)
{
	// compute extrinsic values
	constexpr int idx_b1[8] = {0, 4, 5, 1, 2, 6, 7, 3};
	constexpr int idx_b2[8] = {4, 0, 1, 5, 6, 2, 3, 7};
	constexpr int idx_g2[8] = {0, 0, 1, 1, 1, 1, 0, 0};
	for (auto i = 0; i < this->K; i++)
	{
		RD max0 = (RD)this->alpha[0][i] + (RD)this->beta[idx_b1[0]][i +1] + (RD)this->gamma[idx_g2[0]][i];
		for (auto j = 1; j < 8; j++)
			max0 = MAX2(max0, (RD)this->alpha[j][i] + (RD)this->beta[idx_b1[j]][i +1] + (RD)this->gamma[idx_g2[j]][i]);

		RD max1 = (RD)this->alpha[0][i] + (RD)this->beta[idx_b2[0]][i +1] - (RD)this->gamma[idx_g2[0]][i];
		for (auto j = 1; j < 8; j++)
			max1 = MAX2(max1, (RD)this->alpha[j][i] + (RD)this->beta[idx_b2[j]][i +1] - (RD)this->gamma[idx_g2[j]][i]);

		ext[i] = RSC_BCJR_seq_post<R,RD>::compute(max0 - max1) - sys[i];
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_std<B,R,RD,MAX1,MAX2>
::compute_beta_ext(const R *sys, R *ext)
{
	constexpr int idx_b1[8] = {0, 4, 5, 1, 2, 6, 7, 3};
	constexpr int idx_b2[8] = {4, 0, 1, 5, 6, 2, 3, 7};
	constexpr int idx_g2[8] = {0, 0, 1, 1, 1, 1, 0, 0};

	// compute the first beta values [trellis backward traversal <-]
	R beta_prev[8];
	for (auto j = 0; j < 8; j++)
		beta_prev[j] = this->alpha[j][0];
	for (auto i = this->K +2; i >= this->K; i--)
	{
		R beta_cur[8];
		for (auto j = 0; j < 8; j++)
			beta_cur[j] = MAX1(beta_prev[idx_b1[j]] + this->gamma[idx_g2[j]][i],
			                   beta_prev[idx_b2[j]] - this->gamma[idx_g2[j]][i]);

		RSC_BCJR_seq_normalize<R>::apply(beta_cur, i);

		for (auto j = 0; j < 8; j++)
			beta_prev[j] = beta_cur[j];
	}

	// compute the beta values [trellis backward traversal <-] + compute extrinsic values
	for (auto i = this->K -1; i >= 0; i--)
	{
		RD max0 = (RD)this->alpha[0][i] + (RD)beta_prev[idx_b1[0]] + (RD)this->gamma[idx_g2[0]][i];
		for (auto j = 1; j < 8; j++)
			max0 = MAX2(max0, (RD)this->alpha[j][i] + (RD)beta_prev[idx_b1[j]] + (RD)this->gamma[idx_g2[j]][i]);

		RD max1 = (RD)this->alpha[0][i] + (RD)beta_prev[idx_b2[0]] - (RD)this->gamma[idx_g2[0]][i];
		for (auto j = 1; j < 8; j++)
			max1 = MAX2(max1, (RD)this->alpha[j][i] + (RD)beta_prev[idx_b2[j]] - (RD)this->gamma[idx_g2[j]][i]);

		ext[i] = RSC_BCJR_seq_post<R,RD>::compute(max0 - max1) - sys[i];


		// compute the beta values
		R beta_cur[8];
		for (auto j = 0; j < 8; j++)
			beta_cur[j] = MAX1(beta_prev[idx_b1[j]] + this->gamma[idx_g2[j]][i],
			                   beta_prev[idx_b2[j]] - this->gamma[idx_g2[j]][i]);

		RSC_BCJR_seq_normalize<R>::apply(beta_cur, i);

		for (auto j = 0; j < 8; j++)
			beta_prev[j] = beta_cur[j];
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
int Decoder_RSC_BCJR_seq_std<B,R,RD,MAX1,MAX2>
::_decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	this->compute_gamma   (sys, par);
	this->compute_alpha   (        );
	// this->compute_beta (        );
	// this->compute_ext  (sys, ext);
	this->compute_beta_ext(sys, ext);

	return 0;
}
}
}
