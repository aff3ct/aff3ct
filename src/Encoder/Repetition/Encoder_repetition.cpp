#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_repetition.hpp"

template <typename B>
Encoder_repetition<B>
::Encoder_repetition(const int& K, const int& N)
: K(K), N(N), rep_count(N/K)
{
	assert(N % K == 0); // check if repetition count is consistent
}

template <typename B>
void Encoder_repetition<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	// repetition
    for (auto i = 0; i < K; i++)
        for (auto j = 0; j < rep_count; j++)
            X_N[i * rep_count +j] = U_K[i];
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_repetition<B_8>;
template class Encoder_repetition<B_16>;
template class Encoder_repetition<B_32>;
template class Encoder_repetition<B_64>;
#else
template class Encoder_repetition<B>;
#endif
// ==================================================================================== explicit template instantiation