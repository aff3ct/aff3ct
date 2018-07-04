#include <chrono>
#include <sstream>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Exception/exception.hpp"

#include "Decoder_BCH_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_BCH_std<B, R>
::Decoder_BCH_std(const int& K, const int& N, const tools::BCH_polynomial_generator &GF_poly, const int n_frames)
: Decoder         (K, N,                  n_frames, 1),
  Decoder_BCH<B,R>(K, N, GF_poly.get_t(), n_frames),
  t2(2 * this->t),
  elp(this->N_p2+2, std::vector<int>(this->N_p2)), discrepancy(this->N_p2+2), l(this->N_p2+2), u_lu(this->N_p2+2), s(t2+1), loc(this->t +1), reg(this->t +1),
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
Decoder_BCH_std<B, R>
::~Decoder_BCH_std()
{
}

template <typename B, typename R>
void Decoder_BCH_std<B, R>
::_decode(B *Y_N, const int frame_id)
{
	int i, j, syn_error = 0;

	/* first form the syndromes */
	for (i = 1; i <= t2; i++)
	{
		s.at(i) = 0;
		for (j = 0; j < this->N; j++)
			if (Y_N[j] != 0)
				s.at(i) ^= alpha_to.at((i * j) % this->N_p2);
		if (s.at(i) != 0)
			syn_error = 1; /* set error flag if non-zero syndrome */
		/*
		 * Note:    If the code is used only for ERROR DETECTION, then
		 *          exit program here indicating the presence of errors.
		 */
		/* convert syndrome from polynomial form to index form  */
		s.at(i) = index_of.at(s.at(i));
	}

	this->last_is_codeword = !syn_error;


	if (syn_error)
	{ /* if there are errors, try to correct them */
		/*
		 * Compute the error location polynomial via the Berlekamp
		 * iterative algorithm. Following the terminology of Lin and
		 * Costello's book :   d.at(u) is the 'mu'th discrepancy, where
		 * u='mu'+1 and 'mu' (the Greek letter!) is the step number
		 * ranging from -1 to 2*this->t (see L&C),  l.at(u) is the degree of
		 * the elp at that step, and u_l.at(u) is the difference between
		 * the step number and the degree of the elp.
		 */
		/* initialise table entries */
		discrepancy.at(0) = 0; /* index form */
		discrepancy.at(1) = s.at(1); /* index form */
		elp.at(0).at(0) = 0; /* index form */
		elp.at(1).at(0) = 1; /* polynomial form */
		for (i = 1; i < t2; i++)
		{
			elp.at(0).at(i) = -1; /* index form */
			elp.at(1).at(i) = 0; /* polynomial form */
		}
		l.at(0) = 0;
		l.at(1) = 0;
		u_lu.at(0) = -1;
		u_lu.at(1) = 0;

		int q, u = 0;
		do
		{
			u++;
			if (discrepancy.at(u) == -1)
			{
				l.at(u + 1) = l.at(u);
				for (i = 0; i <= l.at(u); i++)
				{
					elp.at(u + 1).at(i) = elp.at(u).at(i);
					elp.at(u).at(i) = index_of.at(elp.at(u).at(i));
				}
			}
			else
			/*
			 * search for words with greatest u_lu.at(q) for
			 * which d.at(q)!=0
			 */
			{
				q = u - 1;
				while ((discrepancy.at(q) == -1) && (q > 0))
					q--;
				/* have found first non-zero d.at(q)  */
				if (q > 0)
				{
					j = q;
					do
					{
						j--;
						if ((discrepancy.at(j) != -1) && (u_lu.at(q) < u_lu.at(j)))
							q = j;
					}
					while (j > 0);
				}

				/*
				 * have now found q such that d.at(u)!=0 and
				 * u_lu.at(q) is maximum
				 */
				/* store degree of new elp polynomial */
				if (l.at(u) > l.at(q) + u - q)
					l.at(u + 1) = l.at(u);
				else
					l.at(u + 1) = l.at(q) + u - q;

				/* form new elp(x) */
				for (i = 0; i < t2; i++)
					elp.at(u + 1).at(i) = 0;
				for (i = 0; i <= l.at(q); i++)
					if (elp.at(q).at(i) != -1)
						elp.at(u + 1).at(i + u - q) =
							alpha_to.at((discrepancy.at(u) + this->N_p2 - discrepancy.at(q) + elp.at(q).at(i)) % this->N_p2);
				for (i = 0; i <= l.at(u); i++)
				{
					elp.at(u + 1).at(i) ^= elp.at(u).at(i);
					elp.at(u).at(i) = index_of.at(elp.at(u).at(i));
				}
			}
			u_lu.at(u + 1) = u - l.at(u + 1);

			/* form (u+1)th discrepancy */
			if (u < t2)
			{
				/* no discrepancy computed on last iteration */
				if (s.at(u + 1) != -1)
					discrepancy.at(u + 1) = alpha_to.at(s.at(u + 1));
				else
					discrepancy.at(u + 1) = 0;
				for (i = 1; i <= l.at(u + 1); i++)
					if ((s.at(u + 1 - i) != -1) && (elp.at(u + 1).at(i) != 0))
						discrepancy.at(u + 1) ^= alpha_to.at((s.at(u + 1 - i) + index_of.at(elp.at(u + 1).at(i))) % this->N_p2);
				/* put d.at(u+1) into index form */
				discrepancy.at(u + 1) = index_of.at(discrepancy.at(u + 1));
			}
		}
		while ((u < t2) && (l.at(u + 1) <= this->t));

		u++;
		if (l.at(u) <= this->t)
		{/* Can correct errors */
			/* put elp into index form */
			for (i = 0; i <= l.at(u); i++)
				elp.at(u).at(i) = index_of.at(elp.at(u).at(i));

			/* Chien search: find roots of the error location polynomial */
			for (i = 1; i <= l.at(u); i++)
				reg.at(i) = elp.at(u).at(i);

			int count = 0;
			for (i = 1; i <= this->N_p2; i++)
			{
				q = 1;
				for (j = 1; j <= l.at(u); j++)
					if (reg.at(j) != -1)
					{
						reg.at(j) = (reg.at(j) + j) % this->N_p2;
						q ^= alpha_to.at(reg.at(j));
					}
				if (!q)
				{ /* store root and error
				   * location number indices */
					loc.at(count) = this->N_p2 - i;
					count++;
				}
			}

			if (count == l.at(u))
			{
				this->last_is_codeword = true;

				/* no. roots = degree of elp hence <= this->t errors */
				for (i = 0; i < l.at(u); i++)
					if (loc.at(i) < this->N)
						Y_N[loc.at(i)] ^= 1;
			}
		}
	}
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
