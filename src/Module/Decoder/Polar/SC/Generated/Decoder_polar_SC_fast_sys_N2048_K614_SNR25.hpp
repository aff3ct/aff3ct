#ifndef DECODER_POLAR_SC_FAST_SYS_N2048_K614_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N2048_K614_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_2048_614_25[2048] = {
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
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N2048_K614_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N2048_K614_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits)
	{
		assert(N == 2048);
		assert(K == 614);
		
		auto i = 0;
		while (i < 2048 && Frozen_bits_2048_614_25[i] == frozen_bits[i]) i++;
		assert(i == 2048);
	}

	virtual ~Decoder_polar_SC_fast_sys_N2048_K614_SNR25()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <1024>(   l,    0+   0,    0+1024,               0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template g0 < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template g0 < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        496+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  496+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        504+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  504+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        508+   0,    4);
		API_polar::template xo <   4>(s,     504+   0,  504+   4,             504+   0,    4);
		API_polar::template xo <   8>(s,     496+   0,  496+   8,             496+   0,    8);
		API_polar::template xo0<  16>(s,     480+  16,                        480+   0,   16);
		API_polar::template xo0<  32>(s,     448+  32,                        448+   0,   32);
		API_polar::template xo0<  64>(s,     384+  64,                        384+   0,   64);
		API_polar::template xo0< 128>(s,     256+ 128,                        256+   0,  128);
		API_polar::template xo0< 256>(s,       0+ 256,                          0+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512,    0+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template g0 < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template rep<  32>(s, l, 4032+   0,                        704+   0,   32);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32,  704+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        736+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  736+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        752+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  752+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        760+   0,    8);
		API_polar::template xo <   8>(s,     752+   0,  752+   8,             752+   0,    8);
		API_polar::template xo <  16>(s,     736+   0,  736+  16,             736+   0,   16);
		API_polar::template xo <  32>(s,     704+   0,  704+  32,             704+   0,   32);
		API_polar::template xo0<  64>(s,     640+  64,                        640+   0,   64);
		API_polar::template xo0< 128>(s,     512+ 128,                        512+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,  512+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template rep<  64>(s, l, 3968+   0,                        768+   0,   64);
		API_polar::template gr <  64>(s, l, 3840+   0, 3840+  64,  768+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template rep<  32>(s, l, 4032+   0,                        832+   0,   32);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32,  832+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        872+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  872+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        876+   0,    4);
		API_polar::template xo <   4>(s,     872+   0,  872+   4,             872+   0,    4);
		API_polar::template xo0<   8>(s,     864+   8,                        864+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  864+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        880+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  880+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        884+   0,    4);
		API_polar::template xo <   4>(s,     880+   0,  880+   4,             880+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  880+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        888+   0,    8);
		API_polar::template xo <   8>(s,     880+   0,  880+   8,             880+   0,    8);
		API_polar::template xo <  16>(s,     864+   0,  864+  16,             864+   0,   16);
		API_polar::template xo <  32>(s,     832+   0,  832+  32,             832+   0,   32);
		API_polar::template xo <  64>(s,     768+   0,  768+  64,             768+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  768+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        912+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  912+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        920+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  920+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        924+   0,    4);
		API_polar::template xo <   4>(s,     920+   0,  920+   4,             920+   0,    4);
		API_polar::template xo <   8>(s,     912+   0,  912+   8,             912+   0,    8);
		API_polar::template xo0<  16>(s,     896+  16,                        896+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  896+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        928+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  928+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        936+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  936+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        940+   0,    4);
		API_polar::template xo <   4>(s,     936+   0,  936+   4,             936+   0,    4);
		API_polar::template xo <   8>(s,     928+   0,  928+   8,             928+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  928+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        944+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  944+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        948+   0,    4);
		API_polar::template xo <   4>(s,     944+   0,  944+   4,             944+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  944+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        952+   0,    8);
		API_polar::template xo <   8>(s,     944+   0,  944+   8,             944+   0,    8);
		API_polar::template xo <  16>(s,     928+   0,  928+  16,             928+   0,   16);
		API_polar::template xo <  32>(s,     896+   0,  896+  32,             896+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  896+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        960+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  960+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        968+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  968+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        972+   0,    4);
		API_polar::template xo <   4>(s,     968+   0,  968+   4,             968+   0,    4);
		API_polar::template xo <   8>(s,     960+   0,  960+   8,             960+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  960+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        978+   0,    2);
		API_polar::template xo0<   2>(s,     976+   2,                        976+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4,  976+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        980+   0,    4);
		API_polar::template xo <   4>(s,     976+   0,  976+   4,             976+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  976+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        984+   0,    8);
		API_polar::template xo <   8>(s,     976+   0,  976+   8,             976+   0,    8);
		API_polar::template xo <  16>(s,     960+   0,  960+  16,             960+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  960+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                        992+   0,   32);
		API_polar::template xo <  32>(s,     960+   0,  960+  32,             960+   0,   32);
		API_polar::template xo <  64>(s,     896+   0,  896+  64,             896+   0,   64);
		API_polar::template xo < 128>(s,     768+   0,  768+ 128,             768+   0,  128);
		API_polar::template xo < 256>(s,     512+   0,  512+ 256,             512+   0,  256);
		API_polar::template xo < 512>(s,       0+   0,    0+ 512,               0+   0,  512);
		API_polar::template g  <1024>(s, l,    0+   0,    0+1024,    0+   0,    0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template rep< 128>(s, l, 3840+   0,                       1024+   0,  128);
		API_polar::template gr < 128>(s, l, 3584+   0, 3584+ 128, 1024+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template rep<  64>(s, l, 3968+   0,                       1152+   0,   64);
		API_polar::template gr <  64>(s, l, 3840+   0, 3840+  64, 1152+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1240+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1240+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1244+   0,    4);
		API_polar::template xo <   4>(s,    1240+   0, 1240+   4,            1240+   0,    4);
		API_polar::template xo0<   8>(s,    1232+   8,                       1232+   0,    8);
		API_polar::template xo0<  16>(s,    1216+  16,                       1216+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1216+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1248+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1248+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1256+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1256+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1260+   0,    4);
		API_polar::template xo <   4>(s,    1256+   0, 1256+   4,            1256+   0,    4);
		API_polar::template xo <   8>(s,    1248+   0, 1248+   8,            1248+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1248+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1264+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1264+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1268+   0,    4);
		API_polar::template xo <   4>(s,    1264+   0, 1264+   4,            1264+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1264+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1272+   0,    8);
		API_polar::template xo <   8>(s,    1264+   0, 1264+   8,            1264+   0,    8);
		API_polar::template xo <  16>(s,    1248+   0, 1248+  16,            1248+   0,   16);
		API_polar::template xo <  32>(s,    1216+   0, 1216+  32,            1216+   0,   32);
		API_polar::template xo <  64>(s,    1152+   0, 1152+  64,            1152+   0,   64);
		API_polar::template xo < 128>(s,    1024+   0, 1024+ 128,            1024+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1024+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1336+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1336+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1340+   0,    4);
		API_polar::template xo <   4>(s,    1336+   0, 1336+   4,            1336+   0,    4);
		API_polar::template xo0<   8>(s,    1328+   8,                       1328+   0,    8);
		API_polar::template xo0<  16>(s,    1312+  16,                       1312+   0,   16);
		API_polar::template xo0<  32>(s,    1280+  32,                       1280+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1280+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                       1344+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16, 1344+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1360+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1360+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1368+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1368+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1372+   0,    4);
		API_polar::template xo <   4>(s,    1368+   0, 1368+   4,            1368+   0,    4);
		API_polar::template xo <   8>(s,    1360+   0, 1360+   8,            1360+   0,    8);
		API_polar::template xo <  16>(s,    1344+   0, 1344+  16,            1344+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1344+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1376+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1376+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1384+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1384+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1388+   0,    4);
		API_polar::template xo <   4>(s,    1384+   0, 1384+   4,            1384+   0,    4);
		API_polar::template xo <   8>(s,    1376+   0, 1376+   8,            1376+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1376+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1394+   0,    2);
		API_polar::template xo0<   2>(s,    1392+   2,                       1392+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4, 1392+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1396+   0,    4);
		API_polar::template xo <   4>(s,    1392+   0, 1392+   4,            1392+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1392+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1400+   0,    8);
		API_polar::template xo <   8>(s,    1392+   0, 1392+   8,            1392+   0,    8);
		API_polar::template xo <  16>(s,    1376+   0, 1376+  16,            1376+   0,   16);
		API_polar::template xo <  32>(s,    1344+   0, 1344+  32,            1344+   0,   32);
		API_polar::template xo <  64>(s,    1280+   0, 1280+  64,            1280+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1280+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                       1408+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16, 1408+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1424+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1424+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1432+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1432+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1436+   0,    4);
		API_polar::template xo <   4>(s,    1432+   0, 1432+   4,            1432+   0,    4);
		API_polar::template xo <   8>(s,    1424+   0, 1424+   8,            1424+   0,    8);
		API_polar::template xo <  16>(s,    1408+   0, 1408+  16,            1408+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1408+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1446+   0,    2);
		API_polar::template xo0<   2>(s,    1444+   2,                       1444+   0,    2);
		API_polar::template xo0<   4>(s,    1440+   4,                       1440+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1440+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1448+   0,    8);
		API_polar::template xo <   8>(s,    1440+   0, 1440+   8,            1440+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1440+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1456+   0,   16);
		API_polar::template xo <  16>(s,    1440+   0, 1440+  16,            1440+   0,   16);
		API_polar::template xo <  32>(s,    1408+   0, 1408+  32,            1408+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1408+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1472+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1472+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1476+   0,    4);
		API_polar::template xo <   4>(s,    1472+   0, 1472+   4,            1472+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1472+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1480+   0,    8);
		API_polar::template xo <   8>(s,    1472+   0, 1472+   8,            1472+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1472+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1488+   0,   16);
		API_polar::template xo <  16>(s,    1472+   0, 1472+  16,            1472+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1472+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                       1504+   0,   32);
		API_polar::template xo <  32>(s,    1472+   0, 1472+  32,            1472+   0,   32);
		API_polar::template xo <  64>(s,    1408+   0, 1408+  64,            1408+   0,   64);
		API_polar::template xo < 128>(s,    1280+   0, 1280+ 128,            1280+   0,  128);
		API_polar::template xo < 256>(s,    1024+   0, 1024+ 256,            1024+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512, 1024+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template rep<  32>(s, l, 4032+   0,                       1536+   0,   32);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32, 1536+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                       1568+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16, 1568+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1584+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1584+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1592+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1592+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1596+   0,    4);
		API_polar::template xo <   4>(s,    1592+   0, 1592+   4,            1592+   0,    4);
		API_polar::template xo <   8>(s,    1584+   0, 1584+   8,            1584+   0,    8);
		API_polar::template xo <  16>(s,    1568+   0, 1568+  16,            1568+   0,   16);
		API_polar::template xo <  32>(s,    1536+   0, 1536+  32,            1536+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1536+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                       1600+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16, 1600+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1622+   0,    2);
		API_polar::template xo0<   2>(s,    1620+   2,                       1620+   0,    2);
		API_polar::template xo0<   4>(s,    1616+   4,                       1616+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1616+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1624+   0,    8);
		API_polar::template xo <   8>(s,    1616+   0, 1616+   8,            1616+   0,    8);
		API_polar::template xo <  16>(s,    1600+   0, 1600+  16,            1600+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1600+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1632+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1632+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1636+   0,    4);
		API_polar::template xo <   4>(s,    1632+   0, 1632+   4,            1632+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1632+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1640+   0,    8);
		API_polar::template xo <   8>(s,    1632+   0, 1632+   8,            1632+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1632+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1648+   0,   16);
		API_polar::template xo <  16>(s,    1632+   0, 1632+  16,            1632+   0,   16);
		API_polar::template xo <  32>(s,    1600+   0, 1600+  32,            1600+   0,   32);
		API_polar::template xo <  64>(s,    1536+   0, 1536+  64,            1536+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1536+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1676+   0,    4);
		API_polar::template xo0<   4>(s,    1672+   4,                       1672+   0,    4);
		API_polar::template xo0<   8>(s,    1664+   8,                       1664+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1664+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1680+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1680+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1684+   0,    4);
		API_polar::template xo <   4>(s,    1680+   0, 1680+   4,            1680+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1680+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1688+   0,    8);
		API_polar::template xo <   8>(s,    1680+   0, 1680+   8,            1680+   0,    8);
		API_polar::template xo <  16>(s,    1664+   0, 1664+  16,            1664+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1664+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1696+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1696+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1700+   0,    4);
		API_polar::template xo <   4>(s,    1696+   0, 1696+   4,            1696+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1696+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1704+   0,    8);
		API_polar::template xo <   8>(s,    1696+   0, 1696+   8,            1696+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1696+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1712+   0,   16);
		API_polar::template xo <  16>(s,    1696+   0, 1696+  16,            1696+   0,   16);
		API_polar::template xo <  32>(s,    1664+   0, 1664+  32,            1664+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1664+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1728+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1728+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1732+   0,    4);
		API_polar::template xo <   4>(s,    1728+   0, 1728+   4,            1728+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1728+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1736+   0,    8);
		API_polar::template xo <   8>(s,    1728+   0, 1728+   8,            1728+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1728+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1744+   0,   16);
		API_polar::template xo <  16>(s,    1728+   0, 1728+  16,            1728+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1728+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                       1760+   0,   32);
		API_polar::template xo <  32>(s,    1728+   0, 1728+  32,            1728+   0,   32);
		API_polar::template xo <  64>(s,    1664+   0, 1664+  64,            1664+   0,   64);
		API_polar::template xo < 128>(s,    1536+   0, 1536+ 128,            1536+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1536+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1792+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1792+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1800+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1800+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1804+   0,    4);
		API_polar::template xo <   4>(s,    1800+   0, 1800+   4,            1800+   0,    4);
		API_polar::template xo <   8>(s,    1792+   0, 1792+   8,            1792+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1792+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1808+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1808+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1812+   0,    4);
		API_polar::template xo <   4>(s,    1808+   0, 1808+   4,            1808+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1808+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1816+   0,    8);
		API_polar::template xo <   8>(s,    1808+   0, 1808+   8,            1808+   0,    8);
		API_polar::template xo <  16>(s,    1792+   0, 1792+  16,            1792+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1792+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1824+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1824+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1828+   0,    4);
		API_polar::template xo <   4>(s,    1824+   0, 1824+   4,            1824+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1824+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1832+   0,    8);
		API_polar::template xo <   8>(s,    1824+   0, 1824+   8,            1824+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1824+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                       1840+   0,   16);
		API_polar::template xo <  16>(s,    1824+   0, 1824+  16,            1824+   0,   16);
		API_polar::template xo <  32>(s,    1792+   0, 1792+  32,            1792+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1792+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1856+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1856+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1860+   0,    4);
		API_polar::template xo <   4>(s,    1856+   0, 1856+   4,            1856+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1856+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1864+   0,    8);
		API_polar::template xo <   8>(s,    1856+   0, 1856+   8,            1856+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1856+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                       1872+   0,   16);
		API_polar::template xo <  16>(s,    1856+   0, 1856+  16,            1856+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1856+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                       1888+   0,   32);
		API_polar::template xo <  32>(s,    1856+   0, 1856+  32,            1856+   0,   32);
		API_polar::template xo <  64>(s,    1792+   0, 1792+  64,            1792+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1792+   0, 3584+ 256,  128);
		API_polar::template spc< 128>(s, l, 3840+   0,                       1920+   0,  128);
		API_polar::template xo < 128>(s,    1792+   0, 1792+ 128,            1792+   0,  128);
		API_polar::template xo < 256>(s,    1536+   0, 1536+ 256,            1536+   0,  256);
		API_polar::template xo < 512>(s,    1024+   0, 1024+ 512,            1024+   0,  512);
		API_polar::template xo <1024>(s,       0+   0,    0+1024,               0+   0, 1024);
	}
};
#endif
