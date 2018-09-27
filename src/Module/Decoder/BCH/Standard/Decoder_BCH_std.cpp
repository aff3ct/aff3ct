#include <chrono>
#include <sstream>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Exception/exception.hpp"

#include "Decoder_BCH_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_BCH_std<B, R>
::Decoder_BCH_std(const int& K, const int& N, const tools::BCH_polynomial_generator<B> &GF_poly, const int n_frames)
: Decoder         (K, N,                  n_frames, 1),
  Decoder_BCH<B,R>(K, N, GF_poly.get_t(), n_frames),
  t2(2 * this->t), YH_N(N),
  elp(this->N_p2_1+2, std::vector<int>(this->N_p2_1)), discrepancy(this->N_p2_1+2), l(this->N_p2_1+2), u_lu(this->N_p2_1+2), s(t2+1), loc(this->t +1), reg(this->t +1),
  m(GF_poly.get_m()), d(GF_poly.get_d()), alpha_to(GF_poly.get_alpha_to()), index_of(GF_poly.get_index_of())
{
	const std::string name = "Decoder_BCH_std";
	this->set_name(name);

	if ((this->N - this->K) != GF_poly.get_n_rdncy())
	{
		std::stringstream message;
		message << "'N - K' is different than 'GF_poly.get_n_rdncy()' ('K' = " << K << ", 'N' = " << N
		        << ", 'GF_poly.get_n_rdncy()' = " << GF_poly.get_n_rdncy() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
void Decoder_BCH_std<B, R>
::_decode(B *Y_N, const int frame_id)
{
	int i, j, syn_error = 0;

	/* first form the syndromes */
	for (i = 1; i <= t2; i++)
	{
		s[i] = 0;
		for (j = 0; j < this->N; j++)
			if (Y_N[j] != 0)
				s[i] ^= alpha_to[(i * j) % this->N_p2_1];
		if (s[i] != 0)
			syn_error = 1; /* set error flag if non-zero syndrome */
		/* convert syndrome from polynomial form to index form  */
		s[i] = index_of[s[i]];
	}

	this->last_is_codeword[frame_id] = !syn_error;


	if (syn_error)
	{ /* if there are errors, try to correct them */
		/*
		 * Compute the error location polynomial via the Berlekamp
		 * iterative algorithm. Following the terminology of Lin and
		 * Costello's book :   d[u] is the 'mu'th discrepancy, where
		 * u='mu'+1 and 'mu' (the Greek letter!) is the step number
		 * ranging from -1 to 2*this->t (see L&C),  l[u] is the degree of
		 * the elp at that step, and u_l[u] is the difference between
		 * the step number and the degree of the elp.
		 */
		/* initialise table entries */
		discrepancy[0] = 0; /* index form */
		discrepancy[1] = s[1]; /* index form */
		elp[0][0] = 0; /* index form */
		elp[1][0] = 1; /* polynomial form */
		for (i = 1; i < t2; i++)
		{
			elp[0][i] = -1; /* index form */
			elp[1][i] = 0; /* polynomial form */
		}
		l[0] = 0;
		l[1] = 0;
		u_lu[0] = -1;
		u_lu[1] = 0;

		int q, u = 0;
		do
		{
			u++;

			if (discrepancy[u] == -1)
			{
				l[u + 1] = l[u];
				for (i = 0; i <= l[u]; i++)
				{
					elp[u + 1][i] = elp[u][i];
					elp[u][i] = index_of[elp[u][i]];
				}
			}
			else
			{ // search for words with greatest u_lu[q] for which d[q]!=0
				q = u - 1;
				while ((discrepancy[q] == -1) && (q > 0))
					q--;
				/* have found first non-zero d[q]  */
				if (q > 0)
				{
					j = q;
					do
					{
						j--;
						if ((discrepancy[j] != -1) && (u_lu[q] < u_lu[j]))
							q = j;
					}
					while (j > 0);
				}

				/*
				 * have now found q such that d[u]!=0 and
				 * u_lu[q] is maximum
				 */
				/* store degree of new elp polynomial */
				if (l[u] > l[q] + u - q)
					l[u + 1] = l[u];
				else
					l[u + 1] = l[q] + u - q;

				/* form new elp(x) */
				for (i = 0; i < t2; i++)
					elp[u + 1][i] = 0;
				for (i = 0; i <= l[q]; i++)
					if (elp[q][i] != -1)
						elp[u + 1][i + u - q] =
							alpha_to[(discrepancy[u] + this->N_p2_1 - discrepancy[q] + elp[q][i]) % this->N_p2_1];
				for (i = 0; i <= l[u]; i++)
				{
					elp[u + 1][i] ^= elp[u][i];
					elp[u][i] = index_of[elp[u][i]];
				}
			}
			u_lu[u + 1] = u - l[u + 1];

			/* form (u+1)th discrepancy */
			if (u < t2)
			{
				/* no discrepancy computed on last iteration */
				if (s[u + 1] != -1)
					discrepancy[u + 1] = alpha_to[s[u + 1]];
				else
					discrepancy[u + 1] = 0;

				for (i = 1; i <= l[u + 1]; i++)
					if ((s[u + 1 - i] != -1) && (elp[u + 1][i] != 0))
						discrepancy[u + 1] ^= alpha_to[(s[u + 1 - i] + index_of[elp[u + 1][i]]) % this->N_p2_1];
				/* put d[u+1] into index form */
				discrepancy[u + 1] = index_of[discrepancy[u + 1]];
			}
		}
		while ((u < t2) && (l[u + 1] <= this->t));

		u++;
		if (l[u] <= this->t)
		{/* Can correct errors */
			/* put elp into index form */
			for (i = 0; i <= l[u]; i++)
				elp[u][i] = index_of[elp[u][i]];

			/* Chien search: find roots of the error location polynomial */
			for (i = 1; i <= l[u]; i++)
				reg[i] = elp[u][i];

			int count = 0;
			for (i = 1; i <= this->N_p2_1; i++)
			{
				q = 1;
				for (j = 1; j <= l[u]; j++)
					if (reg[j] != -1)
					{
						reg[j] = (reg[j] + j) % this->N_p2_1;
						q ^= alpha_to[reg[j]];
					}
				if (!q)
				{ /* store root and error
				   * location number indices */
					loc[count] = this->N_p2_1 - i;
					count++;
				}
			}

			if (count == l[u])
			{
				this->last_is_codeword[frame_id] = true;

				/* no. roots = degree of elp hence <= this->t errors */
				for (i = 0; i < l[u]; i++)
					if (loc[i] < this->N)
						Y_N[loc[i]] ^= 1;
			}
		}
	}
}


template <typename B, typename R>
void Decoder_BCH_std<B, R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
	std::copy(Y_N, Y_N + this->N, this->YH_N.begin());

	this->_decode(this->YH_N.data(), frame_id);

	std::copy(this->YH_N.data() + this->N - this->K, this->YH_N.data() + this->N, V_K);
}

template <typename B, typename R>
void Decoder_BCH_std<B, R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
	std::copy(Y_N, Y_N + this->N, this->YH_N.begin());

	this->_decode(this->YH_N.data(), frame_id);

	std::copy(this->YH_N.data(), this->YH_N.data() + this->N, V_N);
}

template <typename B, typename R>
void Decoder_BCH_std<B, R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	tools::hard_decide(Y_N, this->YH_N.data(), this->N);

	this->_decode(this->YH_N.data(), frame_id);

	std::copy(this->YH_N.data() + this->N - this->K, this->YH_N.data() + this->N, V_K);
}

template <typename B, typename R>
void Decoder_BCH_std<B, R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
	tools::hard_decide(Y_N, this->YH_N.data(), this->N);

	this->_decode(this->YH_N.data(), frame_id);

	std::copy(this->YH_N.data(), this->YH_N.data() + this->N, V_N);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_BCH_std<B_8,Q_8>;
template class aff3ct::module::Decoder_BCH_std<B_16,Q_16>;
template class aff3ct::module::Decoder_BCH_std<B_32,Q_32>;
template class aff3ct::module::Decoder_BCH_std<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_BCH_std<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
