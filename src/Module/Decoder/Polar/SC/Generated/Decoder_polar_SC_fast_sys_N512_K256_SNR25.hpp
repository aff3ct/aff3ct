#ifndef DECODER_POLAR_SC_FAST_SYS_N512_K256_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N512_K256_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_512_256_25[512] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N512_K256_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N512_K256_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits)
	{
		assert(N == 512);
		assert(K == 256);
		
		auto i = 0;
		while (i < 512 && Frozen_bits_512_256_25[i] == frozen_bits[i]) i++;
		assert(i == 512);
	}

	virtual ~Decoder_polar_SC_fast_sys_N512_K256_SNR25()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <256>(   l,    0+   0,    0+ 256,               0+ 512, 256);
		API_polar::template f  <128>(   l,  512+   0,  512+ 128,             512+ 256, 128);
		API_polar::template g0 < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template rep< 32>(s, l,  960+   0,                         64+   0,  32);
		API_polar::template gr < 32>(s, l,  896+   0,  896+  32,   64+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template rep< 16>(s, l,  992+   0,                         96+   0,  16);
		API_polar::template gr < 16>(s, l,  960+   0,  960+  16,   96+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template rep<  8>(s, l, 1008+   0,                        112+   0,   8);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,  112+   0,  992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template g0 <  2>(   l, 1016+   0, 1016+   2,            1016+   4,   2);
		API_polar::template h  <  2>(s, l, 1020+   0,                        122+   0,   2);
		API_polar::template xo0<  2>(s,     120+   2,                        120+   0,   2);
		API_polar::template g  <  4>(s, l, 1008+   0, 1008+   4,  120+   0, 1008+   8,   4);
		API_polar::template h  <  4>(s, l, 1016+   0,                        124+   0,   4);
		API_polar::template xo <  4>(s,     120+   0,  120+   4,             120+   0,   4);
		API_polar::template xo <  8>(s,     112+   0,  112+   8,             112+   0,   8);
		API_polar::template xo < 16>(s,      96+   0,   96+  16,              96+   0,  16);
		API_polar::template xo < 32>(s,      64+   0,   64+  32,              64+   0,  32);
		API_polar::template xo0< 64>(s,       0+  64,                          0+   0,  64);
		API_polar::template g  <128>(s, l,  512+   0,  512+ 128,    0+   0,  512+ 256, 128);
		API_polar::template f  < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template rep< 32>(s, l,  960+   0,                        128+   0,  32);
		API_polar::template gr < 32>(s, l,  896+   0,  896+  32,  128+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template g0 <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template g0 <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template g0 <  2>(   l, 1016+   0, 1016+   2,            1016+   4,   2);
		API_polar::template h  <  2>(s, l, 1020+   0,                        174+   0,   2);
		API_polar::template xo0<  2>(s,     172+   2,                        172+   0,   2);
		API_polar::template xo0<  4>(s,     168+   4,                        168+   0,   4);
		API_polar::template xo0<  8>(s,     160+   8,                        160+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  160+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                        176+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,  176+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                        180+   0,   4);
		API_polar::template xo <  4>(s,     176+   0,  176+   4,             176+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  176+   0,  992+  16,   8);
		API_polar::template spc<  8>(s, l, 1008+   0,                        184+   0,   8);
		API_polar::template xo <  8>(s,     176+   0,  176+   8,             176+   0,   8);
		API_polar::template xo < 16>(s,     160+   0,  160+  16,             160+   0,  16);
		API_polar::template xo < 32>(s,     128+   0,  128+  32,             128+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  128+   0,  768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template g0 <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                        200+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,  200+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                        204+   0,   4);
		API_polar::template xo <  4>(s,     200+   0,  200+   4,             200+   0,   4);
		API_polar::template xo0<  8>(s,     192+   8,                        192+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  192+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                        208+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,  208+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                        212+   0,   4);
		API_polar::template xo <  4>(s,     208+   0,  208+   4,             208+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  208+   0,  992+  16,   8);
		API_polar::template spc<  8>(s, l, 1008+   0,                        216+   0,   8);
		API_polar::template xo <  8>(s,     208+   0,  208+   8,             208+   0,   8);
		API_polar::template xo < 16>(s,     192+   0,  192+  16,             192+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  192+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                        224+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,  224+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                        228+   0,   4);
		API_polar::template xo <  4>(s,     224+   0,  224+   4,             224+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  224+   0,  992+  16,   8);
		API_polar::template h  <  8>(s, l, 1008+   0,                        232+   0,   8);
		API_polar::template xo <  8>(s,     224+   0,  224+   8,             224+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  224+   0,  960+  32,  16);
		API_polar::template h  < 16>(s, l,  992+   0,                        240+   0,  16);
		API_polar::template xo < 16>(s,     224+   0,  224+  16,             224+   0,  16);
		API_polar::template xo < 32>(s,     192+   0,  192+  32,             192+   0,  32);
		API_polar::template xo < 64>(s,     128+   0,  128+  64,             128+   0,  64);
		API_polar::template xo <128>(s,       0+   0,    0+ 128,               0+   0, 128);
		API_polar::template g  <256>(s, l,    0+   0,    0+ 256,    0+   0,    0+ 512, 256);
		API_polar::template f  <128>(   l,  512+   0,  512+ 128,             512+ 256, 128);
		API_polar::template f  < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template g0 < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template g0 <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                        280+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,  280+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                        284+   0,   4);
		API_polar::template xo <  4>(s,     280+   0,  280+   4,             280+   0,   4);
		API_polar::template xo0<  8>(s,     272+   8,                        272+   0,   8);
		API_polar::template xo0< 16>(s,     256+  16,                        256+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  256+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template rep<  8>(s, l, 1008+   0,                        288+   0,   8);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,  288+   0,  992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                        296+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,  296+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                        300+   0,   4);
		API_polar::template xo <  4>(s,     296+   0,  296+   4,             296+   0,   4);
		API_polar::template xo <  8>(s,     288+   0,  288+   8,             288+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  288+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                        304+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,  304+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                        308+   0,   4);
		API_polar::template xo <  4>(s,     304+   0,  304+   4,             304+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  304+   0,  992+  16,   8);
		API_polar::template h  <  8>(s, l, 1008+   0,                        312+   0,   8);
		API_polar::template xo <  8>(s,     304+   0,  304+   8,             304+   0,   8);
		API_polar::template xo < 16>(s,     288+   0,  288+  16,             288+   0,  16);
		API_polar::template xo < 32>(s,     256+   0,  256+  32,             256+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  256+   0,  768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template rep<  8>(s, l, 1008+   0,                        320+   0,   8);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,  320+   0,  992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                        328+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,  328+   0, 1008+   8,   4);
		API_polar::template h  <  4>(s, l, 1016+   0,                        332+   0,   4);
		API_polar::template xo <  4>(s,     328+   0,  328+   4,             328+   0,   4);
		API_polar::template xo <  8>(s,     320+   0,  320+   8,             320+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  320+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template g0 <  2>(   l, 1016+   0, 1016+   2,            1016+   4,   2);
		API_polar::template h  <  2>(s, l, 1020+   0,                        338+   0,   2);
		API_polar::template xo0<  2>(s,     336+   2,                        336+   0,   2);
		API_polar::template g  <  4>(s, l, 1008+   0, 1008+   4,  336+   0, 1008+   8,   4);
		API_polar::template h  <  4>(s, l, 1016+   0,                        340+   0,   4);
		API_polar::template xo <  4>(s,     336+   0,  336+   4,             336+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  336+   0,  992+  16,   8);
		API_polar::template h  <  8>(s, l, 1008+   0,                        344+   0,   8);
		API_polar::template xo <  8>(s,     336+   0,  336+   8,             336+   0,   8);
		API_polar::template xo < 16>(s,     320+   0,  320+  16,             320+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  320+   0,  896+  64,  32);
		API_polar::template spc< 32>(s, l,  960+   0,                        352+   0,  32);
		API_polar::template xo < 32>(s,     320+   0,  320+  32,             320+   0,  32);
		API_polar::template xo < 64>(s,     256+   0,  256+  64,             256+   0,  64);
		API_polar::template g  <128>(s, l,  512+   0,  512+ 128,  256+   0,  512+ 256, 128);
		API_polar::template f  < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template g0 <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template g0 <  2>(   l, 1016+   0, 1016+   2,            1016+   4,   2);
		API_polar::template h  <  2>(s, l, 1020+   0,                        390+   0,   2);
		API_polar::template xo0<  2>(s,     388+   2,                        388+   0,   2);
		API_polar::template xo0<  4>(s,     384+   4,                        384+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  384+   0,  992+  16,   8);
		API_polar::template spc<  8>(s, l, 1008+   0,                        392+   0,   8);
		API_polar::template xo <  8>(s,     384+   0,  384+   8,             384+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  384+   0,  960+  32,  16);
		API_polar::template spc< 16>(s, l,  992+   0,                        400+   0,  16);
		API_polar::template xo < 16>(s,     384+   0,  384+  16,             384+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  384+   0,  896+  64,  32);
		API_polar::template spc< 32>(s, l,  960+   0,                        416+   0,  32);
		API_polar::template xo < 32>(s,     384+   0,  384+  32,             384+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  384+   0,  768+ 128,  64);
		API_polar::template spc< 64>(s, l,  896+   0,                        448+   0,  64);
		API_polar::template xo < 64>(s,     384+   0,  384+  64,             384+   0,  64);
		API_polar::template xo <128>(s,     256+   0,  256+ 128,             256+   0, 128);
		API_polar::template xo <256>(s,       0+   0,    0+ 256,               0+   0, 256);
	}
};
#endif
