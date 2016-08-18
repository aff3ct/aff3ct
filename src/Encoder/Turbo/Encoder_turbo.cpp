#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_turbo.hpp"

// [sys | pn | pi | tailn | taili]

template <typename B>
Encoder_turbo<B>
::Encoder_turbo(const int& K, const int& N, const Interleaver<short> &pi,
                Encoder_sys<B> &enco_n, Encoder_sys<B> &enco_i, const int n_frames, const std::string name)
: Encoder<B>(n_frames, name.c_str()),
  K(K),
  N(N),
  pi(pi),
  enco_n(enco_n),
  enco_i(enco_i),
  U_K_i(K                                    * n_frames),
  par_n(((N - K) / 2 + enco_n.tail_length()) * n_frames),
  par_i(((N - K) / 2 + enco_i.tail_length()) * n_frames)
{
}

template <typename B>
void Encoder_turbo<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == (unsigned) (K                                                 * this->n_frames));
	assert(X_N.size() == (unsigned) ((N + enco_n.tail_length() + enco_i.tail_length()) * this->n_frames));

	pi.interleave(U_K, U_K_i);

	enco_n.encode_sys(U_K,   par_n);
	enco_i.encode_sys(U_K_i, par_i);

	const auto p_si = (N - K) / 2; // size of the parity
	const auto t_n = enco_n.tail_length(); // tail_n
	const auto t_i = enco_i.tail_length(); // tail_i
	for (auto f = 0; f < this->n_frames; f++)
	{
		const auto off_U  = f * K;               // off_U_K
		const auto off_X  = f * (N + t_n + t_i); // off_U_N
		const auto off_pn = f * (p_si + t_n);    // off_par_n
		const auto off_pi = f * (p_si + t_i);    // off_par_i

		std::copy(  U_K.begin() +off_U       ,   U_K.begin() +off_U  +K        , X_N.begin() +off_X                 );
		std::copy(par_n.begin() +off_pn      , par_n.begin() +off_pn +p_si     , X_N.begin() +off_X +K              );
		std::copy(par_i.begin() +off_pi      , par_i.begin() +off_pi +p_si     , X_N.begin() +off_X +K + 1*p_si     );
		std::copy(par_n.begin() +off_pn +p_si, par_n.begin() +off_pn +p_si +t_n, X_N.begin() +off_X +K + 2*p_si     );
		std::copy(par_i.begin() +off_pi +p_si, par_i.begin() +off_pi +p_si +t_i, X_N.begin() +off_X +K + 2*p_si +t_n);
	}
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_turbo<B_8>;
template class Encoder_turbo<B_16>;
template class Encoder_turbo<B_32>;
template class Encoder_turbo<B_64>;
#else
template class Encoder_turbo<B>;
#endif
// ==================================================================================== explicit template instantiation