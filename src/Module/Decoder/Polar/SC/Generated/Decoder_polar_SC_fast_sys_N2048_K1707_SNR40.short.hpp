#ifndef DECODER_POLAR_SC_FAST_SYS_N2048_K1707_SNR40_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N2048_K1707_SNR40_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Frozen_bits_2048_1707_40[2048] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
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
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N2048_K1707_SNR40 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N2048_K1707_SNR40(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 2048);
		assert(K == 1707);
		
		auto i = 0;
		while (i < 2048 && Frozen_bits_2048_1707_40[i] == frozen_bits[i]) i++;
		assert(i == 2048);
	}

	virtual ~Decoder_polar_SC_fast_sys_N2048_K1707_SNR40()
	{
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 2
	inline void re6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 2
	inline void re7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 9, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void r09(const int off_l, const int off_s)
	{
	}

	// depth = 10, reverse depth = 1, size = 2, calls = 4
	__attribute__((always_inline))
	inline void r010(const int off_l, const int off_s)
	{
	}

	// depth = 10, reverse depth = 1, size = 2, calls = 4
	__attribute__((always_inline))
	inline void r110(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <   2>(s, l, off_l+   0,                       off_s+   0,    2);
	}

	// depth = 9, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void r010r110(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r010(off_l+4, off_s+0);
		API_polar::template g0 <   2>(   l, off_l+   0, off_l+   2,            off_l+   4,    2);
		r110(off_l+4, off_s+2);
		API_polar::template xo0<   2>(s,    off_s+   2,                       off_s+   0,    2);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void r09r010r110(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r09(off_l+8, off_s+0);
		API_polar::template g0 <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		r010r110(off_l+8, off_s+4);
		API_polar::template xo0<   4>(s,    off_s+   4,                       off_s+   0,    4);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 11
	__attribute__((always_inline))
	inline void s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 2
	inline void r09r010r110s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		r09r010r110(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		s8(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void r08(const int off_l, const int off_s)
	{
	}

	// depth = 9, reverse depth = 2, size = 4, calls = 16
	__attribute__((always_inline))
	inline void re9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 9, reverse depth = 2, size = 4, calls = 14
	__attribute__((always_inline))
	inline void s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 13
	__attribute__((always_inline))
	inline void re9s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		re9(off_l+8, off_s+0);
		API_polar::template gr <   4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,    4);
		s9(off_l+8, off_s+4);
		API_polar::template xo <   4>(s,    off_s+   0, off_s+   4,            off_s+   0,    4);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 2
	inline void r08re9s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r08(off_l+16, off_s+0);
		API_polar::template g0 <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re9s9(off_l+16, off_s+8);
		API_polar::template xo0<   8>(s,    off_s+   8,                       off_s+   0,    8);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 7
	inline void re9s9s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re9s9(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		s8(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 5
	__attribute__((always_inline))
	inline void re8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 2
	inline void re8re9s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re8(off_l+16, off_s+0);
		API_polar::template gr <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		re9s9(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 9, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void re9r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		re9(off_l+8, off_s+0);
		API_polar::template gr <   4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,    4);
		r19(off_l+8, off_s+4);
		API_polar::template xo <   4>(s,    off_s+   0, off_s+   4,            off_s+   0,    4);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 4
	__attribute__((always_inline))
	inline void r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 4
	inline void r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 2
	inline void re9s9r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re9s9(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		r18(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 2
	inline void re8re9r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re8(off_l+16, off_s+0);
		API_polar::template gr <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		re9r19(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 7
	inline void s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 4
	inline void s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 4
	inline void re9s9s8s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re9s9s8(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		s7(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 5, reverse depth = 6, size = 64, calls = 2
	inline void re9s9s8s7s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		re9s9s8s7(off_l+64, off_s+0);
		API_polar::template g  <  32>(s, l, off_l+   0, off_l+  32, off_s+   0, off_l+  64,   32);
		s6(off_l+64, off_s+32);
		API_polar::template xo <  32>(s,    off_s+   0, off_s+  32,            off_s+   0,   32);
	}

	// depth = 5, reverse depth = 6, size = 64, calls = 4
	inline void r15(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  64>(s, l, off_l+   0,                       off_s+   0,   64);
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 4
	inline void r16(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	// depth = 4, reverse depth = 7, size = 128, calls = 2
	inline void r14(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 128>(s, l, off_l+   0,                       off_s+   0,  128);
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <1024>(   l,    0+   0,    0+1024,               0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		re6(4032, 64);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32,   64+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		re7(4064, 96);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,   96+   0, 4032+  32,   16);
		r09r010r110s8(4064, 112);
		API_polar::template xo <  16>(s,      96+   0,   96+  16,              96+   0,   16);
		API_polar::template xo <  32>(s,      64+   0,   64+  32,              64+   0,   32);
		API_polar::template xo0<  64>(s,       0+  64,                          0+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,    0+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		re6(4032, 128);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32,  128+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		r08re9s9(4064, 160);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  160+   0, 4032+  32,   16);
		re9s9s8(4064, 176);
		API_polar::template xo <  16>(s,     160+   0,  160+  16,             160+   0,   16);
		API_polar::template xo <  32>(s,     128+   0,  128+  32,             128+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  128+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		re8re9s9(4064, 192);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  192+   0, 4032+  32,   16);
		re9s9s8(4064, 208);
		API_polar::template xo <  16>(s,     192+   0,  192+  16,             192+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  192+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		re9r19(4080, 224);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  224+   0, 4064+  16,    8);
		r18(4080, 232);
		API_polar::template xo <   8>(s,     224+   0,  224+   8,             224+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  224+   0, 4032+  32,   16);
		r17(4064, 240);
		API_polar::template xo <  16>(s,     224+   0,  224+  16,             224+   0,   16);
		API_polar::template xo <  32>(s,     192+   0,  192+  32,             192+   0,   32);
		API_polar::template xo <  64>(s,     128+   0,  128+  64,             128+   0,   64);
		API_polar::template xo < 128>(s,       0+   0,    0+ 128,               0+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,    0+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		r08re9s9(4064, 272);
		API_polar::template xo0<  16>(s,     256+  16,                        256+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  256+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		re8re9s9(4064, 288);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  288+   0, 4032+  32,   16);
		re9s9r18(4064, 304);
		API_polar::template xo <  16>(s,     288+   0,  288+  16,             288+   0,   16);
		API_polar::template xo <  32>(s,     256+   0,  256+  32,             256+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  256+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		re8re9r19(4064, 320);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  320+   0, 4032+  32,   16);
		s7(4064, 336);
		API_polar::template xo <  16>(s,     320+   0,  320+  16,             320+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  320+   0, 3968+  64,   32);
		s6(4032, 352);
		API_polar::template xo <  32>(s,     320+   0,  320+  32,             320+   0,   32);
		API_polar::template xo <  64>(s,     256+   0,  256+  64,             256+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  256+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		r09r010r110s8(4064, 384);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  384+   0, 4032+  32,   16);
		s7(4064, 400);
		API_polar::template xo <  16>(s,     384+   0,  384+  16,             384+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  384+   0, 3968+  64,   32);
		s6(4032, 416);
		API_polar::template xo <  32>(s,     384+   0,  384+  32,             384+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  384+   0, 3840+ 128,   64);
		API_polar::template spc<  64>(s, l, 3968+   0,                        448+   0,   64);
		API_polar::template xo <  64>(s,     384+   0,  384+  64,             384+   0,   64);
		API_polar::template xo < 128>(s,     256+   0,  256+ 128,             256+   0,  128);
		API_polar::template xo < 256>(s,       0+   0,    0+ 256,               0+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512,    0+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		re7(4064, 512);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  512+   0, 4032+  32,   16);
		re8re9r19(4064, 528);
		API_polar::template xo <  16>(s,     512+   0,  512+  16,             512+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  512+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		re8(4080, 544);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  544+   0, 4064+  16,    8);
		s8(4080, 552);
		API_polar::template xo <   8>(s,     544+   0,  544+   8,             544+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  544+   0, 4032+  32,   16);
		s7(4064, 560);
		API_polar::template xo <  16>(s,     544+   0,  544+  16,             544+   0,   16);
		API_polar::template xo <  32>(s,     512+   0,  512+  32,             512+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  512+   0, 3840+ 128,   64);
		re9s9s8s7s6(3968, 576);
		API_polar::template xo <  64>(s,     512+   0,  512+  64,             512+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  512+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		re9s9s8s7s6(3968, 640);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  640+   0, 3840+ 128,   64);
		r15(3968, 704);
		API_polar::template xo <  64>(s,     640+   0,  640+  64,             640+   0,   64);
		API_polar::template xo < 128>(s,     512+   0,  512+ 128,             512+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,  512+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		re9s9s8(4064, 768);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  768+   0, 4032+  32,   16);
		r17(4064, 784);
		API_polar::template xo <  16>(s,     768+   0,  768+  16,             768+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  768+   0, 3968+  64,   32);
		r16(4032, 800);
		API_polar::template xo <  32>(s,     768+   0,  768+  32,             768+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  768+   0, 3840+ 128,   64);
		r15(3968, 832);
		API_polar::template xo <  64>(s,     768+   0,  768+  64,             768+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  768+   0, 3584+ 256,  128);
		r14(3840, 896);
		API_polar::template xo < 128>(s,     768+   0,  768+ 128,             768+   0,  128);
		API_polar::template xo < 256>(s,     512+   0,  512+ 256,             512+   0,  256);
		API_polar::template xo < 512>(s,       0+   0,    0+ 512,               0+   0,  512);
		API_polar::template g  <1024>(s, l,    0+   0,    0+1024,    0+   0,    0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		r08(4080, 1024);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		r09r010r110(4080, 1032);
		API_polar::template xo0<   8>(s,    1024+   8,                       1024+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1024+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		r09(4088, 1040);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		s9(4088, 1044);
		API_polar::template xo0<   4>(s,    1040+   4,                       1040+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1040+   0, 4064+  16,    8);
		s8(4080, 1048);
		API_polar::template xo <   8>(s,    1040+   0, 1040+   8,            1040+   0,    8);
		API_polar::template xo <  16>(s,    1024+   0, 1024+  16,            1024+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1024+   0, 3968+  64,   32);
		re9s9s8s7(4032, 1056);
		API_polar::template xo <  32>(s,    1024+   0, 1024+  32,            1024+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1024+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		re9s9s8s7(4032, 1088);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1088+   0, 3968+  64,   32);
		r16(4032, 1120);
		API_polar::template xo <  32>(s,    1088+   0, 1088+  32,            1088+   0,   32);
		API_polar::template xo <  64>(s,    1024+   0, 1024+  64,            1024+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1024+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		re9s9r18(4064, 1152);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1152+   0, 4032+  32,   16);
		r17(4064, 1168);
		API_polar::template xo <  16>(s,    1152+   0, 1152+  16,            1152+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1152+   0, 3968+  64,   32);
		r16(4032, 1184);
		API_polar::template xo <  32>(s,    1152+   0, 1152+  32,            1152+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1152+   0, 3840+ 128,   64);
		r15(3968, 1216);
		API_polar::template xo <  64>(s,    1152+   0, 1152+  64,            1152+   0,   64);
		API_polar::template xo < 128>(s,    1024+   0, 1024+ 128,            1024+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1024+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		r010r110(4088, 1280);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4, 1280+   0, 4080+   8,    4);
		r19(4088, 1284);
		API_polar::template xo <   4>(s,    1280+   0, 1280+   4,            1280+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1280+   0, 4064+  16,    8);
		r18(4080, 1288);
		API_polar::template xo <   8>(s,    1280+   0, 1280+   8,            1280+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1280+   0, 4032+  32,   16);
		r17(4064, 1296);
		API_polar::template xo <  16>(s,    1280+   0, 1280+  16,            1280+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1280+   0, 3968+  64,   32);
		r16(4032, 1312);
		API_polar::template xo <  32>(s,    1280+   0, 1280+  32,            1280+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1280+   0, 3840+ 128,   64);
		r15(3968, 1344);
		API_polar::template xo <  64>(s,    1280+   0, 1280+  64,            1280+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1280+   0, 3584+ 256,  128);
		r14(3840, 1408);
		API_polar::template xo < 128>(s,    1280+   0, 1280+ 128,            1280+   0,  128);
		API_polar::template xo < 256>(s,    1024+   0, 1024+ 256,            1024+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512, 1024+   0, 2048+1024,  512);
		API_polar::template spc< 512>(s, l, 3072+   0,                       1536+   0,  512);
		API_polar::template xo < 512>(s,    1024+   0, 1024+ 512,            1024+   0,  512);
		API_polar::template xo <1024>(s,       0+   0,    0+1024,               0+   0, 1024);
	}
};
}
}
#endif
