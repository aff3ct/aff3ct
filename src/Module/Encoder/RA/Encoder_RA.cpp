#include <vector>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Encoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_RA<B>
::Encoder_RA(const int& K, const int& N, const Interleaver<int>& interleaver, const int n_frames, const std::string name)
 : Encoder<B>(K, N, n_frames, name), rep_count(N/K), U(N), tmp_X_N(N), interleaver(interleaver)
{
	if (N % K)
	{
		std::stringstream message;
		message << "'K' has to be a multiple of 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)interleaver.get_size() != N)
	{
		std::stringstream message;
		message << "'interleaver.get_size()' has to be equal to 'N' ('interleaver.get_size()' = "
		        << interleaver.get_size() << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
void Encoder_RA<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	// repetition
	for (auto i = 0; i < this->K; i++)
		for (auto j = 0; j < rep_count; j++)
			U[i * rep_count +j] = U_K[i];

	interleaver.interleave(U.data(), tmp_X_N.data(), frame_id);

	// accumulation
	for (auto i = 1; i < this->N; i++)
		tmp_X_N[i] = tmp_X_N[i-1] ^ tmp_X_N[i];

	std::copy(tmp_X_N.begin(), tmp_X_N.end(), X_N);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_RA<B_8>;
template class aff3ct::module::Encoder_RA<B_16>;
template class aff3ct::module::Encoder_RA<B_32>;
template class aff3ct::module::Encoder_RA<B_64>;
#else
template class aff3ct::module::Encoder_RA<B>;
#endif
// ==================================================================================== explicit template instantiation
