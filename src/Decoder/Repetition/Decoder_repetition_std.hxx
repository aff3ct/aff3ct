#include <cassert>
#include <algorithm>

#include "Decoder_repetition_std.hpp"

template <typename B, typename R>
Decoder_repetition_std<B,R>
::Decoder_repetition_std(const int& K, const int& N)
 : Decoder_repetition<B,R>(K,N)
{
}

template <typename B, typename R>
Decoder_repetition_std<B,R>
::~Decoder_repetition_std()
{
}

template <typename B, typename R>
void Decoder_repetition_std<B,R>
::decode()
{
	R soft_accu;
	for (auto i = 0; i < this->K; i++)
	{
		soft_accu = (R)0;
		for(auto j = 0; j < this->rep_count; j++)
			//soft_accu += ((this->Y_N[i*rep_count+j])>0)? 1 : -1; // hard decision
			soft_accu += this->Y_N[i*this->rep_count+j]; // soft decision

		this->V_K[i] = (soft_accu > 0)? (B)0 : (B)1;
	}
}