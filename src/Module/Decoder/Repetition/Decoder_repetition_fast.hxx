#include <cassert>
#include <algorithm>

#include "Decoder_repetition_fast.hpp"

template <typename B, typename R>
Decoder_repetition_fast<B,R>
::Decoder_repetition_fast(const int& K, const int& N, const bool buffered_encoding, const int n_frames,
                          const std::string name)
 : Decoder_repetition<B,R>(K, N, buffered_encoding, n_frames, name)
{
	assert(this->K % mipp::nElReg<R>() == 0);
}

template <typename B, typename R>
Decoder_repetition_fast<B,R>
::~Decoder_repetition_fast()
{
}

template <typename B, typename R>
void Decoder_repetition_fast<B,R>
::soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	for (auto i = 0; i < this->K; i += mipp::nElReg<R>())
	{
		auto r_ext = mipp::Reg<R>(&sys[i]);
		for (auto j = 0; j < this->rep_count; j++)
			r_ext += &par[j*this->K +i];
		r_ext.store(&ext[i]);
	}
}
