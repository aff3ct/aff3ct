#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/fb_assert.h"
#include "Module/Encoder/Polar/Encoder_polar.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_polar<B>
::Encoder_polar(const int& K, const int& N, const std::vector<bool>& frozen_bits)
: Encoder<B>(K, N), m((int)std::log2(N)), frozen_bits(frozen_bits), X_N_tmp(this->N)
{
	const std::string name = "Encoder_polar";
	this->set_name(name);
	this->set_sys(false);

	if (this->N != (int)frozen_bits.size())
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ('frozen_bits.size()' = " << frozen_bits.size()
		        << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->set_frozen_bits(frozen_bits);
}

template <typename B>
Encoder_polar<B>* Encoder_polar<B>
::clone() const
{
	auto m = new Encoder_polar(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_polar<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	this->convert(U_K, X_N);
	this->light_encode(X_N);
}

template <typename B>
void Encoder_polar<B>
::light_encode(B *bits)
{
	for (auto k = (this->N >> 1); k > 0; k >>= 1)
		for (auto j = 0; j < this->N; j += 2 * k)
			for (auto i = 0; i < k; i++)
				bits[j + i] = bits[j + i] ^ bits[k + j + i];
}

template <typename B>
void Encoder_polar<B>
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

template <typename B>
bool Encoder_polar<B>
::is_codeword(const B *X_N)
{
	std::copy(X_N, X_N + this->N, this->X_N_tmp.data());

	for (auto k = (this->N >> 1); k > 0; k >>= 1)
		for (auto j = 0; j < this->N; j += 2 * k)
		{
			for (auto i = 0; i < k; i++)
				this->X_N_tmp[j + i] = this->X_N_tmp[j + i] ^ this->X_N_tmp[k + j + i];

			if (this->frozen_bits[j + k -1] && this->X_N_tmp[j + k -1])
				return false;
		}

	return true;
}

template <typename B>
void Encoder_polar<B>
::set_frozen_bits(const std::vector<bool>& frozen_bits)
{	
	aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
	std::copy(frozen_bits.begin(), frozen_bits.end(), this->frozen_bits.begin());
	auto k = 0;
	for (auto n = 0; n < this->N; n++)
		if (!this->frozen_bits[n])
			this->info_bits_pos[k++] = n;
}


template <typename B>
const std::vector<bool>& Encoder_polar<B>
::get_frozen_bits() const
{
	return this->frozen_bits;
}
// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_polar<B_8>;
template class aff3ct::module::Encoder_polar<B_16>;
template class aff3ct::module::Encoder_polar<B_32>;
template class aff3ct::module::Encoder_polar<B_64>;
#else
template class aff3ct::module::Encoder_polar<B>;
#endif
// ==================================================================================== explicit template instantiation
