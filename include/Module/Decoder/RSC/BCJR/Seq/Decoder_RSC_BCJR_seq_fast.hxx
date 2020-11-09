#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>

#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_fast.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_fast<B,R,RD,MAX1,MAX2>
::Decoder_RSC_BCJR_seq_fast(const int &K,
                            const std::vector<std::vector<int>> &trellis,
                            const bool buffered_encoding)
: Decoder_RSC_BCJR_seq<B,R>(K, trellis, buffered_encoding)
{
	const std::string name = "Decoder_RSC_BCJR_seq_fast";
	this->set_name(name);
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_fast<B,R,RD,MAX1,MAX2>* Decoder_RSC_BCJR_seq_fast<B,R,RD,MAX1,MAX2>
::clone() const
{
	auto m = new Decoder_RSC_BCJR_seq_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_fast<B,R,RD,MAX1,MAX2>
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
void Decoder_RSC_BCJR_seq_fast<B,R,RD,MAX1,MAX2>
::compute_alpha()
{
	// compute alpha values [trellis forward traversal ->]
	for (auto i = 1; i < this->K +3; i++)
	{
		const auto g0 = this->gamma[0][i -1];
		const auto g1 = this->gamma[1][i -1];

		this->alpha[0][i] = MAX1(this->alpha[0][i -1] + g0, this->alpha[1][i -1] - g0);
		this->alpha[1][i] = MAX1(this->alpha[3][i -1] + g1, this->alpha[2][i -1] - g1);
		this->alpha[2][i] = MAX1(this->alpha[4][i -1] + g1, this->alpha[5][i -1] - g1);
		this->alpha[3][i] = MAX1(this->alpha[7][i -1] + g0, this->alpha[6][i -1] - g0);
		this->alpha[4][i] = MAX1(this->alpha[1][i -1] + g0, this->alpha[0][i -1] - g0);
		this->alpha[5][i] = MAX1(this->alpha[2][i -1] + g1, this->alpha[3][i -1] - g1);
		this->alpha[6][i] = MAX1(this->alpha[5][i -1] + g1, this->alpha[4][i -1] - g1);
		this->alpha[7][i] = MAX1(this->alpha[6][i -1] + g0, this->alpha[7][i -1] - g0);

		RSC_BCJR_seq_normalize<R>::apply(this->alpha, i);
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_fast<B,R,RD,MAX1,MAX2>
::compute_beta()
{
	// compute beta values [trellis backward traversal <-]
	for (auto i = this->K +2; i >= 1; i--)
	{
		const auto g0 = this->gamma[0][i];
		const auto g1 = this->gamma[1][i];

		this->beta[0][i] = MAX1(this->beta[0][i +1] + g0, this->beta[4][i +1] - g0);
		this->beta[1][i] = MAX1(this->beta[4][i +1] + g0, this->beta[0][i +1] - g0);
		this->beta[2][i] = MAX1(this->beta[5][i +1] + g1, this->beta[1][i +1] - g1);
		this->beta[3][i] = MAX1(this->beta[1][i +1] + g1, this->beta[5][i +1] - g1);
		this->beta[4][i] = MAX1(this->beta[2][i +1] + g1, this->beta[6][i +1] - g1);
		this->beta[5][i] = MAX1(this->beta[6][i +1] + g1, this->beta[2][i +1] - g1);
		this->beta[6][i] = MAX1(this->beta[7][i +1] + g0, this->beta[3][i +1] - g0);
		this->beta[7][i] = MAX1(this->beta[3][i +1] + g0, this->beta[7][i +1] - g0);

		RSC_BCJR_seq_normalize<R>::apply(this->beta, i);
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_fast<B,R,RD,MAX1,MAX2>
::compute_ext(const R *sys, R *ext)
{
	// compute extrinsic values
	constexpr int block = 16;
	const int ext_vec_loop_range = (this->K / block) * block;
	for (auto i = 0; i < ext_vec_loop_range; i += block)
	{
		RD tmp_post[2][8][block];

		// (auto-vectorized loop)
		for (auto j = 0; j < block; j++)
		{
			const auto g0 = this->gamma[0][i+j];
			const auto g1 = this->gamma[1][i+j];

			tmp_post[0][0][j] = (RD)this->alpha[0][i+j] + (RD)this->beta[0][i+j +1] + (RD)g0;
			tmp_post[0][1][j] = (RD)this->alpha[1][i+j] + (RD)this->beta[4][i+j +1] + (RD)g0;
			tmp_post[0][2][j] = (RD)this->alpha[2][i+j] + (RD)this->beta[5][i+j +1] + (RD)g1;
			tmp_post[0][3][j] = (RD)this->alpha[3][i+j] + (RD)this->beta[1][i+j +1] + (RD)g1;
			tmp_post[0][4][j] = (RD)this->alpha[4][i+j] + (RD)this->beta[2][i+j +1] + (RD)g1;
			tmp_post[0][5][j] = (RD)this->alpha[5][i+j] + (RD)this->beta[6][i+j +1] + (RD)g1;
			tmp_post[0][6][j] = (RD)this->alpha[6][i+j] + (RD)this->beta[7][i+j +1] + (RD)g0;
			tmp_post[0][7][j] = (RD)this->alpha[7][i+j] + (RD)this->beta[3][i+j +1] + (RD)g0;

			tmp_post[1][0][j] = (RD)this->alpha[0][i+j] + (RD)this->beta[4][i+j +1] - (RD)g0;
			tmp_post[1][1][j] = (RD)this->alpha[1][i+j] + (RD)this->beta[0][i+j +1] - (RD)g0;
			tmp_post[1][2][j] = (RD)this->alpha[2][i+j] + (RD)this->beta[1][i+j +1] - (RD)g1;
			tmp_post[1][3][j] = (RD)this->alpha[3][i+j] + (RD)this->beta[5][i+j +1] - (RD)g1;
			tmp_post[1][4][j] = (RD)this->alpha[4][i+j] + (RD)this->beta[6][i+j +1] - (RD)g1;
			tmp_post[1][5][j] = (RD)this->alpha[5][i+j] + (RD)this->beta[2][i+j +1] - (RD)g1;
			tmp_post[1][6][j] = (RD)this->alpha[6][i+j] + (RD)this->beta[3][i+j +1] - (RD)g0;
			tmp_post[1][7][j] = (RD)this->alpha[7][i+j] + (RD)this->beta[7][i+j +1] - (RD)g0;
		}
		// (auto-vectorized loop)
		for (auto j = 0; j < block; j++)
		{
			auto max0 = tmp_post[0][0][j];
			for (auto k = 1; k < 8; k++) // (auto-unrolled loop)
				max0 = MAX2(max0, tmp_post[0][k][j]);

			auto max1 = tmp_post[1][0][j];
			for (auto k = 1; k < 8; k++) // (auto-unrolled loop)
				max1 = MAX2(max1, tmp_post[1][k][j]);

			ext[i+j] = RSC_BCJR_seq_post<R,RD>::compute(max0 - max1) - sys[i+j];
		}
	}

	// remaining sequential computations
	for (auto i = ext_vec_loop_range; i < this->K; i++)
	{
		const auto g0 = this->gamma[0][i];
		const auto g1 = this->gamma[1][i];

		auto max0 = (RD)this->alpha[0][i] + (RD)this->beta[0][i +1] + (RD)g0;
		max0 = MAX2(max0, (RD)this->alpha[1][i] + (RD)this->beta[4][i +1] + (RD)g0);
		max0 = MAX2(max0, (RD)this->alpha[2][i] + (RD)this->beta[5][i +1] + (RD)g1);
		max0 = MAX2(max0, (RD)this->alpha[3][i] + (RD)this->beta[1][i +1] + (RD)g1);
		max0 = MAX2(max0, (RD)this->alpha[4][i] + (RD)this->beta[2][i +1] + (RD)g1);
		max0 = MAX2(max0, (RD)this->alpha[5][i] + (RD)this->beta[6][i +1] + (RD)g1);
		max0 = MAX2(max0, (RD)this->alpha[6][i] + (RD)this->beta[7][i +1] + (RD)g0);
		max0 = MAX2(max0, (RD)this->alpha[7][i] + (RD)this->beta[3][i +1] + (RD)g0);

		auto max1 = (RD)this->alpha[0][i] + (RD)this->beta[4][i +1] - (RD)g0;
		max1 = MAX2(max1, (RD)this->alpha[1][i] + (RD)this->beta[0][i +1] - (RD)g0);
		max1 = MAX2(max1, (RD)this->alpha[2][i] + (RD)this->beta[1][i +1] - (RD)g1);
		max1 = MAX2(max1, (RD)this->alpha[3][i] + (RD)this->beta[5][i +1] - (RD)g1);
		max1 = MAX2(max1, (RD)this->alpha[4][i] + (RD)this->beta[6][i +1] - (RD)g1);
		max1 = MAX2(max1, (RD)this->alpha[5][i] + (RD)this->beta[2][i +1] - (RD)g1);
		max1 = MAX2(max1, (RD)this->alpha[6][i] + (RD)this->beta[3][i +1] - (RD)g0);
		max1 = MAX2(max1, (RD)this->alpha[7][i] + (RD)this->beta[7][i +1] - (RD)g0);

		ext[i] = RSC_BCJR_seq_post<R,RD>::compute(max0 - max1) - sys[i];
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
int Decoder_RSC_BCJR_seq_fast<B,R,RD,MAX1,MAX2>
::_decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	this->compute_gamma(sys, par);
	this->compute_alpha(        );
	this->compute_beta (        );
	this->compute_ext  (sys, ext);

	return 0;
}
}
}
