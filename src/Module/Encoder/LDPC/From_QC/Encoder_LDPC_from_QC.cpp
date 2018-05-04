#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC_from_QC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_from_QC<B>
::Encoder_LDPC_from_QC(const int K, const int N, const tools::Sparse_matrix &_H, const int n_frames)
: Encoder_LDPC<B>(K, N, tools::Sparse_matrix(), _H, n_frames),
  invH2(tools::LDPC_matrix_handler::invert_H2(_H))
{
	const std::string name = "Encoder_LDPC_from_QC";
	this->set_name(name);
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
		auto& links = this->H.get_cols_from_row(i);
		for (unsigned j = 0; j < links.size(); j++)
			if (links[j] < (unsigned)this->K)
				tableauCalcul[i] ^= U_K[ links[j] ];
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
const std::vector<uint32_t>& Encoder_LDPC_from_QC<B>
::get_info_bits_pos() const
{
	return Encoder<B>::get_info_bits_pos();
}

template <typename B>
bool Encoder_LDPC_from_QC<B>
::is_sys() const
{
	return Encoder<B>::is_sys();
}

template <typename B>
void Encoder_LDPC_from_QC<B>
::_check_H_dimensions()
{
	Encoder_LDPC<B>::check_H_dimensions();

	if ((this->N-this->K) != (int)this->H.get_n_rows())
	{
		std::stringstream message;
		message << "The built H matrix has a dimension '(N-K)' different than the given one ('(N-K)' = " << (this->N-this->K)
		        << ", 'H.get_n_rows()' = " << this->H.get_n_rows() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
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
