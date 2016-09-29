#include "Decoder_RSC_BCJR_seq_generic_std.hpp"

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::Decoder_RSC_BCJR_seq_generic_std(const int &K, 
                                   const std::vector<std::vector<int>> &trellis, 
                                   const bool buffered_encoding,
                                   const std::string name)
: Decoder_RSC_BCJR_seq_generic<B,R>(K, trellis, buffered_encoding, name)
{
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::~Decoder_RSC_BCJR_seq_generic_std()
{
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
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

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_gamma(const R *sys, const R *par, const R *tail_sys, const R *tail_par)
{
	for (auto i = 0; i < this->K; i++)
	{
		// there is a big loss of precision here in fixed point
		this->gamma[0][i] = RSC_BCJR_seq_generic_div_or_not<R>::apply(sys[i] + par[i]);
		// there is a big loss of precision here in fixed point
		this->gamma[1][i] = RSC_BCJR_seq_generic_div_or_not<R>::apply(sys[i] - par[i]);
	}

	for (auto i = 0; i < this->n_ff; i++)
	{
		// there is a big loss of precision here in fixed point
		this->gamma[0][this->K +i] = RSC_BCJR_seq_generic_div_or_not<R>::apply(tail_sys[i] + tail_par[i]);
		// there is a big loss of precision here in fixed point
		this->gamma[1][this->K +i] = RSC_BCJR_seq_generic_div_or_not<R>::apply(tail_sys[i] - tail_par[i]);
	}
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
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

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_beta()
{
	// compute beta values [trellis backward traversal <-]
	for (auto i = this->K + this->n_ff -1; i >= 1; i--)
	{
		for (auto j = 0; j < this->n_states; j++)
			this->beta[j][i] = MAX1(
				this->beta[this->trellis[6][j]][i +1] + this->gamma[this->trellis[7 ][j]][i],
				this->beta[this->trellis[8][j]][i +1] - this->gamma[this->trellis[9][j]][i]);

		RSC_BCJR_seq_generic_normalize<R>::apply(this->beta, i, this->n_states);
	}
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext)
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

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_beta_ext(const mipp::vector<R> &sys, mipp::vector<R> &ext)
{
	// compute the first beta values [trellis backward traversal <-]
#ifndef _MSC_VER
	R beta_prev[this->n_states];
#else
	R beta_prev[128];
#endif
	for (auto j = 0; j < this->n_states; j++)
		beta_prev[j] = this->alpha[j][0];
	for (auto i = this->K + this->n_ff -1; i >= this->K; i--)
	{
#ifdef _MSC_VER
		R beta_cur[128];
#else
		R beta_cur[this->n_states];
#endif
		for (auto j = 0; j < this->n_states; j++)
			beta_cur[j] = MAX1(
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
#ifdef _MSC_VER
		R beta_cur[128];
#else
		R beta_cur[this->n_states];
#endif
		for (auto j = 0; j < this->n_states; j++)
			beta_cur[j] = MAX1(
				beta_prev[this->trellis[6][j]] + this->gamma[this->trellis[7][j]][i],
				beta_prev[this->trellis[8][j]] - this->gamma[this->trellis[9][j]][i]);

		RSC_BCJR_seq_generic_normalize<R>::apply(beta_cur, i, this->n_states);
	
		for (auto j = 0; j < this->n_states; j++)
			beta_prev[j] = beta_cur[j];
	}
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_ext_sys(const R *sys, R *ext_sys)
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

		ext_sys[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - sys[i];
	}
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::compute_ext_par(const R *par, R *ext_par)
{
//	 // compute extrinsic values
//	 for (auto i = 0; i < this->K; i++)
//	 {
//	 	RD max0 = -std::numeric_limits<RD>::max();
//	 	RD max1 = -std::numeric_limits<RD>::max();
//
//	 	for (auto j = 0; j < this->n_states; j++)
//	 		if ((this->trellis[1][j] == +1 && this->trellis[2][j] == 0) ||
//	 			(this->trellis[1][j] == -1 && this->trellis[2][j] == 1))
//	 			max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
//	 			                  (RD)this->beta [this->trellis[6][j]][i +1] +
//	 			                  (RD)this->gamma[this->trellis[7][j]][i   ]);
//	 		else
//	 			max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
//	 			                  (RD)this->beta [this->trellis[8][j]][i +1] -
//	 			                  (RD)this->gamma[this->trellis[9][j]][i   ]);
//
//	 	for (auto j = 0; j < this->n_states; j++)
//	 		if ((this->trellis[4][j] == +1 && this->trellis[5][j] == 0) ||
//	 			(this->trellis[4][j] == -1 && this->trellis[5][j] == 1))
//	 			max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
//	 			                  (RD)this->beta [this->trellis[6][j]][i +1] +
//	 			                  (RD)this->gamma[this->trellis[7][j]][i   ]);
//	 		else
//	 			max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
//	 			                  (RD)this->beta [this->trellis[8][j]][i +1] -
//	 			                  (RD)this->gamma[this->trellis[9][j]][i   ]);
//
//	 	ext_par[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - par[i];
//	 }
	// compute extrinsic values
	for (auto i = 0; i < this->K; i++)
	{
		RD max0 = -std::numeric_limits<RD>::max();
		RD max1 = -std::numeric_limits<RD>::max();
		
		for (auto j = 0; j < this->n_states; j++)
			if      (this->trellis[1][j] == +1 && this->trellis[7][j] == 0 )
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

			else if (this->trellis[1][j] == +1 && this->trellis[7][j] == 1 )
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

			else if (this->trellis[1][j] == -1 && this->trellis[9][j] == 1 )
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

			else if (this->trellis[1][j] == -1  && this->trellis[9][j] == 0 )
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		for (auto j = 0; j < this->n_states; j++)
			if      (this->trellis[4][j] == +1 && this->trellis[7][j] == 0 )
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

			else if (this->trellis[4][j] == +1 && this->trellis[7][j] == 1 )
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[6][j]][i +1] +
				                  (RD)this->gamma[this->trellis[7][j]][i   ]);

			else if (this->trellis[4][j] == -1 && this->trellis[9][j] == 1 )
				max0 = MAX2(max0, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

			else if (this->trellis[4][j] == -1  && this->trellis[9][j] == 0 )
				max1 = MAX2(max1, (RD)this->alpha[                 j ][i   ] +
				                  (RD)this->beta [this->trellis[8][j]][i +1] -
				                  (RD)this->gamma[this->trellis[9][j]][i   ]);

		ext_par[i] = RSC_BCJR_seq_generic_post<R,RD>::compute(max0 - max1) - par[i];
	}
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	this->compute_gamma   (sys, par);
	this->compute_alpha   (        );
	// this->compute_beta (        );
	// this->compute_ext  (sys, ext);
	this->compute_beta_ext(sys, ext);
}

template <typename B, typename R, typename RD, proto_max<R> MAX1, proto_max<RD> MAX2>
void Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>
::decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	assert(this->buffered_encoding);
	assert((int) Y_N1.size() == 2 * this->K + this->tail_length());
	assert(Y_N1.size()       == Y_N2.size());

	const R* sys          = Y_N1.data();
	const R* par          = Y_N1.data() + 1 * this->K;
	const R* tail_sys     = Y_N1.data() + 2 * this->K + this->n_ff;
	const R* tail_par     = Y_N1.data() + 2 * this->K;
	      R* ext_sys      = Y_N2.data();
	      R* ext_par      = Y_N2.data() + 1 * this->K;

	this->compute_gamma  (sys, par, tail_sys, tail_par);
	this->compute_alpha  (                            );
	this->compute_beta   (                            );
	this->compute_ext_sys(sys, ext_sys                );
	this->compute_ext_par(par, ext_par                );
}
