#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Encoder_LDPC_from_H.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &H, const int n_frames)
: Encoder_LDPC<B>(K, N, n_frames)
{
	const std::string name = "Encoder_LDPC_from_H";
	this->set_name(name);

	this->G = tools::LDPC_matrix_handler::transform_H_to_G(H.turn(tools::Matrix::Way::HORIZONTAL), this->info_bits_pos);
	this->H = H;

	this->check_G_dimensions();
	this->check_H_dimensions();
}

template <typename B>
bool Encoder_LDPC_from_H<B>
::is_sys() const
{
	return Encoder<B>::is_sys();
}

template <typename B>
const std::vector<uint32_t>& Encoder_LDPC_from_H<B>
::get_info_bits_pos() const
{
	return Encoder<B>::get_info_bits_pos();
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
