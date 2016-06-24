#ifndef DECODER_POLAR_SC_FAST_SYS_N2048_K1024_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N2048_K1024_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_2048_1024_25[2048] = {
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
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N2048_K1024_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N2048_K1024_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits)
	{
		assert(N == 2048);
		assert(K == 1024);
		
		auto i = 0;
		while (i < 2048 && Frozen_bits_2048_1024_25[i] == frozen_bits[i]) i++;
		assert(i == 2048);
	}

	virtual ~Decoder_polar_SC_fast_sys_N2048_K1024_SNR25()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <1024>(   l,    0+   0,    0+1024,               0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template rep< 256>(s, l, 3584+   0,                          0+   0,  256);
		API_polar::template gr < 256>(s, l, 3072+   0, 3072+ 256,    0+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        376+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  376+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        380+   0,    4);
		API_polar::template xo <   4>(s,     376+   0,  376+   4,             376+   0,    4);
		API_polar::template xo0<   8>(s,     368+   8,                        368+   0,    8);
		API_polar::template xo0<  16>(s,     352+  16,                        352+   0,   16);
		API_polar::template xo0<  32>(s,     320+  32,                        320+   0,   32);
		API_polar::template xo0<  64>(s,     256+  64,                        256+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  256+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        416+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  416+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        432+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  432+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        440+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  440+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        444+   0,    4);
		API_polar::template xo <   4>(s,     440+   0,  440+   4,             440+   0,    4);
		API_polar::template xo <   8>(s,     432+   0,  432+   8,             432+   0,    8);
		API_polar::template xo <  16>(s,     416+   0,  416+  16,             416+   0,   16);
		API_polar::template xo0<  32>(s,     384+  32,                        384+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  384+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        448+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  448+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        464+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  464+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        472+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  472+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        476+   0,    4);
		API_polar::template xo <   4>(s,     472+   0,  472+   4,             472+   0,    4);
		API_polar::template xo <   8>(s,     464+   0,  464+   8,             464+   0,    8);
		API_polar::template xo <  16>(s,     448+   0,  448+  16,             448+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  448+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        486+   0,    2);
		API_polar::template xo0<   2>(s,     484+   2,                        484+   0,    2);
		API_polar::template xo0<   4>(s,     480+   4,                        480+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  480+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        488+   0,    8);
		API_polar::template xo <   8>(s,     480+   0,  480+   8,             480+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  480+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        496+   0,   16);
		API_polar::template xo <  16>(s,     480+   0,  480+  16,             480+   0,   16);
		API_polar::template xo <  32>(s,     448+   0,  448+  32,             448+   0,   32);
		API_polar::template xo <  64>(s,     384+   0,  384+  64,             384+   0,   64);
		API_polar::template xo < 128>(s,     256+   0,  256+ 128,             256+   0,  128);
		API_polar::template xo < 256>(s,       0+   0,    0+ 256,               0+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512,    0+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        608+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  608+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        624+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  624+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        632+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  632+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        636+   0,    4);
		API_polar::template xo <   4>(s,     632+   0,  632+   4,             632+   0,    4);
		API_polar::template xo <   8>(s,     624+   0,  624+   8,             624+   0,    8);
		API_polar::template xo <  16>(s,     608+   0,  608+  16,             608+   0,   16);
		API_polar::template xo0<  32>(s,     576+  32,                        576+   0,   32);
		API_polar::template xo0<  64>(s,     512+  64,                        512+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  512+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template rep<  32>(s, l, 4032+   0,                        640+   0,   32);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32,  640+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        672+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  672+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        694+   0,    2);
		API_polar::template xo0<   2>(s,     692+   2,                        692+   0,    2);
		API_polar::template xo0<   4>(s,     688+   4,                        688+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  688+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        696+   0,    8);
		API_polar::template xo <   8>(s,     688+   0,  688+   8,             688+   0,    8);
		API_polar::template xo <  16>(s,     672+   0,  672+  16,             672+   0,   16);
		API_polar::template xo <  32>(s,     640+   0,  640+  32,             640+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  640+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        716+   0,    4);
		API_polar::template xo0<   4>(s,     712+   4,                        712+   0,    4);
		API_polar::template xo0<   8>(s,     704+   8,                        704+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  704+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        720+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  720+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        724+   0,    4);
		API_polar::template xo <   4>(s,     720+   0,  720+   4,             720+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  720+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        728+   0,    8);
		API_polar::template xo <   8>(s,     720+   0,  720+   8,             720+   0,    8);
		API_polar::template xo <  16>(s,     704+   0,  704+  16,             704+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  704+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        736+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  736+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        740+   0,    4);
		API_polar::template xo <   4>(s,     736+   0,  736+   4,             736+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  736+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        744+   0,    8);
		API_polar::template xo <   8>(s,     736+   0,  736+   8,             736+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  736+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        752+   0,   16);
		API_polar::template xo <  16>(s,     736+   0,  736+  16,             736+   0,   16);
		API_polar::template xo <  32>(s,     704+   0,  704+  32,             704+   0,   32);
		API_polar::template xo <  64>(s,     640+   0,  640+  64,             640+   0,   64);
		API_polar::template xo < 128>(s,     512+   0,  512+ 128,             512+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,  512+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        798+   0,    2);
		API_polar::template xo0<   2>(s,     796+   2,                        796+   0,    2);
		API_polar::template xo0<   4>(s,     792+   4,                        792+   0,    4);
		API_polar::template xo0<   8>(s,     784+   8,                        784+   0,    8);
		API_polar::template xo0<  16>(s,     768+  16,                        768+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  768+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        808+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  808+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        812+   0,    4);
		API_polar::template xo <   4>(s,     808+   0,  808+   4,             808+   0,    4);
		API_polar::template xo0<   8>(s,     800+   8,                        800+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  800+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        816+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  816+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        820+   0,    4);
		API_polar::template xo <   4>(s,     816+   0,  816+   4,             816+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  816+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        824+   0,    8);
		API_polar::template xo <   8>(s,     816+   0,  816+   8,             816+   0,    8);
		API_polar::template xo <  16>(s,     800+   0,  800+  16,             800+   0,   16);
		API_polar::template xo <  32>(s,     768+   0,  768+  32,             768+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  768+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        832+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  832+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        840+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  840+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        844+   0,    4);
		API_polar::template xo <   4>(s,     840+   0,  840+   4,             840+   0,    4);
		API_polar::template xo <   8>(s,     832+   0,  832+   8,             832+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  832+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        848+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  848+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        852+   0,    4);
		API_polar::template xo <   4>(s,     848+   0,  848+   4,             848+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  848+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        856+   0,    8);
		API_polar::template xo <   8>(s,     848+   0,  848+   8,             848+   0,    8);
		API_polar::template xo <  16>(s,     832+   0,  832+  16,             832+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  832+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        864+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  864+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        868+   0,    4);
		API_polar::template xo <   4>(s,     864+   0,  864+   4,             864+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  864+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        872+   0,    8);
		API_polar::template xo <   8>(s,     864+   0,  864+   8,             864+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  864+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                        880+   0,   16);
		API_polar::template xo <  16>(s,     864+   0,  864+  16,             864+   0,   16);
		API_polar::template xo <  32>(s,     832+   0,  832+  32,             832+   0,   32);
		API_polar::template xo <  64>(s,     768+   0,  768+  64,             768+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  768+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        896+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  896+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        904+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  904+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        908+   0,    4);
		API_polar::template xo <   4>(s,     904+   0,  904+   4,             904+   0,    4);
		API_polar::template xo <   8>(s,     896+   0,  896+   8,             896+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  896+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        914+   0,    2);
		API_polar::template xo0<   2>(s,     912+   2,                        912+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4,  912+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        916+   0,    4);
		API_polar::template xo <   4>(s,     912+   0,  912+   4,             912+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  912+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        920+   0,    8);
		API_polar::template xo <   8>(s,     912+   0,  912+   8,             912+   0,    8);
		API_polar::template xo <  16>(s,     896+   0,  896+  16,             896+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  896+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                        928+   0,   32);
		API_polar::template xo <  32>(s,     896+   0,  896+  32,             896+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  896+   0, 3840+ 128,   64);
		API_polar::template spc<  64>(s, l, 3968+   0,                        960+   0,   64);
		API_polar::template xo <  64>(s,     896+   0,  896+  64,             896+   0,   64);
		API_polar::template xo < 128>(s,     768+   0,  768+ 128,             768+   0,  128);
		API_polar::template xo < 256>(s,     512+   0,  512+ 256,             512+   0,  256);
		API_polar::template xo < 512>(s,       0+   0,    0+ 512,               0+   0,  512);
		API_polar::template g  <1024>(s, l,    0+   0,    0+1024,    0+   0,    0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template rep<  64>(s, l, 3968+   0,                       1024+   0,   64);
		API_polar::template gr <  64>(s, l, 3840+   0, 3840+  64, 1024+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template rep<  32>(s, l, 4032+   0,                       1088+   0,   32);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32, 1088+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1132+   0,    4);
		API_polar::template xo0<   4>(s,    1128+   4,                       1128+   0,    4);
		API_polar::template xo0<   8>(s,    1120+   8,                       1120+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1120+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1136+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1136+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1140+   0,    4);
		API_polar::template xo <   4>(s,    1136+   0, 1136+   4,            1136+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1136+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1144+   0,    8);
		API_polar::template xo <   8>(s,    1136+   0, 1136+   8,            1136+   0,    8);
		API_polar::template xo <  16>(s,    1120+   0, 1120+  16,            1120+   0,   16);
		API_polar::template xo <  32>(s,    1088+   0, 1088+  32,            1088+   0,   32);
		API_polar::template xo <  64>(s,    1024+   0, 1024+  64,            1024+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1024+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1176+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1176+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1180+   0,    4);
		API_polar::template xo <   4>(s,    1176+   0, 1176+   4,            1176+   0,    4);
		API_polar::template xo0<   8>(s,    1168+   8,                       1168+   0,    8);
		API_polar::template xo0<  16>(s,    1152+  16,                       1152+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1152+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1184+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1184+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1192+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1192+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1196+   0,    4);
		API_polar::template xo <   4>(s,    1192+   0, 1192+   4,            1192+   0,    4);
		API_polar::template xo <   8>(s,    1184+   0, 1184+   8,            1184+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1184+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1200+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1200+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1204+   0,    4);
		API_polar::template xo <   4>(s,    1200+   0, 1200+   4,            1200+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1200+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1208+   0,    8);
		API_polar::template xo <   8>(s,    1200+   0, 1200+   8,            1200+   0,    8);
		API_polar::template xo <  16>(s,    1184+   0, 1184+  16,            1184+   0,   16);
		API_polar::template xo <  32>(s,    1152+   0, 1152+  32,            1152+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1152+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1216+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1216+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1224+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1224+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1228+   0,    4);
		API_polar::template xo <   4>(s,    1224+   0, 1224+   4,            1224+   0,    4);
		API_polar::template xo <   8>(s,    1216+   0, 1216+   8,            1216+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1216+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1232+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1232+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1236+   0,    4);
		API_polar::template xo <   4>(s,    1232+   0, 1232+   4,            1232+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1232+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1240+   0,    8);
		API_polar::template xo <   8>(s,    1232+   0, 1232+   8,            1232+   0,    8);
		API_polar::template xo <  16>(s,    1216+   0, 1216+  16,            1216+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1216+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                       1248+   0,   32);
		API_polar::template xo <  32>(s,    1216+   0, 1216+  32,            1216+   0,   32);
		API_polar::template xo <  64>(s,    1152+   0, 1152+  64,            1152+   0,   64);
		API_polar::template xo < 128>(s,    1024+   0, 1024+ 128,            1024+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1024+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                       1280+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16, 1280+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1296+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1296+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1304+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1304+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1308+   0,    4);
		API_polar::template xo <   4>(s,    1304+   0, 1304+   4,            1304+   0,    4);
		API_polar::template xo <   8>(s,    1296+   0, 1296+   8,            1296+   0,    8);
		API_polar::template xo <  16>(s,    1280+   0, 1280+  16,            1280+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1280+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1312+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1312+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1320+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1320+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1324+   0,    4);
		API_polar::template xo <   4>(s,    1320+   0, 1320+   4,            1320+   0,    4);
		API_polar::template xo <   8>(s,    1312+   0, 1312+   8,            1312+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1312+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1330+   0,    2);
		API_polar::template xo0<   2>(s,    1328+   2,                       1328+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4, 1328+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1332+   0,    4);
		API_polar::template xo <   4>(s,    1328+   0, 1328+   4,            1328+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1328+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1336+   0,    8);
		API_polar::template xo <   8>(s,    1328+   0, 1328+   8,            1328+   0,    8);
		API_polar::template xo <  16>(s,    1312+   0, 1312+  16,            1312+   0,   16);
		API_polar::template xo <  32>(s,    1280+   0, 1280+  32,            1280+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1280+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1350+   0,    2);
		API_polar::template xo0<   2>(s,    1348+   2,                       1348+   0,    2);
		API_polar::template xo0<   4>(s,    1344+   4,                       1344+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1344+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1352+   0,    8);
		API_polar::template xo <   8>(s,    1344+   0, 1344+   8,            1344+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1344+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1360+   0,   16);
		API_polar::template xo <  16>(s,    1344+   0, 1344+  16,            1344+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1344+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                       1376+   0,   32);
		API_polar::template xo <  32>(s,    1344+   0, 1344+  32,            1344+   0,   32);
		API_polar::template xo <  64>(s,    1280+   0, 1280+  64,            1280+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1280+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1408+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1408+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1412+   0,    4);
		API_polar::template xo <   4>(s,    1408+   0, 1408+   4,            1408+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1408+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1416+   0,    8);
		API_polar::template xo <   8>(s,    1408+   0, 1408+   8,            1408+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1408+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1424+   0,   16);
		API_polar::template xo <  16>(s,    1408+   0, 1408+  16,            1408+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1408+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                       1440+   0,   32);
		API_polar::template xo <  32>(s,    1408+   0, 1408+  32,            1408+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1408+   0, 3840+ 128,   64);
		API_polar::template spc<  64>(s, l, 3968+   0,                       1472+   0,   64);
		API_polar::template xo <  64>(s,    1408+   0, 1408+  64,            1408+   0,   64);
		API_polar::template xo < 128>(s,    1280+   0, 1280+ 128,            1280+   0,  128);
		API_polar::template xo < 256>(s,    1024+   0, 1024+ 256,            1024+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512, 1024+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                       1536+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16, 1536+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1552+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1552+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1560+   0,    8);
		API_polar::template xo <   8>(s,    1552+   0, 1552+   8,            1552+   0,    8);
		API_polar::template xo <  16>(s,    1536+   0, 1536+  16,            1536+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1536+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1572+   0,    4);
		API_polar::template xo0<   4>(s,    1568+   4,                       1568+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1568+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1576+   0,    8);
		API_polar::template xo <   8>(s,    1568+   0, 1568+   8,            1568+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1568+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1584+   0,   16);
		API_polar::template xo <  16>(s,    1568+   0, 1568+  16,            1568+   0,   16);
		API_polar::template xo <  32>(s,    1536+   0, 1536+  32,            1536+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1536+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1600+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1600+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1604+   0,    4);
		API_polar::template xo <   4>(s,    1600+   0, 1600+   4,            1600+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1600+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1608+   0,    8);
		API_polar::template xo <   8>(s,    1600+   0, 1600+   8,            1600+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1600+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1616+   0,   16);
		API_polar::template xo <  16>(s,    1600+   0, 1600+  16,            1600+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1600+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                       1632+   0,   32);
		API_polar::template xo <  32>(s,    1600+   0, 1600+  32,            1600+   0,   32);
		API_polar::template xo <  64>(s,    1536+   0, 1536+  64,            1536+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1536+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1664+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1664+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1668+   0,    4);
		API_polar::template xo <   4>(s,    1664+   0, 1664+   4,            1664+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1664+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1672+   0,    8);
		API_polar::template xo <   8>(s,    1664+   0, 1664+   8,            1664+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1664+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1680+   0,   16);
		API_polar::template xo <  16>(s,    1664+   0, 1664+  16,            1664+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1664+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                       1696+   0,   32);
		API_polar::template xo <  32>(s,    1664+   0, 1664+  32,            1664+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1664+   0, 3840+ 128,   64);
		API_polar::template h  <  64>(s, l, 3968+   0,                       1728+   0,   64);
		API_polar::template xo <  64>(s,    1664+   0, 1664+  64,            1664+   0,   64);
		API_polar::template xo < 128>(s,    1536+   0, 1536+ 128,            1536+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1536+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1792+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1792+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1796+   0,    4);
		API_polar::template xo <   4>(s,    1792+   0, 1792+   4,            1792+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1792+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1800+   0,    8);
		API_polar::template xo <   8>(s,    1792+   0, 1792+   8,            1792+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1792+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                       1808+   0,   16);
		API_polar::template xo <  16>(s,    1792+   0, 1792+  16,            1792+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1792+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                       1824+   0,   32);
		API_polar::template xo <  32>(s,    1792+   0, 1792+  32,            1792+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1792+   0, 3840+ 128,   64);
		API_polar::template h  <  64>(s, l, 3968+   0,                       1856+   0,   64);
		API_polar::template xo <  64>(s,    1792+   0, 1792+  64,            1792+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1792+   0, 3584+ 256,  128);
		API_polar::template h  < 128>(s, l, 3840+   0,                       1920+   0,  128);
		API_polar::template xo < 128>(s,    1792+   0, 1792+ 128,            1792+   0,  128);
		API_polar::template xo < 256>(s,    1536+   0, 1536+ 256,            1536+   0,  256);
		API_polar::template xo < 512>(s,    1024+   0, 1024+ 512,            1024+   0,  512);
		API_polar::template xo <1024>(s,       0+   0,    0+1024,               0+   0, 1024);
	}
};
#endif
