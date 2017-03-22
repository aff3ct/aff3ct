#include "Decoder_BCH.hpp"

using namespace aff3ct::module;

template <typename B, typename R>
Decoder_BCH<B, R>
::Decoder_BCH(const int& K, const int& N, const int& m, const int&t, const tools::Galois &GF, const int n_frames,
              const std::string name)
 : Decoder<B,R>(K, N, n_frames, 1, name),
   elp(N+2), discrepancy(N+2), l(N+2), u_lu(N+2), s(N+1), loc(200), reg(201), K(K), N(N), m(m), t(t), d(2*t+1), alpha_to(N+1), index_of(N+1), YH_N(N), Y_N(N), V_K(K)
{	
	alpha_to = GF.alpha_to;
	index_of = GF.index_of;
	for (auto i = 0; i < N; i++)
	{
		elp[i].resize(N);
	}
	assert( K <= N );
	assert( K > 3 );
}

template <typename B, typename R>
Decoder_BCH<B, R>
::~Decoder_BCH()
{
}

template <typename B, typename R>
void Decoder_BCH<B, R>
::load(const mipp::vector<R>& Y_N)
{
	this->Y_N = Y_N;
	for (int j = 0; j < N; j++)
		this->YH_N[j] = (Y_N[j] > 0)? 0 : 1; // hard decision on the input
}

template <typename B, typename R>
void Decoder_BCH<B, R>
::_hard_decode()
{
	register int i, j, u, q, t2, count = 0, syn_error = 0;


	/*clog << "YH_N bef cor:";
	for (int i = 0; i < N; i++)
		clog << " " << YH_N[i];
	clog << endl;*/
	
	t2 = 2 * t;
	
	/* first form the syndromes */
	//printf("S(x) = ");
	for (i = 1; i <= t2; i++)
	{
		s[i] = 0;
		for (j = 0; j < N; j++)
			if (YH_N[j] != 0)
				s[i] ^= alpha_to[(i * j) % N];
		if (s[i] != 0)
			syn_error = 1; /* set error flag if non-zero syndrome */
		/*
		 * Note:    If the code is used only for ERROR DETECTION, then
		 *          exit program here indicating the presence of errors.
		 */
		/* convert syndrome from polynomial form to index form  */
		s[i] = index_of[s[i]];
		//printf("%3d ", s[i]);
	}
	//printf("\n");

	if (syn_error)
	{ /* if there are errors, try to correct them */
		/*
		 * Compute the error location polynomial via the Berlekamp
		 * iterative algorithm. Following the terminology of Lin and
		 * Costello's book :   d[u] is the 'mu'th discrepancy, where
		 * u='mu'+1 and 'mu' (the Greek letter!) is the step number
		 * ranging from -1 to 2*t (see L&C),  l[u] is the degree of
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
		u = 0;

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
				/*
				 * search for words with greatest u_lu[q] for
				 * which d[q]!=0 
				 */
			{
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
							alpha_to[(discrepancy[u] + N - discrepancy[q] + elp[q][i]) % N];
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
						discrepancy[u + 1] ^= alpha_to[(s[u + 1 - i]
							+ index_of[elp[u + 1][i]]) % N];
				/* put d[u+1] into index form */
				discrepancy[u + 1] = index_of[discrepancy[u + 1]];
			}
		}
		while ((u < t2) && (l[u + 1] <= t));

		u++;
		if (l[u] <= t)
		{/* Can correct errors */
			/* put elp into index form */
			for (i = 0; i <= l[u]; i++)
				elp[u][i] = index_of[elp[u][i]];

			//printf("sigma(x) = ");
			for (i = 0; i <= l[u]; i++)
			//printf("%3d ", elp[u][i]);
			//printf("\n");
			//printf("Roots: ");

			/* Chien search: find roots of the error location polynomial */
			for (i = 1; i <= l[u]; i++)
				reg[i] = elp[u][i];
			count = 0;
			for (i = 1; i <= N; i++)
			{
				q = 1;
				for (j = 1; j <= l[u]; j++)
					if (reg[j] != -1)
					{
						reg[j] = (reg[j] + j) % N;
						q ^= alpha_to[reg[j]];
					}
				if (!q)
				{ /* store root and error
						 * location number indices */
					loc[count] = N - i;
					count++;
					//printf("%3d ", N - i);
				}
			}
			//printf("\n");
			//std::clog << "before correction, YH_N=" << std::endl;
			//this->display_bit_vector(YH_N);
			if (count == l[u])
				/* no. roots = degree of elp hence <= t errors */
				for (i = 0; i < l[u]; i++)
					YH_N[loc[i]] ^= 1;

			//std::clog << "after correction, YH_N=" << std::endl;
			//this->display_bit_vector(YH_N);
			//else /* elp has degree >t hence cannot solve */
				//printf("Incomplete decoding: errors detected\n");
		}
	}
	/*clog << "YH_N aft cor:";
	for (int i = 0; i < N; i++)
		clog << " " << YH_N[i];
	clog << endl;*/
	
	for (i = 0; i < K; i++)
		this->V_K[i] = YH_N[i+N-K];
}

template <typename B, typename R>
void Decoder_BCH<B, R>
::store(mipp::vector<B>& V_K) const
{
	V_K = this->V_K;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_BCH<B_8,Q_8>;
template class aff3ct::module::Decoder_BCH<B_16,Q_16>;
template class aff3ct::module::Decoder_BCH<B_32,Q_32>;
template class aff3ct::module::Decoder_BCH<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_BCH<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
