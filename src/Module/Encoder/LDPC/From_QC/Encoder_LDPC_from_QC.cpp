#include <string>
#include <cstdint>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/LDPC/From_QC/Encoder_LDPC_from_QC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_from_QC<B>
::Encoder_LDPC_from_QC(const int K, const int N, const tools::Sparse_matrix &_H)
: Encoder_LDPC<B>(K, N),
  invH2(tools::LDPC_matrix_handler::LU_decomposition(_H))
{
	const std::string name = "Encoder_LDPC_from_QC";
	this->set_name(name);

	this->H = _H;

	this->check_H_dimensions();
}

template <typename B>
Encoder_LDPC_from_QC<B>* Encoder_LDPC_from_QC<B>
::clone() const
{
	auto m = new Encoder_LDPC_from_QC(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_LDPC_from_QC<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	int M = this->N - this->K;

	//Systematic part
	std::copy_n(U_K, this->K, X_N);

	//Calculate parity part
	mipp::vector<int8_t> parity(M, 0);

	for (auto i = 0; i < M; i++)
		for (auto& l : this->H.get_rows_from_col(i))
			if (l < (unsigned)this->K)
				parity[i] ^= U_K[l];

	auto* X_N_ptr = X_N + this->K;
	for (auto i = 0; i < M; i++)
	{
		X_N_ptr[i] = 0;
		for (auto j = 0; j < M; j++)
			X_N_ptr[i] ^= parity[j] & invH2[i][j];
	}
}

template <typename B>
void Encoder_LDPC_from_QC<B>
::_check_H_dimensions()
{
	Encoder_LDPC<B>::_check_H_dimensions();

	if ((this->N-this->K) != (int)this->H.get_n_cols())
	{
		std::stringstream message;
		message << "The built H matrix has a dimension '(N-K)' different than the given one ('(N-K)' = " << (this->N-this->K)
		        << ", 'H.get_n_cols()' = " << this->H.get_n_cols() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_LDPC_from_QC<B_8>;
template class aff3ct::module::Encoder_LDPC_from_QC<B_16>;
template class aff3ct::module::Encoder_LDPC_from_QC<B_32>;
template class aff3ct::module::Encoder_LDPC_from_QC<B_64>;
#else
template class aff3ct::module::Encoder_LDPC_from_QC<B>;
#endif
// ==================================================================================== explicit template instantiation
