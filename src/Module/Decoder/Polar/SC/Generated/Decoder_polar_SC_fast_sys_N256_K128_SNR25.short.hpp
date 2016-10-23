#ifndef DECODER_POLAR_SC_FAST_SYS_N256_K128_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N256_K128_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_256_128_25[256] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N256_K128_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N256_K128_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 256);
		assert(K == 128);
		
		auto i = 0;
		while (i < 256 && Frozen_bits_256_128_25[i] == frozen_bits[i]) i++;
		assert(i == 256);
	}

	virtual ~Decoder_polar_SC_fast_sys_N256_K128_SNR25()
	{
	}

	// depth = 5, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void re5(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  8>(s, l, off_l+  0,                   off_s+  0,   8);
	}

	// depth = 6, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void re6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  4>(s, l, off_l+  0,                   off_s+  0,   4);
	}

	// depth = 6, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  4>(s, l, off_l+  0,                   off_s+  0,   4);
	}

	// depth = 5, reverse depth = 3, size = 8, calls = 4
	__attribute__((always_inline))
	inline void re6s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  4>(   l, off_l+  0, off_l+  4,          off_l+  8,   4);
		re6(off_l+8, off_s+0);
		API_polar::template gr <  4>(s, l, off_l+  0, off_l+  4, off_s+  0, off_l+  8,   4);
		s6(off_l+8, off_s+4);
		API_polar::template xo <  4>(s,    off_s+  0, off_s+  4,          off_s+  0,   4);
	}

	// depth = 4, reverse depth = 4, size = 16, calls = 2
	inline void re5re6s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  8>(   l, off_l+  0, off_l+  8,          off_l+ 16,   8);
		re5(off_l+16, off_s+0);
		API_polar::template gr <  8>(s, l, off_l+  0, off_l+  8, off_s+  0, off_l+ 16,   8);
		re6s6(off_l+16, off_s+8);
		API_polar::template xo <  8>(s,    off_s+  0, off_s+  8,          off_s+  0,   8);
	}

	// depth = 4, reverse depth = 4, size = 16, calls = 2
	inline void re4(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep< 16>(s, l, off_l+  0,                   off_s+  0,  16);
	}

	// depth = 7, reverse depth = 1, size = 2, calls = 2
	__attribute__((always_inline))
	inline void r07(const int off_l, const int off_s)
	{
	}

	// depth = 7, reverse depth = 1, size = 2, calls = 2
	__attribute__((always_inline))
	inline void r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  2>(s, l, off_l+  0,                   off_s+  0,   2);
	}

	// depth = 6, reverse depth = 2, size = 4, calls = 2
	__attribute__((always_inline))
	inline void r07r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r07(off_l+4, off_s+0);
		API_polar::template g0 <  2>(   l, off_l+  0, off_l+  2,          off_l+  4,   2);
		r17(off_l+4, off_s+2);
		API_polar::template xo0<  2>(s,    off_s+  2,                   off_s+  0,   2);
	}

	// depth = 4, reverse depth = 4, size = 16, calls = 2
	inline void s4(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc< 16>(s, l, off_l+  0,                   off_s+  0,  16);
	}

	// depth = 5, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void s5(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  8>(s, l, off_l+  0,                   off_s+  0,   8);
	}

	// depth = 4, reverse depth = 4, size = 16, calls = 2
	inline void re6s6s5(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  8>(   l, off_l+  0, off_l+  8,          off_l+ 16,   8);
		re6s6(off_l+16, off_s+0);
		API_polar::template g  <  8>(s, l, off_l+  0, off_l+  8, off_s+  0, off_l+ 16,   8);
		s5(off_l+16, off_s+8);
		API_polar::template xo <  8>(s,    off_s+  0, off_s+  8,          off_s+  0,   8);
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <128>(   l,   0+  0,   0+128,            0+256, 128);
		API_polar::template f  < 64>(   l, 256+  0, 256+ 64,          256+128,  64);
		API_polar::template g0 < 32>(   l, 384+  0, 384+ 32,          384+ 64,  32);
		API_polar::template g0 < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		re5re6s6(480, 48);
		API_polar::template xo0< 16>(s,     32+ 16,                    32+  0,  16);
		API_polar::template xo0< 32>(s,      0+ 32,                     0+  0,  32);
		API_polar::template g  < 64>(s, l, 256+  0, 256+ 64,   0+  0, 256+128,  64);
		API_polar::template f  < 32>(   l, 384+  0, 384+ 32,          384+ 64,  32);
		API_polar::template f  < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		re4(480, 64);
		API_polar::template gr < 16>(s, l, 448+  0, 448+ 16,  64+  0, 448+ 32,  16);
		re5re6s6(480, 80);
		API_polar::template xo < 16>(s,     64+  0,  64+ 16,           64+  0,  16);
		API_polar::template g  < 32>(s, l, 384+  0, 384+ 32,  64+  0, 384+ 64,  32);
		API_polar::template f  < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		API_polar::template f  <  8>(   l, 480+  0, 480+  8,          480+ 16,   8);
		re5(496, 96);
		API_polar::template gr <  8>(s, l, 480+  0, 480+  8,  96+  0, 480+ 16,   8);
		API_polar::template f  <  4>(   l, 496+  0, 496+  4,          496+  8,   4);
		r07r17(504, 104);
		API_polar::template g  <  4>(s, l, 496+  0, 496+  4, 104+  0, 496+  8,   4);
		API_polar::template h  <  4>(s, l, 504+  0,                   108+  0,   4);
		API_polar::template xo <  4>(s,    104+  0, 104+  4,          104+  0,   4);
		API_polar::template xo <  8>(s,     96+  0,  96+  8,           96+  0,   8);
		API_polar::template g  < 16>(s, l, 448+  0, 448+ 16,  96+  0, 448+ 32,  16);
		s4(480, 112);
		API_polar::template xo < 16>(s,     96+  0,  96+ 16,           96+  0,  16);
		API_polar::template xo < 32>(s,     64+  0,  64+ 32,           64+  0,  32);
		API_polar::template xo < 64>(s,      0+  0,   0+ 64,            0+  0,  64);
		API_polar::template g  <128>(s, l,   0+  0,   0+128,   0+  0,   0+256, 128);
		API_polar::template f  < 64>(   l, 256+  0, 256+ 64,          256+128,  64);
		API_polar::template f  < 32>(   l, 384+  0, 384+ 32,          384+ 64,  32);
		API_polar::template f  < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		re4(480, 128);
		API_polar::template gr < 16>(s, l, 448+  0, 448+ 16, 128+  0, 448+ 32,  16);
		API_polar::template f  <  8>(   l, 480+  0, 480+  8,          480+ 16,   8);
		API_polar::template g0 <  4>(   l, 496+  0, 496+  4,          496+  8,   4);
		r07r17(504, 148);
		API_polar::template xo0<  4>(s,    144+  4,                   144+  0,   4);
		API_polar::template g  <  8>(s, l, 480+  0, 480+  8, 144+  0, 480+ 16,   8);
		s5(496, 152);
		API_polar::template xo <  8>(s,    144+  0, 144+  8,          144+  0,   8);
		API_polar::template xo < 16>(s,    128+  0, 128+ 16,          128+  0,  16);
		API_polar::template g  < 32>(s, l, 384+  0, 384+ 32, 128+  0, 384+ 64,  32);
		API_polar::template f  < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		re6s6s5(480, 160);
		API_polar::template g  < 16>(s, l, 448+  0, 448+ 16, 160+  0, 448+ 32,  16);
		s4(480, 176);
		API_polar::template xo < 16>(s,    160+  0, 160+ 16,          160+  0,  16);
		API_polar::template xo < 32>(s,    128+  0, 128+ 32,          128+  0,  32);
		API_polar::template g  < 64>(s, l, 256+  0, 256+ 64, 128+  0, 256+128,  64);
		API_polar::template f  < 32>(   l, 384+  0, 384+ 32,          384+ 64,  32);
		API_polar::template f  < 16>(   l, 448+  0, 448+ 16,          448+ 32,  16);
		re6s6s5(480, 192);
		API_polar::template g  < 16>(s, l, 448+  0, 448+ 16, 192+  0, 448+ 32,  16);
		API_polar::template h  < 16>(s, l, 480+  0,                   208+  0,  16);
		API_polar::template xo < 16>(s,    192+  0, 192+ 16,          192+  0,  16);
		API_polar::template g  < 32>(s, l, 384+  0, 384+ 32, 192+  0, 384+ 64,  32);
		API_polar::template h  < 32>(s, l, 448+  0,                   224+  0,  32);
		API_polar::template xo < 32>(s,    192+  0, 192+ 32,          192+  0,  32);
		API_polar::template xo < 64>(s,    128+  0, 128+ 64,          128+  0,  64);
		API_polar::template xo <128>(s,      0+  0,   0+128,            0+  0, 128);
	}
};
#endif
