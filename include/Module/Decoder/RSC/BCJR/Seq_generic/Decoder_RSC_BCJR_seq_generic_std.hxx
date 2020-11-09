#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>
#include <limits>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::Decoder_RSC_BCJR_seq_generic_std(const int &K,
                                   const std::vector<std::vector<int>> &trellis,
                                   const bool buffered_encoding)
: Decoder_RSC_BCJR_seq_generic<B,R>(K, trellis, buffered_encoding),
  beta_prev(trellis[0].size()), beta_cur(trellis[0].size())
{
	const std::string name = "Decoder_RSC_BCJR_seq_generic_std";
	this->set_name(name);
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>* Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::clone() const
{
	auto m = new Decoder_RSC_BCJR_seq_generic_std(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_gamma(const R *sys, const R *par)
{
	// compute gamma values (auto-vectorized loop)
	for (auto i = 0; i < this->K + this->n_ff; i++)
	{
		// there is a big loss of precision here in fixed point
		this->gamma[0][i] = RSC_BCJR_seq_generic_div_or_not<R>::apply(sys[i] + par[i]);
		// there is a big loss of precision here in fixed point
		this->gamma[1][i] = RSC_BCJR_seq_generic_div_or_not<R>::apply(sys[i] - par[i]);
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_alpha()
{
	// compute alpha values [trellis forward traversal ->]
	for (auto i = 1; i < this->K + this->n_ff; i++)
	{
		for (auto j = 0; j < this->n_states; j++)
			this->alpha[j][i] = MAX1(
				this->alpha[this->trellis[0][j]][i -1] + this->trellis[1][j] * this->gamma[this->trellis[2][j]][(i -1)],
				this->alpha[this->trellis[3][j]][i -1] + this->trellis[4][j] * this->gamma[this->trellis[5][j]][(i -1)]);

		RSC_BCJR_seq_generic_normalize<R>::apply(this->alpha, i, this->n_states);
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_beta()
{
	// compute beta values [trellis backward traversal <-]
	for (auto i = this->K + this->n_ff -1; i >= 1; i--)
	{
		for (auto j = 0; j < this->n_states; j++)
			this->beta[j][i] = MAX1(
				this->beta[this->trellis[6][j]][i +1] + this->gamma[this->trellis[7][j]][i],
				this->beta[this->trellis[8][j]][i +1] - this->gamma[this->trellis[9][j]][i]);

		RSC_BCJR_seq_generic_normalize<R>::apply(this->beta, i, this->n_states);
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_ext(const R *sys, R *ext)
{
	// compute extrinsic values
	for (auto i = 0; i < this->K; i++)
	{
		RD max0 = -std::numeric_limits<RD>::max();
		RD max1 = -std::numeric_limits<RD>::max();

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[1][j] == +1)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);
			else
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[4][j] == +1)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);
			else
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		ext[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - sys[i];
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_beta_ext(const R *sys, R *ext)
{
	// compute the first beta values [trellis backward traversal <-]
	for (auto j = 0; j < this->n_states; j++)
		beta_prev[j] = this->alpha[j][0];
	for (auto i = this->K + this->n_ff -1; i >= this->K; i--)
	{
		for (auto j = 0; j < this->n_states; j++)
			beta_cur[j] = MAX1(
				beta_prev[this->trellis[6][j]] + this->gamma[this->trellis[7][j]][i],
				beta_prev[this->trellis[8][j]] - this->gamma[this->trellis[9][j]][i]);

		RSC_BCJR_seq_generic_normalize<R>::apply(beta_cur.data(), i, this->n_states);

		for (auto j = 0; j < this->n_states; j++)
			beta_prev[j] = beta_cur[j];
	}

	// compute the beta values [trellis backward traversal <-] + compute extrinsic values
	for (auto i = this->K -1; i >= 0; i--)
	{
		RD max0 = (RD)this->alpha[                 0 ][i] +
		          (RD)beta_prev  [this->trellis[6][0]   ] +
		          (RD)this->gamma[this->trellis[7][0]][i];

		RD max1 = (RD)this->alpha[                 0 ][i] +
		          (RD)beta_prev  [this->trellis[8][0]   ] -
		          (RD)this->gamma[this->trellis[9][0]][i];

		for (auto j = 1; j < this->n_states; j++)
			if (this->trellis[1][j] == 1)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i] +
				                  (RD)beta_prev  [this->trellis[6][j]   ] +
				                  (RD)this->gamma[this->trellis[7][j]][i]);
			else
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i] +
				                  (RD)beta_prev  [this->trellis[8][j]   ] -
				                  (RD)this->gamma[this->trellis[9][j]][i]);

		for (auto j = 1; j < this->n_states; j++)
			if (this->trellis[4][j] == 1)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i] +
				                  (RD)beta_prev  [this->trellis[6][j]   ] +
				                  (RD)this->gamma[this->trellis[7][j]][i]);
			else
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i] +
				                  (RD)beta_prev  [this->trellis[8][j]   ] -
				                  (RD)this->gamma[this->trellis[9][j]][i]);

		ext[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - sys[i];

		// compute the beta values
		for (auto j = 0; j < this->n_states; j++)
			beta_cur[j] = MAX1(
				beta_prev[this->trellis[6][j]] + this->gamma[this->trellis[7][j]][i],
				beta_prev[this->trellis[8][j]] - this->gamma[this->trellis[9][j]][i]);

		RSC_BCJR_seq_generic_normalize<R>::apply(beta_cur.data(), i, this->n_states);

		for (auto j = 0; j < this->n_states; j++)
			beta_prev[j] = beta_cur[j];
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_ext_sys(const R *sys, R *ext_sys)
{
	// compute extrinsic values
	for (auto i = 0; i < this->K + this->n_ff; i++)
	{
		RD max0 = -std::numeric_limits<RD>::max();
		RD max1 = -std::numeric_limits<RD>::max();

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[1][j] == +1)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);
			else
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[4][j] == +1)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);
			else
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		ext_sys[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - sys[i];
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_ext_par(const R *par, R *ext_par)
{
	// compute extrinsic values
	for (auto i = 0; i < this->K + this->n_ff; i++)
	{
		RD max0 = -std::numeric_limits<RD>::max();
		RD max1 = -std::numeric_limits<RD>::max();

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[1][j] == +1 && this->trellis[7][j] == 0)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

			else if (this->trellis[1][j] == +1 && this->trellis[7][j] == 1)
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

			else if (this->trellis[1][j] == -1 && this->trellis[9][j] == 1)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

			else if (this->trellis[1][j] == -1  && this->trellis[9][j] == 0)
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[4][j] == +1 && this->trellis[7][j] == 0)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

			else if (this->trellis[4][j] == +1 && this->trellis[7][j] == 1)
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

			else if (this->trellis[4][j] == -1 && this->trellis[9][j] == 1)
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

			else if (this->trellis[4][j] == -1  && this->trellis[9][j] == 0)
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		ext_par[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - par[i];
	}
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
int Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::_decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	this->compute_gamma   (sys, par);
	this->compute_alpha   (        );
	// this->compute_beta (        );
	// this->compute_ext  (sys, ext);
	this->compute_beta_ext(sys, ext);

	return 0;
}

template <typename B, typename R, typename RD, tools::proto_max<R> MAX1, tools::proto_max<RD> MAX2>
int Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::_decode_siso(const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	if (!this->buffered_encoding)
		throw tools::runtime_error(__FILE__, __LINE__, __func__,  "'buffered_encoding' has to be enabled.");

	const R* sys     = Y_N1;
	const R* par     = Y_N1 + this->K + this->n_ff;
	      R* ext_sys = Y_N2;
	      R* ext_par = Y_N2 + this->K + this->n_ff;

	this->compute_gamma  (sys, par    );
	this->compute_alpha  (            );
	this->compute_beta   (            );
	this->compute_ext_sys(sys, ext_sys);
	this->compute_ext_par(par, ext_par);

	return 0;
}
}
}
