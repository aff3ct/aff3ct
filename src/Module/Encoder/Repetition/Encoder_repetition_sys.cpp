#include <stdexcept>
#include <vector>
#include <cmath>

#include "Encoder_repetition_sys.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_repetition_sys<B>
::Encoder_repetition_sys(const int& K, const int& N, const bool buffered_encoding, const int n_frames, const std::string name)
: Encoder_sys<B>(K, N, n_frames, name), rep_count((N/K) -1), buffered_encoding(buffered_encoding)
{
	if (N % K)
		throw std::invalid_argument("aff3ct::module::Encoder_repetition_sys: \"K\" has to be a multiple of \"N\".");
}

template <typename B>
void Encoder_repetition_sys<B>
::_encode_sys(const B *U_K, B *par)
{
	if (!buffered_encoding)
		throw std::runtime_error("aff3ct::module::Encoder_repetition_sys: the \"_encode_sys\" method works only with "
		                         "the \"buffered_encoding\" enabled.");

	for (auto i = 0; i < rep_count; i++) // parity bits
		std::copy(U_K, U_K + this->K, par + i * this->K);
}

template <typename B>
void Encoder_repetition_sys<B>
::_encode(const B *U_K, B *X_N)
{
	// repetition
	if (!buffered_encoding)
	{
		for (auto i = 0; i < this->K; i++)
		{
			const auto off1 = i * (rep_count +1);
			const auto off2 = off1 +1;

			const auto bit = U_K[i];

			X_N[off1] = bit; // systematic bit

			// parity bits
			for (auto j = 0; j < rep_count; j++)
				X_N[off2 +j] = bit;
		}
	}
	else
	{
		// systematic bits
		std::copy(U_K, U_K + this->K, X_N);

		// parity bits
		for (auto i = 1; i <= rep_count; i++)
			std::copy(U_K, U_K + this->K, X_N + i * this->K);
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_repetition_sys<B_8>;
template class aff3ct::module::Encoder_repetition_sys<B_16>;
template class aff3ct::module::Encoder_repetition_sys<B_32>;
template class aff3ct::module::Encoder_repetition_sys<B_64>;
#else
template class aff3ct::module::Encoder_repetition_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
