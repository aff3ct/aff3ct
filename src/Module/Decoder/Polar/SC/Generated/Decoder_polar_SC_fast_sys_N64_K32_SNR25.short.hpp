#ifndef DECODER_POLAR_SC_FAST_SYS_N64_K32_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N64_K32_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_64_32_25[64] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N64_K32_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N64_K32_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 64);
		assert(K == 32);
		
		auto i = 0;
		while (i < 64 && Frozen_bits_64_32_25[i] == frozen_bits[i]) i++;
		assert(i == 64);
	}

	virtual ~Decoder_polar_SC_fast_sys_N64_K32_SNR25()
	{
	}

	// depth = 5, reverse depth = 1, size = 2, calls = 2
	__attribute__((always_inline))
	inline void r05(const int off_l, const int off_s)
	{
	}

	// depth = 5, reverse depth = 1, size = 2, calls = 2
	__attribute__((always_inline))
	inline void r15(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 2>(s, l, off_l+  0,                   off_s+  0,  2);
	}

	// depth = 4, reverse depth = 2, size = 4, calls = 2
	__attribute__((always_inline))
	inline void r05r15(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r05(off_l+4, off_s+0);
		API_polar::template g0 < 2>(   l, off_l+  0, off_l+  2,          off_l+  4,  2);
		r15(off_l+4, off_s+2);
		API_polar::template xo0< 2>(s,    off_s+  2,                   off_s+  0,  2);
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <32>(   l,   0+  0,   0+ 32,            0+ 64, 32);
		API_polar::template f  <16>(   l,  64+  0,  64+ 16,           64+ 32, 16);
		API_polar::template rep<16>(s, l,  96+  0,                     0+  0, 16);
		API_polar::template gr <16>(s, l,  64+  0,  64+ 16,   0+  0,  64+ 32, 16);
		API_polar::template f  < 8>(   l,  96+  0,  96+  8,           96+ 16,  8);
		API_polar::template rep< 8>(s, l, 112+  0,                    16+  0,  8);
		API_polar::template gr < 8>(s, l,  96+  0,  96+  8,  16+  0,  96+ 16,  8);
		API_polar::template f  < 4>(   l, 112+  0, 112+  4,          112+  8,  4);
		r05r15(120, 24);
		API_polar::template g  < 4>(s, l, 112+  0, 112+  4,  24+  0, 112+  8,  4);
		API_polar::template h  < 4>(s, l, 120+  0,                    28+  0,  4);
		API_polar::template xo < 4>(s,     24+  0,  24+  4,           24+  0,  4);
		API_polar::template xo < 8>(s,     16+  0,  16+  8,           16+  0,  8);
		API_polar::template xo <16>(s,      0+  0,   0+ 16,            0+  0, 16);
		API_polar::template g  <32>(s, l,   0+  0,   0+ 32,   0+  0,   0+ 64, 32);
		API_polar::template f  <16>(   l,  64+  0,  64+ 16,           64+ 32, 16);
		API_polar::template f  < 8>(   l,  96+  0,  96+  8,           96+ 16,  8);
		API_polar::template g0 < 4>(   l, 112+  0, 112+  4,          112+  8,  4);
		r05r15(120, 36);
		API_polar::template xo0< 4>(s,     32+  4,                    32+  0,  4);
		API_polar::template g  < 8>(s, l,  96+  0,  96+  8,  32+  0,  96+ 16,  8);
		API_polar::template spc< 8>(s, l, 112+  0,                    40+  0,  8);
		API_polar::template xo < 8>(s,     32+  0,  32+  8,           32+  0,  8);
		API_polar::template g  <16>(s, l,  64+  0,  64+ 16,  32+  0,  64+ 32, 16);
		API_polar::template spc<16>(s, l,  96+  0,                    48+  0, 16);
		API_polar::template xo <16>(s,     32+  0,  32+ 16,           32+  0, 16);
		API_polar::template xo <32>(s,      0+  0,   0+ 32,            0+  0, 32);
	}
};
#endif
