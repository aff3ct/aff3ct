#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_RSC4_sys.hpp"

template <typename B>
Encoder_RSC4_sys<B>
::Encoder_RSC4_sys(const int& K, const int& N, const int& n_frames, const bool buffered_encoding)
: Encoder_RSC_sys<B>(K, N, 4, n_frames, buffered_encoding)
{
}

template <typename B>
int Encoder_RSC4_sys<B>
::inner_encode(const int bit_sys, int &state)
{
	auto xor1 = ((state >> 1) & 0x1) ^ (state & 0x1);
	auto xor2 = xor1 ^ bit_sys;
	auto xor3 = xor2 ^ ((state >> 3) & 0x1);
	auto xor4 = xor3 ^ ((state >> 1) & 0x1);
	auto xor5 = xor4 ^ (state & 0x1);

	state = (state >> 1) | (xor2 << 3);

	return xor5;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_RSC4_sys<B_8>;
template class Encoder_RSC4_sys<B_16>;
template class Encoder_RSC4_sys<B_32>;
template class Encoder_RSC4_sys<B_64>;
#else
template class Encoder_RSC4_sys<B>;
#endif
// ==================================================================================== explicit template instantiation