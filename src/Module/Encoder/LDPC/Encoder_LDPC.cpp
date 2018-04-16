#include <numeric>
#include <iostream>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC<B>
::Encoder_LDPC(const int K, const int N, const int n_frames)
: Encoder<B>(K, N, n_frames)
{
	const std::string name = "Encoder_LDPC";
	this->set_name(name);
}

template <typename B>
Encoder_LDPC<B>
::Encoder_LDPC(const int K, const int N, const tools::Sparse_matrix &G, const int n_frames)
: Encoder<B>(K, N, n_frames), tG(N * K, 0)
{
	const std::string name = "Encoder_LDPC";
	this->set_name(name);

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

	auto CN_to_VN = G.get_col_to_rows();

	std::vector<B> full_G(K * N,0);
	for (auto i = 0; i < K; i++)
		for (size_t j = 0; j < CN_to_VN[i].size(); ++j)
			full_G[i * N + CN_to_VN[i][j]] = 1;

	tools::real_transpose(K, N, full_G, tG); // transposed for computation matter
}

template <typename B>
Encoder_LDPC<B>
::~Encoder_LDPC()
{
}

template <typename B>
void Encoder_LDPC<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	// Real General Matrix Multiplication
	tools::rgemm(1, this->N, this->K, U_K, tG.data(), X_N);

	for (auto j = 0; j < this->N; ++j)
		X_N[j] %= 2;
}

template <typename B>
const std::vector<uint32_t>& Encoder_LDPC<B>
::get_info_bits_pos()
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
bool Encoder_LDPC<B>
::is_sys() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_LDPC<B_8>;
template class aff3ct::module::Encoder_LDPC<B_16>;
template class aff3ct::module::Encoder_LDPC<B_32>;
template class aff3ct::module::Encoder_LDPC<B_64>;
#else
template class aff3ct::module::Encoder_LDPC<B>;
#endif
// ==================================================================================== explicit template instantiation
