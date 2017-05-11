#ifdef _MSC_VER
#include <iterator>
#endif

#include <stdexcept>

#include "Encoder_RSC_sys.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_RSC_sys<B>
::Encoder_RSC_sys(const int& K, const int& N, const int n_ff, const int& n_frames, const bool buffered_encoding,
                  const std::string name)
: Encoder_sys<B>(K, N, n_frames, name), n_ff(n_ff), n_states(1 << n_ff),
  buffered_encoding(buffered_encoding)
{
	if (N - 2*n_ff !=  2 * K)
		throw std::invalid_argument("aff3ct::module::Encoder_RSC_sys: \"N\" / \"K\" has to be a equal to 2.");
	if (n_ff <= 0)
		throw std::invalid_argument("aff3ct::module::Encoder_RSC_sys: \"n_ff\" has to be greater than 0.");
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
::_encode(const B *U_K, B *X_N)
{
	if (buffered_encoding)
	{
		std::copy(U_K, U_K + this->K, X_N); // sys
		__encode(U_K, X_N + this->K, 1, true); // par + tail bits
	}
	else
		__encode(U_K, X_N);
}

template <typename B>
void Encoder_RSC_sys<B>
::_encode_sys(const B *U_K, B *par)
{
	// par bits: [par | tail bit sys | tail bits par]
	__encode(U_K, par, 1, true);
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
::__encode(const B* U_K, B* X_N, const int stride, const bool only_parity)
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

	if (state != 0)
		throw std::runtime_error("aff3ct::module::Encoder_RSC_sys: \"state\" should be equal to 0.");

	if (!only_parity)
	{
		if (j != this->N * stride)
			throw std::runtime_error("aff3ct::module::Encoder_RSC_sys: \"j\" should be equal to \"N\" * \"stride\".");
	}
	else
	{
		j += this->n_ff * stride;
		if (j != (this->K + 2*this->n_ff) * stride)
			throw std::runtime_error("aff3ct::module::Encoder_RSC_sys: \"j\" should be equal to "
			                         "(\"K\" + 2 * \"n_ff\") * \"stride\".");
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
