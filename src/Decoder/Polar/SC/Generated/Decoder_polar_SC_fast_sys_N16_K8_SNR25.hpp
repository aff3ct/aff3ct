#ifndef DECODER_POLAR_SC_FAST_SYS_N16_K8_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N16_K8_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_16_8_25[16] = {
1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N16_K8_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N16_K8_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits)
	{
		assert(N == 16);
		assert(K == 8);
		
		auto i = 0;
		while (i < 16 && Frozen_bits_16_8_25[i] == frozen_bits[i]) i++;
		assert(i == 16);
	}

	virtual ~Decoder_polar_SC_fast_sys_N16_K8_SNR25()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <8>(   l,  0+ 0,  0+ 8,         0+16, 8);
		API_polar::template rep<8>(s, l, 16+ 0,                0+ 0, 8);
		API_polar::template gr <8>(s, l,  0+ 0,  0+ 8,  0+ 0,  0+16, 8);
		API_polar::template spc<8>(s, l, 16+ 0,                8+ 0, 8);
		API_polar::template xo <8>(s,     0+ 0,  0+ 8,         0+ 0, 8);
	}
};
#endif
