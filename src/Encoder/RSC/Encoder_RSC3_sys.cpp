#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_RSC3_sys.hpp"

template <typename B>
Encoder_RSC3_sys<B>
::Encoder_RSC3_sys(const int& K, const int& N, const int& n_frames, const bool buffered_encoding)
: Encoder_RSC<B>(3),
  K(K),
  N(N),
  n_frames(n_frames),
  buffered_encoding(buffered_encoding)
{
	assert(N == (2 * K));
}

template <typename B>
void Encoder_RSC3_sys<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == (unsigned) (K * n_frames));
	assert(X_N.size() == (unsigned) ((N + 2*3) * n_frames));

	if (buffered_encoding)
		for (auto f = 0; f < n_frames; f++)
		{
			std::copy   (U_K.data() + f*K, U_K.data() + f*K +K, X_N.data() + f*2*(K+3)); // sys
			frame_encode(U_K.data() + f*K, X_N.data() + f*2*(K+3) +K, 1, true);          // par + tail bits
		}
	else
		for (auto f = 0; f < n_frames; f++)
			frame_encode(U_K.data() + f*K, X_N.data() + f*2*(K+3));
}

template <typename B>
void Encoder_RSC3_sys<B>
::encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par)
{
	assert(par.size() == (unsigned) ((K +2*3) * n_frames));

	// par bits: [par | tail bit sys | tail bits par]
	for (auto f = 0 ; f < n_frames ; f++)
		frame_encode(U_K.data() + f*K, par.data() + f*(K+2*3), 1, true);
}

template <typename B>
void Encoder_RSC3_sys<B>
::frame_encode(const B* U_K, B* X_N, const int stride, const bool only_parity)
{
	// update offsets for data accesses
	auto off_d0 = 0, off_d1 = 1, off_d2 = 2;
	auto prev_off_d1 = off_d1, prev_off_d2 = off_d2;
	auto up_offsets = [&]() __attribute__((always_inline))
	{
		prev_off_d1 = off_d1; prev_off_d2 = off_d2;
		off_d0 = (3 + (off_d0 -1)) % 3;
		off_d1 = (3 + (off_d1 -1)) % 3;
		off_d2 = (3 + (off_d2 -1)) % 3;
	};

	// inner encoding block used multiple time in the whole encoding process
	auto inner_enc_block = [&](B cur_bit, B states[3]) __attribute__((always_inline))
	{
		return ((cur_bit ^ (states[off_d1] ^ states[off_d2])) ^ states[off_d0]) ^ states[off_d2];
	};

	auto j = 0; // cur offset in X_N buffer
	B states[3] = {0, 0, 0}; // initial (and final) states: 0 0 0

	// standard frame encoding process
	for (auto i = 0; i < K; i++)
	{
		if (!only_parity)
		{
			X_N[j] = U_K[i];
			j += stride; // systematic transmission of the bit
		}
		X_N[j] = inner_enc_block(U_K[i], states); j += stride; // encoding block

		up_offsets(); // update offsets

		states[off_d0] = U_K[i] ^ (states[prev_off_d1] ^ states[prev_off_d2]); // update state d0
	}

	// tail bits for initialization conditions (states of data "states" have to be 0 0 0)
	for (auto i = 0; i < 3; i++)
	{
		B cur_bit = 0 ^ (states[off_d1] ^ states[off_d2]);

		if (!only_parity)
		{
			X_N[j] = cur_bit; // systematic transmission of the bit
			j += stride;
		}
		else
			X_N[j+3] = cur_bit; // systematic transmission of the bit

		X_N[j] = inner_enc_block(cur_bit, states); j += stride; // encoding block

		up_offsets(); // update offsets

		states[off_d0] = cur_bit ^ (states[prev_off_d1] ^ states[prev_off_d2]); // update states with cur bit
	}

	if (!only_parity)
		assert(j == (N + 2*3) * stride);
	else
	{
		j += 3 * stride;
		assert(j == (K + 2*3) * stride);
	}
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_RSC3_sys<B_8>;
template class Encoder_RSC3_sys<B_16>;
template class Encoder_RSC3_sys<B_32>;
template class Encoder_RSC3_sys<B_64>;
#else
template class Encoder_RSC3_sys<B>;
#endif
// ==================================================================================== explicit template instantiation