#ifndef DECODER_POLAR_SC_FAST_SYS_N2048_K1638_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N2048_K1638_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_2048_1638_25[2048] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
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
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
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
class Decoder_polar_SC_fast_sys_N2048_K1638_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N2048_K1638_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits)
	{
		assert(N == 2048);
		assert(K == 1638);
		
		auto i = 0;
		while (i < 2048 && Frozen_bits_2048_1638_25[i] == frozen_bits[i]) i++;
		assert(i == 2048);
	}

	virtual ~Decoder_polar_SC_fast_sys_N2048_K1638_SNR25()
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
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        112+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  112+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        120+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  120+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        124+   0,    4);
		API_polar::template xo <   4>(s,     120+   0,  120+   4,             120+   0,    4);
		API_polar::template xo <   8>(s,     112+   0,  112+   8,             112+   0,    8);
		API_polar::template xo0<  16>(s,      96+  16,                         96+   0,   16);
		API_polar::template xo0<  32>(s,      64+  32,                         64+   0,   32);
		API_polar::template xo0<  64>(s,       0+  64,                          0+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,    0+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        160+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  160+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        176+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  176+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        184+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  184+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        188+   0,    4);
		API_polar::template xo <   4>(s,     184+   0,  184+   4,             184+   0,    4);
		API_polar::template xo <   8>(s,     176+   0,  176+   8,             176+   0,    8);
		API_polar::template xo <  16>(s,     160+   0,  160+  16,             160+   0,   16);
		API_polar::template xo0<  32>(s,     128+  32,                        128+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  128+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        192+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  192+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        214+   0,    2);
		API_polar::template xo0<   2>(s,     212+   2,                        212+   0,    2);
		API_polar::template xo0<   4>(s,     208+   4,                        208+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  208+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        216+   0,    8);
		API_polar::template xo <   8>(s,     208+   0,  208+   8,             208+   0,    8);
		API_polar::template xo <  16>(s,     192+   0,  192+  16,             192+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  192+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        224+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  224+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        228+   0,    4);
		API_polar::template xo <   4>(s,     224+   0,  224+   4,             224+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  224+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        232+   0,    8);
		API_polar::template xo <   8>(s,     224+   0,  224+   8,             224+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  224+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        240+   0,   16);
		API_polar::template xo <  16>(s,     224+   0,  224+  16,             224+   0,   16);
		API_polar::template xo <  32>(s,     192+   0,  192+  32,             192+   0,   32);
		API_polar::template xo <  64>(s,     128+   0,  128+  64,             128+   0,   64);
		API_polar::template xo < 128>(s,       0+   0,    0+ 128,               0+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,    0+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template rep<  32>(s, l, 4032+   0,                        256+   0,   32);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32,  256+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        302+   0,    2);
		API_polar::template xo0<   2>(s,     300+   2,                        300+   0,    2);
		API_polar::template xo0<   4>(s,     296+   4,                        296+   0,    4);
		API_polar::template xo0<   8>(s,     288+   8,                        288+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  288+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        308+   0,    4);
		API_polar::template xo0<   4>(s,     304+   4,                        304+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  304+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        312+   0,    8);
		API_polar::template xo <   8>(s,     304+   0,  304+   8,             304+   0,    8);
		API_polar::template xo <  16>(s,     288+   0,  288+  16,             288+   0,   16);
		API_polar::template xo <  32>(s,     256+   0,  256+  32,             256+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  256+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        328+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  328+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        332+   0,    4);
		API_polar::template xo <   4>(s,     328+   0,  328+   4,             328+   0,    4);
		API_polar::template xo0<   8>(s,     320+   8,                        320+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  320+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        336+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  336+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        340+   0,    4);
		API_polar::template xo <   4>(s,     336+   0,  336+   4,             336+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  336+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        344+   0,    8);
		API_polar::template xo <   8>(s,     336+   0,  336+   8,             336+   0,    8);
		API_polar::template xo <  16>(s,     320+   0,  320+  16,             320+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  320+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        352+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  352+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        356+   0,    4);
		API_polar::template xo <   4>(s,     352+   0,  352+   4,             352+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  352+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        360+   0,    8);
		API_polar::template xo <   8>(s,     352+   0,  352+   8,             352+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  352+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                        368+   0,   16);
		API_polar::template xo <  16>(s,     352+   0,  352+  16,             352+   0,   16);
		API_polar::template xo <  32>(s,     320+   0,  320+  32,             320+   0,   32);
		API_polar::template xo <  64>(s,     256+   0,  256+  64,             256+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  256+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        384+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  384+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        392+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  392+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        396+   0,    4);
		API_polar::template xo <   4>(s,     392+   0,  392+   4,             392+   0,    4);
		API_polar::template xo <   8>(s,     384+   0,  384+   8,             384+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  384+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        400+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  400+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        404+   0,    4);
		API_polar::template xo <   4>(s,     400+   0,  400+   4,             400+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  400+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        408+   0,    8);
		API_polar::template xo <   8>(s,     400+   0,  400+   8,             400+   0,    8);
		API_polar::template xo <  16>(s,     384+   0,  384+  16,             384+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  384+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                        416+   0,   32);
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
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        536+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  536+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        540+   0,    4);
		API_polar::template xo <   4>(s,     536+   0,  536+   4,             536+   0,    4);
		API_polar::template xo0<   8>(s,     528+   8,                        528+   0,    8);
		API_polar::template xo0<  16>(s,     512+  16,                        512+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  512+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        544+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  544+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        552+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  552+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        556+   0,    4);
		API_polar::template xo <   4>(s,     552+   0,  552+   4,             552+   0,    4);
		API_polar::template xo <   8>(s,     544+   0,  544+   8,             544+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  544+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        560+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  560+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        564+   0,    4);
		API_polar::template xo <   4>(s,     560+   0,  560+   4,             560+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  560+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        568+   0,    8);
		API_polar::template xo <   8>(s,     560+   0,  560+   8,             560+   0,    8);
		API_polar::template xo <  16>(s,     544+   0,  544+  16,             544+   0,   16);
		API_polar::template xo <  32>(s,     512+   0,  512+  32,             512+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  512+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        576+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  576+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        584+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  584+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        588+   0,    4);
		API_polar::template xo <   4>(s,     584+   0,  584+   4,             584+   0,    4);
		API_polar::template xo <   8>(s,     576+   0,  576+   8,             576+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  576+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        592+   0,   16);
		API_polar::template xo <  16>(s,     576+   0,  576+  16,             576+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  576+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                        608+   0,   32);
		API_polar::template xo <  32>(s,     576+   0,  576+  32,             576+   0,   32);
		API_polar::template xo <  64>(s,     512+   0,  512+  64,             512+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  512+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        646+   0,    2);
		API_polar::template xo0<   2>(s,     644+   2,                        644+   0,    2);
		API_polar::template xo0<   4>(s,     640+   4,                        640+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  640+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        648+   0,    8);
		API_polar::template xo <   8>(s,     640+   0,  640+   8,             640+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  640+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        656+   0,   16);
		API_polar::template xo <  16>(s,     640+   0,  640+  16,             640+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  640+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                        672+   0,   32);
		API_polar::template xo <  32>(s,     640+   0,  640+  32,             640+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  640+   0, 3840+ 128,   64);
		API_polar::template h  <  64>(s, l, 3968+   0,                        704+   0,   64);
		API_polar::template xo <  64>(s,     640+   0,  640+  64,             640+   0,   64);
		API_polar::template xo < 128>(s,     512+   0,  512+ 128,             512+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,  512+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        768+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  768+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        772+   0,    4);
		API_polar::template xo <   4>(s,     768+   0,  768+   4,             768+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  768+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        776+   0,    8);
		API_polar::template xo <   8>(s,     768+   0,  768+   8,             768+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  768+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                        784+   0,   16);
		API_polar::template xo <  16>(s,     768+   0,  768+  16,             768+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  768+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                        800+   0,   32);
		API_polar::template xo <  32>(s,     768+   0,  768+  32,             768+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  768+   0, 3840+ 128,   64);
		API_polar::template h  <  64>(s, l, 3968+   0,                        832+   0,   64);
		API_polar::template xo <  64>(s,     768+   0,  768+  64,             768+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  768+   0, 3584+ 256,  128);
		API_polar::template h  < 128>(s, l, 3840+   0,                        896+   0,  128);
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
		API_polar::template rep<  16>(s, l, 4064+   0,                       1024+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16, 1024+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1040+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1040+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1048+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1048+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1052+   0,    4);
		API_polar::template xo <   4>(s,    1048+   0, 1048+   4,            1048+   0,    4);
		API_polar::template xo <   8>(s,    1040+   0, 1040+   8,            1040+   0,    8);
		API_polar::template xo <  16>(s,    1024+   0, 1024+  16,            1024+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1024+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1062+   0,    2);
		API_polar::template xo0<   2>(s,    1060+   2,                       1060+   0,    2);
		API_polar::template xo0<   4>(s,    1056+   4,                       1056+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1056+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1064+   0,    8);
		API_polar::template xo <   8>(s,    1056+   0, 1056+   8,            1056+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1056+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1072+   0,   16);
		API_polar::template xo <  16>(s,    1056+   0, 1056+  16,            1056+   0,   16);
		API_polar::template xo <  32>(s,    1024+   0, 1024+  32,            1024+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1024+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1088+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1088+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1092+   0,    4);
		API_polar::template xo <   4>(s,    1088+   0, 1088+   4,            1088+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1088+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1096+   0,    8);
		API_polar::template xo <   8>(s,    1088+   0, 1088+   8,            1088+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1088+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1104+   0,   16);
		API_polar::template xo <  16>(s,    1088+   0, 1088+  16,            1088+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1088+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                       1120+   0,   32);
		API_polar::template xo <  32>(s,    1088+   0, 1088+  32,            1088+   0,   32);
		API_polar::template xo <  64>(s,    1024+   0, 1024+  64,            1024+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1024+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1152+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1152+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1156+   0,    4);
		API_polar::template xo <   4>(s,    1152+   0, 1152+   4,            1152+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1152+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1160+   0,    8);
		API_polar::template xo <   8>(s,    1152+   0, 1152+   8,            1152+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1152+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                       1168+   0,   16);
		API_polar::template xo <  16>(s,    1152+   0, 1152+  16,            1152+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1152+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                       1184+   0,   32);
		API_polar::template xo <  32>(s,    1152+   0, 1152+  32,            1152+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1152+   0, 3840+ 128,   64);
		API_polar::template h  <  64>(s, l, 3968+   0,                       1216+   0,   64);
		API_polar::template xo <  64>(s,    1152+   0, 1152+  64,            1152+   0,   64);
		API_polar::template xo < 128>(s,    1024+   0, 1024+ 128,            1024+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1024+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1282+   0,    2);
		API_polar::template xo0<   2>(s,    1280+   2,                       1280+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4, 1280+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1284+   0,    4);
		API_polar::template xo <   4>(s,    1280+   0, 1280+   4,            1280+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1280+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1288+   0,    8);
		API_polar::template xo <   8>(s,    1280+   0, 1280+   8,            1280+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1280+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                       1296+   0,   16);
		API_polar::template xo <  16>(s,    1280+   0, 1280+  16,            1280+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1280+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                       1312+   0,   32);
		API_polar::template xo <  32>(s,    1280+   0, 1280+  32,            1280+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1280+   0, 3840+ 128,   64);
		API_polar::template h  <  64>(s, l, 3968+   0,                       1344+   0,   64);
		API_polar::template xo <  64>(s,    1280+   0, 1280+  64,            1280+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1280+   0, 3584+ 256,  128);
		API_polar::template h  < 128>(s, l, 3840+   0,                       1408+   0,  128);
		API_polar::template xo < 128>(s,    1280+   0, 1280+ 128,            1280+   0,  128);
		API_polar::template xo < 256>(s,    1024+   0, 1024+ 256,            1024+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512, 1024+   0, 2048+1024,  512);
		API_polar::template spc< 512>(s, l, 3072+   0,                       1536+   0,  512);
		API_polar::template xo < 512>(s,    1024+   0, 1024+ 512,            1024+   0,  512);
		API_polar::template xo <1024>(s,       0+   0,    0+1024,               0+   0, 1024);
	}
};
#endif
