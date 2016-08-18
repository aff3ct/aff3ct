#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_turbo_legacy.hpp"

template <typename B>
Encoder_turbo_legacy<B>
::Encoder_turbo_legacy(const int& K, const int& N, const Interleaver<short> &pi, Encoder_sys<B> &sub_enc, 
                       const int n_frames, const std::string name)
: Encoder<B>(n_frames, name.c_str()),
  K(K),
  N(N),
  pi(pi),
  sub_enc(sub_enc),
  U_K_i(K*n_frames),
  X_N_n((2 * (K + sub_enc.tail_length()/2))*n_frames),
  X_N_i((2 * (K + sub_enc.tail_length()/2))*n_frames)
{
	assert(n_frames > 0);
}

template <typename B>
void Encoder_turbo_legacy<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == (unsigned) (K * this->n_frames));
	assert(X_N.size() == (unsigned) ((N + (2 * sub_enc.tail_length())) * this->n_frames));
	assert((N / K) == 3);

	pi.interleave (U_K,   U_K_i);
	sub_enc.encode(U_K,   X_N_n);
	sub_enc.encode(U_K_i, X_N_i);

	for (auto f = 0; f < this->n_frames; f++)
	{
		const auto off1 = (3*K + (2 * sub_enc.tail_length())) * f;
		const auto off2 = (2*K + (1 * sub_enc.tail_length())) * f;
		for (auto i = 0; i < K; i++)
		{
			X_N[off1 + 3*i +0] = X_N_n[off2 + 2*i +0];
			X_N[off1 + 3*i +1] = X_N_n[off2 + 2*i +1];
			X_N[off1 + 3*i +2] = X_N_i[off2 + 2*i +1];
		}

		const auto off1_tails_n = off1 + 3*K;
		const auto off2_tails_n = off2 + 2*K;
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
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_turbo_legacy<B_8>;
template class Encoder_turbo_legacy<B_16>;
template class Encoder_turbo_legacy<B_32>;
template class Encoder_turbo_legacy<B_64>;
#else
template class Encoder_turbo_legacy<B>;
#endif
// ==================================================================================== explicit template instantiation