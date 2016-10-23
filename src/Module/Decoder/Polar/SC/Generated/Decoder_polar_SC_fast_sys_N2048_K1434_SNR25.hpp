#ifndef DECODER_POLAR_SC_FAST_SYS_N2048_K1434_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N2048_K1434_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_2048_1434_25[2048] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
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
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N2048_K1434_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N2048_K1434_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 2048);
		assert(K == 1434);
		
		auto i = 0;
		while (i < 2048 && Frozen_bits_2048_1434_25[i] == frozen_bits[i]) i++;
		assert(i == 2048);
	}

	virtual ~Decoder_polar_SC_fast_sys_N2048_K1434_SNR25()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <1024>(   l,    0+   0,    0+1024,               0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template rep< 128>(s, l, 3840+   0,                          0+   0,  128);
		API_polar::template gr < 128>(s, l, 3584+   0, 3584+ 128,    0+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template rep<  64>(s, l, 3968+   0,                        128+   0,   64);
		API_polar::template gr <  64>(s, l, 3840+   0, 3840+  64,  128+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        222+   0,    2);
		API_polar::template xo0<   2>(s,     220+   2,                        220+   0,    2);
		API_polar::template xo0<   4>(s,     216+   4,                        216+   0,    4);
		API_polar::template xo0<   8>(s,     208+   8,                        208+   0,    8);
		API_polar::template xo0<  16>(s,     192+  16,                        192+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  192+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        232+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  232+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        236+   0,    4);
		API_polar::template xo <   4>(s,     232+   0,  232+   4,             232+   0,    4);
		API_polar::template xo0<   8>(s,     224+   8,                        224+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  224+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        240+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  240+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        244+   0,    4);
		API_polar::template xo <   4>(s,     240+   0,  240+   4,             240+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  240+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        248+   0,    8);
		API_polar::template xo <   8>(s,     240+   0,  240+   8,             240+   0,    8);
		API_polar::template xo <  16>(s,     224+   0,  224+  16,             224+   0,   16);
		API_polar::template xo <  32>(s,     192+   0,  192+  32,             192+   0,   32);
		API_polar::template xo <  64>(s,     128+   0,  128+  64,             128+   0,   64);
		API_polar::template xo < 128>(s,       0+   0,    0+ 128,               0+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,    0+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        316+   0,    4);
		API_polar::template xo0<   4>(s,     312+   4,                        312+   0,    4);
		API_polar::template xo0<   8>(s,     304+   8,                        304+   0,    8);
		API_polar::template xo0<  16>(s,     288+  16,                        288+   0,   16);
		API_polar::template xo0<  32>(s,     256+  32,                        256+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  256+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        336+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  336+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        344+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  344+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        348+   0,    4);
		API_polar::template xo <   4>(s,     344+   0,  344+   4,             344+   0,    4);
		API_polar::template xo <   8>(s,     336+   0,  336+   8,             336+   0,    8);
		API_polar::template xo0<  16>(s,     320+  16,                        320+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  320+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        352+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  352+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        360+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  360+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        364+   0,    4);
		API_polar::template xo <   4>(s,     360+   0,  360+   4,             360+   0,    4);
		API_polar::template xo <   8>(s,     352+   0,  352+   8,             352+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  352+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        370+   0,    2);
		API_polar::template xo0<   2>(s,     368+   2,                        368+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4,  368+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        372+   0,    4);
		API_polar::template xo <   4>(s,     368+   0,  368+   4,             368+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  368+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        376+   0,    8);
		API_polar::template xo <   8>(s,     368+   0,  368+   8,             368+   0,    8);
		API_polar::template xo <  16>(s,     352+   0,  352+  16,             352+   0,   16);
		API_polar::template xo <  32>(s,     320+   0,  320+  32,             320+   0,   32);
		API_polar::template xo <  64>(s,     256+   0,  256+  64,             256+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  256+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        384+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  384+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        400+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  400+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        410+   0,    2);
		API_polar::template xo0<   2>(s,     408+   2,                        408+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4,  408+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        412+   0,    4);
		API_polar::template xo <   4>(s,     408+   0,  408+   4,             408+   0,    4);
		API_polar::template xo <   8>(s,     400+   0,  400+   8,             400+   0,    8);
		API_polar::template xo <  16>(s,     384+   0,  384+  16,             384+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  384+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        422+   0,    2);
		API_polar::template xo0<   2>(s,     420+   2,                        420+   0,    2);
		API_polar::template xo0<   4>(s,     416+   4,                        416+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  416+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        424+   0,    8);
		API_polar::template xo <   8>(s,     416+   0,  416+   8,             416+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  416+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        432+   0,   16);
		API_polar::template xo <  16>(s,     416+   0,  416+  16,             416+   0,   16);
		API_polar::template xo <  32>(s,     384+   0,  384+  32,             384+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  384+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        448+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  448+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        452+   0,    4);
		API_polar::template xo <   4>(s,     448+   0,  448+   4,             448+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  448+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        456+   0,    8);
		API_polar::template xo <   8>(s,     448+   0,  448+   8,             448+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  448+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        464+   0,   16);
		API_polar::template xo <  16>(s,     448+   0,  448+  16,             448+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  448+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                        480+   0,   32);
		API_polar::template xo <  32>(s,     448+   0,  448+  32,             448+   0,   32);
		API_polar::template xo <  64>(s,     384+   0,  384+  64,             384+   0,   64);
		API_polar::template xo < 128>(s,     256+   0,  256+ 128,             256+   0,  128);
		API_polar::template xo < 256>(s,       0+   0,    0+ 256,               0+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512,    0+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        544+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  544+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        560+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  560+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        568+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  568+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        572+   0,    4);
		API_polar::template xo <   4>(s,     568+   0,  568+   4,             568+   0,    4);
		API_polar::template xo <   8>(s,     560+   0,  560+   8,             560+   0,    8);
		API_polar::template xo <  16>(s,     544+   0,  544+  16,             544+   0,   16);
		API_polar::template xo0<  32>(s,     512+  32,                        512+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  512+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        576+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  576+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        598+   0,    2);
		API_polar::template xo0<   2>(s,     596+   2,                        596+   0,    2);
		API_polar::template xo0<   4>(s,     592+   4,                        592+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  592+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        600+   0,    8);
		API_polar::template xo <   8>(s,     592+   0,  592+   8,             592+   0,    8);
		API_polar::template xo <  16>(s,     576+   0,  576+  16,             576+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  576+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        612+   0,    4);
		API_polar::template xo0<   4>(s,     608+   4,                        608+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  608+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        616+   0,    8);
		API_polar::template xo <   8>(s,     608+   0,  608+   8,             608+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  608+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        624+   0,   16);
		API_polar::template xo <  16>(s,     608+   0,  608+  16,             608+   0,   16);
		API_polar::template xo <  32>(s,     576+   0,  576+  32,             576+   0,   32);
		API_polar::template xo <  64>(s,     512+   0,  512+  64,             512+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  512+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        652+   0,    4);
		API_polar::template xo0<   4>(s,     648+   4,                        648+   0,    4);
		API_polar::template xo0<   8>(s,     640+   8,                        640+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  640+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        656+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  656+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        660+   0,    4);
		API_polar::template xo <   4>(s,     656+   0,  656+   4,             656+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  656+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        664+   0,    8);
		API_polar::template xo <   8>(s,     656+   0,  656+   8,             656+   0,    8);
		API_polar::template xo <  16>(s,     640+   0,  640+  16,             640+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  640+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        672+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  672+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        676+   0,    4);
		API_polar::template xo <   4>(s,     672+   0,  672+   4,             672+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  672+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        680+   0,    8);
		API_polar::template xo <   8>(s,     672+   0,  672+   8,             672+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  672+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                        688+   0,   16);
		API_polar::template xo <  16>(s,     672+   0,  672+  16,             672+   0,   16);
		API_polar::template xo <  32>(s,     640+   0,  640+  32,             640+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  640+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        704+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  704+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        708+   0,    4);
		API_polar::template xo <   4>(s,     704+   0,  704+   4,             704+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  704+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        712+   0,    8);
		API_polar::template xo <   8>(s,     704+   0,  704+   8,             704+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  704+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                        720+   0,   16);
		API_polar::template xo <  16>(s,     704+   0,  704+  16,             704+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  704+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                        736+   0,   32);
		API_polar::template xo <  32>(s,     704+   0,  704+  32,             704+   0,   32);
		API_polar::template xo <  64>(s,     640+   0,  640+  64,             640+   0,   64);
		API_polar::template xo < 128>(s,     512+   0,  512+ 128,             512+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,  512+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        768+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  768+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        776+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  776+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        780+   0,    4);
		API_polar::template xo <   4>(s,     776+   0,  776+   4,             776+   0,    4);
		API_polar::template xo <   8>(s,     768+   0,  768+   8,             768+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  768+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        784+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  784+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        788+   0,    4);
		API_polar::template xo <   4>(s,     784+   0,  784+   4,             784+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  784+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        792+   0,    8);
		API_polar::template xo <   8>(s,     784+   0,  784+   8,             784+   0,    8);
		API_polar::template xo <  16>(s,     768+   0,  768+  16,             768+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  768+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        802+   0,    2);
		API_polar::template xo0<   2>(s,     800+   2,                        800+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4,  800+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        804+   0,    4);
		API_polar::template xo <   4>(s,     800+   0,  800+   4,             800+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  800+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        808+   0,    8);
		API_polar::template xo <   8>(s,     800+   0,  800+   8,             800+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  800+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                        816+   0,   16);
		API_polar::template xo <  16>(s,     800+   0,  800+  16,             800+   0,   16);
		API_polar::template xo <  32>(s,     768+   0,  768+  32,             768+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  768+   0, 3840+ 128,   64);
		API_polar::template spc<  64>(s, l, 3968+   0,                        832+   0,   64);
		API_polar::template xo <  64>(s,     768+   0,  768+  64,             768+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  768+   0, 3584+ 256,  128);
		API_polar::template spc< 128>(s, l, 3840+   0,                        896+   0,  128);
		API_polar::template xo < 128>(s,     768+   0,  768+ 128,             768+   0,  128);
		API_polar::template xo < 256>(s,     512+   0,  512+ 256,             512+   0,  256);
		API_polar::template xo < 512>(s,       0+   0,    0+ 512,               0+   0,  512);
		API_polar::template g  <1024>(s, l,    0+   0,    0+1024,    0+   0,    0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template rep<  32>(s, l, 4032+   0,                       1024+   0,   32);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32, 1024+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1070+   0,    2);
		API_polar::template xo0<   2>(s,    1068+   2,                       1068+   0,    2);
		API_polar::template xo0<   4>(s,    1064+   4,                       1064+   0,    4);
		API_polar::template xo0<   8>(s,    1056+   8,                       1056+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1056+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1072+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1072+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1076+   0,    4);
		API_polar::template xo <   4>(s,    1072+   0, 1072+   4,            1072+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1072+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1080+   0,    8);
		API_polar::template xo <   8>(s,    1072+   0, 1072+   8,            1072+   0,    8);
		API_polar::template xo <  16>(s,    1056+   0, 1056+  16,            1056+   0,   16);
		API_polar::template xo <  32>(s,    1024+   0, 1024+  32,            1024+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1024+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1096+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1096+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1100+   0,    4);
		API_polar::template xo <   4>(s,    1096+   0, 1096+   4,            1096+   0,    4);
		API_polar::template xo0<   8>(s,    1088+   8,                       1088+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1088+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1104+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1104+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1108+   0,    4);
		API_polar::template xo <   4>(s,    1104+   0, 1104+   4,            1104+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1104+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1112+   0,    8);
		API_polar::template xo <   8>(s,    1104+   0, 1104+   8,            1104+   0,    8);
		API_polar::template xo <  16>(s,    1088+   0, 1088+  16,            1088+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1088+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1120+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1120+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1124+   0,    4);
		API_polar::template xo <   4>(s,    1120+   0, 1120+   4,            1120+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1120+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1128+   0,    8);
		API_polar::template xo <   8>(s,    1120+   0, 1120+   8,            1120+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1120+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                       1136+   0,   16);
		API_polar::template xo <  16>(s,    1120+   0, 1120+  16,            1120+   0,   16);
		API_polar::template xo <  32>(s,    1088+   0, 1088+  32,            1088+   0,   32);
		API_polar::template xo <  64>(s,    1024+   0, 1024+  64,            1024+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1024+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1152+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1152+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1160+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1160+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1164+   0,    4);
		API_polar::template xo <   4>(s,    1160+   0, 1160+   4,            1160+   0,    4);
		API_polar::template xo <   8>(s,    1152+   0, 1152+   8,            1152+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1152+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1168+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1168+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1172+   0,    4);
		API_polar::template xo <   4>(s,    1168+   0, 1168+   4,            1168+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1168+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1176+   0,    8);
		API_polar::template xo <   8>(s,    1168+   0, 1168+   8,            1168+   0,    8);
		API_polar::template xo <  16>(s,    1152+   0, 1152+  16,            1152+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1152+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                       1184+   0,   32);
		API_polar::template xo <  32>(s,    1152+   0, 1152+  32,            1152+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1152+   0, 3840+ 128,   64);
		API_polar::template spc<  64>(s, l, 3968+   0,                       1216+   0,   64);
		API_polar::template xo <  64>(s,    1152+   0, 1152+  64,            1152+   0,   64);
		API_polar::template xo < 128>(s,    1024+   0, 1024+ 128,            1024+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1024+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1280+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1280+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1288+   0,    8);
		API_polar::template xo <   8>(s,    1280+   0, 1280+   8,            1280+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1280+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1296+   0,   16);
		API_polar::template xo <  16>(s,    1280+   0, 1280+  16,            1280+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1280+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                       1312+   0,   32);
		API_polar::template xo <  32>(s,    1280+   0, 1280+  32,            1280+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1280+   0, 3840+ 128,   64);
		API_polar::template spc<  64>(s, l, 3968+   0,                       1344+   0,   64);
		API_polar::template xo <  64>(s,    1280+   0, 1280+  64,            1280+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1280+   0, 3584+ 256,  128);
		API_polar::template spc< 128>(s, l, 3840+   0,                       1408+   0,  128);
		API_polar::template xo < 128>(s,    1280+   0, 1280+ 128,            1280+   0,  128);
		API_polar::template xo < 256>(s,    1024+   0, 1024+ 256,            1024+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512, 1024+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1536+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1536+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1540+   0,    4);
		API_polar::template xo <   4>(s,    1536+   0, 1536+   4,            1536+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1536+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1544+   0,    8);
		API_polar::template xo <   8>(s,    1536+   0, 1536+   8,            1536+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1536+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1552+   0,   16);
		API_polar::template xo <  16>(s,    1536+   0, 1536+  16,            1536+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1536+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                       1568+   0,   32);
		API_polar::template xo <  32>(s,    1536+   0, 1536+  32,            1536+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1536+   0, 3840+ 128,   64);
		API_polar::template h  <  64>(s, l, 3968+   0,                       1600+   0,   64);
		API_polar::template xo <  64>(s,    1536+   0, 1536+  64,            1536+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1536+   0, 3584+ 256,  128);
		API_polar::template h  < 128>(s, l, 3840+   0,                       1664+   0,  128);
		API_polar::template xo < 128>(s,    1536+   0, 1536+ 128,            1536+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1536+   0, 3072+ 512,  256);
		API_polar::template h  < 256>(s, l, 3584+   0,                       1792+   0,  256);
		API_polar::template xo < 256>(s,    1536+   0, 1536+ 256,            1536+   0,  256);
		API_polar::template xo < 512>(s,    1024+   0, 1024+ 512,            1024+   0,  512);
		API_polar::template xo <1024>(s,       0+   0,    0+1024,               0+   0, 1024);
	}
};
#endif
