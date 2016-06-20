#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_RSC3_sys.hpp"

template <typename B>
Encoder_RSC3_sys<B>
::Encoder_RSC3_sys(const int& K, const int& N, const int& n_frames, const bool buffered_encoding)
: Encoder_RSC<B>(3, n_frames),
  K(K),
  N(N),
  buffered_encoding(buffered_encoding)
{
	assert(N == (2 * K));
}

template <typename B>
void Encoder_RSC3_sys<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == (unsigned) (K * this->n_frames));
	assert(X_N.size() == (unsigned) ((N + 2*3) * this->n_frames));

	if (buffered_encoding)
		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy   (U_K.data() + f*K, U_K.data() + f*K +K, X_N.data() + f*2*(K+3)); // sys
			frame_encode(U_K.data() + f*K, X_N.data() + f*2*(K+3) +K, 1, true);          // par + tail bits
		}
	else
		for (auto f = 0; f < this->n_frames; f++)
			frame_encode(U_K.data() + f*K, X_N.data() + f*2*(K+3));
}

template <typename B>
void Encoder_RSC3_sys<B>
::encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par)
{
	assert(par.size() == (unsigned) ((K +2*3) * this->n_frames));

	// par bits: [par | tail bit sys | tail bits par]
	for (auto f = 0 ; f < this->n_frames ; f++)
		frame_encode(U_K.data() + f*K, par.data() + f*(K+2*3), 1, true);
}

template <typename B>
int Encoder_RSC3_sys<B>
::inner_enc(const int bit_sys, int &state)
{
	auto xor1 = ((state >> 1) & 0x1) ^ (state & 0x1);
	auto xor2 = xor1 ^ bit_sys;
	auto xor3 = xor2 ^ ((state >> 2) & 0x1);
	auto xor4 = xor3 ^ (state & 0x1);

	state = (state >> 1) | (xor2 << 2);

	return xor4;
}

template <typename B>
mipp::vector<mipp::vector<int>> Encoder_RSC3_sys<B>
::get_trellis()
{
	mipp::vector<mipp::vector<int>> trellis(4);

	for (unsigned i = 0; i < trellis.size(); i++)
		trellis[i].resize(8);

	for (auto i = 0; i < 8; i++)
	{
		auto state = i;
		auto bit_sys = 0;
		auto bit_par = inner_enc(bit_sys, state);

		trellis[0][           i ] = state;
		trellis[2][trellis[0][i]] = bit_sys ^ bit_par;

		// ----------

		state = i;
		bit_sys = 1;
		bit_par = inner_enc(bit_sys, state);

		trellis[1][i] = state;
		trellis[3][i] = bit_sys ^ bit_par;
	}

	// for (auto i = 0; i < this->n_states; i++)
	// 	std::cout << "trellis[0][" << i << "] = " << trellis[0][i] << std::endl;
	// std::cout << std::endl;
	// for (auto i = 0; i < this->n_states; i++)
	// 	std::cout << "trellis[1][" << i << "] = " << trellis[1][i] << std::endl;
	// std::cout << std::endl;
	// for (auto i = 0; i < this->n_states; i++)
	// 	std::cout << "trellis[2][" << i << "] = " << trellis[2][i] << std::endl;
	// std::cout << std::endl;
	// for (auto i = 0; i < this->n_states; i++)
	// 	std::cout << "trellis[3][" << i << "] = " << trellis[3][i] << std::endl;

	return trellis;
}

template <typename B>
void Encoder_RSC3_sys<B>
::frame_encode(const B* U_K, B* X_N, const int stride, const bool only_parity)
{
	auto j = 0; // cur offset in X_N buffer
	auto state = 0; // initial (and final) state 0 0 0

	// standard frame encoding process
	for (auto i = 0; i < K; i++)
	{
		if (!only_parity)
		{
			X_N[j] = U_K[i];
			j += stride; // systematic transmission of the bit
		}
		X_N[j] = inner_enc(U_K[i], state); j += stride; // encoding block
	}

	// tail bits for initialization conditions (state of data "state" have to be 0 0 0)
	for (auto i = 0; i < 3; i++)
	{
		B bit_sys = 0 ^ ((state >> 1) & 0x1) ^ (state & 0x1);

		if (!only_parity)
		{
			X_N[j] = bit_sys; // systematic transmission of the bit
			j += stride;
		}
		else
			X_N[j+3] = bit_sys; // systematic transmission of the bit

		X_N[j] = inner_enc(bit_sys, state); j += stride; // encoding block
	}

	assert(state == 0);
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