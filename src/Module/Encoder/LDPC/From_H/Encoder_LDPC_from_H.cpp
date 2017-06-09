#include <stdexcept>
#include <iostream>
#include <vector>
#include <numeric>
#include <functional>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC_from_H.hpp"

using namespace aff3ct;
using namespace module;

template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &H, const int n_frames,
                      const std::string name)
: Encoder_LDPC<B>(K, N, n_frames, name), G(tools::LDPC_matrix_handler::transform_H_to_G(H, info_bits_pos))
{
	// warning G is transposed !
	if (this->K != (int)G.get_n_cols())
		throw std::runtime_error("aff3ct::module::Encoder_LDPC_from_H: the built G matrix has a dimension \"K\" "
		                         "different than the given one.");

	if (this->N != (int)G.get_n_rows())
		throw std::runtime_error("aff3ct::module::Encoder_LDPC_from_H: the built G matrix has a dimension \"N\" "
		                         "different than the given one.");
}

template <typename B>
Encoder_LDPC_from_H<B>
::~Encoder_LDPC_from_H()
{
}

template <typename B>
void Encoder_LDPC_from_H<B>
::get_info_bits_pos(std::vector<unsigned>& info_bits_pos)
{
	if (this->K != (int)info_bits_pos.size())
		throw std::length_error("aff3ct::module::Encoder_LDPC_from_H: \"info_bits_pos.size()\" has to be equal "
		                        "or greater than \"K\".");

	std::copy(this->info_bits_pos.begin(), this->info_bits_pos.end(), info_bits_pos.begin());
}

template <typename B>
void Encoder_LDPC_from_H<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	for (unsigned i = 0; i < G.get_n_rows(); i++)
	{
		X_N[i] = 0;
		for (unsigned j = 0; j < G.get_cols_from_row(i).size(); j++)
			X_N[i] += U_K[ G.get_cols_from_row(i)[j] ];
		X_N[i] %= 2;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_LDPC_from_H<B_8>;
template class aff3ct::module::Encoder_LDPC_from_H<B_16>;
template class aff3ct::module::Encoder_LDPC_from_H<B_32>;
template class aff3ct::module::Encoder_LDPC_from_H<B_64>;
#else
template class aff3ct::module::Encoder_LDPC_from_H<B>;
#endif
// ==================================================================================== explicit template instantiation
