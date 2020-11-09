#include <string>
#include <sstream>

#include "Tools/Code/LDPC/Syndrome/LDPC_syndrome.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC<B>
::Encoder_LDPC(const int K, const int N)
: Encoder<B>(K, N)
{
	const std::string name = "Encoder_LDPC";
	this->set_name(name);
}

template <typename B>
Encoder_LDPC<B>* Encoder_LDPC<B>
::clone() const
{
	auto m = new Encoder_LDPC(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
Encoder_LDPC<B>
::Encoder_LDPC(const int K, const int N, const tools::Sparse_matrix &G)
: Encoder<B>(K, N), G(G)
{
	const std::string name = "Encoder_LDPC";
	this->set_name(name);

	check_G_dimensions();
}

template <typename B>
Encoder_LDPC<B>
::Encoder_LDPC(const int K, const int N, const tools::Sparse_matrix &G, const tools::Sparse_matrix &H)
: Encoder<B>(K, N), G(G), H(H)
{
	const std::string name = "Encoder_LDPC";
	this->set_name(name);

	check_G_dimensions();
	check_H_dimensions();
}

template <typename B>
void Encoder_LDPC<B>
::check_G_dimensions()
{
	G.self_turn(tools::Sparse_matrix::Way::VERTICAL);
	this->_check_G_dimensions();
}

template <typename B>
void Encoder_LDPC<B>
::check_H_dimensions()
{
	H.self_turn(tools::Sparse_matrix::Way::VERTICAL);
	this->_check_H_dimensions();
}

template <typename B>
void Encoder_LDPC<B>
::_check_G_dimensions()
{
	if (this->K != (int)this->G.get_n_cols())
	{
		std::stringstream message;
		message << "The built G matrix has a dimension 'K' different than the given one ('K' = " << this->K
		        << ", 'G.get_n_cols()' = " << this->G.get_n_cols() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != (int)this->G.get_n_rows())
	{
		std::stringstream message;
		message << "The built G matrix has a dimension 'N' different than the given one ('N' = " << this->N
		        << ", 'G.get_n_rows()' = " << this->G.get_n_rows() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
void Encoder_LDPC<B>
::_check_H_dimensions()
{
	if (this->N != (int)this->H.get_n_rows())
	{
		std::stringstream message;
		message << "The built H matrix has a dimension 'N' different than the given one ('N' = " << this->N
		        << ", 'H.get_n_rows()' = " << this->H.get_n_rows() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
void Encoder_LDPC<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	for (auto i = 0; i < this->N; i++)
	{
		X_N[i] = 0;
		auto& links = this->G.get_cols_from_row(i);
		for (unsigned j = 0; j < links.size(); j++)
			X_N[i] += U_K[ links[j] ];
		X_N[i] &= (B)1; // modulo 2
	}
}

template <typename B>
bool Encoder_LDPC<B>
::is_codeword(const B *X_N)
{
	if (this->H.get_n_connections() == 0)
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);

	return tools::LDPC_syndrome::check_hard(X_N, this->H);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_LDPC<B_8>;
template class aff3ct::module::Encoder_LDPC<B_16>;
template class aff3ct::module::Encoder_LDPC<B_32>;
template class aff3ct::module::Encoder_LDPC<B_64>;
#else
template class aff3ct::module::Encoder_LDPC<B>;
#endif
// ==================================================================================== explicit template instantiation
