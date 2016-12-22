#include <cassert>

#include "Tools/Display/bash_tools.h"
#include "Tools/Algo/Bit_packer.hpp"

#include "CRC_polynomial_fast.hpp"

template <typename B>
CRC_polynomial_fast<B>
::CRC_polynomial_fast(const int K, std::string poly_key, const int size, const int n_frames, const std::string name)
: CRC_polynomial<B>(K, poly_key, size, n_frames, name), polynomial_packed_rev(0)
{
	assert(this->size() <= 32);

	// reverse the order of the bits in the bitpacked polynomial
	for (auto i = 0; i < sizeof(this->polynomial_packed) * 8; i++)
	{
		polynomial_packed_rev <<= 1;
		polynomial_packed_rev |= (this->polynomial_packed >> i) & 1;
	}
	polynomial_packed_rev >>= (sizeof(this->polynomial_packed) * 8) - (this->size());
}

// Source of inspiration: http://create.stephan-brumme.com/crc32/
template <typename B>
bool CRC_polynomial_fast<B>
::check(const mipp::vector<B>& V_K, const int n_frames)
{
	assert(V_K.size() == this->buff_crc.size());
	Bit_packer<B>::pack(V_K, this->buff_crc);
	return this->check_packed(this->buff_crc, n_frames);
}

// Source of inspiration: http://create.stephan-brumme.com/crc32/
template <typename B>
bool CRC_polynomial_fast<B>
::check_packed(const mipp::vector<B>& V_K, const int n_frames)
{
#if __BYTE_ORDER != __LITTLE_ENDIAN
	std::cout << bold_red("(EE) This fast CRC code works only on little endian CPUs (x86, ARM, ...).") << std::endl;
	std::exit(-1);
#endif

	assert((n_frames == -1 && this->n_frames == 1) || n_frames == 1);

	unsigned char* current = (unsigned char*)V_K.data();

	unsigned crc = 0;
	auto length = (V_K.size() - this->size()) / 8;
	while (length--)
	{
		crc ^= *current++;
		for (unsigned int j = 0; j < 8; j++)
//			crc = (crc & 1) ? (crc >> 1) ^ polybyte_rev : crc >> 1;
			crc = (crc >> 1) ^ (-int(crc & 1) & polynomial_packed_rev);
	}

	auto rest = (V_K.size() - this->size()) % 8;
	if (rest != 0)
	{
		auto cur = *current;
		cur <<= 8 - rest;
		cur >>= 8 - rest;

		crc ^= cur;
		for (unsigned int j = 0; j < rest; j++)
//			crc = (crc & 1) ? (crc >> 1) ^ polybyte_rev : crc >> 1;;
			crc = (crc >> 1) ^ (-int(crc & 1) & polynomial_packed_rev);
	}

	unsigned crc_ref = 0;
	const auto size = this->size();
	auto n_bits = this->size();
	if (rest)
	{
		auto cur = *current++;
		for (auto j = rest; j < 8 && n_bits > 0; j++)
			crc_ref |= ((cur >> j) & 1) << (size - n_bits--);
	}
	while (n_bits)
	{
		auto cur = *current++;
		for (auto j = 0; j < 8 && n_bits > 0; j++)
			crc_ref |= ((cur >> j) & 1) << (size - n_bits--);
	}

	return crc == crc_ref;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class CRC_polynomial_fast<B_8>;
template class CRC_polynomial_fast<B_16>;
template class CRC_polynomial_fast<B_32>;
template class CRC_polynomial_fast<B_64>;
#else
template class CRC_polynomial_fast<B>;
#endif
// ==================================================================================== explicit template instantiation
