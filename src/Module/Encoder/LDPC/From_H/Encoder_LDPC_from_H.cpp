#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC_from_H.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &H, const int n_frames)
: Encoder_LDPC<B>(K, N, n_frames), G(tools::LDPC_matrix_handler::transform_H_to_G(H, this->info_bits_pos)), H(H)
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

template <typename B>
bool Encoder_LDPC_from_H<B>
::is_codeword(const B *X_N)
{
	auto syndrome = false;

	const auto n_CN = (int)this->H.get_n_cols();
	auto i = 0;
	while (i < n_CN && !syndrome)
	{
		auto sign = 0;

		const auto n_VN = (int)this->H[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			const auto bit = X_N[this->H[i][j]];
			const auto tmp_sign = bit ? -1 : 0;

			sign ^= tmp_sign;
		}

		syndrome = syndrome || sign;
		i++;
	}

	return !syndrome;
}

template <typename B>
const std::vector<uint32_t>& Encoder_LDPC_from_H<B>
::get_info_bits_pos()
{
	return Encoder<B>::get_info_bits_pos();
}

template <typename B>
bool Encoder_LDPC_from_H<B>
::is_sys() const
{
	return Encoder<B>::is_sys();
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
