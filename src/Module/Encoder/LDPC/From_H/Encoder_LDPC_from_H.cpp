#include <stdexcept>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/matrix.h"
#include "Tools/Code/LDPC/G/LDPC_G.hpp"

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
	tools::LDPC_G::build_H(alist_H.get_n_CN(), alist_H.get_n_VN(), alist_H.get_CN_to_VN(), H);
	tools::LDPC_G::triangularization_H(H, swapped);

	if ((int) H[0].size() != N)
		throw std::length_error("aff3ct::module::Encoder_LDPC_from_H: \"H[0].size()\" has to be equal to \"N\".");

	if ((int)(H[0].size() - H.size()) != K)
		throw std::length_error("aff3ct::module::Encoder_LDPC_from_H: \"H[0].size()\" - \"H.size()\"  has to be "
		                        "equal to \"K\".");

	tools::LDPC_G::identity_H(H);
	tools::LDPC_G::transformation_H_to_G(H, G, swapped);
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
	if (this->K > (int)info_bits_pos.size())
		throw std::length_error("aff3ct::module::Encoder_LDPC_from_H: \"info_bits_pos.size()\" has to be equal "
		                        "or greater than \"K\".");

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
::_encode(const B *U_K, B *X_N)
{
	for (unsigned i = 0; i < G.size(); i++)
	{
		X_N[i] = 0;
		for (unsigned j = 0; j < G[i].size(); j++)
			X_N[i] += U_K[ G[i][j] - 1 ];
		X_N[i] %= 2;
	}
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
