#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/matrix.h"
#include "Tools/Code/LDPC/G_tools/G_tools.hpp"

#include "Encoder_LDPC_from_H.hpp"

using namespace aff3ct;
using namespace module;


template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::AList_reader &alist_H, const int n_frames,
                      const std::string name)
: Encoder_LDPC<B>(K, N, n_frames, name)
{
	std::vector<mipp::vector<B>> H;
	tools::G_tools::build_H(alist_H.get_n_CN(), alist_H.get_n_VN(), alist_H.get_CN_to_VN(), H);
	tools::G_tools::triangularization_H(H, swapped);
	assert((int) H[0].size() == N);
	assert((int)(H[0].size() - H.size()) == K);
	tools::G_tools::identity_H(H);
	tools::G_tools::transformation_H_to_G(H, G, swapped);
	tools::real_transpose(K, N, G, this->tG);
}

template <typename B>
Encoder_LDPC_from_H<B>
::~Encoder_LDPC_from_H()
{
}

template <typename B>
void Encoder_LDPC_from_H<B>
::get_info_bits_pos(mipp::vector<B>& info_bits_pos)
{
	assert(this->K <= (int)info_bits_pos.size());

	mipp::vector<B> tmp(this->N);
	std::iota(tmp.begin(), tmp.begin() + this->N, 0);

	for (unsigned l = 1; l <= (swapped.size() / 2); l++)
	{
		auto tmp_val = tmp[swapped[l * 2 -1]];
		tmp[swapped[l * 2 -1]] = tmp[swapped[(l -1) * 2]];
		tmp[swapped[(l -1) * 2]] = tmp_val;
	}

	std::copy(tmp.begin() + this->N - this->K, tmp.end(), info_bits_pos.begin());
}

template <typename B>
void Encoder_LDPC_from_H<B>
::get_G(mipp::vector<B>& matrix_G)
{
	assert(this->G.size() == G.size());

	std::copy(this->G.begin(), this->G.end(), matrix_G.begin());
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
