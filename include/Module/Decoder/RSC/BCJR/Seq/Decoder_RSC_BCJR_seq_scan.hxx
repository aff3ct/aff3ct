#include <algorithm>
#include <limits>
#include <string>

#include "Tools/Math/utils.h"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_scan.hpp"

#define WINDOW 32
#define BLOCK  32

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename RD>
Decoder_RSC_BCJR_seq_scan<B,R,RD>
::Decoder_RSC_BCJR_seq_scan(const int &K,
                            const std::vector<std::vector<int>> &trellis,
                            const bool buffered_encoding)
: Decoder_RSC_BCJR_seq<B,R>(K, trellis, buffered_encoding)
{
	const std::string name = "Decoder_RSC_BCJR_seq_scan";
	this->set_name(name);
}

template <typename B, typename R, typename RD>
Decoder_RSC_BCJR_seq_scan<B,R,RD>* Decoder_RSC_BCJR_seq_scan<B,R,RD>
::clone() const
{
	auto m = new Decoder_RSC_BCJR_seq_scan(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename RD>
void Decoder_RSC_BCJR_seq_scan<B,R,RD>
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

template <typename R>
struct RSC_BCJR_seq_scan_init
{
	static void apply(R metrics[WINDOW][8][8])
	{
		for (auto i = 0; i < WINDOW; i += BLOCK)
			for (auto j = 0; j < 8; j++)
				for (auto k = 0; k < 8; k++)
					metrics[i][j][k] = -std::numeric_limits<R>::max();
	}
};

template <>
struct RSC_BCJR_seq_scan_init <short>
{
	static void apply(short metrics[WINDOW][8][8])
	{
		for (auto i = 0; i < WINDOW; i += BLOCK)
			for (auto j = 0; j < 8; j++)
				for (auto k = 0; k < 8; k++)
					metrics[i][j][k] = -(1 << (sizeof(short) * 8 -2)); // -inf in fixed point representation
	}
};

template <>
struct RSC_BCJR_seq_scan_init <signed char>
{
	static void apply(signed char metrics[WINDOW][8][8])
	{
		for (auto i = 0; i < WINDOW; i += BLOCK)
			for (auto j = 0; j < 8; j++)
				for (auto k = 0; k < 8; k++)
					metrics[i][j][k] = -63; // -inf in fixed point representation
	}
};

template <typename R>
struct RSC_BCJR_seq_scan_normalize1
{
	static void apply(R metrics[8][8], const int i)
	{
		// no need to normalize
	}
};

template <>
struct RSC_BCJR_seq_scan_normalize1 <short>
{
	static void apply(short metrics[8][8], const int i)
	{
		// normalization
		if (i % 8 == 0)
		{
			const auto norm_val = metrics[0][0];
			for (auto k = 0; k < 8; k++)
				for (auto l = 0; l < 8; l++)
					metrics[l][k] -= norm_val;
		}
	}
};

template <>
struct RSC_BCJR_seq_scan_normalize1 <signed char>
{
	static void apply(signed char metrics[8][8], const int i)
	{
		// normalization & saturation
		const auto norm_val = metrics[0][0];
		for (auto k = 0; k < 8; k++)
			for (auto l = 0; l < 8; l++)
				metrics[l][k] = tools::saturate<signed char>(metrics[l][k] - norm_val, -63, +63);
	}
};

template <typename R>
struct RSC_BCJR_seq_scan_normalize2
{
	static void apply(R &ab0, R &ab1, R &ab2, R &ab3, R &ab4, R &ab5, R &ab6, R &ab7, const int i)
	{
		// no need to normalize
	}
};

template <>
struct RSC_BCJR_seq_scan_normalize2 <short>
{
	static void apply(short &ab0, short &ab1, short &ab2, short &ab3, short &ab4, short &ab5, short &ab6, short &ab7,
	                  const int i)
	{
		// normalization
		if (i % 8 == 0)
		{
			const auto norm_val = ab0;
			ab0 -= norm_val;
			ab1 -= norm_val;
			ab2 -= norm_val;
			ab3 -= norm_val;
			ab4 -= norm_val;
			ab5 -= norm_val;
			ab6 -= norm_val;
			ab7 -= norm_val;
		}
	}
};

template <>
struct RSC_BCJR_seq_scan_normalize2 <signed char>
{
	static void apply(signed char &ab0, signed char &ab1, signed char &ab2, signed char &ab3,
	                  signed char &ab4, signed char &ab5, signed char &ab6, signed char &ab7,
	                  const int i)
	{
		// normalization & saturation
		const auto norm_val = ab0;
		ab0 = tools::saturate<signed char>(ab0 - norm_val, -63, +63);
		ab1 = tools::saturate<signed char>(ab1 - norm_val, -63, +63);
		ab2 = tools::saturate<signed char>(ab2 - norm_val, -63, +63);
		ab3 = tools::saturate<signed char>(ab3 - norm_val, -63, +63);
		ab4 = tools::saturate<signed char>(ab4 - norm_val, -63, +63);
		ab5 = tools::saturate<signed char>(ab5 - norm_val, -63, +63);
		ab6 = tools::saturate<signed char>(ab6 - norm_val, -63, +63);
		ab7 = tools::saturate<signed char>(ab7 - norm_val, -63, +63);
	}
};


template <typename B, typename R, typename RD>
void Decoder_RSC_BCJR_seq_scan<B,R,RD>
::compute_alpha()
{
	constexpr int window = WINDOW;
	constexpr int block  = BLOCK; // block have to be a multiple of window

	// compute alpha values [trellis forward traversal ->]
	R tmp_alpha[window][8][8];
	RSC_BCJR_seq_scan_init<R>::apply(tmp_alpha);

	const int alpha_vec_loop_range = ((this->K +3) / window) * window;

	for (auto w = 1; w < alpha_vec_loop_range; w += window)
	{
		for (auto i = 0; i < window; i += block) // SCAN implementation part1 (parallel)
		{
			auto g0 = this->gamma[0][w + i -1];
			auto g1 = this->gamma[1][w + i -1];

			tmp_alpha[i][0][0] = +g0; tmp_alpha[i][0][1] = -g0;
			tmp_alpha[i][1][3] = +g1; tmp_alpha[i][1][2] = -g1;
			tmp_alpha[i][2][4] = +g1; tmp_alpha[i][2][5] = -g1;
			tmp_alpha[i][3][7] = +g0; tmp_alpha[i][3][6] = -g0;
			tmp_alpha[i][4][1] = +g0; tmp_alpha[i][4][0] = -g0;
			tmp_alpha[i][5][2] = +g1; tmp_alpha[i][5][3] = -g1;
			tmp_alpha[i][6][5] = +g1; tmp_alpha[i][6][4] = -g1;
			tmp_alpha[i][7][6] = +g0; tmp_alpha[i][7][7] = -g0;

			for (auto j = 1; j < block; j++)
			{
				g0 = this->gamma[0][w + i+j -1];
				g1 = this->gamma[1][w + i+j -1];

				// (auto-vectorized loop)
				for (auto k = 0; k < 8; k++)
				{
					tmp_alpha[i+j][0][k] = std::max(tmp_alpha[i+j -1][0][k] + g0, tmp_alpha[i+j -1][1][k] - g0);
					tmp_alpha[i+j][1][k] = std::max(tmp_alpha[i+j -1][3][k] + g1, tmp_alpha[i+j -1][2][k] - g1);
					tmp_alpha[i+j][2][k] = std::max(tmp_alpha[i+j -1][4][k] + g1, tmp_alpha[i+j -1][5][k] - g1);
					tmp_alpha[i+j][3][k] = std::max(tmp_alpha[i+j -1][7][k] + g0, tmp_alpha[i+j -1][6][k] - g0);
					tmp_alpha[i+j][4][k] = std::max(tmp_alpha[i+j -1][1][k] + g0, tmp_alpha[i+j -1][0][k] - g0);
					tmp_alpha[i+j][5][k] = std::max(tmp_alpha[i+j -1][2][k] + g1, tmp_alpha[i+j -1][3][k] - g1);
					tmp_alpha[i+j][6][k] = std::max(tmp_alpha[i+j -1][5][k] + g1, tmp_alpha[i+j -1][4][k] - g1);
					tmp_alpha[i+j][7][k] = std::max(tmp_alpha[i+j -1][6][k] + g0, tmp_alpha[i+j -1][7][k] - g0);
				}

				// normalization
				RSC_BCJR_seq_scan_normalize1<R>::apply(tmp_alpha[i+j], w+i+j);
			}
		}
		for (auto i = 0; i < window; i += block) // SCAN implementation part2 (sequential)
		{
			const R alpha_origin[8] = {this->alpha[0][w+i-1], this->alpha[1][w+i-1],
			                           this->alpha[2][w+i-1], this->alpha[3][w+i-1],
			                           this->alpha[4][w+i-1], this->alpha[5][w+i-1],
			                           this->alpha[6][w+i-1], this->alpha[7][w+i-1]};

			for (auto j = 0; j < block; j++)
			{
				R max[8];
				for (auto l = 0; l < 8; l++)
					max[l] = (R)(alpha_origin[0] + tmp_alpha[i+j][l][0]);

				// (auto-vectorized loop)
				for (auto k = 1; k < 8; k++)
					for (auto l = 0; l < 8; l++)
						max[l] = std::max(max[l], (R)(alpha_origin[k] + tmp_alpha[i+j][l][k]));

				// (auto-unrolled loop)
				for (auto k = 0; k < 8; k++)
					this->alpha[k][w + i+j] = max[k];

				// normalization
				RSC_BCJR_seq_scan_normalize2<R>::apply(this->alpha[0][w+i+j], this->alpha[1][w+i+j],
				                                       this->alpha[2][w+i+j], this->alpha[3][w+i+j],
				                                       this->alpha[4][w+i+j], this->alpha[5][w+i+j],
				                                       this->alpha[6][w+i+j], this->alpha[7][w+i+j],
				                                       w+i+j);
			}
		}
	}
	for (auto i = alpha_vec_loop_range; i < this->K +3; i++) // naive traversal (sequential)
	{
		const auto g0 = this->gamma[0][i -1];
		const auto g1 = this->gamma[1][i -1];

		this->alpha[0][i] = std::max(this->alpha[0][i -1] + g0, this->alpha[1][i -1] - g0);
		this->alpha[1][i] = std::max(this->alpha[3][i -1] + g1, this->alpha[2][i -1] - g1);
		this->alpha[2][i] = std::max(this->alpha[4][i -1] + g1, this->alpha[5][i -1] - g1);
		this->alpha[3][i] = std::max(this->alpha[7][i -1] + g0, this->alpha[6][i -1] - g0);
		this->alpha[4][i] = std::max(this->alpha[1][i -1] + g0, this->alpha[0][i -1] - g0);
		this->alpha[5][i] = std::max(this->alpha[2][i -1] + g1, this->alpha[3][i -1] - g1);
		this->alpha[6][i] = std::max(this->alpha[5][i -1] + g1, this->alpha[4][i -1] - g1);
		this->alpha[7][i] = std::max(this->alpha[6][i -1] + g0, this->alpha[7][i -1] - g0);

		// normalization
		RSC_BCJR_seq_scan_normalize2<R>::apply(this->alpha[0][i], this->alpha[1][i], this->alpha[2][i],
		                                       this->alpha[3][i], this->alpha[4][i], this->alpha[5][i],
		                                       this->alpha[6][i], this->alpha[7][i], i);
	}
}

template <typename B, typename R, typename RD>
void Decoder_RSC_BCJR_seq_scan<B,R,RD>
::compute_beta()
{
	constexpr int window = WINDOW;
	constexpr int block  = BLOCK; // block have to be a multiple of window

	// compute beta values [trellis backward traversal <-]
	R tmp_beta[window][8][8];
	RSC_BCJR_seq_scan_init<R>::apply(tmp_beta);

	const int beta_vec_loop_range = (this->K +2) - (((this->K +2) / window) * window);
	for (auto w = this->K +2; w > beta_vec_loop_range; w -= window)
	{
		for (auto i = 0; i < window; i += block) // SCAN implementation part1 (parallel)
		{
			auto g0 = this->gamma[0][w - i];
			auto g1 = this->gamma[1][w - i];

			tmp_beta[i][0][0] = +g0; tmp_beta[i][0][4] = -g0;
			tmp_beta[i][1][4] = +g0; tmp_beta[i][1][0] = -g0;
			tmp_beta[i][2][5] = +g1; tmp_beta[i][2][1] = -g1;
			tmp_beta[i][3][1] = +g1; tmp_beta[i][3][5] = -g1;
			tmp_beta[i][4][2] = +g1; tmp_beta[i][4][6] = -g1;
			tmp_beta[i][5][6] = +g1; tmp_beta[i][5][2] = -g1;
			tmp_beta[i][6][7] = +g0; tmp_beta[i][6][3] = -g0;
			tmp_beta[i][7][3] = +g0; tmp_beta[i][7][7] = -g0;

			for (auto j = 1; j < block; j++)
			{
				g0 = this->gamma[0][w - (i+j)];
				g1 = this->gamma[1][w - (i+j)];

				// (auto-vectorized loop)
				for (auto k = 0; k < 8; k++)
				{
					tmp_beta[i +j][0][k] = std::max(tmp_beta[i +j -1][0][k] + g0, tmp_beta[i +j -1][4][k] - g0);
					tmp_beta[i +j][1][k] = std::max(tmp_beta[i +j -1][4][k] + g0, tmp_beta[i +j -1][0][k] - g0);
					tmp_beta[i +j][2][k] = std::max(tmp_beta[i +j -1][5][k] + g1, tmp_beta[i +j -1][1][k] - g1);
					tmp_beta[i +j][3][k] = std::max(tmp_beta[i +j -1][1][k] + g1, tmp_beta[i +j -1][5][k] - g1);
					tmp_beta[i +j][4][k] = std::max(tmp_beta[i +j -1][2][k] + g1, tmp_beta[i +j -1][6][k] - g1);
					tmp_beta[i +j][5][k] = std::max(tmp_beta[i +j -1][6][k] + g1, tmp_beta[i +j -1][2][k] - g1);
					tmp_beta[i +j][6][k] = std::max(tmp_beta[i +j -1][7][k] + g0, tmp_beta[i +j -1][3][k] - g0);
					tmp_beta[i +j][7][k] = std::max(tmp_beta[i +j -1][3][k] + g0, tmp_beta[i +j -1][7][k] - g0);
				}

				// normalization
				RSC_BCJR_seq_scan_normalize1<R>::apply(tmp_beta[i+j], w+i+j);
			}
		}
		for (auto i = 0; i < window; i += block) // SCAN implementation part2 (sequential)
		{
			const R beta_origin[8] = {this->beta[0][w -i +1], this->beta[1][w -i +1],
			                          this->beta[2][w -i +1], this->beta[3][w -i +1],
			                          this->beta[4][w -i +1], this->beta[5][w -i +1],
			                          this->beta[6][w -i +1], this->beta[7][w -i +1]};

			for (auto j = 0; j < block; j++)
			{
				R max[8];
				for (auto l = 0; l < 8; l++)
					max[l] = (R)(beta_origin[0] + tmp_beta[i+j][l][0]);

				// (auto-vectorized loop)
				for (auto k = 1; k < 8; k++)
					for (auto l = 0; l < 8; l++)
						max[l] = std::max(max[l], (R)(beta_origin[k] + tmp_beta[i +j][l][k]));

				// (auto-unrolled loop)
				for (auto k = 0; k < 8; k++)
					this->beta[k][w - (i+j)] = max[k];

				// normalization
				RSC_BCJR_seq_scan_normalize2<R>::apply(this->beta[0][w-(i+j)], this->beta[1][w-(i+j)],
				                                       this->beta[2][w-(i+j)], this->beta[3][w-(i+j)],
				                                       this->beta[4][w-(i+j)], this->beta[5][w-(i+j)],
				                                       this->beta[6][w-(i+j)], this->beta[7][w-(i+j)],
				                                       w-(i+j));
			}
		}
	}
	for (auto i = beta_vec_loop_range; i > 0; i--) // naive traversal (sequential)
	{
		const auto g0 = this->gamma[0][i];
		const auto g1 = this->gamma[1][i];

		this->beta[0][i] = std::max(this->beta[0][i +1] + g0, this->beta[4][i +1] - g0);
		this->beta[1][i] = std::max(this->beta[4][i +1] + g0, this->beta[0][i +1] - g0);
		this->beta[2][i] = std::max(this->beta[5][i +1] + g1, this->beta[1][i +1] - g1);
		this->beta[3][i] = std::max(this->beta[1][i +1] + g1, this->beta[5][i +1] - g1);
		this->beta[4][i] = std::max(this->beta[2][i +1] + g1, this->beta[6][i +1] - g1);
		this->beta[5][i] = std::max(this->beta[6][i +1] + g1, this->beta[2][i +1] - g1);
		this->beta[6][i] = std::max(this->beta[7][i +1] + g0, this->beta[3][i +1] - g0);
		this->beta[7][i] = std::max(this->beta[3][i +1] + g0, this->beta[7][i +1] - g0);

		// normalization
		RSC_BCJR_seq_scan_normalize2<R>::apply(this->beta[0][i], this->beta[1][i], this->beta[2][i],
		                                       this->beta[3][i], this->beta[4][i], this->beta[5][i],
		                                       this->beta[6][i], this->beta[7][i], i);
	}
}

template <typename B, typename R, typename RD>
void Decoder_RSC_BCJR_seq_scan<B,R,RD>
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
				max0 = std::max(max0, tmp_post[0][k][j]);

			auto max1 = tmp_post[1][0][j];
			for (auto k = 1; k < 8; k++) // (auto-unrolled loop)
				max1 = std::max(max1, tmp_post[1][k][j]);

			ext[i+j] = RSC_BCJR_seq_post<R,RD>::compute(max0 - max1) - sys[i+j];
		}
	}

	// remaining sequential computations
	for (auto i = ext_vec_loop_range; i < this->K; i++)
	{
		const auto g0 = this->gamma[0][i];
		const auto g1 = this->gamma[1][i];

		auto max0 = (RD)this->alpha[0][i] + (RD)this->beta[0][i +1] + (RD)g0;
		max0 = std::max(max0, (RD)this->alpha[1][i] + (RD)this->beta[4][i +1] + (RD)g0);
		max0 = std::max(max0, (RD)this->alpha[2][i] + (RD)this->beta[5][i +1] + (RD)g1);
		max0 = std::max(max0, (RD)this->alpha[3][i] + (RD)this->beta[1][i +1] + (RD)g1);
		max0 = std::max(max0, (RD)this->alpha[4][i] + (RD)this->beta[2][i +1] + (RD)g1);
		max0 = std::max(max0, (RD)this->alpha[5][i] + (RD)this->beta[6][i +1] + (RD)g1);
		max0 = std::max(max0, (RD)this->alpha[6][i] + (RD)this->beta[7][i +1] + (RD)g0);
		max0 = std::max(max0, (RD)this->alpha[7][i] + (RD)this->beta[3][i +1] + (RD)g0);

		auto max1 = (RD)this->alpha[0][i] + (RD)this->beta[4][i +1] - (RD)g0;
		max1 = std::max(max1, (RD)this->alpha[1][i] + (RD)this->beta[0][i +1] - (RD)g0);
		max1 = std::max(max1, (RD)this->alpha[2][i] + (RD)this->beta[1][i +1] - (RD)g1);
		max1 = std::max(max1, (RD)this->alpha[3][i] + (RD)this->beta[5][i +1] - (RD)g1);
		max1 = std::max(max1, (RD)this->alpha[4][i] + (RD)this->beta[6][i +1] - (RD)g1);
		max1 = std::max(max1, (RD)this->alpha[5][i] + (RD)this->beta[2][i +1] - (RD)g1);
		max1 = std::max(max1, (RD)this->alpha[6][i] + (RD)this->beta[3][i +1] - (RD)g0);
		max1 = std::max(max1, (RD)this->alpha[7][i] + (RD)this->beta[7][i +1] - (RD)g0);

		ext[i] = RSC_BCJR_seq_post<R,RD>::compute(max0 - max1) - sys[i];
	}
}

template <typename B, typename R, typename RD>
int Decoder_RSC_BCJR_seq_scan<B,R,RD>
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
