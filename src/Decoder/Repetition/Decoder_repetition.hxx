#include <cassert>
#include <algorithm>

#include "Decoder_repetition.hpp"

template <typename B, typename R>
Decoder_repetition<B,R>
::Decoder_repetition(const int& K, const int& N)
 : K(K), N(N), rep_count(N/K), Y_N(N), V_K(K)
{
	assert(N % K == 0);
}

template <typename B, typename R>
Decoder_repetition<B,R>
::~Decoder_repetition()
{
}

template <typename B, typename R>
void Decoder_repetition<B,R>
::load(const mipp::vector<R>& Y_N)
{
	this->Y_N = Y_N;
}

template <typename B, typename R>
void Decoder_repetition<B,R>
::store(mipp::vector<B>& V_K) const
{
	V_K = this->V_K;
}
