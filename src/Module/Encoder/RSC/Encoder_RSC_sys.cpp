#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_RSC_sys<B>
::Encoder_RSC_sys(const int& K, const int& N, const int n_ff, const bool buffered_encoding)
: Encoder<B>(K, N), n_ff(n_ff), n_states(1 << n_ff),
  buffered_encoding(buffered_encoding)
{
	const std::string name = "Encoder_RSC_sys";
	this->set_name(name);

	if (n_ff <= 0)
	{
		std::stringstream message;
		message << "'n_ff' has to be greater than 0 ('n_ff' = " << n_ff << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N - 2 * n_ff !=  2 * K)
	{
		std::stringstream message;
		message << "'N' - 2 * 'n_ff' has to be equal to 2 * 'K' ('N' = " << N << ", 'n_ff' = " << n_ff
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (!buffered_encoding)
		for (auto k = 0; k < this->K; k++)
			this->info_bits_pos[k] = 2 * k;
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
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	if (buffered_encoding)
		__encode(U_K,
		         X_N,                             // sys
		         X_N + 1 * this->K,               // tail sys
		         X_N + 1 * this->K + this->n_ff,  // par
		         X_N + 2 * this->K + this->n_ff); // tail par
	else
		__encode(U_K,
		         X_N,                   // sys
		         X_N + 2 * this->K,     // tail sys
		         X_N + 1,               // par
		         X_N + 2 * this->K + 1, // tail par
		         2,                     // stride
		         2);                    // stride tail bits
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
::__encode(const B* U_K, B* sys, B* tail_sys, B* par, B* tail_par, const int stride, const int stride_tail)
{
	auto state = 0; // initial (and final) state 0 0 0

	if (sys != nullptr)
		for (auto i = 0; i < this->K; i++)
			sys[i * stride] = U_K[i];

	// standard frame encoding process
	if (par != nullptr)
		for (auto i = 0; i < this->K; i++)
			par[i * stride] = inner_encode((int)U_K[i], state); // encoding block
	else
		for (auto i = 0; i < this->K; i++)
			inner_encode((int)U_K[i], state); // encoding block

	// tail bits for initialization conditions (state of data "state" have to be 0 0 0)
	for (auto i = 0; i < this->n_ff; i++)
	{
		B bit_sys = tail_bit_sys(state);

		if (tail_sys != nullptr)
			tail_sys[i * stride_tail] = bit_sys; // systematic transmission of the bit

		auto p = inner_encode((int)bit_sys, state); // encoding block

		if (tail_par != nullptr)
			tail_par[i * stride_tail] = p;
	}

	if (state != 0)
	{
		std::stringstream message;
		message << "'state' should be equal to 0 ('state' = " <<  state << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
bool Encoder_RSC_sys<B>
::_is_codeword(const B* sys, const B* tail_sys, const B* par, const B* tail_par,
               const int stride, const int stride_tail)
{
	auto state = 0; // initial (and final) state 0 0 0

	// standard frame encoding process
	if (par != nullptr)
	{
		for (auto i = 0; i < this->K; i++)
			if (par[i * stride] != inner_encode((int)sys[i * stride], state)) // encoding block
				return false;
	}
	else
	{
		for (auto i = 0; i < this->K; i++)
			inner_encode((int)sys[i * stride], state); // encoding block
	}

	// tail bits for initialization conditions (state of data "state" have to be 0 0 0)
	for (auto i = 0; i < this->n_ff; i++)
	{
		B bit_sys = tail_bit_sys(state);

		if (tail_sys != nullptr)
			if (tail_sys[i * stride_tail] != bit_sys) // systematic transmission of the bit
				return false;

		auto p = inner_encode((int)bit_sys, state); // encoding block

		if (tail_par != nullptr)
			if (tail_par[i * stride_tail] != p)
				return false;
	}

	return true;
}

template <typename B>
bool Encoder_RSC_sys<B>
::is_codeword(const B *X_N)
{
	if (buffered_encoding)
		return _is_codeword(X_N,                             // sys
		                    X_N + 1 * this->K,               // tail sys
		                    X_N + 1 * this->K + this->n_ff,  // par
		                    X_N + 2 * this->K + this->n_ff); // tail par
	else
		return _is_codeword(X_N,                   // sys
		                    X_N + 2 * this->K,     // tail sys
		                    X_N + 1,               // par
		                    X_N + 2 * this->K + 1, // tail par
		                    2,                     // stride
		                    2);                    // stride tail bits
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_RSC_sys<B_8>;
template class aff3ct::module::Encoder_RSC_sys<B_16>;
template class aff3ct::module::Encoder_RSC_sys<B_32>;
template class aff3ct::module::Encoder_RSC_sys<B_64>;
#else
template class aff3ct::module::Encoder_RSC_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
