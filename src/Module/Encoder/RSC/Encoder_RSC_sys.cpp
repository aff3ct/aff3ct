#ifdef _MSC_VER
#include <iterator>
#endif

#include "Encoder_RSC_sys.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_RSC_sys<B>
::Encoder_RSC_sys(const int& K, const int& N, const int n_ff, const int& n_frames, const bool buffered_encoding,
                  const std::string name)
: Encoder_sys<B>(K, N + 2*n_ff, n_frames, name), n_ff(n_ff), n_states(1 << n_ff), 
  buffered_encoding(buffered_encoding)
{
	assert(N == (2 * K));
}

template <typename B>
int Encoder_RSC_sys<B>
::get_n_ff()
{
	return n_ff;
}
template <typename B>
int Encoder_RSC_sys<B>
::tail_length() const
{
	return 2 * n_ff;
}

template <typename B>
void Encoder_RSC_sys<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == (unsigned) (this->K * this->n_frames));
	assert(X_N.size() == (unsigned) (this->N * this->n_frames));

	if (buffered_encoding)
		for (auto f = 0; f < this->n_frames; f++)
		{
#ifdef _MSC_VER
			std::copy(U_K.data() + f*this->K,
			          U_K.data() + f*this->K + this->K,
			          stdext::checked_array_iterator<B*>(X_N.data() + f * 2 * (this->K + this->n_ff), this->K)); // sys
#else
			std::copy(U_K.data() + f*this->K, U_K.data() + f*this->K + this->K, X_N.data() + f * 2 * (this->K + this->n_ff)); // sys
#endif
			frame_encode(U_K.data() + f*this->K, X_N.data() + f * 2 * (this->K + this->n_ff) + this->K, 1, true);             // par + tail bits
		}
	else
		for (auto f = 0; f < this->n_frames; f++)
			frame_encode(U_K.data() + f*this->K, X_N.data() + f*2*(this->K+this->n_ff));
}

template <typename B>
void Encoder_RSC_sys<B>
::encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par)
{
	assert(par.size() == (unsigned) ((this->K +2*this->n_ff) * this->n_frames));

	// par bits: [par | tail bit sys | tail bits par]
	for (auto f = 0 ; f < this->n_frames ; f++)
		frame_encode(U_K.data() + f*this->K, par.data() + f*(this->K+2*this->n_ff), 1, true);
}

template <typename B>
std::vector<std::vector<int>> Encoder_RSC_sys<B>
::get_trellis()
{
	std::vector<std::vector<int>> trellis(10, std::vector<int>(this->n_states));

	std::vector<bool> occurrence(this->n_states, false);

	for (auto i = 0; i < this->n_states; i++)
	{
		// sys = 0
		auto state   = i;
		auto bit_sys = 0;
		auto bit_par = inner_encode(bit_sys, state);

		trellis[0 + (occurrence[state] ? 3 : 0)][state] = i;                 // initial state
		trellis[1 + (occurrence[state] ? 3 : 0)][state] = +1;                // gamma coeff
		trellis[2 + (occurrence[state] ? 3 : 0)][state] = bit_sys ^ bit_par; // gamma
		trellis[6                              ][i    ] = state;             // final state, bit syst = 0
		trellis[7                              ][i    ] = bit_sys ^ bit_par; // gamma      , bit syst = 0

		occurrence[state] = true;

		// sys = 1
		state   = i;
		bit_sys = 1;
		bit_par = inner_encode(bit_sys, state);

		trellis[0 + (occurrence[state] ? 3 : 0)][state] = i;                 // initial state
		trellis[1 + (occurrence[state] ? 3 : 0)][state] = -1;                // gamma coeff
		trellis[2 + (occurrence[state] ? 3 : 0)][state] = bit_sys ^ bit_par; // gamma
		trellis[8                              ][i    ] = state;             // initial state, bit syst = 1
		trellis[9                              ][i    ] = bit_sys ^ bit_par; // gamma        , bit syst = 1

		occurrence[state] = true;
	}

	return trellis;
}

template <typename B>
void Encoder_RSC_sys<B>
::frame_encode(const B* U_K, B* X_N, const int stride, const bool only_parity)
{
	auto j = 0; // cur offset in X_N buffer
	auto state = 0; // initial (and final) state 0 0 0

	// standard frame encoding process
	for (auto i = 0; i < this->K; i++)
	{
		if (!only_parity)
		{
			X_N[j] = U_K[i];
			j += stride; // systematic transmission of the bit
		}
		X_N[j] = inner_encode((int)U_K[i], state); j += stride; // encoding block
	}

	// tail bits for initialization conditions (state of data "state" have to be 0 0 0)
	for (auto i = 0; i < this->n_ff; i++)
	{
		B bit_sys = tail_bit_sys(state);

		if (!only_parity)
		{
			X_N[j] = bit_sys; // systematic transmission of the bit
			j += stride;
		}
		else
			X_N[j+this->n_ff] = bit_sys; // systematic transmission of the bit

		X_N[j] = inner_encode((int)bit_sys, state); j += stride; // encoding block
	}

	assert(state == 0);
	if (!only_parity)
		assert(j == this->N * stride);
	else
	{
		j += this->n_ff * stride;
		assert(j == (this->K + 2*this->n_ff) * stride);
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_RSC_sys<B_8>;
template class aff3ct::module::Encoder_RSC_sys<B_16>;
template class aff3ct::module::Encoder_RSC_sys<B_32>;
template class aff3ct::module::Encoder_RSC_sys<B_64>;
#else
template class aff3ct::module::Encoder_RSC_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
