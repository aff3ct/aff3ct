#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_BCH.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_BCH<B>
::Encoder_BCH(const int& K, const int& N, const int& m, const tools::Galois &GF,
              const int n_frames, const std::string name)
 : Encoder<B>(K, N, n_frames, name), m(m), g(N-K+1), bb(N-K)
{
	//assert(2**m == N+1);
	//assertion on K required

	// set polynomial coefficients
	this->g = GF.g;
}

template <typename B>
void Encoder_BCH<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(this->K * this->n_frames == (int)U_K.size());
	assert(this->N * this->n_frames == (int)X_N.size());

	for (auto f = 0; f < this->n_frames; f++)
		this->_encode(U_K.data() + f * this->K, X_N.data() + f * this->N);
}

template <typename B>
void Encoder_BCH<B>
::_encode(const B* U_K, B* X_N)
{
	register int i, j;
	register int feedback;

	for (i = 0; i < this->N - this->K; i++)
		bb[i] = 0;
	for (i = this->K - 1; i >= 0; i--)
	{
		feedback = U_K[i] ^ bb[this->N - this->K - 1];
		if (feedback != 0)
		{
			for (j = this->N - this->K - 1; j > 0; j--)
				if (g[j] != 0)
					bb[j] = bb[j - 1] ^ feedback;
				else
					bb[j] = bb[j - 1];
			bb[0] = g[0] && feedback;
		}
		else
		{
			for (j = this->N - this->K - 1; j > 0; j--)
				bb[j] = bb[j - 1];
			bb[0] = 0;
		}
	}

	for (i = 0; i < this->N - this->K; i++)
		X_N[i] = bb[i];
	for (i = 0; i < this->K; i++)
		X_N[i + this->N - this->K] = U_K[i];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_BCH<B_8>;
template class aff3ct::module::Encoder_BCH<B_16>;
template class aff3ct::module::Encoder_BCH<B_32>;
template class aff3ct::module::Encoder_BCH<B_64>;
#else
template class aff3ct::module::Encoder_BCH<B>;
#endif
// ==================================================================================== explicit template instantiation
