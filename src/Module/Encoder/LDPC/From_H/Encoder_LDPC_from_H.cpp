#include <cassert>
#include <iostream>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC_from_H.hpp"

using namespace aff3ct;
using namespace module;

template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::AList_reader &alist_H, const int n_frames,
                      const std::string name)
: Encoder<B>(K, N, n_frames, name), tG(N * K, 0)
{
	// TODO: compute G
//	mipp::vector<B> G; // declare the G matrix

	// transpose G
//	tools::real_transpose(K, N, G, tG);
}

template <typename B>
Encoder_LDPC_from_H<B>
::~Encoder_LDPC_from_H()
{
}

template <typename B>
void Encoder_LDPC_from_H<B>
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
template class Encoder_LDPC_from_H<B_8>;
template class Encoder_LDPC_from_H<B_16>;
template class Encoder_LDPC_from_H<B_32>;
template class Encoder_LDPC_from_H<B_64>;
#else
template class Encoder_LDPC_from_H<B>;
#endif
// ==================================================================================== explicit template instantiation
