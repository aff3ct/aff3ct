#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC_from_H.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &H, const int n_frames)
: Encoder_LDPC<B>(K, N, n_frames), G(tools::LDPC_matrix_handler::transform_H_to_G(H, info_bits_pos))
{
	const std::string name = "Encoder_LDPC_from_H";
	this->set_name(name);
	
	// warning G is transposed !
	if (K != (int)G.get_n_cols())
	{
		std::stringstream message;
		message << "The built G matrix has a dimension 'K' different than the given one ('K' = " << K
		        << ", 'G.get_n_cols()' = " << G.get_n_cols() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != (int)G.get_n_rows())
	{
		std::stringstream message;
		message << "The built G matrix has a dimension 'N' different than the given one ('N' = " << N
		        << ", 'G.get_n_rows()' = " << G.get_n_rows() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
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
	{
		std::stringstream message;
		message << "'info_bits_pos.size()' has to be equal to 'K' ('info_bits_pos.size()' = " << info_bits_pos.size()
		        << ", 'K' = " << this->K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

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
