#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_turbo.hpp"

using namespace aff3ct::module;

// [sys | pn | pi | tailn | taili]

template <typename B>
Encoder_turbo<B>
::Encoder_turbo(const int& K, const int& N_without_tb, Interleaver<short> &pi,
                Encoder_sys<B> &enco_n, Encoder_sys<B> &enco_i, const int n_frames, const std::string name)
: Encoder<B>(K, N_without_tb + enco_n.tail_length() + enco_i.tail_length(), n_frames, name),
  pi(pi),
  enco_n(enco_n),
  enco_i(enco_i),
  U_K_i(K                                               * n_frames),
  par_n(((N_without_tb - K) / 2 + enco_n.tail_length()) * n_frames),
  par_i(((N_without_tb - K) / 2 + enco_i.tail_length()) * n_frames)
{
}

template <typename B>
void Encoder_turbo<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == (unsigned) (this->K * this->n_frames));
	assert(X_N.size() == (unsigned) (this->N * this->n_frames));

	pi.regen_lookup_tables();
	pi.interleave(U_K, U_K_i);

	enco_n.encode_sys(U_K,   par_n);
	enco_i.encode_sys(U_K_i, par_i);

	const auto N_without_tb = this->N - (enco_n.tail_length() + enco_i.tail_length());
	const auto p_si = (N_without_tb - this->K) / 2; // size of the parity
	const auto t_n = enco_n.tail_length(); // tail_n
	const auto t_i = enco_i.tail_length(); // tail_i
	for (auto f = 0; f < this->n_frames; f++)
	{
		const auto off_U  = f * this->K;                    // off_U_K
		const auto off_X  = f * (N_without_tb + t_n + t_i); // off_U_N
		const auto off_pn = f * (p_si + t_n);               // off_par_n
		const auto off_pi = f * (p_si + t_i);               // off_par_i

		std::copy(  U_K.begin() +off_U       ,   U_K.begin() +off_U  +this->K  , X_N.begin() +off_X                       );
		std::copy(par_n.begin() +off_pn      , par_n.begin() +off_pn +p_si     , X_N.begin() +off_X +this->K              );
		std::copy(par_i.begin() +off_pi      , par_i.begin() +off_pi +p_si     , X_N.begin() +off_X +this->K + 1*p_si     );
		std::copy(par_n.begin() +off_pn +p_si, par_n.begin() +off_pn +p_si +t_n, X_N.begin() +off_X +this->K + 2*p_si     );
		std::copy(par_i.begin() +off_pi +p_si, par_i.begin() +off_pi +p_si +t_i, X_N.begin() +off_X +this->K + 2*p_si +t_n);
	}
}

template <typename B>
void Encoder_turbo<B>
::set_n_frames(const int n_frames)
{
	assert(n_frames > 0);
	
	const int N_without_tb = this->N - (enco_n.tail_length() + enco_i.tail_length());

	Encoder<B>::set_n_frames(n_frames);
	enco_n.set_n_frames(n_frames);
	enco_i.set_n_frames(n_frames);

	U_K_i.resize(this->K * n_frames);
	par_n.resize(((N_without_tb - this->K) / 2 + enco_n.tail_length()) * n_frames);
	par_i.resize(((N_without_tb - this->K) / 2 + enco_i.tail_length()) * n_frames);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_turbo<B_8>;
template class Encoder_turbo<B_16>;
template class Encoder_turbo<B_32>;
template class Encoder_turbo<B_64>;
#else
template class Encoder_turbo<B>;
#endif
// ==================================================================================== explicit template instantiation
