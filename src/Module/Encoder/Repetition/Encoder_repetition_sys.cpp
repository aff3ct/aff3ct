#include <vector>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Encoder_repetition_sys.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_repetition_sys<B>
::Encoder_repetition_sys(const int& K, const int& N, const bool buffered_encoding, const int n_frames, const std::string name)
: Encoder<B>(K, N, n_frames, name), rep_count((N/K) -1), buffered_encoding(buffered_encoding)
{
	if (N % K)
	{
		std::stringstream message;
		message << "'K' has to be a multiple of 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
void Encoder_repetition_sys<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
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
