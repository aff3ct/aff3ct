#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_BCH.hpp"
#include "../../Tools/Algebraic/Galois.hpp"


template <typename B>
Encoder_BCH<B>
::Encoder_BCH(const int& K, const int& N, const int& m, Galois* GF)
 : K(K), N(N), m(m), g(N-K+1), bb(N-K)
{	
	//assert(2**m == N+1);
	//assertion on K required

	// set polynomial coefficients
	this->g = GF->g;
}

template <typename B>
void Encoder_BCH<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	register int i, j;
	register int feedback;
	
	for (i = 0; i < N - K; i++)
		bb[i] = 0;
	for (i = K - 1; i >= 0; i--)
	{
		feedback = U_K[i] ^ bb[N - K - 1];
		if (feedback != 0)
		{
			for (j = N - K - 1; j > 0; j--)
				if (g[j] != 0)
					bb[j] = bb[j - 1] ^ feedback;
				else
					bb[j] = bb[j - 1];
			bb[0] = g[0] && feedback;
		}
		else
		{
			for (j = N - K - 1; j > 0; j--)
				bb[j] = bb[j - 1];
			bb[0] = 0;
		}
	}

	for (i = 0; i < N - K; i++)
		X_N[i] = bb[i];
	for (i = 0; i < K; i++)
		X_N[i + N - K] = U_K[i];
	
}