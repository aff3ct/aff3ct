#include <string>
#include <algorithm>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/LDPC/From_IRA/Encoder_LDPC_from_IRA.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_from_IRA<B>
::Encoder_LDPC_from_IRA(const int K, const int N, const tools::Sparse_matrix &_H)
: Encoder_LDPC<B>(K, N)
{
	const std::string name = "Encoder_LDPC_from_IRA";
	this->set_name(name);

	this->H = _H;

	this->check_H_dimensions();
}

template <typename B>
Encoder_LDPC_from_IRA<B>* Encoder_LDPC_from_IRA<B>
::clone() const
{
	auto m = new Encoder_LDPC_from_IRA(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_LDPC_from_IRA<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	int M = this->N - this->K;

	//Systematic part
	std::copy_n(U_K, this->K, X_N);

	//init the parity part
	B* parity = X_N + this->K;
	std::fill_n(parity, M, 0);


	//Calculate parity part
	for (auto& l : this->H.get_rows_from_col(0))
		if (l < (unsigned)this->K)
			parity[0] ^= U_K[l];

	for (auto i = 1; i < M; i++)
	{
		parity[i] = parity[i -1];

		for (auto& l : this->H.get_rows_from_col(i))
			if (l < (unsigned)this->K)
				parity[i] ^= U_K[l];
	}
}

template <typename B>
void Encoder_LDPC_from_IRA<B>
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
template class aff3ct::module::Encoder_LDPC_from_IRA<B_8>;
template class aff3ct::module::Encoder_LDPC_from_IRA<B_16>;
template class aff3ct::module::Encoder_LDPC_from_IRA<B_32>;
template class aff3ct::module::Encoder_LDPC_from_IRA<B_64>;
#else
template class aff3ct::module::Encoder_LDPC_from_IRA<B>;
#endif
// ==================================================================================== explicit template instantiation
