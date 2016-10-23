#ifndef DECODER_POLAR_SC_FAST_SYS_N1024_K512_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N1024_K512_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_1024_512_25[1024] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N1024_K512_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N1024_K512_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 1024);
		assert(K == 512);
		
		auto i = 0;
		while (i < 1024 && Frozen_bits_1024_512_25[i] == frozen_bits[i]) i++;
		assert(i == 1024);
	}

	virtual ~Decoder_polar_SC_fast_sys_N1024_K512_SNR25()
	{
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 3
	inline void r06(const int off_l, const int off_s)
	{
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 4
	__attribute__((always_inline))
	inline void r07(const int off_l, const int off_s)
	{
	}

	// depth = 8, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void r08(const int off_l, const int off_s)
	{
	}

	// depth = 8, reverse depth = 2, size = 4, calls = 15
	__attribute__((always_inline))
	inline void s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void r08s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r08(off_l+8, off_s+0);
		API_polar::template g0 <  4>(   l, off_l+   0, off_l+   4,            off_l+   8,   4);
		s8(off_l+8, off_s+4);
		API_polar::template xo0<  4>(s,    off_s+   4,                       off_s+   0,   4);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 2
	inline void r07r08s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r07(off_l+16, off_s+0);
		API_polar::template g0 <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		r08s8(off_l+16, off_s+8);
		API_polar::template xo0<  8>(s,    off_s+   8,                       off_s+   0,   8);
	}

	// depth = 8, reverse depth = 2, size = 4, calls = 15
	__attribute__((always_inline))
	inline void re8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 13
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
	inline void r07re8s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r07(off_l+16, off_s+0);
		API_polar::template g0 <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		re8s8(off_l+16, off_s+8);
		API_polar::template xo0<  8>(s,    off_s+   8,                       off_s+   0,   8);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 8
	__attribute__((always_inline))
	inline void s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 6
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
	inline void r05(const int off_l, const int off_s)
	{
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 5
	__attribute__((always_inline))
	inline void re7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 4
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

	// depth = 9, reverse depth = 1, size = 2, calls = 4
	__attribute__((always_inline))
	inline void r09(const int off_l, const int off_s)
	{
	}

	// depth = 9, reverse depth = 1, size = 2, calls = 4
	__attribute__((always_inline))
	inline void r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  2>(s, l, off_l+   0,                       off_s+   0,   2);
	}

	// depth = 8, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void r09r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r09(off_l+4, off_s+0);
		API_polar::template g0 <  2>(   l, off_l+   0, off_l+   2,            off_l+   4,   2);
		r19(off_l+4, off_s+2);
		API_polar::template xo0<  2>(s,    off_s+   2,                       off_s+   0,   2);
	}

	// depth = 8, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  4>(s, l, off_l+   0,                       off_s+   0,   4);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void r09r19r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  4>(   l, off_l+   0, off_l+   4,            off_l+   8,   4);
		r09r19(off_l+8, off_s+0);
		API_polar::template g  <  4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,   4);
		r18(off_l+8, off_s+4);
		API_polar::template xo <  4>(s,    off_s+   0, off_s+   4,            off_s+   0,   4);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 4
	__attribute__((always_inline))
	inline void r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  8>(s, l, off_l+   0,                       off_s+   0,   8);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 2
	inline void r09r19r18r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		r09r19r18(off_l+16, off_s+0);
		API_polar::template g  <  8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,   8);
		r17(off_l+16, off_s+8);
		API_polar::template xo <  8>(s,    off_s+   0, off_s+   8,            off_s+   0,   8);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 3
	inline void re6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep< 16>(s, l, off_l+   0,                       off_s+   0,  16);
	}

	// depth = 7, reverse depth = 3, size = 8, calls = 2
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

	// depth = 7, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void r08r09r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r08(off_l+8, off_s+0);
		API_polar::template g0 <  4>(   l, off_l+   0, off_l+   4,            off_l+   8,   4);
		r09r19(off_l+8, off_s+4);
		API_polar::template xo0<  4>(s,    off_s+   4,                       off_s+   0,   4);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 2
	inline void r08r09r19s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  8>(   l, off_l+   0, off_l+   8,            off_l+  16,   8);
		r08r09r19(off_l+16, off_s+0);
		API_polar::template g  <  8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,   8);
		s7(off_l+16, off_s+8);
		API_polar::template xo <  8>(s,    off_s+   0, off_s+   8,            off_s+   0,   8);
	}

	// depth = 6, reverse depth = 4, size = 16, calls = 4
	inline void s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc< 16>(s, l, off_l+   0,                       off_s+   0,  16);
	}

	// depth = 5, reverse depth = 5, size = 32, calls = 3
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

	// depth = 6, reverse depth = 4, size = 16, calls = 3
	inline void r16(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 16>(s, l, off_l+   0,                       off_s+   0,  16);
	}

	// depth = 5, reverse depth = 5, size = 32, calls = 2
	inline void r15(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 32>(s, l, off_l+   0,                       off_s+   0,  32);
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <512>(   l,    0+   0,    0+ 512,               0+1024, 512);
		API_polar::template f  <256>(   l, 1024+   0, 1024+ 256,            1024+ 512, 256);
		API_polar::template f  <128>(   l, 1536+   0, 1536+ 128,            1536+ 256, 128);
		API_polar::template rep<128>(s, l, 1792+   0,                          0+   0, 128);
		API_polar::template gr <128>(s, l, 1536+   0, 1536+ 128,    0+   0, 1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template rep< 64>(s, l, 1920+   0,                        128+   0,  64);
		API_polar::template gr < 64>(s, l, 1792+   0, 1792+  64,  128+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		r06(2016, 192);
		API_polar::template g0 < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		r07r08s8(2016, 208);
		API_polar::template xo0< 16>(s,     192+  16,                        192+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  192+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		r07re8s8(2016, 224);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  224+   0, 1984+  32,  16);
		re8s8s7(2016, 240);
		API_polar::template xo < 16>(s,     224+   0,  224+  16,             224+   0,  16);
		API_polar::template xo < 32>(s,     192+   0,  192+  32,             192+   0,  32);
		API_polar::template xo < 64>(s,     128+   0,  128+  64,             128+   0,  64);
		API_polar::template xo <128>(s,       0+   0,    0+ 128,               0+   0, 128);
		API_polar::template g  <256>(s, l, 1024+   0, 1024+ 256,    0+   0, 1024+ 512, 256);
		API_polar::template f  <128>(   l, 1536+   0, 1536+ 128,            1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		r05(1984, 256);
		API_polar::template g0 < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		r06(2016, 288);
		API_polar::template g0 < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		r07re8s8(2016, 304);
		API_polar::template xo0< 16>(s,     288+  16,                        288+   0,  16);
		API_polar::template xo0< 32>(s,     256+  32,                        256+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  256+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		r06(2016, 320);
		API_polar::template g0 < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re7re8s8(2016, 336);
		API_polar::template xo0< 16>(s,     320+  16,                        320+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  320+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re7re8s8(2016, 352);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  352+   0, 1984+  32,  16);
		r09r19r18r17(2016, 368);
		API_polar::template xo < 16>(s,     352+   0,  352+  16,             352+   0,  16);
		API_polar::template xo < 32>(s,     320+   0,  320+  32,             320+   0,  32);
		API_polar::template xo < 64>(s,     256+   0,  256+  64,             256+   0,  64);
		API_polar::template g  <128>(s, l, 1536+   0, 1536+ 128,  256+   0, 1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re6(2016, 384);
		API_polar::template gr < 16>(s, l, 1984+   0, 1984+  16,  384+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		re7(2032, 400);
		API_polar::template gr <  8>(s, l, 2016+   0, 2016+   8,  400+   0, 2016+  16,   8);
		re8r18(2032, 408);
		API_polar::template xo <  8>(s,     400+   0,  400+   8,             400+   0,   8);
		API_polar::template xo < 16>(s,     384+   0,  384+  16,             384+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  384+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		r08r09r19s7(2016, 416);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  416+   0, 1984+  32,  16);
		s6(2016, 432);
		API_polar::template xo < 16>(s,     416+   0,  416+  16,             416+   0,  16);
		API_polar::template xo < 32>(s,     384+   0,  384+  32,             384+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  384+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		re8s8s7s6(1984, 448);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  448+   0, 1920+  64,  32);
		API_polar::template spc< 32>(s, l, 1984+   0,                        480+   0,  32);
		API_polar::template xo < 32>(s,     448+   0,  448+  32,             448+   0,  32);
		API_polar::template xo < 64>(s,     384+   0,  384+  64,             384+   0,  64);
		API_polar::template xo <128>(s,     256+   0,  256+ 128,             256+   0, 128);
		API_polar::template xo <256>(s,       0+   0,    0+ 256,               0+   0, 256);
		API_polar::template g  <512>(s, l,    0+   0,    0+ 512,    0+   0,    0+1024, 512);
		API_polar::template f  <256>(   l, 1024+   0, 1024+ 256,            1024+ 512, 256);
		API_polar::template f  <128>(   l, 1536+   0, 1536+ 128,            1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		r05(1984, 512);
		API_polar::template g0 < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re6(2016, 544);
		API_polar::template gr < 16>(s, l, 1984+   0, 1984+  16,  544+   0, 1984+  32,  16);
		re7re8s8(2016, 560);
		API_polar::template xo < 16>(s,     544+   0,  544+  16,             544+   0,  16);
		API_polar::template xo0< 32>(s,     512+  32,                        512+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  512+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re6(2016, 576);
		API_polar::template gr < 16>(s, l, 1984+   0, 1984+  16,  576+   0, 1984+  32,  16);
		r08r09r19s7(2016, 592);
		API_polar::template xo < 16>(s,     576+   0,  576+  16,             576+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  576+   0, 1920+  64,  32);
		re8s8s7s6(1984, 608);
		API_polar::template xo < 32>(s,     576+   0,  576+  32,             576+   0,  32);
		API_polar::template xo < 64>(s,     512+   0,  512+  64,             512+   0,  64);
		API_polar::template g  <128>(s, l, 1536+   0, 1536+ 128,  512+   0, 1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		r07r08s8(2016, 640);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  640+   0, 1984+  32,  16);
		re8s8s7(2016, 656);
		API_polar::template xo < 16>(s,     640+   0,  640+  16,             640+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  640+   0, 1920+  64,  32);
		re8s8s7s6(1984, 672);
		API_polar::template xo < 32>(s,     640+   0,  640+  32,             640+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  640+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		re8s8(2032, 704);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  704+   0, 2016+  16,   8);
		r17(2032, 712);
		API_polar::template xo <  8>(s,     704+   0,  704+   8,             704+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  704+   0, 1984+  32,  16);
		r16(2016, 720);
		API_polar::template xo < 16>(s,     704+   0,  704+  16,             704+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  704+   0, 1920+  64,  32);
		r15(1984, 736);
		API_polar::template xo < 32>(s,     704+   0,  704+  32,             704+   0,  32);
		API_polar::template xo < 64>(s,     640+   0,  640+  64,             640+   0,  64);
		API_polar::template xo <128>(s,     512+   0,  512+ 128,             512+   0, 128);
		API_polar::template g  <256>(s, l, 1024+   0, 1024+ 256,  512+   0, 1024+ 512, 256);
		API_polar::template f  <128>(   l, 1536+   0, 1536+ 128,            1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		re7re8s8(2016, 768);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  768+   0, 1984+  32,  16);
		re8s8s7(2016, 784);
		API_polar::template xo < 16>(s,     768+   0,  768+  16,             768+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  768+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		re8r18(2032, 800);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  800+   0, 2016+  16,   8);
		r17(2032, 808);
		API_polar::template xo <  8>(s,     800+   0,  800+   8,             800+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  800+   0, 1984+  32,  16);
		r16(2016, 816);
		API_polar::template xo < 16>(s,     800+   0,  800+  16,             800+   0,  16);
		API_polar::template xo < 32>(s,     768+   0,  768+  32,             768+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  768+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		r09r19r18r17(2016, 832);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  832+   0, 1984+  32,  16);
		r16(2016, 848);
		API_polar::template xo < 16>(s,     832+   0,  832+  16,             832+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  832+   0, 1920+  64,  32);
		r15(1984, 864);
		API_polar::template xo < 32>(s,     832+   0,  832+  32,             832+   0,  32);
		API_polar::template xo < 64>(s,     768+   0,  768+  64,             768+   0,  64);
		API_polar::template g  <128>(s, l, 1536+   0, 1536+ 128,  768+   0, 1536+ 256, 128);
		API_polar::template spc<128>(s, l, 1792+   0,                        896+   0, 128);
		API_polar::template xo <128>(s,     768+   0,  768+ 128,             768+   0, 128);
		API_polar::template xo <256>(s,     512+   0,  512+ 256,             512+   0, 256);
		API_polar::template xo <512>(s,       0+   0,    0+ 512,               0+   0, 512);
	}
};
#endif
