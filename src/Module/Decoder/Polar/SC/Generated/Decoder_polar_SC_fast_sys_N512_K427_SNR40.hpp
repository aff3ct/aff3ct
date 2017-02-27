#ifndef DECODER_POLAR_SC_FAST_SYS_N512_K427_SNR40_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N512_K427_SNR40_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Frozen_bits_512_427_40[512] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N512_K427_SNR40 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N512_K427_SNR40(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 512);
		assert(K == 427);
		
		auto i = 0;
		while (i < 512 && Frozen_bits_512_427_40[i] == frozen_bits[i]) i++;
		assert(i == 512);
	}

	virtual ~Decoder_polar_SC_fast_sys_N512_K427_SNR40()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <256>(   l,    0+   0,    0+ 256,               0+ 512, 256);
		API_polar::template f  <128>(   l,  512+   0,  512+ 128,             512+ 256, 128);
		API_polar::template f  < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template g0 < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template g0 <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template g0 <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template g0 <  2>(   l, 1016+   0, 1016+   2,            1016+   4,   2);
		API_polar::template h  <  2>(s, l, 1020+   0,                         30+   0,   2);
		API_polar::template xo0<  2>(s,      28+   2,                         28+   0,   2);
		API_polar::template xo0<  4>(s,      24+   4,                         24+   0,   4);
		API_polar::template xo0<  8>(s,      16+   8,                         16+   0,   8);
		API_polar::template xo0< 16>(s,       0+  16,                          0+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,    0+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template g0 <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                         40+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,   40+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                         44+   0,   4);
		API_polar::template xo <  4>(s,      40+   0,   40+   4,              40+   0,   4);
		API_polar::template xo0<  8>(s,      32+   8,                         32+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,   32+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                         48+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,   48+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                         52+   0,   4);
		API_polar::template xo <  4>(s,      48+   0,   48+   4,              48+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,   48+   0,  992+  16,   8);
		API_polar::template spc<  8>(s, l, 1008+   0,                         56+   0,   8);
		API_polar::template xo <  8>(s,      48+   0,   48+   8,              48+   0,   8);
		API_polar::template xo < 16>(s,      32+   0,   32+  16,              32+   0,  16);
		API_polar::template xo < 32>(s,       0+   0,    0+  32,               0+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,    0+   0,  768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template rep<  8>(s, l, 1008+   0,                         64+   0,   8);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,   64+   0,  992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                         72+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,   72+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                         76+   0,   4);
		API_polar::template xo <  4>(s,      72+   0,   72+   4,              72+   0,   4);
		API_polar::template xo <  8>(s,      64+   0,   64+   8,              64+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,   64+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template rep<  4>(s, l, 1016+   0,                         80+   0,   4);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,   80+   0, 1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                         84+   0,   4);
		API_polar::template xo <  4>(s,      80+   0,   80+   4,              80+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,   80+   0,  992+  16,   8);
		API_polar::template h  <  8>(s, l, 1008+   0,                         88+   0,   8);
		API_polar::template xo <  8>(s,      80+   0,   80+   8,              80+   0,   8);
		API_polar::template xo < 16>(s,      64+   0,   64+  16,              64+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,   64+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template g0 <  2>(   l, 1016+   0, 1016+   2,            1016+   4,   2);
		API_polar::template h  <  2>(s, l, 1020+   0,                         98+   0,   2);
		API_polar::template xo0<  2>(s,      96+   2,                         96+   0,   2);
		API_polar::template g  <  4>(s, l, 1008+   0, 1008+   4,   96+   0, 1008+   8,   4);
		API_polar::template h  <  4>(s, l, 1016+   0,                        100+   0,   4);
		API_polar::template xo <  4>(s,      96+   0,   96+   4,              96+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,   96+   0,  992+  16,   8);
		API_polar::template h  <  8>(s, l, 1008+   0,                        104+   0,   8);
		API_polar::template xo <  8>(s,      96+   0,   96+   8,              96+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,   96+   0,  960+  32,  16);
		API_polar::template h  < 16>(s, l,  992+   0,                        112+   0,  16);
		API_polar::template xo < 16>(s,      96+   0,   96+  16,              96+   0,  16);
		API_polar::template xo < 32>(s,      64+   0,   64+  32,              64+   0,  32);
		API_polar::template xo < 64>(s,       0+   0,    0+  64,               0+   0,  64);
		API_polar::template g  <128>(s, l,  512+   0,  512+ 128,    0+   0,  512+ 256, 128);
		API_polar::template f  < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template rep<  8>(s, l, 1008+   0,                        128+   0,   8);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,  128+   0,  992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template g0 <  2>(   l, 1016+   0, 1016+   2,            1016+   4,   2);
		API_polar::template h  <  2>(s, l, 1020+   0,                        138+   0,   2);
		API_polar::template xo0<  2>(s,     136+   2,                        136+   0,   2);
		API_polar::template g  <  4>(s, l, 1008+   0, 1008+   4,  136+   0, 1008+   8,   4);
		API_polar::template h  <  4>(s, l, 1016+   0,                        140+   0,   4);
		API_polar::template xo <  4>(s,     136+   0,  136+   4,             136+   0,   4);
		API_polar::template xo <  8>(s,     128+   0,  128+   8,             128+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  128+   0,  960+  32,  16);
		API_polar::template spc< 16>(s, l,  992+   0,                        144+   0,  16);
		API_polar::template xo < 16>(s,     128+   0,  128+  16,             128+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  128+   0,  896+  64,  32);
		API_polar::template spc< 32>(s, l,  960+   0,                        160+   0,  32);
		API_polar::template xo < 32>(s,     128+   0,  128+  32,             128+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  128+   0,  768+ 128,  64);
		API_polar::template spc< 64>(s, l,  896+   0,                        192+   0,  64);
		API_polar::template xo < 64>(s,     128+   0,  128+  64,             128+   0,  64);
		API_polar::template xo <128>(s,       0+   0,    0+ 128,               0+   0, 128);
		API_polar::template g  <256>(s, l,    0+   0,    0+ 256,    0+   0,    0+ 512, 256);
		API_polar::template f  <128>(   l,  512+   0,  512+ 128,             512+ 256, 128);
		API_polar::template f  < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		API_polar::template g0 <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		API_polar::template spc<  4>(s, l, 1016+   0,                        260+   0,   4);
		API_polar::template xo0<  4>(s,     256+   4,                        256+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  256+   0,  992+  16,   8);
		API_polar::template spc<  8>(s, l, 1008+   0,                        264+   0,   8);
		API_polar::template xo <  8>(s,     256+   0,  256+   8,             256+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  256+   0,  960+  32,  16);
		API_polar::template spc< 16>(s, l,  992+   0,                        272+   0,  16);
		API_polar::template xo < 16>(s,     256+   0,  256+  16,             256+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  256+   0,  896+  64,  32);
		API_polar::template spc< 32>(s, l,  960+   0,                        288+   0,  32);
		API_polar::template xo < 32>(s,     256+   0,  256+  32,             256+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  256+   0,  768+ 128,  64);
		API_polar::template spc< 64>(s, l,  896+   0,                        320+   0,  64);
		API_polar::template xo < 64>(s,     256+   0,  256+  64,             256+   0,  64);
		API_polar::template g  <128>(s, l,  512+   0,  512+ 128,  256+   0,  512+ 256, 128);
		API_polar::template h  <128>(s, l,  768+   0,                        384+   0, 128);
		API_polar::template xo <128>(s,     256+   0,  256+ 128,             256+   0, 128);
		API_polar::template xo <256>(s,       0+   0,    0+ 256,               0+   0, 256);
	}
};
}
}
#endif
