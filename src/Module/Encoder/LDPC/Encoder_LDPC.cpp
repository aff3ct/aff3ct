#include <cassert>
#include <numeric>
#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC.hpp"

using namespace aff3ct;
using namespace module;

template <typename B>
Encoder_LDPC<B>
::Encoder_LDPC(const int K, const int N, const int n_frames, const std::string name)
: Encoder<B>(K, N, n_frames, name), tG(N * K, 0)
{
}

template <typename B>
Encoder_LDPC<B>
::Encoder_LDPC(const int K, const int N, const tools::AList_reader &alist_G, const int n_frames,
               const std::string name)
: Encoder<B>(K, N, n_frames, name), tG(N * K, 0)
{
	assert(K == (int)alist_G.get_n_CN());
	assert(N == (int)alist_G.get_n_VN());

	auto CN_to_VN = alist_G.get_CN_to_VN();

	mipp::vector<B> G(K * N,0);
	for (auto i = 0; i < K; i++)
		for (size_t j = 0; j < CN_to_VN[i].size(); ++j)
			G[i * N + CN_to_VN[i][j]] = 1;

	tools::real_transpose(K, N, G, tG); // transposed for computation matter
}

template <typename B>
Encoder_LDPC<B>
::~Encoder_LDPC()
{
}

template <typename B>
void Encoder_LDPC<B>
::get_info_bits_pos(mipp::vector<B>& info_bits_pos)
{
	assert(this->K <= (int)info_bits_pos.size());
	std::iota(info_bits_pos.begin(), info_bits_pos.begin() + this->K, 0);
}

template <typename B>
void Encoder_LDPC<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(this->K == (int)U_K.size());
	assert(this->N == (int)X_N.size());

	// Real General Matrix Multiplication
	tools::rgemm(1, this->N, this->K, U_K, tG, X_N);

	for (auto j = 0; j < this->N; ++j)
		X_N[j] %= 2;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_LDPC<B_8>;
template class Encoder_LDPC<B_16>;
template class Encoder_LDPC<B_32>;
template class Encoder_LDPC<B_64>;
#else
template class Encoder_LDPC<B>;
#endif
// ==================================================================================== explicit template instantiation
