#ifndef DECODER_POLAR_SC_FAST_SYS_N256_K213_SNR40_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N256_K213_SNR40_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Frozen_bits_256_213_40[256] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N256_K213_SNR40 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N256_K213_SNR40(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 256);
		assert(K == 213);
		
		auto i = 0;
		while (i < 256 && Frozen_bits_256_213_40[i] == frozen_bits[i]) i++;
		assert(i == 256);
	}

	virtual ~Decoder_polar_SC_fast_sys_N256_K213_SNR40()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <128>(   l,   0+  0,   0+128,            0+256, 128);
		API_polar::template f  < 64>(   l, 256+  0, 256+ 64,          256+128,  64);
		API_polar::template f  < 32>(   l, 384+  0, 384+ 32,          384+ 64,  32);
		API_polar::template f  < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		API_polar::template rep< 16>(s, l, 480+  0,                     0+  0,  16);
		API_polar::template gr < 16>(s, l, 448+  0, 448+ 16,   0+  0, 448+ 32,  16);
		API_polar::template f  <  8>(   l, 480+  0, 480+  8,          480+ 16,   8);
		API_polar::template rep<  8>(s, l, 496+  0,                    16+  0,   8);
		API_polar::template gr <  8>(s, l, 480+  0, 480+  8,  16+  0, 480+ 16,   8);
		API_polar::template f  <  4>(   l, 496+  0, 496+  4,          496+  8,   4);
		API_polar::template g0 <  2>(   l, 504+  0, 504+  2,          504+  4,   2);
		API_polar::template h  <  2>(s, l, 508+  0,                    26+  0,   2);
		API_polar::template xo0<  2>(s,     24+  2,                    24+  0,   2);
		API_polar::template g  <  4>(s, l, 496+  0, 496+  4,  24+  0, 496+  8,   4);
		API_polar::template h  <  4>(s, l, 504+  0,                    28+  0,   4);
		API_polar::template xo <  4>(s,     24+  0,  24+  4,           24+  0,   4);
		API_polar::template xo <  8>(s,     16+  0,  16+  8,           16+  0,   8);
		API_polar::template xo < 16>(s,      0+  0,   0+ 16,            0+  0,  16);
		API_polar::template g  < 32>(s, l, 384+  0, 384+ 32,   0+  0, 384+ 64,  32);
		API_polar::template f  < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		API_polar::template f  <  8>(   l, 480+  0, 480+  8,          480+ 16,   8);
		API_polar::template g0 <  4>(   l, 496+  0, 496+  4,          496+  8,   4);
		API_polar::template g0 <  2>(   l, 504+  0, 504+  2,          504+  4,   2);
		API_polar::template h  <  2>(s, l, 508+  0,                    38+  0,   2);
		API_polar::template xo0<  2>(s,     36+  2,                    36+  0,   2);
		API_polar::template xo0<  4>(s,     32+  4,                    32+  0,   4);
		API_polar::template g  <  8>(s, l, 480+  0, 480+  8,  32+  0, 480+ 16,   8);
		API_polar::template spc<  8>(s, l, 496+  0,                    40+  0,   8);
		API_polar::template xo <  8>(s,     32+  0,  32+  8,           32+  0,   8);
		API_polar::template g  < 16>(s, l, 448+  0, 448+ 16,  32+  0, 448+ 32,  16);
		API_polar::template spc< 16>(s, l, 480+  0,                    48+  0,  16);
		API_polar::template xo < 16>(s,     32+  0,  32+ 16,           32+  0,  16);
		API_polar::template xo < 32>(s,      0+  0,   0+ 32,            0+  0,  32);
		API_polar::template g  < 64>(s, l, 256+  0, 256+ 64,   0+  0, 256+128,  64);
		API_polar::template f  < 32>(   l, 384+  0, 384+ 32,          384+ 64,  32);
		API_polar::template f  < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		API_polar::template f  <  8>(   l, 480+  0, 480+  8,          480+ 16,   8);
		API_polar::template f  <  4>(   l, 496+  0, 496+  4,          496+  8,   4);
		API_polar::template rep<  4>(s, l, 504+  0,                    64+  0,   4);
		API_polar::template gr <  4>(s, l, 496+  0, 496+  4,  64+  0, 496+  8,   4);
		API_polar::template spc<  4>(s, l, 504+  0,                    68+  0,   4);
		API_polar::template xo <  4>(s,     64+  0,  64+  4,           64+  0,   4);
		API_polar::template g  <  8>(s, l, 480+  0, 480+  8,  64+  0, 480+ 16,   8);
		API_polar::template spc<  8>(s, l, 496+  0,                    72+  0,   8);
		API_polar::template xo <  8>(s,     64+  0,  64+  8,           64+  0,   8);
		API_polar::template g  < 16>(s, l, 448+  0, 448+ 16,  64+  0, 448+ 32,  16);
		API_polar::template spc< 16>(s, l, 480+  0,                    80+  0,  16);
		API_polar::template xo < 16>(s,     64+  0,  64+ 16,           64+  0,  16);
		API_polar::template g  < 32>(s, l, 384+  0, 384+ 32,  64+  0, 384+ 64,  32);
		API_polar::template h  < 32>(s, l, 448+  0,                    96+  0,  32);
		API_polar::template xo < 32>(s,     64+  0,  64+ 32,           64+  0,  32);
		API_polar::template xo < 64>(s,      0+  0,   0+ 64,            0+  0,  64);
		API_polar::template g  <128>(s, l,   0+  0,   0+128,   0+  0,   0+256, 128);
		API_polar::template f  < 64>(   l, 256+  0, 256+ 64,          256+128,  64);
		API_polar::template f  < 32>(   l, 384+  0, 384+ 32,          384+ 64,  32);
		API_polar::template f  < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		API_polar::template f  <  8>(   l, 480+  0, 480+  8,          480+ 16,   8);
		API_polar::template f  <  4>(   l, 496+  0, 496+  4,          496+  8,   4);
		API_polar::template rep<  4>(s, l, 504+  0,                   128+  0,   4);
		API_polar::template gr <  4>(s, l, 496+  0, 496+  4, 128+  0, 496+  8,   4);
		API_polar::template spc<  4>(s, l, 504+  0,                   132+  0,   4);
		API_polar::template xo <  4>(s,    128+  0, 128+  4,          128+  0,   4);
		API_polar::template g  <  8>(s, l, 480+  0, 480+  8, 128+  0, 480+ 16,   8);
		API_polar::template spc<  8>(s, l, 496+  0,                   136+  0,   8);
		API_polar::template xo <  8>(s,    128+  0, 128+  8,          128+  0,   8);
		API_polar::template g  < 16>(s, l, 448+  0, 448+ 16, 128+  0, 448+ 32,  16);
		API_polar::template h  < 16>(s, l, 480+  0,                   144+  0,  16);
		API_polar::template xo < 16>(s,    128+  0, 128+ 16,          128+  0,  16);
		API_polar::template g  < 32>(s, l, 384+  0, 384+ 32, 128+  0, 384+ 64,  32);
		API_polar::template h  < 32>(s, l, 448+  0,                   160+  0,  32);
		API_polar::template xo < 32>(s,    128+  0, 128+ 32,          128+  0,  32);
		API_polar::template g  < 64>(s, l, 256+  0, 256+ 64, 128+  0, 256+128,  64);
		API_polar::template h  < 64>(s, l, 384+  0,                   192+  0,  64);
		API_polar::template xo < 64>(s,    128+  0, 128+ 64,          128+  0,  64);
		API_polar::template xo <128>(s,      0+  0,   0+128,            0+  0, 128);
	}
};
}
}
#endif
