#ifndef DECODER_POLAR_SC_FAST_SYS_N512_K427_SNR40_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N512_K427_SNR40_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

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
	Decoder_polar_SC_fast_sys_N512_K427_SNR40(const int& K, const int& N, const mipp::vector<B>& frozen_bits)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits)
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

	// depth = 6, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void r06(const int off_l, const int off_s)
	{
	}

	// depth = 7, reverse depth = 2, size = 4, calls = 2
	__attribute__((always_inline))
	inline void r07(const int off_l, const int off_s)
	{
	}

	// depth = 8, reverse depth = 1, size = 2, calls = 3
	__attribute__((always_inline))
	inline void r08(const int off_l, const int off_s)
	{
	}

	// depth = 8, reverse depth = 1, size = 2, calls = 3
	__attribute__((always_inline))
	inline void r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  2>(s, l, off_l+   0,                       off_s+   0,   2);
	}

	// depth = 7, reverse depth = 2, size = 4, calls = 3
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

	// depth = 7, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void re7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 7, reverse depth = 2, size = 4, calls = 5
	__attribute__((always_inline))
	inline void s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 4
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

	// depth = 6, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void re6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 6, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void r16(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 7, reverse depth = 2, size = 4, calls = 2
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

	// depth = 5, reverse depth = 4, size = 16, calls = 2
	inline void s5(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc< 16>(s, l, off_l+   0,                       off_s+   0,  16);
	}

	// depth = 4, reverse depth = 5, size = 32, calls = 2
	inline void s4(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc< 32>(s, l, off_l+   0,                       off_s+   0,  32);
	}

	// depth = 3, reverse depth = 6, size = 64, calls = 2
	inline void s3(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc< 64>(s, l, off_l+   0,                       off_s+   0,  64);
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
		r06(1008, 16);
		API_polar::template g0 <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		r07(1016, 24);
		API_polar::template g0 <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		r08r18(1016, 28);
		API_polar::template xo0<  4>(s,      24+   4,                         24+   0,   4);
		API_polar::template xo0<  8>(s,      16+   8,                         16+   0,   8);
		API_polar::template xo0< 16>(s,       0+  16,                          0+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,    0+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		r06(1008, 32);
		API_polar::template g0 <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		re7s7(1008, 40);
		API_polar::template xo0<  8>(s,      32+   8,                         32+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,   32+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		re7s7(1008, 48);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,   48+   0,  992+  16,   8);
		s6(1008, 56);
		API_polar::template xo <  8>(s,      48+   0,   48+   8,              48+   0,   8);
		API_polar::template xo < 16>(s,      32+   0,   32+  16,              32+   0,  16);
		API_polar::template xo < 32>(s,       0+   0,    0+  32,               0+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,    0+   0,  768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		re6(1008, 64);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,   64+   0,  992+  16,   8);
		re7s7(1008, 72);
		API_polar::template xo <  8>(s,      64+   0,   64+   8,              64+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,   64+   0,  960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		re7s7(1008, 80);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,   80+   0,  992+  16,   8);
		r16(1008, 88);
		API_polar::template xo <  8>(s,      80+   0,   80+   8,              80+   0,   8);
		API_polar::template xo < 16>(s,      64+   0,   64+  16,              64+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,   64+   0,  896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		r08r18r17(1008, 96);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,   96+   0,  992+  16,   8);
		r16(1008, 104);
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
		re6(1008, 128);
		API_polar::template gr <  8>(s, l,  992+   0,  992+   8,  128+   0,  992+  16,   8);
		r08r18r17(1008, 136);
		API_polar::template xo <  8>(s,     128+   0,  128+   8,             128+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  128+   0,  960+  32,  16);
		s5(992, 144);
		API_polar::template xo < 16>(s,     128+   0,  128+  16,             128+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  128+   0,  896+  64,  32);
		s4(960, 160);
		API_polar::template xo < 32>(s,     128+   0,  128+  32,             128+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  128+   0,  768+ 128,  64);
		s3(896, 192);
		API_polar::template xo < 64>(s,     128+   0,  128+  64,             128+   0,  64);
		API_polar::template xo <128>(s,       0+   0,    0+ 128,               0+   0, 128);
		API_polar::template g  <256>(s, l,    0+   0,    0+ 256,    0+   0,    0+ 512, 256);
		API_polar::template f  <128>(   l,  512+   0,  512+ 128,             512+ 256, 128);
		API_polar::template f  < 64>(   l,  768+   0,  768+  64,             768+ 128,  64);
		API_polar::template f  < 32>(   l,  896+   0,  896+  32,             896+  64,  32);
		API_polar::template f  < 16>(   l,  960+   0,  960+  16,             960+  32,  16);
		API_polar::template f  <  8>(   l,  992+   0,  992+   8,             992+  16,   8);
		r07(1016, 256);
		API_polar::template g0 <  4>(   l, 1008+   0, 1008+   4,            1008+   8,   4);
		s7(1016, 260);
		API_polar::template xo0<  4>(s,     256+   4,                        256+   0,   4);
		API_polar::template g  <  8>(s, l,  992+   0,  992+   8,  256+   0,  992+  16,   8);
		s6(1008, 264);
		API_polar::template xo <  8>(s,     256+   0,  256+   8,             256+   0,   8);
		API_polar::template g  < 16>(s, l,  960+   0,  960+  16,  256+   0,  960+  32,  16);
		s5(992, 272);
		API_polar::template xo < 16>(s,     256+   0,  256+  16,             256+   0,  16);
		API_polar::template g  < 32>(s, l,  896+   0,  896+  32,  256+   0,  896+  64,  32);
		s4(960, 288);
		API_polar::template xo < 32>(s,     256+   0,  256+  32,             256+   0,  32);
		API_polar::template g  < 64>(s, l,  768+   0,  768+  64,  256+   0,  768+ 128,  64);
		s3(896, 320);
		API_polar::template xo < 64>(s,     256+   0,  256+  64,             256+   0,  64);
		API_polar::template g  <128>(s, l,  512+   0,  512+ 128,  256+   0,  512+ 256, 128);
		API_polar::template h  <128>(s, l,  768+   0,                        384+   0, 128);
		API_polar::template xo <128>(s,     256+   0,  256+ 128,             256+   0, 128);
		API_polar::template xo <256>(s,       0+   0,    0+ 256,               0+   0, 256);
	}
};
#endif
