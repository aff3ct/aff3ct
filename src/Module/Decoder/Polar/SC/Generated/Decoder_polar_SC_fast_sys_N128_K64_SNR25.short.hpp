#ifndef DECODER_POLAR_SC_FAST_SYS_N128_K64_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N128_K64_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_128_64_25[128] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N128_K64_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N128_K64_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 128);
		assert(K == 64);
		
		auto i = 0;
		while (i < 128 && Frozen_bits_128_64_25[i] == frozen_bits[i]) i++;
		assert(i == 128);
	}

	virtual ~Decoder_polar_SC_fast_sys_N128_K64_SNR25()
	{
	}

	// depth = 4, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void r04(const int off_l, const int off_s)
	{
	}

	// depth = 6, reverse depth = 1, size = 2, calls = 2
	__attribute__((always_inline))
	inline void r06(const int off_l, const int off_s)
	{
	}

	// depth = 6, reverse depth = 1, size = 2, calls = 2
	__attribute__((always_inline))
	inline void r16(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 2>(s, l, off_l+  0,                   off_s+  0,  2);
	}

	// depth = 5, reverse depth = 2, size = 4, calls = 2
	__attribute__((always_inline))
	inline void r06r16(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r06(off_l+4, off_s+0);
		API_polar::template g0 < 2>(   l, off_l+  0, off_l+  2,          off_l+  4,  2);
		r16(off_l+4, off_s+2);
		API_polar::template xo0< 2>(s,    off_s+  2,                   off_s+  0,  2);
	}

	// depth = 5, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void re5(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep< 4>(s, l, off_l+  0,                   off_s+  0,  4);
	}

	// depth = 5, reverse depth = 2, size = 4, calls = 4
	__attribute__((always_inline))
	inline void s5(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc< 4>(s, l, off_l+  0,                   off_s+  0,  4);
	}

	// depth = 4, reverse depth = 3, size = 8, calls = 4
	__attribute__((always_inline))
	inline void re5s5(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  < 4>(   l, off_l+  0, off_l+  4,          off_l+  8,  4);
		re5(off_l+8, off_s+0);
		API_polar::template gr < 4>(s, l, off_l+  0, off_l+  4, off_s+  0, off_l+  8,  4);
		s5(off_l+8, off_s+4);
		API_polar::template xo < 4>(s,    off_s+  0, off_s+  4,          off_s+  0,  4);
	}

	// depth = 4, reverse depth = 3, size = 8, calls = 2
	__attribute__((always_inline))
	inline void r14(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 8>(s, l, off_l+  0,                   off_s+  0,  8);
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <64>(   l,   0+  0,   0+ 64,            0+128, 64);
		API_polar::template f  <32>(   l, 128+  0, 128+ 32,          128+ 64, 32);
		API_polar::template g0 <16>(   l, 192+  0, 192+ 16,          192+ 32, 16);
		r04(240, 16);
		API_polar::template g0 < 8>(   l, 224+  0, 224+  8,          224+ 16,  8);
		API_polar::template g0 < 4>(   l, 240+  0, 240+  4,          240+  8,  4);
		r06r16(248, 28);
		API_polar::template xo0< 4>(s,     24+  4,                    24+  0,  4);
		API_polar::template xo0< 8>(s,     16+  8,                    16+  0,  8);
		API_polar::template xo0<16>(s,      0+ 16,                     0+  0, 16);
		API_polar::template g  <32>(s, l, 128+  0, 128+ 32,   0+  0, 128+ 64, 32);
		API_polar::template f  <16>(   l, 192+  0, 192+ 16,          192+ 32, 16);
		r04(240, 32);
		API_polar::template g0 < 8>(   l, 224+  0, 224+  8,          224+ 16,  8);
		re5s5(240, 40);
		API_polar::template xo0< 8>(s,     32+  8,                    32+  0,  8);
		API_polar::template g  <16>(s, l, 192+  0, 192+ 16,  32+  0, 192+ 32, 16);
		API_polar::template f  < 8>(   l, 224+  0, 224+  8,          224+ 16,  8);
		re5s5(240, 48);
		API_polar::template g  < 8>(s, l, 224+  0, 224+  8,  48+  0, 224+ 16,  8);
		API_polar::template spc< 8>(s, l, 240+  0,                    56+  0,  8);
		API_polar::template xo < 8>(s,     48+  0,  48+  8,           48+  0,  8);
		API_polar::template xo <16>(s,     32+  0,  32+ 16,           32+  0, 16);
		API_polar::template xo <32>(s,      0+  0,   0+ 32,            0+  0, 32);
		API_polar::template g  <64>(s, l,   0+  0,   0+ 64,   0+  0,   0+128, 64);
		API_polar::template f  <32>(   l, 128+  0, 128+ 32,          128+ 64, 32);
		API_polar::template f  <16>(   l, 192+  0, 192+ 16,          192+ 32, 16);
		API_polar::template f  < 8>(   l, 224+  0, 224+  8,          224+ 16,  8);
		API_polar::template rep< 8>(s, l, 240+  0,                    64+  0,  8);
		API_polar::template gr < 8>(s, l, 224+  0, 224+  8,  64+  0, 224+ 16,  8);
		re5s5(240, 72);
		API_polar::template xo < 8>(s,     64+  0,  64+  8,           64+  0,  8);
		API_polar::template g  <16>(s, l, 192+  0, 192+ 16,  64+  0, 192+ 32, 16);
		API_polar::template f  < 8>(   l, 224+  0, 224+  8,          224+ 16,  8);
		re5s5(240, 80);
		API_polar::template g  < 8>(s, l, 224+  0, 224+  8,  80+  0, 224+ 16,  8);
		r14(240, 88);
		API_polar::template xo < 8>(s,     80+  0,  80+  8,           80+  0,  8);
		API_polar::template xo <16>(s,     64+  0,  64+ 16,           64+  0, 16);
		API_polar::template g  <32>(s, l, 128+  0, 128+ 32,  64+  0, 128+ 64, 32);
		API_polar::template f  <16>(   l, 192+  0, 192+ 16,          192+ 32, 16);
		API_polar::template f  < 8>(   l, 224+  0, 224+  8,          224+ 16,  8);
		API_polar::template f  < 4>(   l, 240+  0, 240+  4,          240+  8,  4);
		r06r16(248, 96);
		API_polar::template g  < 4>(s, l, 240+  0, 240+  4,  96+  0, 240+  8,  4);
		API_polar::template h  < 4>(s, l, 248+  0,                   100+  0,  4);
		API_polar::template xo < 4>(s,     96+  0,  96+  4,           96+  0,  4);
		API_polar::template g  < 8>(s, l, 224+  0, 224+  8,  96+  0, 224+ 16,  8);
		r14(240, 104);
		API_polar::template xo < 8>(s,     96+  0,  96+  8,           96+  0,  8);
		API_polar::template g  <16>(s, l, 192+  0, 192+ 16,  96+  0, 192+ 32, 16);
		API_polar::template h  <16>(s, l, 224+  0,                   112+  0, 16);
		API_polar::template xo <16>(s,     96+  0,  96+ 16,           96+  0, 16);
		API_polar::template xo <32>(s,     64+  0,  64+ 32,           64+  0, 32);
		API_polar::template xo <64>(s,      0+  0,   0+ 64,            0+  0, 64);
	}
};
#endif
