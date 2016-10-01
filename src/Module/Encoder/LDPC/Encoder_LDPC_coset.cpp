#include <cassert>

#include "Tools/Display/bash_tools.h"
#include "hard_coded_matrices.h"

#include "Encoder_LDPC_coset.hpp"

template <typename B>
Encoder_LDPC_coset<B>
::Encoder_LDPC_coset(const int K, const int N, const int seed, const int n_frames, const std::string name)
: Encoder_LDPC_sys<B>(K, N,       n_frames, name),
  Encoder_coset   <B>(K, N, seed, n_frames, name)
{
	assert(N == (int)n_parities_per_variable_4224.size());
	assert(K == N - (int)n_variables_per_parity_2112.size());
}

template <typename B>
Encoder_LDPC_coset<B>
::~Encoder_LDPC_coset()
{
}

template <typename B>
std::vector<unsigned char> Encoder_LDPC_coset<B>
::get_n_variables_per_parity()
{
	return n_variables_per_parity_2112;
}

template <typename B>
std::vector<unsigned char> Encoder_LDPC_coset<B>
::get_n_parities_per_variable()
{
	return n_parities_per_variable_4224;
}

template <typename B>
std::vector<unsigned int > Encoder_LDPC_coset<B>
::get_transpose()
{
	return transpose_7392;
}

template <typename B>
void Encoder_LDPC_coset<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	Encoder_coset<B>::encode(U_K, X_N);
}

template <typename B>
void Encoder_LDPC_coset<B>
::encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par)
{
	Encoder_coset<B>::encode_sys(U_K, par);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_LDPC_coset<B_8>;
template class Encoder_LDPC_coset<B_16>;
template class Encoder_LDPC_coset<B_32>;
template class Encoder_LDPC_coset<B_64>;
#else
template class Encoder_LDPC_coset<B>;
#endif
// ==================================================================================== explicit template instantiation
