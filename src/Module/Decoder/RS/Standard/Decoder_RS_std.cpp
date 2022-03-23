#include <string>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/RS/Standard/Decoder_RS_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_RS_std<B,R>
::Decoder_RS_std(const int& K, const int& N, const tools::RS_polynomial_generator &GF)
: Decoder_RS<B,R>(K, N, GF                                      ),
  t2             (2 * this->t                                   ),
  elp            (this->N_p2_1+2, std::vector<int>(this->N_p2_1)),
  discrepancy    (this->N_p2_1+2                                ),
  l              (this->N_p2_1+2                                ),
  u_lu           (this->N_p2_1+2                                ),
  s              (t2+1                                          ),
  loc            (this->t +1                                    ),
  root           (this->t +1                                    ),
  reg            (this->t +1                                    ),
  z              (this->t +1                                    ),
  err            (this->N_p2_1                                  )
{
	const std::string name = "Decoder_RS_std";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_RS_std<B,R>* Decoder_RS_std<B,R>
::clone() const
{
	auto m = new Decoder_RS_std(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
int Decoder_RS_std<B,R>
::_decode(S *Y_N, const size_t frame_id)
{
	bool syn_error = false;

	// first form the syndromes
	for (auto i = 1; i <= t2; i++)
	{
		s[i] = 0;
		for (auto j = 0; j < this->N_rs; j++)
		{
			auto y_idx = this->index_of[Y_N[j]];
			if (y_idx != -1)
				s[i] ^= this->alpha_to[(y_idx + i * j) % this->N_p2_1];
		}

		syn_error |= s[i] != 0; // set error flag if non-zero syndrome

		s[i] = this->index_of[s[i]]; // convert syndrome from polynomial form to index form
	}

	this->last_is_codeword = !syn_error;


	if (syn_error) // if there are errors, try to correct them
	{
		/*
		 * Compute the error location polynomial via the Berlekamp
		 * iterative algorithm. Following the terminology of Lin and
		 * Costello's book :   discrepancy[u] is the 'mu'th discrepancy, where
		 * u='mu'+1 and 'mu' (the Greek letter!) is the step number
		 * ranging from -1 to 2*t (see L&C),  l[u] is the degree of
		 * the elp at that step, and u_l[u] is the difference between
		 * the step number and the degree of the elp.
		 */
		// initialise table entries
		discrepancy[0] = 0; // index form
		discrepancy[1] = s[1]; // index form
		elp[0][0] = 0; // index form
		elp[1][0] = 1; // polynomial form
		for (auto i = 1; i < t2; i++)
		{
			elp[0][i] = -1; // index form
			elp[1][i] = 0; // polynomial form
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
				for (auto i = 0; i <= l[u]; i++)
				{
					elp[u + 1][i] = elp[u][i];
					elp[u][i] = this->index_of[elp[u][i]];
				}
			}
			else
			{ // search for words with greatest u_lu[q] for which discrepancy[q]!=0
				q = u - 1;
				while ((discrepancy[q] == -1) && (q > 0))
					q--;

				// have found first non-zero discrepancy[q]
				if (q > 0)
				{
					int j = q;
					do
					{
						j--;
						if ((discrepancy[j] != -1) && (u_lu[q] < u_lu[j]))
							q = j;
					}
					while (j > 0);
				}

				// have now found q such that discrepancy[u]!=0 and u_lu[q] is maximum
				// store degree of new elp polynomial
				if (l[u] > l[q] + u - q)
					l[u + 1] = l[u];
				else
					l[u + 1] = l[q] + u - q;

				// form new elp(x)
				for (auto i = 0; i < t2; i++)
					elp[u + 1][i] = 0;

				for (auto i = 0; i <= l[q]; i++)
					if (elp[q][i] != -1)
						elp[u + 1][i + u - q] = this->alpha_to[(discrepancy[u] + this->N_p2_1 - discrepancy[q] + elp[q][i])
						                                       % this->N_p2_1];

				for (auto i = 0; i <= l[u]; i++)
				{
					elp[u + 1][i] ^= elp[u][i];
					elp[u][i] = this->index_of[elp[u][i]];
				}
			}
			u_lu[u + 1] = u - l[u + 1];

			// form (u+1)th discrepancy
			if (u < t2)
			{
				// no discrepancy computed on last iteration
				if (s[u + 1] != -1)
					discrepancy[u + 1] = this->alpha_to[s[u + 1]];
				else
					discrepancy[u + 1] = 0;

				for (auto i = 1; i <= l[u + 1]; i++)
					if (s[u + 1 - i] != -1 && elp[u + 1][i] != 0)
						discrepancy[u + 1] ^= this->alpha_to[(s[u + 1 - i] + this->index_of[elp[u + 1][i]])
						                                      % this->N_p2_1];

				// put discrepancy[u+1] into index form
				discrepancy[u + 1] = this->index_of[discrepancy[u + 1]];
			}
		}
		while ((u < t2) && (l[u + 1] <= this->t));


		u++;
		if (l[u] <= this->t) // Can correct errors
		{
			// put elp into index form
			for (auto i = 0; i <= l[u]; i++)
				elp[u][i] = this->index_of[elp[u][i]];

			// Chien search: find roots of the error location polynomial
			for (auto i = 1; i <= l[u]; i++)
				reg[i] = elp[u][i];

			int count = 0;
			for (auto i = 1; i <= this->N_p2_1; i++)
			{
				q = 1;
				for (auto j = 1; j <= l[u]; j++)
					if (reg[j] != -1)
					{
						reg[j] = (reg[j] + j) % this->N_p2_1;
						q ^= this->alpha_to[reg[j]];
					}

				if (!q)
				{ // store root and error location number indices
					root[count] = i;
					loc [count] = this->N_p2_1 - i;
					count++;
				}
			}


			if (count == l[u]) // no. roots = degree of elp hence <= t errors
			{
				this->last_is_codeword = true;

				// form polynomial z(x)
				for (auto i = 1; i <= l[u]; i++) // Z[0] = 1 always - do not need
				{
					if (s[i] != -1 && elp[u][i] != -1)
						z[i] = this->alpha_to[s[i]] ^ this->alpha_to[elp[u][i]];
					else if (s[i] != -1 && elp[u][i] == -1)
						z[i] = this->alpha_to[s[i]];
					else if (s[i] == -1 && elp[u][i] != -1)
						z[i] = this->alpha_to[elp[u][i]];
					else
						z[i] = 0;

					for (auto j = 1; j < i; j++)
						if (s[j] != -1 && elp[u][i - j] != -1)
							z[i] ^= this->alpha_to[(elp[u][i - j] + s[j]) % this->N_p2_1];

					z[i] = this->index_of[z[i]]; // put into index form
				}

				// evaluate errors at locations given by error location numbers loc[i]
				std::fill(err.begin(), err.end(), 0);

				for (auto i = 0; i < l[u]; i++) // compute numerator of error term first
				{
					err[loc[i]] = 1; // accounts for z[0]
					for (auto j = 1; j <= l[u]; j++)
						if (z[j] != -1)
							err[loc[i]] ^= this->alpha_to[(z[j] + j * root[i]) % this->N_p2_1];

					if (err[loc[i]] != 0)
					{
						err[loc[i]] = this->index_of[err[loc[i]]];
						q = 0; // form denominator of error term

						for (auto j = 0; j < l[u]; j++)
							if (j != i)
								q += this->index_of[1 ^ this->alpha_to[(loc[j] + root[i]) % this->N_p2_1]];

						q = q % this->N_p2_1;
						err[loc[i]]  = this->alpha_to[(err[loc[i]] - q + this->N_p2_1) % this->N_p2_1];
						Y_N[loc[i]] ^= err[loc[i]]; // Y_N[i] must be in polynomial form
					}
				}
				syn_error = false;
			}
			// else // no. roots != degree of elp => over t errors and cannot solve
		}
		// else // elp has degree has degree > t hence cannot solve
	}
	// else // no non-zero syndromes => no errors: output received codeword

	return (int)syn_error;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_RS_std<B_8,Q_8>;
template class aff3ct::module::Decoder_RS_std<B_16,Q_16>;
template class aff3ct::module::Decoder_RS_std<B_32,Q_32>;
template class aff3ct::module::Decoder_RS_std<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_RS_std<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
