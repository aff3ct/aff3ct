#include <vector>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Encoder_turbo_legacy.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_turbo_legacy<B>
::Encoder_turbo_legacy(const int& K, const int& N, const Interleaver<B> &pi, Encoder<B> &sub_enc,
                       const int n_frames)
: Encoder<B>(K, N, n_frames),
  pi(pi),
  sub_enc(sub_enc),
  U_K_i(K*n_frames),
  X_N_n((2 * (K + sub_enc.tail_length()/2))*n_frames),
  X_N_i((2 * (K + sub_enc.tail_length()/2))*n_frames)
{
	const std::string name = "Encoder_turbo_legacy";
	this->set_name(name);
	
	if (N - 2 * sub_enc.tail_length() != 3 * K)
	{
		std::stringstream message;
		message << "'N' - 2 * 'sub_enc.tail_length()' has to be equal to 3 * 'K' ('N' = " << N
		        << ", 'sub_enc.tail_length()' = " << sub_enc.tail_length() << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)pi.get_core().get_size() != K)
	{
		std::stringstream message;
		message << "'pi.get_core().get_size()' has to be equal to 'K' ('pi.get_core().get_size()' = "
		        << pi.get_core().get_size() << ", 'K' = " << K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
void Encoder_turbo_legacy<B>
::encode(const B *U_K, B *X_N)
{
	pi.interleave (U_K,          U_K_i.data(), 0, this->n_frames);
	sub_enc.encode(U_K,          X_N_n.data()                   );
	sub_enc.encode(U_K_i.data(), X_N_i.data()                   );

	for (auto f = 0; f < this->n_frames; f++)
	{
		const auto off1 = (3*this->K + (2 * sub_enc.tail_length())) * f;
		const auto off2 = (2*this->K + (1 * sub_enc.tail_length())) * f;
		for (auto i = 0; i < this->K; i++)
		{
			X_N[off1 + 3*i +0] = X_N_n[off2 + 2*i +0];
			X_N[off1 + 3*i +1] = X_N_n[off2 + 2*i +1];
			X_N[off1 + 3*i +2] = X_N_i[off2 + 2*i +1];
		}

		const auto off1_tails_n = off1 + 3*this->K;
		const auto off2_tails_n = off2 + 2*this->K;
		for (auto i = 0; i < sub_enc.tail_length() / 2; i++)
		{
			X_N[off1_tails_n + 2*i +0] = X_N_n[off2_tails_n + 2*i +0];
			X_N[off1_tails_n + 2*i +1] = X_N_n[off2_tails_n + 2*i +1];
		}

		const auto off1_tails_i = off1_tails_n + sub_enc.tail_length();
		const auto off2_tails_i = off2_tails_n;
		for (auto i = 0; i < sub_enc.tail_length() / 2; i++)
		{
			X_N[off1_tails_i + 2*i +0] = X_N_i[off2_tails_i + 2*i +0];
			X_N[off1_tails_i + 2*i +1] = X_N_i[off2_tails_i + 2*i +1];
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_turbo_legacy<B_8>;
template class aff3ct::module::Encoder_turbo_legacy<B_16>;
template class aff3ct::module::Encoder_turbo_legacy<B_32>;
template class aff3ct::module::Encoder_turbo_legacy<B_64>;
#else
template class aff3ct::module::Encoder_turbo_legacy<B>;
#endif
// ==================================================================================== explicit template instantiation
