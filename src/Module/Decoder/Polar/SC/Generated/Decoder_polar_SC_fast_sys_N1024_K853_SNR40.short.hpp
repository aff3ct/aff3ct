#ifndef DECODER_POLAR_SC_FAST_SYS_N1024_K853_SNR40_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N1024_K853_SNR40_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_1024_853_40[1024] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N1024_K853_SNR40 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N1024_K853_SNR40(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 1024);
		assert(K == 853);
		
		auto i = 0;
		while (i < 1024 && Frozen_bits_1024_853_40[i] == frozen_bits[i]) i++;
		assert(i == 1024);
	}

	virtual ~Decoder_polar_SC_fast_sys_N1024_K853_SNR40()
	{
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 4
	__attribute__((always_inline))
	inline void re7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 8, reverse depth = 2, size = 4, calls = 10
	__attribute__((always_inline))
	inline void re8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 8, reverse depth = 2, size = 4, calls = 8
	__attribute__((always_inline))
	inline void s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 7
	__attribute__((always_inline))
	inline void re8s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  4>(   l, off_l+   0, off_l+   4,            off_l+   8,   4);
		re8(off_l+8, off_s+0);
		API_polar::template gr <  4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,   4);
		s8(off_l+8, off_s+4);
		API_polar::template xo <  4>(s,    off_s+   0, off_s+   4,            off_s+   0,   4);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 2
	inline void re7re8s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		re7(off_l+16, off_s+0);
		API_polar::template gr <  8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,   8);
		re8s8(off_l+16, off_s+8);
		API_polar::template xo <  8>(s,    off_s+   0, off_s+   8,            off_s+   0,   8);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 2
	inline void re6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep< 16>(s, l, off_l+   0,                       off_s+   0,  16);
	}

	// depth = 8, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void re8r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  4>(   l, off_l+   0, off_l+   4,            off_l+   8,   4);
		re8(off_l+8, off_s+0);
		API_polar::template gr <  4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,   4);
		r18(off_l+8, off_s+4);
		API_polar::template xo <  4>(s,    off_s+   0, off_s+   4,            off_s+   0,   4);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 6
	__attribute__((always_inline))
	inline void s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 3
	inline void s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc< 16>(s, l, off_l+   0,                       off_s+   0,  16);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 4
	inline void re8s8s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		re8s8(off_l+16, off_s+0);
		API_polar::template g  <  8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,   8);
		s7(off_l+16, off_s+8);
		API_polar::template xo <  8>(s,    off_s+   0, off_s+   8,            off_s+   0,   8);
	}

	// depth = 5, reverse depth = 5, size = 32, calls = 2
	inline void re8s8s7s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  < 16>(   l, off_l+   0, off_l+  16,            off_l+  32,  16);
		re8s8s7(off_l+32, off_s+0);
		API_polar::template g  < 16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,  16);
		s6(off_l+32, off_s+16);
		API_polar::template xo < 16>(s,    off_s+   0, off_s+  16,            off_s+   0,  16);
	}

	// depth = 5, reverse depth = 5, size = 32, calls = 2
	inline void r15(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 32>(s, l, off_l+   0,                       off_s+   0,  32);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 3
	inline void r16(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 16>(s, l, off_l+   0,                       off_s+   0,  16);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 2
	inline void re8r18r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		re8r18(off_l+16, off_s+0);
		API_polar::template g  <  8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,   8);
		r17(off_l+16, off_s+8);
		API_polar::template xo <  8>(s,    off_s+   0, off_s+   8,            off_s+   0,   8);
	}

	// depth = 3, reverse depth = 7, size = 128, calls = 2
	inline void s3(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<128>(s, l, off_l+   0,                       off_s+   0, 128);
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <512>(   l,    0+   0,    0+ 512,               0+1024, 512);
		API_polar::template f  <256>(   l, 1024+   0, 1024+ 256,            1024+ 512, 256);
		API_polar::template f  <128>(   l, 1536+   0, 1536+ 128,            1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template g0 < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template g0 < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re7re8s8(2016, 48);
		API_polar::template xo0< 16>(s,      32+  16,                         32+   0,  16);
		API_polar::template xo0< 32>(s,       0+  32,                          0+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,    0+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re6(2016, 64);
		API_polar::template gr < 16>(s, l, 1984+   0, 1984+  16,   64+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		re7(2032, 80);
		API_polar::template gr <  8>(s, l, 2016+   0, 2016+   8,   80+   0, 2016+  16,   8);
		re8r18(2032, 88);
		API_polar::template xo <  8>(s,      80+   0,   80+   8,              80+   0,   8);
		API_polar::template xo < 16>(s,      64+   0,   64+  16,              64+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,   64+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		re7(2032, 96);
		API_polar::template gr <  8>(s, l, 2016+   0, 2016+   8,   96+   0, 2016+  16,   8);
		s7(2032, 104);
		API_polar::template xo <  8>(s,      96+   0,   96+   8,              96+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,   96+   0, 1984+  32,  16);
		s6(2016, 112);
		API_polar::template xo < 16>(s,      96+   0,   96+  16,              96+   0,  16);
		API_polar::template xo < 32>(s,      64+   0,   64+  32,              64+   0,  32);
		API_polar::template xo < 64>(s,       0+   0,    0+  64,               0+   0,  64);
		API_polar::template g  <128>(s, l, 1536+   0, 1536+ 128,    0+   0, 1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re6(2016, 128);
		API_polar::template gr < 16>(s, l, 1984+   0, 1984+  16,  128+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template g0 <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		s8(2040, 148);
		API_polar::template xo0<  4>(s,     144+   4,                        144+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  144+   0, 2016+  16,   8);
		s7(2032, 152);
		API_polar::template xo <  8>(s,     144+   0,  144+   8,             144+   0,   8);
		API_polar::template xo < 16>(s,     128+   0,  128+  16,             128+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  128+   0, 1920+  64,  32);
		re8s8s7s6(1984, 160);
		API_polar::template xo < 32>(s,     128+   0,  128+  32,             128+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  128+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		re8s8s7s6(1984, 192);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  192+   0, 1920+  64,  32);
		r15(1984, 224);
		API_polar::template xo < 32>(s,     192+   0,  192+  32,             192+   0,  32);
		API_polar::template xo < 64>(s,     128+   0,  128+  64,             128+   0,  64);
		API_polar::template xo <128>(s,       0+   0,    0+ 128,               0+   0, 128);
		API_polar::template g  <256>(s, l, 1024+   0, 1024+ 256,    0+   0, 1024+ 512, 256);
		API_polar::template f  <128>(   l, 1536+   0, 1536+ 128,            1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template g0 <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		re8s8(2032, 264);
		API_polar::template xo0<  8>(s,     256+   8,                        256+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  256+   0, 1984+  32,  16);
		re8s8s7(2016, 272);
		API_polar::template xo < 16>(s,     256+   0,  256+  16,             256+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  256+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re8s8s7(2016, 288);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  288+   0, 1984+  32,  16);
		r16(2016, 304);
		API_polar::template xo < 16>(s,     288+   0,  288+  16,             288+   0,  16);
		API_polar::template xo < 32>(s,     256+   0,  256+  32,             256+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  256+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re8r18r17(2016, 320);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  320+   0, 1984+  32,  16);
		r16(2016, 336);
		API_polar::template xo < 16>(s,     320+   0,  320+  16,             320+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  320+   0, 1920+  64,  32);
		r15(1984, 352);
		API_polar::template xo < 32>(s,     320+   0,  320+  32,             320+   0,  32);
		API_polar::template xo < 64>(s,     256+   0,  256+  64,             256+   0,  64);
		API_polar::template g  <128>(s, l, 1536+   0, 1536+ 128,  256+   0, 1536+ 256, 128);
		s3(1792, 384);
		API_polar::template xo <128>(s,     256+   0,  256+ 128,             256+   0, 128);
		API_polar::template xo <256>(s,       0+   0,    0+ 256,               0+   0, 256);
		API_polar::template g  <512>(s, l,    0+   0,    0+ 512,    0+   0,    0+1024, 512);
		API_polar::template f  <256>(   l, 1024+   0, 1024+ 256,            1024+ 512, 256);
		API_polar::template f  <128>(   l, 1536+   0, 1536+ 128,            1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re7re8s8(2016, 512);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  512+   0, 1984+  32,  16);
		re8r18r17(2016, 528);
		API_polar::template xo < 16>(s,     512+   0,  512+  16,             512+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  512+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template g0 <  2>(   l, 2040+   0, 2040+   2,            2040+   4,   2);
		API_polar::template h  <  2>(s, l, 2044+   0,                        546+   0,   2);
		API_polar::template xo0<  2>(s,     544+   2,                        544+   0,   2);
		API_polar::template g  <  4>(s, l, 2032+   0, 2032+   4,  544+   0, 2032+   8,   4);
		r18(2040, 548);
		API_polar::template xo <  4>(s,     544+   0,  544+   4,             544+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  544+   0, 2016+  16,   8);
		r17(2032, 552);
		API_polar::template xo <  8>(s,     544+   0,  544+   8,             544+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  544+   0, 1984+  32,  16);
		r16(2016, 560);
		API_polar::template xo < 16>(s,     544+   0,  544+  16,             544+   0,  16);
		API_polar::template xo < 32>(s,     512+   0,  512+  32,             512+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  512+   0, 1792+ 128,  64);
		API_polar::template spc< 64>(s, l, 1920+   0,                        576+   0,  64);
		API_polar::template xo < 64>(s,     512+   0,  512+  64,             512+   0,  64);
		API_polar::template g  <128>(s, l, 1536+   0, 1536+ 128,  512+   0, 1536+ 256, 128);
		s3(1792, 640);
		API_polar::template xo <128>(s,     512+   0,  512+ 128,             512+   0, 128);
		API_polar::template g  <256>(s, l, 1024+   0, 1024+ 256,  512+   0, 1024+ 512, 256);
		API_polar::template spc<256>(s, l, 1536+   0,                        768+   0, 256);
		API_polar::template xo <256>(s,     512+   0,  512+ 256,             512+   0, 256);
		API_polar::template xo <512>(s,       0+   0,    0+ 512,               0+   0, 512);
	}
};
#endif
