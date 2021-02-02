#include <algorithm>
#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/fb_assert.h"
#include "Module/Encoder/Polar_MK/Encoder_polar_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_polar_MK<B>
::Encoder_polar_MK(const int& K, const int& N, const tools::Polar_code& code, const std::vector<bool>& frozen_bits)
: Encoder<B>(K, N),
  code(code),
  frozen_bits(frozen_bits),
  Ke(code.get_kernel_matrices().size()),
  idx(code.get_biggest_kernel_size()),
  u(code.get_biggest_kernel_size())
{
	const std::string name = "Encoder_polar_MK";
	this->set_name(name);
	this->set_sys(false);

	for (auto ke = 0; ke < (int)this->code.get_kernel_matrices().size(); ke++)
		Ke[ke].resize(this->code.get_kernel_matrices()[ke].size() * this->code.get_kernel_matrices()[ke].size());

	if (this->N != (int)this->frozen_bits.size())
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ("
		        << "'frozen_bits.size()' = " << this->frozen_bits.size() << ", "
		        << "'N' = " << this->N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto k = 0; for (auto i = 0; i < this->N; i++) if (this->frozen_bits[i] == 0) k++;
	if (this->K != k)
	{
		std::stringstream message;
		message << "The number of information bits in the frozen bits is invalid ("
		        << "'K' = " << this->K << ", "
		        << "'k' = " << k << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto ke = 0; ke < (int)this->code.get_kernel_matrices().size(); ke++)
	{
		const auto kernel_size = (int)this->code.get_kernel_matrices()[ke].size();
		for (auto i = 0; i < kernel_size; i++)
			for (auto j = 0; j < kernel_size; j++)
				this->Ke[ke][i * kernel_size +j] = (B)this->code.get_kernel_matrices()[ke][j][i];
	}

	this->set_frozen_bits(frozen_bits);
}

template <typename B>
Encoder_polar_MK<B>* Encoder_polar_MK<B>
::clone() const
{
	auto m = new Encoder_polar_MK(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_polar_MK<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	this->convert(U_K, X_N);
	this->light_encode(X_N);
}

template <typename B>
void Encoder_polar_MK<B>
::light_encode(B *X_N)
{
	auto apply_polar_kernel = [](const B *u, const uint32_t *idx, const B *Ke, B *x, const int size)
	{
		for (auto i = 0; i < size; i++)
		{
			const auto stride = i * size;
			auto sum = 0;
			for (auto j = 0; j < size; j++)
				sum += u[j] & Ke[stride +j];
			x[idx[i]] = sum & (B)1;
		}
	};

	auto n_kernels = (int)1;
	for (auto s = 0; s < (int)this->code.get_stages().size(); s++)
	{
		const auto kernel_size = (int)this->code.get_kernel_matrices()[code.get_stages()[s]].size();
		const auto n_blocks = this->N / (n_kernels * kernel_size);

		for (auto b = 0; b < n_blocks; b++)
		{
			for (auto k = 0; k < n_kernels; k++)
			{
				for (auto i = 0; i < kernel_size; i++)
				{
					this->idx[i] = (uint32_t)(b * n_kernels * kernel_size + n_kernels * i +k);
					this->u[i] = X_N[this->idx[i]];
				}

				apply_polar_kernel(this->u.data(),
				                   this->idx.data(),
				                   this->Ke[code.get_stages()[s]].data(),
				                   X_N,
				                   kernel_size);
			}
		}

		n_kernels *= kernel_size;
	}
}

template <typename B>
void Encoder_polar_MK<B>
::convert(const B *U_K, B *U_N)
{
	if (U_K == U_N)
	{
		std::vector<B> U_K_tmp(this->K);
		std::copy(U_K, U_K + this->K, U_K_tmp.begin());

		auto j = 0;
		for (unsigned i = 0; i < frozen_bits.size(); i++)
			U_N[i] = (frozen_bits[i]) ? (B)0 : U_K_tmp[j++];
	}
	else
	{
		auto j = 0;
		for (unsigned i = 0; i < frozen_bits.size(); i++)
			U_N[i] = (frozen_bits[i]) ? (B)0 : U_K[j++];
	}
}


// template <typename B>
// bool Encoder_polar_MK<B>
// ::is_codeword(const B *X_N)
// {
// 	std::copy(X_N, X_N + this->N, this->X_N_tmp.data());

// 	for (auto k = (this->N >> 1); k > 0; k >>= 1)
// 		for (auto j = 0; j < this->N; j += 2 * k)
// 		{
// 			for (auto i = 0; i < k; i++)
// 				this->X_N_tmp[j + i] = this->X_N_tmp[j + i] ^ this->X_N_tmp[k + j + i];

// 			if (this->frozen_bits[j + k -1] && this->X_N_tmp[j + k -1])
// 				return false;
// 		}

// 	return true;
// }

template <typename B>
void Encoder_polar_MK<B>
::set_frozen_bits(const std::vector<bool>& frozen_bits)
{
	aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
	std::copy(frozen_bits.begin(), frozen_bits.end(), this->frozen_bits.begin());
	auto k = 0;
	for (auto n = 0; n < this->N; n++)
		if (!frozen_bits[n])
			this->info_bits_pos[k++] = n;
}

template <typename B>
const std::vector<bool>& Encoder_polar_MK<B>
::get_frozen_bits() const
{
	return this->frozen_bits;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_polar_MK<B_8>;
template class aff3ct::module::Encoder_polar_MK<B_16>;
template class aff3ct::module::Encoder_polar_MK<B_32>;
template class aff3ct::module::Encoder_polar_MK<B_64>;
#else
template class aff3ct::module::Encoder_polar_MK<B>;
#endif
// ==================================================================================== explicit template instantiation
