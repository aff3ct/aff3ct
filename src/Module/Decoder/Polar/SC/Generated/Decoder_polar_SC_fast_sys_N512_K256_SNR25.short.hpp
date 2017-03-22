#ifndef DECODER_POLAR_SC_FAST_SYS_N512_K256_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N512_K256_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
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
	Decoder_polar_SC_fast_sys_N512_K256_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
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

	// depth = 4, reverse depth = 5, size = 32, calls = 2
	inline void re4(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep< 32>(s, l, off_l+   0,                       off_s+   0,  32);
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void re6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 8, reverse depth = 1, size = 2, calls = 4
	__attribute__((always_inline))
	inline void r08(const int off_l, const int off_s)
	{
	}

	// depth = 8, reverse depth = 1, size = 2, calls = 4
	__attribute__((always_inline))
	inline void r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  2>(s, l, off_l+   0,                       off_s+   0,   2);
	}

	// depth = 7, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void r08r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r08(off_l+4, off_s+0);
		API_polar::template g0 <  2>(   l, off_l+   0, off_l+   2,            off_l+   4,   2);
		r18(off_l+4, off_s+2);
		API_polar::template xo0<  2>(s,    off_s+   2,                       off_s+   0,   2);
	}

	// depth = 7, reverse depth = 2, size = 4, calls = 3
	__attribute__((always_inline))
	inline void r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void r08r18r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  4>(   l, off_l+   0, off_l+   4,            off_l+   8,   4);
		r08r18(off_l+8, off_s+0);
		API_polar::template g  <  4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,   4);
		r17(off_l+8, off_s+4);
		API_polar::template xo <  4>(s,    off_s+   0, off_s+   4,            off_s+   0,   4);
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void r06(const int off_l, const int off_s)
	{
	}

	// depth = 7, reverse depth = 2, size = 4, calls = 2
	__attribute__((always_inline))
	inline void r07(const int off_l, const int off_s)
	{
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void r07r08r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r07(off_l+8, off_s+0);
		API_polar::template g0 <  4>(   l, off_l+   0, off_l+   4,            off_l+   8,   4);
		r08r18(off_l+8, off_s+4);
		API_polar::template xo0<  4>(s,    off_s+   4,                       off_s+   0,   4);
	}

	// depth = 7, reverse depth = 2, size = 4, calls = 8
	__attribute__((always_inline))
	inline void re7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 7, reverse depth = 2, size = 4, calls = 7
	__attribute__((always_inline))
	inline void s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 7
	__attribute__((always_inline))
	inline void re7s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  4>(   l, off_l+   0, off_l+   4,            off_l+   8,   4);
		re7(off_l+8, off_s+0);
		API_polar::template gr <  4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,   4);
		s7(off_l+8, off_s+4);
		API_polar::template xo <  4>(s,    off_s+   0, off_s+   4,            off_s+   0,   4);
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 5, reverse depth = 4, size = 16, calls = 2
	inline void re7s7s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		re7s7(off_l+16, off_s+0);
		API_polar::template g  <  8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,   8);
		s6(off_l+16, off_s+8);
		API_polar::template xo <  8>(s,    off_s+   0, off_s+   8,            off_s+   0,   8);
	}

	// depth = 5, reverse depth = 4, size = 16, calls = 2
	inline void r06re7s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r06(off_l+16, off_s+0);
		API_polar::template g0 <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		re7s7(off_l+16, off_s+8);
		API_polar::template xo0<  8>(s,    off_s+   8,                       off_s+   0,   8);
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void r16(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 5, reverse depth = 4, size = 16, calls = 2
	inline void re7s7r16(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		re7s7(off_l+16, off_s+0);
		API_polar::template g  <  8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,   8);
		r16(off_l+16, off_s+8);
		API_polar::template xo <  8>(s,    off_s+   0, off_s+   8,            off_s+   0,   8);
	}

	// depth = 4, reverse depth = 5, size = 32, calls = 2
	inline void s4(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc< 32>(s, l, off_l+   0,                       off_s+   0,  32);
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <256>(   l,    0+   0,    0+ 256,               0+ 512, 256);
		API_polar::template f  <128>(   l,  512+   0,  512+ 128,             512+ 256, 128);
		API_polar::template g0 < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		re4(960, 64);
		API_polar::template gr < 32>(s, l,  896+   0,  896+  32,   64+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template rep< 16>(s, l,  992+   0,                         96+   0,  16);
		API_polar::template gr < 16>(s, l,  960+   0,  960+  16,   96+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		re6(1008, 112);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,  112+   0,  992+  16,   8);
		r08r18r17(1008, 120);
		API_polar::template xo <  8>(s,     112+   0,  112+   8,             112+   0,   8);
		API_polar::template xo < 16>(s,      96+   0,   96+  16,              96+   0,  16);
		API_polar::template xo < 32>(s,      64+   0,   64+  32,              64+   0,  32);
		API_polar::template xo0< 64>(s,       0+  64,                          0+   0,  64);
		API_polar::template g  <128>(s, l,  512+   0,  512+ 128,    0+   0,  512+ 256, 128);
		API_polar::template f  < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		re4(960, 128);
		API_polar::template gr < 32>(s, l,  896+   0,  896+  32,  128+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		r06(1008, 160);
		API_polar::template g0 <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		r07r08r18(1008, 168);
		API_polar::template xo0<  8>(s,     160+   8,                        160+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  160+   0,  960+  32,  16);
		re7s7s6(992, 176);
		API_polar::template xo < 16>(s,     160+   0,  160+  16,             160+   0,  16);
		API_polar::template xo < 32>(s,     128+   0,  128+  32,             128+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  128+   0,  768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		r06re7s7(992, 192);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  192+   0,  960+  32,  16);
		re7s7s6(992, 208);
		API_polar::template xo < 16>(s,     192+   0,  192+  16,             192+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  192+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		re7s7r16(992, 224);
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
		r06re7s7(992, 272);
		API_polar::template xo0< 16>(s,     256+  16,                        256+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  256+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		re6(1008, 288);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,  288+   0,  992+  16,   8);
		re7s7(1008, 296);
		API_polar::template xo <  8>(s,     288+   0,  288+   8,             288+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  288+   0,  960+  32,  16);
		re7s7r16(992, 304);
		API_polar::template xo < 16>(s,     288+   0,  288+  16,             288+   0,  16);
		API_polar::template xo < 32>(s,     256+   0,  256+  32,             256+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  256+   0,  768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		re6(1008, 320);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,  320+   0,  992+  16,   8);
		API_polar::template f  <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		re7(1016, 328);
		API_polar::template gr <  4>(s, l, 1008+   0, 1008+   4,  328+   0, 1008+   8,   4);
		r17(1016, 332);
		API_polar::template xo <  4>(s,     328+   0,  328+   4,             328+   0,   4);
		API_polar::template xo <  8>(s,     320+   0,  320+   8,             320+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  320+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		r08r18r17(1008, 336);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  336+   0,  992+  16,   8);
		r16(1008, 344);
		API_polar::template xo <  8>(s,     336+   0,  336+   8,             336+   0,   8);
		API_polar::template xo < 16>(s,     320+   0,  320+  16,             320+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  320+   0,  896+  64,  32);
		s4(960, 352);
		API_polar::template xo < 32>(s,     320+   0,  320+  32,             320+   0,  32);
		API_polar::template xo < 64>(s,     256+   0,  256+  64,             256+   0,  64);
		API_polar::template g  <128>(s, l,  512+   0,  512+ 128,  256+   0,  512+ 256, 128);
		API_polar::template f  < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		r07r08r18(1008, 384);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  384+   0,  992+  16,   8);
		s6(1008, 392);
		API_polar::template xo <  8>(s,     384+   0,  384+   8,             384+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  384+   0,  960+  32,  16);
		API_polar::template spc< 16>(s, l,  992+   0,                        400+   0,  16);
		API_polar::template xo < 16>(s,     384+   0,  384+  16,             384+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  384+   0,  896+  64,  32);
		s4(960, 416);
		API_polar::template xo < 32>(s,     384+   0,  384+  32,             384+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  384+   0,  768+ 128,  64);
		API_polar::template spc< 64>(s, l,  896+   0,                        448+   0,  64);
		API_polar::template xo < 64>(s,     384+   0,  384+  64,             384+   0,  64);
		API_polar::template xo <128>(s,     256+   0,  256+ 128,             256+   0, 128);
		API_polar::template xo <256>(s,       0+   0,    0+ 256,               0+   0, 256);
	}
};
}
}
#endif
