#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_RA.hpp"

template <typename B>
Encoder_RA<B>
::Encoder_RA(const int& K, const int& N, Interleaver<int>& interleaver, const std::string name)
 : Encoder<B>(K, N, 1, name.c_str()), rep_count(N/K), U(N), interleaver(interleaver)
{	
	assert(N % K == 0); // check if RA count is consistent
}

template <typename B>
void Encoder_RA<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(this->n_frames == 1);
	
	// repetition
	for (auto i = 0; i < this->K; i++)
		for (auto j = 0; j < rep_count; j++)
			U[i * rep_count +j] = U_K[i];

	interleaver.interleave(U, X_N);

	// accumulation
	for (auto i = 1; i < this->N; i++)
		X_N[i] = X_N[i-1] ^ X_N[i];
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_RA<B_8>;
template class Encoder_RA<B_16>;
template class Encoder_RA<B_32>;
template class Encoder_RA<B_64>;
#else
template class Encoder_RA<B>;
#endif
// ==================================================================================== explicit template instantiation