#include <cassert>
#include <iostream>

#include "Tools/Display/bash_tools.h"

#include "Galois.hpp"

using namespace aff3ct::tools;

Galois
::Galois(const int& m, const int& N, const int& K, const int& t)
 : m(m), K(K), N(N), t(t), d(2*t+1), alpha_to(N+1), index_of(N+1), p(m+1), g(N-K+1)
{
	Select_Polynomial();
	Generate_GF();
	Compute_BCH_Generator_Polynomial();
	assert(N == ((1 << m) -1));
	assert(K <= N);
}

Galois
::~Galois()
{
}

void Galois
::Select_Polynomial()
{
	for (int i=1; i<m; i++)
		p[i] = 0;

	p[0] = p[m] = 1;
	if      (m ==  2) p[1] = 1;
	else if (m ==  3) p[1] = 1;
	else if (m ==  4) p[1] = 1;
	else if (m ==  5) p[2] = 1;
	else if (m ==  6) p[1] = 1;
	else if (m ==  7) p[1] = 1;
	else if (m ==  8) p[4] = p[5] = p[6] = 1;
	else if (m ==  9) p[4] = 1;
	else if (m == 10) p[3] = 1;
	else if (m == 11) p[2] = 1;
	else if (m == 12) p[3] = p[4] = p[7] = 1;
	else if (m == 13) p[1] = p[3] = p[4] = 1;
	else if (m == 14) p[1] = p[11] = p[12] = 1;
	else if (m == 15) p[1] = 1;
	else if (m == 16) p[2] = p[3] = p[5] = 1;
	else if (m == 17) p[3] = 1;
	else if (m == 18) p[7] = 1;
	else if (m == 19) p[1] = p[5] = p[6] = 1;
	else if (m == 20) p[3] = 1;
}

void Galois
::Generate_GF()
{
	register int i, mask;

	mask = 1;
	alpha_to[m] = 0;
	for (i = 0; i < m; i++)
	{
		alpha_to[i] = mask;
		index_of[alpha_to[i]] = i;
		if (p[i] != 0)
			alpha_to[m] ^= mask;
		mask <<= 1;
	}
	index_of[alpha_to[m]] = m;
	mask >>= 1;
	for (i = m + 1; i < N; i++)
	{
		if (alpha_to[i - 1] >= mask)
			alpha_to[i] = alpha_to[m] ^ ((alpha_to[i - 1] ^ mask) << 1);
		else
			alpha_to[i] = alpha_to[i - 1] << 1;
		const auto idx = alpha_to[i];
		index_of[idx] = i;
	}
	index_of[0] = -1;
}

void Galois
::Compute_BCH_Generator_Polynomial()
{
	register int ii, jj, ll, kaux;
	register int test, aux, nocycles, root, noterms, rdncy;
	int cycle[1024][21], size[1024], min[1024], zeros[1024];

	/* Generate cycle sets modulo n, n = 2**m - 1 */
	cycle[0][0] = 0;
	size[0] = 1;
	cycle[1][0] = 1;
	size[1] = 1;
	jj = 1; /* cycle set index */
	do
	{
		/* Generate the jj-th cycle set */
		ii = 0;
		do
		{
			ii++;
			cycle[jj][ii] = (cycle[jj][ii - 1] * 2) % N;
			size[jj]++;
			aux = (cycle[jj][ii] * 2) % N;
		}
		while (aux != cycle[jj][0]);
		/* Next cycle set representative */
		ll = 0;
		do
		{
			ll++;
			test = 0;
			for (ii = 1; ((ii <= jj) && (!test)); ii++)
				/* Examine previous cycle sets */
				for (kaux = 0; ((kaux < size[ii]) && (!test)); kaux++)
					if (ll == cycle[ii][kaux])
						test = 1;
		}
		while ((test) && (ll < (N - 1)));
		if (!(test))
		{
			jj++; /* next cycle set index */
			cycle[jj][0] = ll;
			size[jj] = 1;
		}
	}
	while (ll < (N - 1));
	nocycles = jj; /* number of cycle sets modulo n */

	//d = 2 * t + 1;

	/* Search for roots 1, 2, ..., d-1 in cycle sets */
	kaux = 0;
	rdncy = 0;
	for (ii = 1; ii <= nocycles; ii++)
	{
		min[kaux] = 0;
		test = 0;
		for (jj = 0; ((jj < size[ii]) && (!test)); jj++)
			for (root = 1; ((root < d) && (!test)); root++)
				if (root == cycle[ii][jj])
				{
					test = 1;
					min[kaux] = ii;
				}
		if (min[kaux])
		{
			rdncy += size[min[kaux]];
			kaux++;
		}
	}
	noterms = kaux;
	kaux = 1;
	for (ii = 0; ii < noterms; ii++)
		for (jj = 0; jj < size[min[ii]]; jj++)
		{
			zeros[kaux] = cycle[min[ii]][jj];
			kaux++;
		}

	//K = N - rdncy;

	if (K < 0)
	{
		std::cerr << bold_red("Galois: parameters invalid!") << std::endl;
		std::exit(0);
	}

	/* Compute the generator polynomial */
	g[0] = alpha_to[zeros[1]];
	g[1] = 1; /* g(x) = (X + zeros[1]) initially */
	for (ii = 2; ii <= rdncy; ii++)
	{
		g[ii] = 1;
		for (jj = ii - 1; jj > 0; jj--)
			if (g[jj] != 0)
				g[jj] = g[jj - 1] ^ alpha_to[(index_of[g[jj]] + zeros[ii]) % N];
			else
				g[jj] = g[jj - 1];
		g[0] = alpha_to[(index_of[g[0]] + zeros[ii]) % N];
	}
}
