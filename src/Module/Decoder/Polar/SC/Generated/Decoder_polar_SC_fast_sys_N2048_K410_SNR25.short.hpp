#ifndef DECODER_POLAR_SC_FAST_SYS_N2048_K410_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N2048_K410_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Frozen_bits_2048_410_25[2048] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N2048_K410_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N2048_K410_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 2048);
		assert(K == 410);
		
		auto i = 0;
		while (i < 2048 && Frozen_bits_2048_410_25[i] == frozen_bits[i]) i++;
		assert(i == 2048);
	}

	virtual ~Decoder_polar_SC_fast_sys_N2048_K410_SNR25()
	{
	}

	// depth = 5, reverse depth = 6, size = 64, calls = 3
	inline void r05(const int off_l, const int off_s)
	{
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 3
	inline void r06(const int off_l, const int off_s)
	{
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 3
	inline void r07(const int off_l, const int off_s)
	{
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 11
	__attribute__((always_inline))
	inline void re8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 9, reverse depth = 2, size = 4, calls = 16
	__attribute__((always_inline))
	inline void re9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 9, reverse depth = 2, size = 4, calls = 18
	__attribute__((always_inline))
	inline void s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 15
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

	// depth = 7, reverse depth = 4, size = 16, calls = 8
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

	// depth = 6, reverse depth = 5, size = 32, calls = 2
	inline void r07re8re9s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r07(off_l+32, off_s+0);
		API_polar::template g0 <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re8re9s9(off_l+32, off_s+16);
		API_polar::template xo0<  16>(s,    off_s+  16,                       off_s+   0,   16);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 5
	inline void re7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 3
	inline void re7re8re9s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re7(off_l+32, off_s+0);
		API_polar::template gr <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		re8re9s9(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 5, reverse depth = 6, size = 64, calls = 2
	inline void r06re7re8re9s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r06(off_l+64, off_s+0);
		API_polar::template g0 <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		re7re8re9s9(off_l+64, off_s+32);
		API_polar::template xo0<  32>(s,    off_s+  32,                       off_s+   0,   32);
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
	inline void r010r110r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		r010r110(off_l+8, off_s+0);
		API_polar::template g  <   4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,    4);
		r19(off_l+8, off_s+4);
		API_polar::template xo <   4>(s,    off_s+   0, off_s+   4,            off_s+   0,    4);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 2
	inline void re8r010r110r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re8(off_l+16, off_s+0);
		API_polar::template gr <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		r010r110r19(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 2
	inline void re7re8r010r110r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re7(off_l+32, off_s+0);
		API_polar::template gr <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		re8r010r110r19(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 9, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void r09(const int off_l, const int off_s)
	{
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void r09s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r09(off_l+8, off_s+0);
		API_polar::template g0 <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		s9(off_l+8, off_s+4);
		API_polar::template xo0<   4>(s,    off_s+   4,                       off_s+   0,    4);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 8
	__attribute__((always_inline))
	inline void s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 2
	inline void r09s9s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		r09s9(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		s8(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 5
	inline void s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 2
	inline void r09s9s8s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		r09s9s8(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		s7(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 2
	inline void re6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	// depth = 8, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void r08(const int off_l, const int off_s)
	{
	}

	// depth = 7, reverse depth = 4, size = 16, calls = 6
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

	// depth = 6, reverse depth = 5, size = 32, calls = 2
	inline void re8re9s9re9s9s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re8re9s9(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		re9s9s8(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 6, reverse depth = 5, size = 32, calls = 2
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

	// depth = 6, reverse depth = 5, size = 32, calls = 3
	inline void s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <1024>(   l,    0+   0,    0+1024,               0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template rep< 512>(s, l, 3072+   0,                          0+   0,  512);
		API_polar::template gr < 512>(s, l, 2048+   0, 2048+ 512,    0+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template rep< 256>(s, l, 3584+   0,                        512+   0,  256);
		API_polar::template gr < 256>(s, l, 3072+   0, 3072+ 256,  512+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		r05(3968, 768);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		r06(4032, 832);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		r07re8re9s9(4032, 864);
		API_polar::template xo0<  32>(s,     832+  32,                        832+   0,   32);
		API_polar::template xo0<  64>(s,     768+  64,                        768+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  768+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		r06re7re8re9s9(3968, 896);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  896+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		re7re8r010r110r19(4032, 960);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  960+   0, 3968+  64,   32);
		r09s9s8s7(4032, 992);
		API_polar::template xo <  32>(s,     960+   0,  960+  32,             960+   0,   32);
		API_polar::template xo <  64>(s,     896+   0,  896+  64,             896+   0,   64);
		API_polar::template xo < 128>(s,     768+   0,  768+ 128,             768+   0,  128);
		API_polar::template xo < 256>(s,     512+   0,  512+ 256,             512+   0,  256);
		API_polar::template xo < 512>(s,       0+   0,    0+ 512,               0+   0,  512);
		API_polar::template g  <1024>(s, l,    0+   0,    0+1024,    0+   0,    0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template g0 < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		r05(3968, 1152);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		r06re7re8re9s9(3968, 1216);
		API_polar::template xo0<  64>(s,    1152+  64,                       1152+   0,   64);
		API_polar::template xo0< 128>(s,    1024+ 128,                       1024+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1024+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		r05(3968, 1280);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		re6(4032, 1344);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32, 1344+   0, 3968+  64,   32);
		re7re8r010r110r19(4032, 1376);
		API_polar::template xo <  32>(s,    1344+   0, 1344+  32,            1344+   0,   32);
		API_polar::template xo0<  64>(s,    1280+  64,                       1280+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1280+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		re6(4032, 1408);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32, 1408+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		r08(4080, 1440);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		r09s9(4080, 1448);
		API_polar::template xo0<   8>(s,    1440+   8,                       1440+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1440+   0, 4032+  32,   16);
		re9s9s8(4064, 1456);
		API_polar::template xo <  16>(s,    1440+   0, 1440+  16,            1440+   0,   16);
		API_polar::template xo <  32>(s,    1408+   0, 1408+  32,            1408+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1408+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		re8re9s9re9s9s8(4032, 1472);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1472+   0, 3968+  64,   32);
		re9s9s8s7(4032, 1504);
		API_polar::template xo <  32>(s,    1472+   0, 1472+  32,            1472+   0,   32);
		API_polar::template xo <  64>(s,    1408+   0, 1408+  64,            1408+   0,   64);
		API_polar::template xo < 128>(s,    1280+   0, 1280+ 128,            1280+   0,  128);
		API_polar::template xo < 256>(s,    1024+   0, 1024+ 256,            1024+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512, 1024+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template rep<  64>(s, l, 3968+   0,                       1536+   0,   64);
		API_polar::template gr <  64>(s, l, 3840+   0, 3840+  64, 1536+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		r07(4064, 1600);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		r08(4080, 1616);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		r09(4088, 1624);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		r010r110(4088, 1628);
		API_polar::template xo0<   4>(s,    1624+   4,                       1624+   0,    4);
		API_polar::template xo0<   8>(s,    1616+   8,                       1616+   0,    8);
		API_polar::template xo0<  16>(s,    1600+  16,                       1600+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1600+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		r08(4080, 1632);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		re9s9(4080, 1640);
		API_polar::template xo0<   8>(s,    1632+   8,                       1632+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1632+   0, 4032+  32,   16);
		re9s9s8(4064, 1648);
		API_polar::template xo <  16>(s,    1632+   0, 1632+  16,            1632+   0,   16);
		API_polar::template xo <  32>(s,    1600+   0, 1600+  32,            1600+   0,   32);
		API_polar::template xo <  64>(s,    1536+   0, 1536+  64,            1536+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1536+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		r07re8re9s9(4032, 1664);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1664+   0, 3968+  64,   32);
		re8re9s9re9s9s8(4032, 1696);
		API_polar::template xo <  32>(s,    1664+   0, 1664+  32,            1664+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1664+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		re8re9s9(4064, 1728);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1728+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		r010r110r19(4080, 1744);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1744+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1752+   0,    8);
		API_polar::template xo <   8>(s,    1744+   0, 1744+   8,            1744+   0,    8);
		API_polar::template xo <  16>(s,    1728+   0, 1728+  16,            1728+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1728+   0, 3968+  64,   32);
		s6(4032, 1760);
		API_polar::template xo <  32>(s,    1728+   0, 1728+  32,            1728+   0,   32);
		API_polar::template xo <  64>(s,    1664+   0, 1664+  64,            1664+   0,   64);
		API_polar::template xo < 128>(s,    1536+   0, 1536+ 128,            1536+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1536+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		re7re8re9s9(4032, 1792);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1792+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		re8(4080, 1824);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1824+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		re9(4088, 1832);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1832+   0, 4080+   8,    4);
		r19(4088, 1836);
		API_polar::template xo <   4>(s,    1832+   0, 1832+   4,            1832+   0,    4);
		API_polar::template xo <   8>(s,    1824+   0, 1824+   8,            1824+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1824+   0, 4032+  32,   16);
		s7(4064, 1840);
		API_polar::template xo <  16>(s,    1824+   0, 1824+  16,            1824+   0,   16);
		API_polar::template xo <  32>(s,    1792+   0, 1792+  32,            1792+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1792+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		r09s9s8s7(4032, 1856);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1856+   0, 3968+  64,   32);
		s6(4032, 1888);
		API_polar::template xo <  32>(s,    1856+   0, 1856+  32,            1856+   0,   32);
		API_polar::template xo <  64>(s,    1792+   0, 1792+  64,            1792+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1792+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		re9s9s8s7(4032, 1920);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1920+   0, 3968+  64,   32);
		s6(4032, 1952);
		API_polar::template xo <  32>(s,    1920+   0, 1920+  32,            1920+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1920+   0, 3840+ 128,   64);
		API_polar::template spc<  64>(s, l, 3968+   0,                       1984+   0,   64);
		API_polar::template xo <  64>(s,    1920+   0, 1920+  64,            1920+   0,   64);
		API_polar::template xo < 128>(s,    1792+   0, 1792+ 128,            1792+   0,  128);
		API_polar::template xo < 256>(s,    1536+   0, 1536+ 256,            1536+   0,  256);
		API_polar::template xo < 512>(s,    1024+   0, 1024+ 512,            1024+   0,  512);
		API_polar::template xo <1024>(s,       0+   0,    0+1024,               0+   0, 1024);
	}
};
}
}
#endif
