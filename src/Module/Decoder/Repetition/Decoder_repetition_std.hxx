#include <cassert>
#include <algorithm>

#include "Decoder_repetition_std.hpp"

template <typename B, typename R>
Decoder_repetition_std<B,R>
::Decoder_repetition_std(const int& K, const int& N, const bool buffered_encoding, const int n_frames,
                         const std::string name)
 : Decoder_repetition<B,R>(K,N,buffered_encoding, n_frames, name)
{
}

template <typename B, typename R>
Decoder_repetition_std<B,R>
::~Decoder_repetition_std()
{
}

template <typename B, typename R>
void Decoder_repetition_std<B,R>
::soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	for (auto i = 0; i < this->K; i++)
	{
		ext[i] = sys[i];
		for (auto j = 0; j < this->rep_count; j++)
			// ext[i] += (par[j*this->K +i] > 0) ? 1 : -1; // hard decision
			ext[i] += par[j*this->K +i]; // soft decision
	}
}
