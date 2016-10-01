#include <cassert>

#include "Tools/Display/bash_tools.h"
#include "hard_coded_matrices.h"

#include "Encoder_LDPC_AZCW.hpp"

template <typename B>
Encoder_LDPC_AZCW<B>
::Encoder_LDPC_AZCW(const int K, const int N, const int n_frames, const std::string name)
: Encoder_LDPC_sys<B>(K, N, n_frames, name),
  Encoder_AZCW    <B>(K, N, n_frames, name)
{
	assert(N == (int)n_parities_per_variable_4224.size());
	assert(K == N - (int)n_variables_per_parity_2112.size());
}

template <typename B>
Encoder_LDPC_AZCW<B>
::~Encoder_LDPC_AZCW()
{
}

template <typename B>
std::vector<unsigned char> Encoder_LDPC_AZCW<B>
::get_n_variables_per_parity()
{
	return n_variables_per_parity_2112;
}

template <typename B>
std::vector<unsigned char> Encoder_LDPC_AZCW<B>
::get_n_parities_per_variable()
{
	return n_parities_per_variable_4224;
}

template <typename B>
std::vector<unsigned int > Encoder_LDPC_AZCW<B>
::get_transpose()
{
	return transpose_7392;
}

template <typename B>
void Encoder_LDPC_AZCW<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	Encoder_AZCW<B>::encode(U_K, X_N);
}

template <typename B>
void Encoder_LDPC_AZCW<B>
::encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par)
{
	Encoder_AZCW<B>::encode_sys(U_K, par);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_LDPC_AZCW<B_8>;
template class Encoder_LDPC_AZCW<B_16>;
template class Encoder_LDPC_AZCW<B_32>;
template class Encoder_LDPC_AZCW<B_64>;
#else
template class Encoder_LDPC_AZCW<B>;
#endif
// ==================================================================================== explicit template instantiation
