#include <algorithm>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
CRC_polynomial_fast<B>
::CRC_polynomial_fast(const int K, const std::string &poly_key, const int size)
: CRC_polynomial<B>(K, poly_key, size), lut_crc32(256), polynomial_packed_rev(0)
{
	const std::string name = "CRC_polynomial_fast";
	this->set_name(name);

	if (this->size > 32)
	{
		std::stringstream message;
		message << "'size' has to be equal or smaller than 32 ('size' = " << this->size << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	// reverse the order of the bits in the bitpacked polynomial
	for (auto i = 0; i < (int)sizeof(this->polynomial_packed) * 8; i++)
	{
		polynomial_packed_rev <<= 1;
		polynomial_packed_rev |= (this->polynomial_packed >> i) & 1;
	}
	polynomial_packed_rev >>= (sizeof(this->polynomial_packed) * 8) - (this->size);

	// precompute a lookup table pour the v3 implem. of the CRC
	for (auto i = 0; i < 256; i++)
	{
		unsigned crc = i;
		for (unsigned int j = 0; j < 8; j++)
			crc = (crc >> 1) ^ (-int(crc & 1) & polynomial_packed_rev);
		lut_crc32[i] = crc;
	}
}

template <typename B>
CRC_polynomial_fast<B>* CRC_polynomial_fast<B>
::clone() const
{
	auto m = new CRC_polynomial_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void CRC_polynomial_fast<B>
::_build(const B *U_K1, B *U_K2, const size_t frame_id)
{
#if __BYTE_ORDER != __LITTLE_ENDIAN
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "The code of the fast CRC works only on little endian CPUs.");
#endif

	const auto data = (unsigned char*)this->buff_crc.data();
	tools::Bit_packer::pack(U_K1, data, this->K);

	const auto crc  = this->compute_crc_v3((void*)data, this->K);

	std::copy(U_K1, U_K1 + this->K, U_K2);
	for (auto i = 0; i < this->size; i++)
		U_K2[this->K +i] = (crc >> i) & 1;
}

template <typename B>
bool CRC_polynomial_fast<B>
::_check(const B *V_K, const size_t frame_id)
{
	tools::Bit_packer::pack(V_K, (unsigned char*)this->buff_crc.data(), this->K + this->size);
	return this->_check_packed(this->buff_crc.data(), frame_id);
}

template <typename B>
bool CRC_polynomial_fast<B>
::_check_packed(const B *V_K, const size_t frame_id)
{
#if __BYTE_ORDER != __LITTLE_ENDIAN
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "The code of the fast CRC works only on little endian CPUs.");
#endif

	const auto crc_size = this->size;
	const auto rest     = this->K % 8;

	unsigned char* bytes = (unsigned char*)V_K;

	unsigned crc_invalid = 0;

	const auto data = bytes;
	const auto crc  = this->compute_crc_v3((void*)data, this->K);

	auto n_bits_crc = crc_size;
	auto current = data + (this->K / 8);

	unsigned crc_ref = 0;
	if (rest)
	{
		auto cur = *current++;
		for (auto j = rest; j < 8 && n_bits_crc > 0; j++)
			crc_ref |= ((cur >> j) & 1) << (crc_size - n_bits_crc--);
	}
	while (n_bits_crc)
	{
		auto cur = *current++;
		for (auto j = 0; j < 8 && n_bits_crc > 0; j++)
			crc_ref |= ((cur >> j) & 1) << (crc_size - n_bits_crc--);
	}

	crc_invalid |= crc ^ crc_ref;

	return !crc_invalid;
}

// Source of inspiration: http://create.stephan-brumme.com/crc32/ (Fastest Bitwise CRC32)
template <typename B>
unsigned CRC_polynomial_fast<B>
::compute_crc_v1(const void* data, const int n_bits)
{
#if __BYTE_ORDER != __LITTLE_ENDIAN
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "The code of the fast CRC works only on little endian CPUs.");
#endif

	unsigned crc = 0;

	auto current = (unsigned char*)data;
	auto length = n_bits / 8;
	while (length--)
	{
		crc ^= *current++;
		for (auto j = 0; j < 8; j++)
//			crc = (crc & 1) ? (crc >> 1) ^ polynomial_packed_rev : crc >> 1;
			crc = (crc >> 1) ^ (-int(crc & 1) & polynomial_packed_rev);
	}

	auto rest = n_bits % 8;
	if (rest != 0)
	{
		auto cur = *current;
		cur <<= 8 - rest;
		cur >>= 8 - rest;

		crc ^= cur;
		for (auto j = 0; j < rest; j++)
//			crc = (crc & 1) ? (crc >> 1) ^ polynomial_packed_rev : crc >> 1;
			crc = (crc >> 1) ^ (-int(crc & 1) & polynomial_packed_rev);
	}

	return crc;
}

// Source of inspiration: http://create.stephan-brumme.com/crc32/ (Fastest Bitwise CRC32)
template <typename B>
unsigned CRC_polynomial_fast<B>
::compute_crc_v2(const void* data, const int n_bits)
{
#if __BYTE_ORDER != __LITTLE_ENDIAN
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "The code of the fast CRC works only on little endian CPUs.");
#endif

	unsigned crc = 0;

	auto current = (unsigned int*)data;
	auto length = n_bits / 32;
	while (length--)
	{
		crc ^= *current++;
		for (auto j = 0; j < 32; j++)
//			crc = (crc & 1) ? (crc >> 1) ^ polynomial_packed_rev : crc >> 1;
			crc = (crc >> 1) ^ (-int(crc & 1) & polynomial_packed_rev);
	}

	auto rest = n_bits % 32;
	if (rest != 0)
	{
		auto cur = *current;
		cur <<= 32 - rest;
		cur >>= 32 - rest;

		crc ^= cur;
		for (auto j = 0; j < rest; j++)
//			crc = (crc & 1) ? (crc >> 1) ^ polynomial_packed_rev : crc >> 1;
			crc = (crc >> 1) ^ (-int(crc & 1) & polynomial_packed_rev);
	}

	return crc;
}

// Source of inspiration: http://create.stephan-brumme.com/crc32/ (Standard Implementation)
template <typename B>
unsigned CRC_polynomial_fast<B>
::compute_crc_v3(const void* data, const int n_bits)
{
#if __BYTE_ORDER != __LITTLE_ENDIAN
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "The code of the fast CRC works only on little endian CPUs.");
#endif

	unsigned crc = 0;

	auto current = (unsigned char*)data;
	auto length = n_bits / 8;
	while (length--)
		crc = (crc >> 8) ^ lut_crc32[(crc & 0xFF) ^ *current++];

	auto rest = n_bits % 8;
	if (rest != 0)
	{
		auto cur = *current;
		cur <<= 8 - rest;
		cur >>= 8 - rest;

		crc ^= cur;
		for (auto j = 0; j < rest; j++)
			crc = (crc >> 1) ^ (-int(crc & 1) & polynomial_packed_rev);
	}

	return crc;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::CRC_polynomial_fast<B_8>;
template class aff3ct::module::CRC_polynomial_fast<B_16>;
template class aff3ct::module::CRC_polynomial_fast<B_32>;
template class aff3ct::module::CRC_polynomial_fast<B_64>;
#else
template class aff3ct::module::CRC_polynomial_fast<B>;
#endif
// ==================================================================================== explicit template instantiation
