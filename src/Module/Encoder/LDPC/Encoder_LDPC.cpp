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
: Encoder<B>(K, N, n_frames), G(G)
{
	const std::string name = "Encoder_LDPC";
	this->set_name(name);

	check_G_dimensions();
}

template <typename B>
Encoder_LDPC<B>
::Encoder_LDPC(const int K, const int N, const tools::Sparse_matrix &G, const tools::Sparse_matrix &H, const int n_frames)
: Encoder<B>(K, N, n_frames), G(G), H(H)
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
	try
	{
		this->_check_G_dimensions();
	}
	catch(tools::invalid_argument&)
	{
		this->G.self_transpose();
		this->_check_G_dimensions();
	}
}

template <typename B>
void Encoder_LDPC<B>
::check_H_dimensions()
{
	try
	{
		this->_check_H_dimensions();
	}
	catch(tools::invalid_argument&)
	{
		this->H.self_transpose();
		this->_check_H_dimensions();
	}
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
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	for (unsigned i = 0; i < this->N; i++)
	{
		X_N[i] = 0;
		auto& links = this->G.get_cols_from_row(i);
		for (unsigned j = 0; j < links.size(); j++)
			X_N[i] += U_K[ links[j] ];
		X_N[i] &= (B)1; // modulo 2
	}
}

template <typename B>
const std::vector<uint32_t>& Encoder_LDPC<B>
::get_info_bits_pos() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
bool Encoder_LDPC<B>
::is_sys() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
bool Encoder_LDPC<B>
::is_codeword(const B *X_N)
{
	if (this->H.get_n_connections() == 0)
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);

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
template class aff3ct::module::Encoder_LDPC<B_8>;
template class aff3ct::module::Encoder_LDPC<B_16>;
template class aff3ct::module::Encoder_LDPC<B_32>;
template class aff3ct::module::Encoder_LDPC<B_64>;
#else
template class aff3ct::module::Encoder_LDPC<B>;
#endif
// ==================================================================================== explicit template instantiation
