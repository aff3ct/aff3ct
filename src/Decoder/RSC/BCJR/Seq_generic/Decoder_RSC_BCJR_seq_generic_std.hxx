#include "Decoder_RSC_BCJR_seq_generic_std.hpp"

template <typename B, typename R, typename RD, proto_map<R> MAP1, proto_map<RD> MAP2>
Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAP1,MAP2>
::Decoder_RSC_BCJR_seq_generic_std(const int &K, 
                                   const std::vector<std::vector<int>> &trellis, 
                                   const bool buffered_encoding)
: Decoder_RSC_BCJR_seq_generic<B,R>(K, trellis, buffered_encoding)
{
}

template <typename B, typename R, typename RD, proto_map<R> MAP1, proto_map<RD> MAP2>
Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAP1,MAP2>
::~Decoder_RSC_BCJR_seq_generic_std()
{
}

template <typename B, typename R, typename RD, proto_map<R> MAP1, proto_map<RD> MAP2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAP1,MAP2>
::compute_gamma(const mipp::vector<R> &sys, const mipp::vector<R> &par)
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

template <typename B, typename R, typename RD, proto_map<R> MAP1, proto_map<RD> MAP2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAP1,MAP2>
::compute_alpha()
{
	// compute alpha values [trellis forward traversal ->]
	for (auto i = 1; i < this->K + this->n_ff; i++)
	{
		for (auto j = 0; j < this->n_states; j++)
			this->alpha[j][i] = MAP1(
				this->alpha[this->trellis[0][j]][i -1] + this->trellis[1][j] * this->gamma[this->trellis[2][j]][(i -1)],
				this->alpha[this->trellis[3][j]][i -1] + this->trellis[4][j] * this->gamma[this->trellis[5][j]][(i -1)]);

		RSC_BCJR_seq_generic_normalize<R>::apply(this->alpha, i, this->n_states);
	}
}

template <typename B, typename R, typename RD, proto_map<R> MAP1, proto_map<RD> MAP2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAP1,MAP2>
::compute_beta()
{
	// compute beta values [trellis backward traversal <-]
	for (auto i = this->K + this->n_ff -1; i >= 1; i--)
	{
		for (auto j = 0; j < this->n_states; j++)
			this->beta[j][i] = MAP1(
				this->beta[this->trellis[6][j]][i +1] + this->gamma[this->trellis[7 ][j]][i],
				this->beta[this->trellis[8][j]][i +1] - this->gamma[this->trellis[9][j]][i]);

		RSC_BCJR_seq_generic_normalize<R>::apply(this->beta, i, this->n_states);
	}
}

template <typename B, typename R, typename RD, proto_map<R> MAP1, proto_map<RD> MAP2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAP1,MAP2>
::compute_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext)
{
	RD max0;
	RD max1;
	// compute extrinsic values
	for (auto i = 0; i < this->K; i++)
	{
		max0 = -std::numeric_limits<RD>::max();
		max1 = -std::numeric_limits<RD>::max();
		
		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[1][j] == +1)
				max0 = MAP2(max0, (RD)this->alpha[                 j ][i   ] + 
				                  (RD)this->beta [this->trellis[6][j]][i +1] + 
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);
			else
				max1 = MAP2(max1, (RD)this->alpha[                 j ][i   ] + 
				                  (RD)this->beta [this->trellis[8][j]][i +1] - 
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		for (auto j = 0; j < this->n_states; j++)
			if (this->trellis[4][j] == +1)
				max0 = MAP2(max0, (RD)this->alpha[                 j ][i   ] + 
				                  (RD)this->beta [this->trellis[6][j]][i +1] + 
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);
			else
				max1 = MAP2(max1, (RD)this->alpha[                 j ][i   ] + 
				                  (RD)this->beta [this->trellis[8][j]][i +1] - 
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		ext[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - sys[i];
	}
}

template <typename B, typename R, typename RD, proto_map<R> MAP1, proto_map<RD> MAP2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAP1,MAP2>
::compute_beta_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext)
{
	// compute the first beta values [trellis backward traversal <-]
	R beta_prev[this->n_states];
	for (auto j = 0; j < this->n_states; j++)
		beta_prev[j] = this->alpha[j][0];
	for (auto i = this->K + this->n_ff -1; i >= this->K; i--)
	{
		R beta_cur[this->n_states];
		for (auto j = 0; j < this->n_states; j++)
			beta_cur[j] = MAP1(
				beta_prev[this->trellis[6][j]] + this->gamma[this->trellis[7][j]][i],
				beta_prev[this->trellis[8][j]] - this->gamma[this->trellis[9][j]][i]);

		RSC_BCJR_seq_generic_normalize<R>::apply(beta_cur, i, this->n_states);
	
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
				max0 = MAP2(max0, (RD)this->alpha[                 j ][i] + 
				                  (RD)beta_prev  [this->trellis[6][j]   ] + 
				                  (RD)this->gamma[this->trellis[7][j]][i]);
			else
				max1 = MAP2(max1, (RD)this->alpha[                 j ][i] + 
				                  (RD)beta_prev  [this->trellis[8][j]   ] - 
				                  (RD)this->gamma[this->trellis[9][j]][i]);

		for (auto j = 1; j < this->n_states; j++)
			if (this->trellis[4][j] == 1)
				max0 = MAP2(max0, (RD)this->alpha[                 j ][i] + 
				                  (RD)beta_prev  [this->trellis[6][j]   ] + 
				                  (RD)this->gamma[this->trellis[7][j]][i]);
			else
				max1 = MAP2(max1, (RD)this->alpha[                 j ][i] + 
				                  (RD)beta_prev  [this->trellis[8][j]   ] - 
				                  (RD)this->gamma[this->trellis[9][j]][i]);

		ext[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - sys[i];

		// compute the beta values
		R beta_cur[this->n_states];
		for (auto j = 0; j < this->n_states; j++)
			beta_cur[j] = MAP1(
				beta_prev[this->trellis[6][j]] + this->gamma[this->trellis[7][j]][i],
				beta_prev[this->trellis[8][j]] - this->gamma[this->trellis[9][j]][i]);

		RSC_BCJR_seq_generic_normalize<R>::apply(beta_cur, i, this->n_states);
	
		for (auto j = 0; j < this->n_states; j++)
			beta_prev[j] = beta_cur[j];
	}
}

template <typename B, typename R, typename RD, proto_map<R> MAP1, proto_map<RD> MAP2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAP1,MAP2>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	this->compute_gamma   (sys, par);
	this->compute_alpha   (        );
	// this->compute_beta (        );
	// this->compute_ext  (sys, ext);
	this->compute_beta_ext(sys, ext);
}