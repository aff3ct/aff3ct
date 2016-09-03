#include <cassert>

#include "hard_coded_matrices.h"

#include "Encoder_LDPC_fake.hpp"

template <typename B>
Encoder_LDPC_fake<B>
::Encoder_LDPC_fake(const int K, const int N, const int n_frames, const std::string name)
: Encoder_LDPC_sys<B>(K, N, n_frames, name)
{
	assert(N == (int)n_parities_per_variable_4224.size());
	assert(K == N - (int)n_variables_per_parity_2112.size());
}

template <typename B>
Encoder_LDPC_fake<B>
::~Encoder_LDPC_fake()
{
}

template <typename B>
std::vector<unsigned char> Encoder_LDPC_fake<B>
::get_n_variables_per_parity()
{
	return n_variables_per_parity_2112;
}

template <typename B>
std::vector<unsigned char> Encoder_LDPC_fake<B>
::get_n_parities_per_variable()
{
	return n_parities_per_variable_4224;
}

template <typename B>
std::vector<unsigned int > Encoder_LDPC_fake<B>
::get_transpose()
{
	return transpose_7392;
}

template <typename B>
void Encoder_LDPC_fake<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	std::fill(X_N.begin(), X_N.end(), (B)0);
}

template <typename B>
void Encoder_LDPC_fake<B>
::encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par)
{
	assert(par.size() == 0);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_LDPC_fake<B_8>;
template class Encoder_LDPC_fake<B_16>;
template class Encoder_LDPC_fake<B_32>;
template class Encoder_LDPC_fake<B_64>;
#else
template class Encoder_LDPC_fake<B>;
#endif
// ==================================================================================== explicit template instantiation