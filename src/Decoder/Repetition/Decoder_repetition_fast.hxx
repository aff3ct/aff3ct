#include <cassert>
#include <algorithm>

#include "Decoder_repetition_fast.hpp"

template <typename B, typename R>
Decoder_repetition_fast<B,R>
::Decoder_repetition_fast(const int& K, const int& N)
 : Decoder_repetition<B,R>(K, N)
{
}

template <typename B, typename R>
Decoder_repetition_fast<B,R>
::~Decoder_repetition_fast()
{
}

template <typename B, typename R>
void Decoder_repetition_fast<B,R>
::decode()
{
	for (auto i = 0; i < this->K; i++)
	{
		auto accum = mipp::Reduction<R,mipp::add>::apply(&this->Y_N[i*this->rep_count], this->rep_count);
		this->V_K[i] = (accum > 0) ? (B)0 : (B)1;
	}
}
