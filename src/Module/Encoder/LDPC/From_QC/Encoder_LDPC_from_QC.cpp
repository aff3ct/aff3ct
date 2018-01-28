#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC_from_QC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_from_QC<B>
::Encoder_LDPC_from_QC(const int K, const int N, const tools::Sparse_matrix &_H, const int n_frames)
: Encoder_LDPC<B>(K, N, n_frames),
  H((_H.get_n_rows() > _H.get_n_cols())?_H.transpose():_H),
  invH2(tools::LDPC_matrix_handler::invert_H2(_H))
{
	const std::string name = "Encoder_LDPC_from_QC";
	this->set_name(name);
	
	if ((N-K) != (int)H.get_n_rows())
	{
		std::stringstream message;
		message << "The built H matrix has a dimension '(N-K)' different than the given one ('(N-K)' = " << (N-K)
		        << ", 'H.get_n_rows()' = " << H.get_n_rows() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != (int)H.get_n_cols())
	{
		std::stringstream message;
		message << "The built H matrix has a dimension 'N' different than the given one ('N' = " << N
		        << ", 'H.get_n_cols()' = " << H.get_n_cols() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
Encoder_LDPC_from_QC<B>
::~Encoder_LDPC_from_QC()
{
}

template <typename B>
void Encoder_LDPC_from_QC<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	unsigned M = this->N - this->K;

	//Systematic part
	std::copy_n(U_K, this->K, X_N);

	//Calculate parity part
	mipp::vector<int8_t> tableauCalcul(M, 0);
	for (unsigned i = 0; i < M; i++)
	{
		for (unsigned j = 0; j < H.get_cols_from_row(i).size(); j++)
			if (H.get_cols_from_row(i)[j] < (unsigned)this->K)
				tableauCalcul[i] ^= U_K[ H.get_cols_from_row(i)[j] ];
			else
				break;
	}

	for (unsigned i = 0; i < M; i++)
	{
		X_N[this->K + i] = 0;
		for (unsigned j = 0; j < M; j++)
			X_N[this->K + i] += tableauCalcul[j] & invH2[i][j];
		X_N[this->K + i] %= 2;
	}
}

template <typename B>
bool Encoder_LDPC_from_QC<B>
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

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_LDPC_from_QC<B_8>;
template class aff3ct::module::Encoder_LDPC_from_QC<B_16>;
template class aff3ct::module::Encoder_LDPC_from_QC<B_32>;
template class aff3ct::module::Encoder_LDPC_from_QC<B_64>;
#else
template class aff3ct::module::Encoder_LDPC_from_QC<B>;
#endif
// ==================================================================================== explicit template instantiation
