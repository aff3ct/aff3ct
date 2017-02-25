#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_RA.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_RA<B>
::Encoder_RA(const int& K, const int& N, Interleaver<int>& interleaver, const std::string name)
 : Encoder<B>(K, N, 1, name), rep_count(N/K), U(N), tmp_X_N(N), interleaver(interleaver)
{	
	assert(N % K == 0); // check if RA count is consistent
}

template <typename B>
void Encoder_RA<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	for (auto f = 0; f < this->n_frames; f++)
	{
		// repetition
		for (auto i = 0; i < this->K; i++)
			for (auto j = 0; j < rep_count; j++)
				U[i * rep_count +j] = U_K[f * this->K +i];

		interleaver.interleave(U, tmp_X_N, false, 1);

		// accumulation
		for (auto i = 1; i < this->N; i++)
			tmp_X_N[i] = tmp_X_N[i-1] ^ tmp_X_N[i];

		std::copy(tmp_X_N.begin(), tmp_X_N.end(), X_N.begin() + f * this->N);
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_RA<B_8>;
template class Encoder_RA<B_16>;
template class Encoder_RA<B_32>;
template class Encoder_RA<B_64>;
#else
template class Encoder_RA<B>;
#endif
// ==================================================================================== explicit template instantiation
