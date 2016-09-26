#ifndef DECODER_POLAR_SC_FAST_SYS_N128_K107_SNR40_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N128_K107_SNR40_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_128_107_40[128] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N128_K107_SNR40 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N128_K107_SNR40(const int& K, const int& N, const mipp::vector<B>& frozen_bits)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits)
	{
		assert(N == 128);
		assert(K == 107);
		
		auto i = 0;
		while (i < 128 && Frozen_bits_128_107_40[i] == frozen_bits[i]) i++;
		assert(i == 128);
	}

	virtual ~Decoder_polar_SC_fast_sys_N128_K107_SNR40()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <64>(   l,   0+  0,   0+ 64,            0+128, 64);
		API_polar::template f  <32>(   l, 128+  0, 128+ 32,          128+ 64, 32);
		API_polar::template f  <16>(   l, 192+  0, 192+ 16,          192+ 32, 16);
		API_polar::template g0 < 8>(   l, 224+  0, 224+  8,          224+ 16,  8);
		API_polar::template f  < 4>(   l, 240+  0, 240+  4,          240+  8,  4);
		API_polar::template rep< 4>(s, l, 248+  0,                     8+  0,  4);
		API_polar::template gr < 4>(s, l, 240+  0, 240+  4,   8+  0, 240+  8,  4);
		API_polar::template spc< 4>(s, l, 248+  0,                    12+  0,  4);
		API_polar::template xo < 4>(s,      8+  0,   8+  4,            8+  0,  4);
		API_polar::template xo0< 8>(s,      0+  8,                     0+  0,  8);
		API_polar::template g  <16>(s, l, 192+  0, 192+ 16,   0+  0, 192+ 32, 16);
		API_polar::template f  < 8>(   l, 224+  0, 224+  8,          224+ 16,  8);
		API_polar::template f  < 4>(   l, 240+  0, 240+  4,          240+  8,  4);
		API_polar::template rep< 4>(s, l, 248+  0,                    16+  0,  4);
		API_polar::template gr < 4>(s, l, 240+  0, 240+  4,  16+  0, 240+  8,  4);
		API_polar::template spc< 4>(s, l, 248+  0,                    20+  0,  4);
		API_polar::template xo < 4>(s,     16+  0,  16+  4,           16+  0,  4);
		API_polar::template g  < 8>(s, l, 224+  0, 224+  8,  16+  0, 224+ 16,  8);
		API_polar::template spc< 8>(s, l, 240+  0,                    24+  0,  8);
		API_polar::template xo < 8>(s,     16+  0,  16+  8,           16+  0,  8);
		API_polar::template xo <16>(s,      0+  0,   0+ 16,            0+  0, 16);
		API_polar::template g  <32>(s, l, 128+  0, 128+ 32,   0+  0, 128+ 64, 32);
		API_polar::template f  <16>(   l, 192+  0, 192+ 16,          192+ 32, 16);
		API_polar::template f  < 8>(   l, 224+  0, 224+  8,          224+ 16,  8);
		API_polar::template f  < 4>(   l, 240+  0, 240+  4,          240+  8,  4);
		API_polar::template rep< 4>(s, l, 248+  0,                    32+  0,  4);
		API_polar::template gr < 4>(s, l, 240+  0, 240+  4,  32+  0, 240+  8,  4);
		API_polar::template h  < 4>(s, l, 248+  0,                    36+  0,  4);
		API_polar::template xo < 4>(s,     32+  0,  32+  4,           32+  0,  4);
		API_polar::template g  < 8>(s, l, 224+  0, 224+  8,  32+  0, 224+ 16,  8);
		API_polar::template h  < 8>(s, l, 240+  0,                    40+  0,  8);
		API_polar::template xo < 8>(s,     32+  0,  32+  8,           32+  0,  8);
		API_polar::template g  <16>(s, l, 192+  0, 192+ 16,  32+  0, 192+ 32, 16);
		API_polar::template h  <16>(s, l, 224+  0,                    48+  0, 16);
		API_polar::template xo <16>(s,     32+  0,  32+ 16,           32+  0, 16);
		API_polar::template xo <32>(s,      0+  0,   0+ 32,            0+  0, 32);
		API_polar::template g  <64>(s, l,   0+  0,   0+ 64,   0+  0,   0+128, 64);
		API_polar::template spc<64>(s, l, 128+  0,                    64+  0, 64);
		API_polar::template xo <64>(s,      0+  0,   0+ 64,            0+  0, 64);
	}
};
#endif
