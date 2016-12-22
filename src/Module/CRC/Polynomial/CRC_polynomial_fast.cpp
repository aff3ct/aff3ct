#include <cassert>

#include "Tools/Display/bash_tools.h"
#include "Tools/Algo/Bit_packer.hpp"

#include "CRC_polynomial_fast.hpp"

template <typename B>
CRC_polynomial_fast<B>
::CRC_polynomial_fast(const int K, std::string poly_key, const int n_frames, const std::string name)
: CRC_polynomial<B>(K, poly_key, n_frames, name), polybyte(0), polybyte_rev(0)
{
	assert(this->polynomial.size() <= 32 +1);

	// bitpack the selected polynomial
	for (auto i = 1; i < this->polynomial.size(); i++)
	{
		polybyte <<= 1;
		polybyte |= this->polynomial[i];
	}

	// reverse the order of the bits in the bitpacked polynomial
	for (auto i = 0; i < sizeof(polybyte) * 8; i++)
	{
		polybyte_rev <<= 1;
		polybyte_rev |= (polybyte >> i) & 1;
	}
	polybyte_rev >>= (sizeof(polybyte) * 8) - (this->polynomial.size() -1);
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
			crc = (crc >> 1) ^ (-int(crc & 1) & polybyte_rev);
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
			crc = (crc >> 1) ^ (-int(crc & 1) & polybyte_rev);
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
