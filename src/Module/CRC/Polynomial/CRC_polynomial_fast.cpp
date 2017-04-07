#include <stdexcept>

#include "Tools/Display/bash_tools.h"
#include "Tools/Algo/Bit_packer.hpp"

#include "CRC_polynomial_fast.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
CRC_polynomial_fast<B>
::CRC_polynomial_fast(const int K, std::string poly_key, const int size, const int n_frames, const std::string name)
: CRC_polynomial<B>(K, poly_key, size, n_frames, name), lut_crc32(256), polynomial_packed_rev(0)
{
	if (this->size() > 32)
		throw std::length_error("aff3ct::module::CRC_polynomial_fast: \"this->size()\" has to be equal or smaller "
		                        "than 32.");

	// reverse the order of the bits in the bitpacked polynomial
	for (auto i = 0; i < (int)sizeof(this->polynomial_packed) * 8; i++)
	{
		polynomial_packed_rev <<= 1;
		polynomial_packed_rev |= (this->polynomial_packed >> i) & 1;
	}
	polynomial_packed_rev >>= (sizeof(this->polynomial_packed) * 8) - (this->size());

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
void CRC_polynomial_fast<B>
::_build(B *U_K)
{
#if __BYTE_ORDER != __LITTLE_ENDIAN
	throw std::runtime_error("aff3ct::module::CRC_polynomial_fast: the code of the fast CRC works only on "
	                         "little endian CPUs (x86, ARM, ...)");
#endif

	Bit_packer<B>::pack(U_K, this->buff_crc.data(), this->K);

	const auto data = (unsigned char*)this->buff_crc.data();
	const auto crc  = this->compute_crc_v3((void*)data, this->K - this->size());

	for (auto i = 0; i < this->size(); i++)
		U_K[this->K - this->size() +i] = (crc >> i) & 1;
}

template <typename B>
bool CRC_polynomial_fast<B>
::_check(const B *V_K)
{
	Bit_packer<B>::pack(V_K, this->buff_crc.data(), this->K);
	return this->_check_packed(this->buff_crc.data());
}

template <typename B>
bool CRC_polynomial_fast<B>
::_check_packed(const B *V_K)
{
#if __BYTE_ORDER != __LITTLE_ENDIAN
	throw std::runtime_error("aff3ct::module::CRC_polynomial_fast: the code of the fast CRC works only on "
	                         "little endian CPUs (x86, ARM, ...)");
#endif

	const auto crc_size = this->size();
	const auto rest     = this->K % 8;

	unsigned char* bytes = (unsigned char*)V_K;

	unsigned crc_invalid = 0;

	const auto data = bytes;
	const auto crc  = this->compute_crc_v3((void*)data, this->K - crc_size);

	auto n_bits_crc = crc_size;
	auto current = data + ((this->K - crc_size) / 8);

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
	throw std::runtime_error("aff3ct::module::CRC_polynomial_fast: the code of the fast CRC works only on "
	                         "little endian CPUs (x86, ARM, ...)");
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
	throw std::runtime_error("aff3ct::module::CRC_polynomial_fast: the code of the fast CRC works only on "
	                         "little endian CPUs (x86, ARM, ...)");
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
	throw std::runtime_error("aff3ct::module::CRC_polynomial_fast: the code of the fast CRC works only on "
	                         "little endian CPUs (x86, ARM, ...)");
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
#ifdef MULTI_PREC
template class aff3ct::module::CRC_polynomial_fast<B_8>;
template class aff3ct::module::CRC_polynomial_fast<B_16>;
template class aff3ct::module::CRC_polynomial_fast<B_32>;
template class aff3ct::module::CRC_polynomial_fast<B_64>;
#else
template class aff3ct::module::CRC_polynomial_fast<B>;
#endif
// ==================================================================================== explicit template instantiation
