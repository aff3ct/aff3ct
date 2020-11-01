#include <cmath>
#include <string>
#include <limits>
#include <algorithm>

#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_chase_pyndiah_fast<B,R>
::Decoder_chase_pyndiah_fast(const int K, const int N, // N includes the parity bit if any
                             const Decoder_BCH<B,R> &dec_,
                             const Encoder    <B  > &enc_,
                             const int n_least_reliable_positions_,
                             const int n_test_vectors_,
                             const int n_competitors_,
                             const std::vector<float>& cp_coef)
: Decoder_chase_pyndiah<B,R>(K, N, dec_, enc_, n_least_reliable_positions_, n_test_vectors_, n_competitors_, cp_coef)
{
	const std::string name = "Decoder_chase_pyndiah_fast";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_chase_pyndiah_fast<B,R>* Decoder_chase_pyndiah_fast<B,R>
::clone() const
{
	auto m = new Decoder_chase_pyndiah_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_chase_pyndiah_fast<B,R>
::compute_metrics(const R* Y_N)
{
	const auto N_loop_size = (this->N / mipp::nElReg<B>()) * mipp::nElReg<B>();

	std::fill(this->metrics.begin(), this->metrics.end(), (R)0);

	this->n_good_competitors = 0;
	for (int c = 0; c < this->n_test_vectors; c++)
	{
		if (this->is_wrong[c])
		{
			this->metrics[c] = std::numeric_limits<R>::max()/2;
			continue;
		}
		this->n_good_competitors++;

		mipp::Reg<R> r_metric = (R)0;
		const mipp::Reg<R> r_zero = (R)0;
		int tv_off = c * this->N;

		for (auto i = 0; i < N_loop_size; i += mipp::nElReg<R>())
		{
			const mipp::Reg<R> r_yn = &Y_N[i];
			const mipp::Reg<B> r_yh = &this->hard_Y_N[i];
			const mipp::Reg<B> r_tv = &this->test_vect[tv_off + i];

			r_metric += mipp::blend(mipp::abs(r_yn), r_zero, r_yh != r_tv);
		}

		this->metrics[c] = mipp::hadd(r_metric);

		for (int i = N_loop_size; i < this->N; i++)
			if (this->hard_Y_N[i] ^ this->test_vect[tv_off + i])
				this->metrics[c] += std::abs(Y_N[i]);
	}

	// reorder this->metrics -> decided word is at first position of this->competitors list
	for (int c = 0; c < this->n_test_vectors; c++)
	{
		this->competitors[c].metric = this->metrics[c];
		this->competitors[c].pos    = c*this->N;
	}

	using I = typename Decoder_chase_pyndiah<B,R>::info; // trick to avoid g++4.x compilation error

	std::sort(this->competitors.begin(), this->competitors.end(),
	          [](const I& a,
	             const I& b) { return a.metric < b.metric; });
}

template <typename B, typename R>
void Decoder_chase_pyndiah_fast<B,R>
::compute_reliability(const R* Y_N1, R* Y_N2)
{
	const auto & DW = this->competitors.front(); // the Decided Word

	R beta_applied = this->beta;

	if (!this->beta_is_set)
	{
		// compute beta, the sum of the least reliable position reliabilities in the decided word
		beta_applied = 0;
		int max_sum = this->cp_coef[4] ? std::min((int)this->cp_coef[4], this->n_least_reliable_positions) : this->n_least_reliable_positions;
		for (int i = 0; i < max_sum; i++)
			beta_applied += this->least_reliable_pos[i].metric;

		beta_applied -= (R)this->cp_coef[2] * DW.metric;
	}


	this->n_good_competitors = std::min(this->n_good_competitors, this->n_competitors); // if there is less than 'this->n_competitors' good this->competitors
	                                                                  // then take only them for reliability calculation

	for (int j = 1; j < this->n_good_competitors; j++)
		this->competitors[j].metric = (this->competitors[j].metric - DW.metric) * (R)this->cp_coef[1];



	const auto N_loop_size = (this->N / mipp::nElReg<B>()) * mipp::nElReg<B>();
	const mipp::Reg<R> r_zero = (R)0;
	const mipp::Reg<R> r_beta = beta_applied;


	if (this->beta_is_set)
	{
		for (auto i = 0; i < N_loop_size; i += mipp::nElReg<R>())
		{
			const mipp::Reg<B> r_DB = &this->test_vect[DW.pos + i];
			const mipp::Reg<R> r_Y1 = &Y_N1[i];

			mipp::Reg<R> r_reliability = r_beta;


			for (B j = this->n_good_competitors -1; j > 0; j--) // j = 0  == DW
			{
				const mipp::Reg<B> r_comp_bit = &this->test_vect[this->competitors[j].pos + i];
				const mipp::Reg<R> r_comp_met = this->competitors[j].metric;

				r_reliability = mipp::blend(r_reliability, r_comp_met, r_comp_bit == r_DB);
			}

			r_reliability = mipp::neg(r_reliability, r_DB != (B)0);
			r_reliability -= r_Y1 * (R)this->cp_coef[0];

			r_reliability.store(&Y_N2[i]);
		}
	}
	else
	{
		for (auto i = 0; i < N_loop_size; i += mipp::nElReg<R>())
		{
			const mipp::Reg<B> r_DB = &this->test_vect[DW.pos + i];
			const mipp::Reg<R> r_Y1 = &Y_N1[i];

			mipp::Reg<R> r_reliability = mipp::abs(r_Y1) * (R)this->cp_coef[3] + r_beta; // reliability when no competitor with different bit
			r_reliability = mipp::blend(r_zero, r_reliability, r_reliability < (R)0);


			for (B j = this->n_good_competitors -1; j > 0; j--) // j = 0  == DW
			{
				const mipp::Reg<B> r_comp_bit = &this->test_vect[this->competitors[j].pos + i];
				const mipp::Reg<R> r_comp_met = this->competitors[j].metric;

				r_reliability = mipp::blend(r_reliability, r_comp_met, r_comp_bit == r_DB);
			}

			r_reliability = mipp::neg(r_reliability, r_DB != (B)0);
			r_reliability -= r_Y1 * (R)this->cp_coef[0];

			r_reliability.store(&Y_N2[i]);
		}
	}


	for (int i = N_loop_size; i < this->N; i++)
	{
		const auto DB = this->test_vect[DW.pos + i]; // decided bit at the position i

		int j = 1;
		for (; j < this->n_good_competitors; j++)
			if (this->test_vect[this->competitors[j].pos + i] != DB)
				break;

		R reliability;

		if (j < this->n_good_competitors) // then the competitor j has a different bit at the position i
		{
			reliability = this->competitors[j].metric;
		}
		else if (this->beta_is_set)
		{
			reliability = this->beta;
		}
		else // same bits for each candidates
		{
			reliability = beta_applied + (R)this->cp_coef[3] * std::abs(Y_N1[i]);
			if (reliability < 0)
				reliability = 0;
		}

		if (DB) // if DB is a 1
			reliability = -reliability; // set as negative

		Y_N2[i] = reliability - (R)this->cp_coef[0] * Y_N1[i];
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_chase_pyndiah_fast<B_8,Q_8>;
template class aff3ct::module::Decoder_chase_pyndiah_fast<B_16,Q_16>;
template class aff3ct::module::Decoder_chase_pyndiah_fast<B_32,Q_32>;
template class aff3ct::module::Decoder_chase_pyndiah_fast<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_chase_pyndiah_fast<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
