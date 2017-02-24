#include <cassert>
#include <algorithm>

#include "Decoder_repetition_std.hpp"
using namespace aff3ct;

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

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_repetition_std<B_8,Q_8>;
template class Decoder_repetition_std<B_16,Q_16>;
template class Decoder_repetition_std<B_32,Q_32>;
template class Decoder_repetition_std<B_64,Q_64>;
#else
template class Decoder_repetition_std<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
