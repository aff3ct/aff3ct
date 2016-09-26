#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_RSC3_sys.hpp"

template <typename B>
Encoder_RSC3_sys<B>
::Encoder_RSC3_sys(const int& K, const int& N, const int& n_frames, const bool buffered_encoding,
                   const std::string name)
: Encoder_RSC_sys<B>(K, N, 3, n_frames, buffered_encoding, name)
{
}

template <typename B>
int Encoder_RSC3_sys<B>
::inner_encode(const int bit_sys, int &state)
{
	auto xor1 = ((state >> 1) & 0x1) ^ (state & 0x1);
	auto xor2 = xor1 ^ bit_sys;
	auto xor3 = xor2 ^ ((state >> 2) & 0x1);
	auto xor4 = xor3 ^ (state & 0x1);

	state = (state >> 1) | (xor2 << 2);

	return xor4;
}

template <typename B>
int Encoder_RSC3_sys<B>
::tail_bit_sys(const int &state)
{
	return 0 ^ ((state >> 1) & 0x1) ^ (state & 0x1);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_RSC3_sys<B_8>;
template class Encoder_RSC3_sys<B_16>;
template class Encoder_RSC3_sys<B_32>;
template class Encoder_RSC3_sys<B_64>;
#else
template class Encoder_RSC3_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
