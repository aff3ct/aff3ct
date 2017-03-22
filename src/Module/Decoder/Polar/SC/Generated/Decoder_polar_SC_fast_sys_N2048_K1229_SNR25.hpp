#ifndef DECODER_POLAR_SC_FAST_SYS_N2048_K1229_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N2048_K1229_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Frozen_bits_2048_1229_25[2048] = {
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
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
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
class Decoder_polar_SC_fast_sys_N2048_K1229_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N2048_K1229_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 2048);
		assert(K == 1229);
		
		auto i = 0;
		while (i < 2048 && Frozen_bits_2048_1229_25[i] == frozen_bits[i]) i++;
		assert(i == 2048);
	}

	virtual ~Decoder_polar_SC_fast_sys_N2048_K1229_SNR25()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <1024>(   l,    0+   0,    0+1024,               0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template g0 < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        224+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  224+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        240+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  240+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        248+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  248+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        252+   0,    4);
		API_polar::template xo <   4>(s,     248+   0,  248+   4,             248+   0,    4);
		API_polar::template xo <   8>(s,     240+   0,  240+   8,             240+   0,    8);
		API_polar::template xo <  16>(s,     224+   0,  224+  16,             224+   0,   16);
		API_polar::template xo0<  32>(s,     192+  32,                        192+   0,   32);
		API_polar::template xo0<  64>(s,     128+  64,                        128+   0,   64);
		API_polar::template xo0< 128>(s,       0+ 128,                          0+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,    0+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template g0 <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template rep<  32>(s, l, 4032+   0,                        320+   0,   32);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32,  320+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        352+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  352+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        374+   0,    2);
		API_polar::template xo0<   2>(s,     372+   2,                        372+   0,    2);
		API_polar::template xo0<   4>(s,     368+   4,                        368+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  368+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        376+   0,    8);
		API_polar::template xo <   8>(s,     368+   0,  368+   8,             368+   0,    8);
		API_polar::template xo <  16>(s,     352+   0,  352+  16,             352+   0,   16);
		API_polar::template xo <  32>(s,     320+   0,  320+  32,             320+   0,   32);
		API_polar::template xo0<  64>(s,     256+  64,                        256+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  256+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template rep<  32>(s, l, 4032+   0,                        384+   0,   32);
		API_polar::template gr <  32>(s, l, 3968+   0, 3968+  32,  384+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        424+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  424+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        428+   0,    4);
		API_polar::template xo <   4>(s,     424+   0,  424+   4,             424+   0,    4);
		API_polar::template xo0<   8>(s,     416+   8,                        416+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  416+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        432+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  432+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        436+   0,    4);
		API_polar::template xo <   4>(s,     432+   0,  432+   4,             432+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  432+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        440+   0,    8);
		API_polar::template xo <   8>(s,     432+   0,  432+   8,             432+   0,    8);
		API_polar::template xo <  16>(s,     416+   0,  416+  16,             416+   0,   16);
		API_polar::template xo <  32>(s,     384+   0,  384+  32,             384+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  384+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        448+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  448+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        456+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  456+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        460+   0,    4);
		API_polar::template xo <   4>(s,     456+   0,  456+   4,             456+   0,    4);
		API_polar::template xo <   8>(s,     448+   0,  448+   8,             448+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  448+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        464+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  464+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        468+   0,    4);
		API_polar::template xo <   4>(s,     464+   0,  464+   4,             464+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  464+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        472+   0,    8);
		API_polar::template xo <   8>(s,     464+   0,  464+   8,             464+   0,    8);
		API_polar::template xo <  16>(s,     448+   0,  448+  16,             448+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  448+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        480+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  480+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        484+   0,    4);
		API_polar::template xo <   4>(s,     480+   0,  480+   4,             480+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  480+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        488+   0,    8);
		API_polar::template xo <   8>(s,     480+   0,  480+   8,             480+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  480+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                        496+   0,   16);
		API_polar::template xo <  16>(s,     480+   0,  480+  16,             480+   0,   16);
		API_polar::template xo <  32>(s,     448+   0,  448+  32,             448+   0,   32);
		API_polar::template xo <  64>(s,     384+   0,  384+  64,             384+   0,   64);
		API_polar::template xo < 128>(s,     256+   0,  256+ 128,             256+   0,  128);
		API_polar::template xo < 256>(s,       0+   0,    0+ 256,               0+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512,    0+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template rep<  64>(s, l, 3968+   0,                        512+   0,   64);
		API_polar::template gr <  64>(s, l, 3840+   0, 3840+  64,  512+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        604+   0,    4);
		API_polar::template xo0<   4>(s,     600+   4,                        600+   0,    4);
		API_polar::template xo0<   8>(s,     592+   8,                        592+   0,    8);
		API_polar::template xo0<  16>(s,     576+  16,                        576+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  576+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        608+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  608+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        616+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  616+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        620+   0,    4);
		API_polar::template xo <   4>(s,     616+   0,  616+   4,             616+   0,    4);
		API_polar::template xo <   8>(s,     608+   0,  608+   8,             608+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  608+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        624+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  624+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        628+   0,    4);
		API_polar::template xo <   4>(s,     624+   0,  624+   4,             624+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  624+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        632+   0,    8);
		API_polar::template xo <   8>(s,     624+   0,  624+   8,             624+   0,    8);
		API_polar::template xo <  16>(s,     608+   0,  608+  16,             608+   0,   16);
		API_polar::template xo <  32>(s,     576+   0,  576+  32,             576+   0,   32);
		API_polar::template xo <  64>(s,     512+   0,  512+  64,             512+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  512+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        656+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  656+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        664+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  664+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        668+   0,    4);
		API_polar::template xo <   4>(s,     664+   0,  664+   4,             664+   0,    4);
		API_polar::template xo <   8>(s,     656+   0,  656+   8,             656+   0,    8);
		API_polar::template xo0<  16>(s,     640+  16,                        640+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  640+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        672+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  672+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        680+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  680+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        684+   0,    4);
		API_polar::template xo <   4>(s,     680+   0,  680+   4,             680+   0,    4);
		API_polar::template xo <   8>(s,     672+   0,  672+   8,             672+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  672+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        690+   0,    2);
		API_polar::template xo0<   2>(s,     688+   2,                        688+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4,  688+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        692+   0,    4);
		API_polar::template xo <   4>(s,     688+   0,  688+   4,             688+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  688+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                        696+   0,    8);
		API_polar::template xo <   8>(s,     688+   0,  688+   8,             688+   0,    8);
		API_polar::template xo <  16>(s,     672+   0,  672+  16,             672+   0,   16);
		API_polar::template xo <  32>(s,     640+   0,  640+  32,             640+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  640+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        704+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  704+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        712+   0,    8);
		API_polar::template xo <   8>(s,     704+   0,  704+   8,             704+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  704+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        720+   0,   16);
		API_polar::template xo <  16>(s,     704+   0,  704+  16,             704+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  704+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                        736+   0,   32);
		API_polar::template xo <  32>(s,     704+   0,  704+  32,             704+   0,   32);
		API_polar::template xo <  64>(s,     640+   0,  640+  64,             640+   0,   64);
		API_polar::template xo < 128>(s,     512+   0,  512+ 128,             512+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256,  512+   0, 3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                        768+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16,  768+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                        784+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8,  784+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        792+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  792+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                        796+   0,    4);
		API_polar::template xo <   4>(s,     792+   0,  792+   4,             792+   0,    4);
		API_polar::template xo <   8>(s,     784+   0,  784+   8,             784+   0,    8);
		API_polar::template xo <  16>(s,     768+   0,  768+  16,             768+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  768+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                        806+   0,    2);
		API_polar::template xo0<   2>(s,     804+   2,                        804+   0,    2);
		API_polar::template xo0<   4>(s,     800+   4,                        800+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  800+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        808+   0,    8);
		API_polar::template xo <   8>(s,     800+   0,  800+   8,             800+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  800+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        816+   0,   16);
		API_polar::template xo <  16>(s,     800+   0,  800+  16,             800+   0,   16);
		API_polar::template xo <  32>(s,     768+   0,  768+  32,             768+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  768+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        832+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  832+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        836+   0,    4);
		API_polar::template xo <   4>(s,     832+   0,  832+   4,             832+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  832+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        840+   0,    8);
		API_polar::template xo <   8>(s,     832+   0,  832+   8,             832+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  832+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        848+   0,   16);
		API_polar::template xo <  16>(s,     832+   0,  832+  16,             832+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  832+   0, 3968+  64,   32);
		API_polar::template spc<  32>(s, l, 4032+   0,                        864+   0,   32);
		API_polar::template xo <  32>(s,     832+   0,  832+  32,             832+   0,   32);
		API_polar::template xo <  64>(s,     768+   0,  768+  64,             768+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128,  768+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                        896+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4,  896+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                        900+   0,    4);
		API_polar::template xo <   4>(s,     896+   0,  896+   4,             896+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8,  896+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                        904+   0,    8);
		API_polar::template xo <   8>(s,     896+   0,  896+   8,             896+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16,  896+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                        912+   0,   16);
		API_polar::template xo <  16>(s,     896+   0,  896+  16,             896+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32,  896+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                        928+   0,   32);
		API_polar::template xo <  32>(s,     896+   0,  896+  32,             896+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64,  896+   0, 3840+ 128,   64);
		API_polar::template h  <  64>(s, l, 3968+   0,                        960+   0,   64);
		API_polar::template xo <  64>(s,     896+   0,  896+  64,             896+   0,   64);
		API_polar::template xo < 128>(s,     768+   0,  768+ 128,             768+   0,  128);
		API_polar::template xo < 256>(s,     512+   0,  512+ 256,             512+   0,  256);
		API_polar::template xo < 512>(s,       0+   0,    0+ 512,               0+   0,  512);
		API_polar::template g  <1024>(s, l,    0+   0,    0+1024,    0+   0,    0+2048, 1024);
		API_polar::template f  < 512>(   l, 2048+   0, 2048+ 512,            2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template g0 <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template g0 <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1072+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1072+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1080+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1080+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1084+   0,    4);
		API_polar::template xo <   4>(s,    1080+   0, 1080+   4,            1080+   0,    4);
		API_polar::template xo <   8>(s,    1072+   0, 1072+   8,            1072+   0,    8);
		API_polar::template xo0<  16>(s,    1056+  16,                       1056+   0,   16);
		API_polar::template xo0<  32>(s,    1024+  32,                       1024+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1024+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                       1088+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16, 1088+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1104+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1104+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1112+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1112+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1116+   0,    4);
		API_polar::template xo <   4>(s,    1112+   0, 1112+   4,            1112+   0,    4);
		API_polar::template xo <   8>(s,    1104+   0, 1104+   8,            1104+   0,    8);
		API_polar::template xo <  16>(s,    1088+   0, 1088+  16,            1088+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1088+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1120+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1120+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1130+   0,    2);
		API_polar::template xo0<   2>(s,    1128+   2,                       1128+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4, 1128+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1132+   0,    4);
		API_polar::template xo <   4>(s,    1128+   0, 1128+   4,            1128+   0,    4);
		API_polar::template xo <   8>(s,    1120+   0, 1120+   8,            1120+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1120+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1136+   0,   16);
		API_polar::template xo <  16>(s,    1120+   0, 1120+  16,            1120+   0,   16);
		API_polar::template xo <  32>(s,    1088+   0, 1088+  32,            1088+   0,   32);
		API_polar::template xo <  64>(s,    1024+   0, 1024+  64,            1024+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1024+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template rep<  16>(s, l, 4064+   0,                       1152+   0,   16);
		API_polar::template gr <  16>(s, l, 4032+   0, 4032+  16, 1152+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1174+   0,    2);
		API_polar::template xo0<   2>(s,    1172+   2,                       1172+   0,    2);
		API_polar::template xo0<   4>(s,    1168+   4,                       1168+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1168+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1176+   0,    8);
		API_polar::template xo <   8>(s,    1168+   0, 1168+   8,            1168+   0,    8);
		API_polar::template xo <  16>(s,    1152+   0, 1152+  16,            1152+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1152+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1184+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1184+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1188+   0,    4);
		API_polar::template xo <   4>(s,    1184+   0, 1184+   4,            1184+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1184+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1192+   0,    8);
		API_polar::template xo <   8>(s,    1184+   0, 1184+   8,            1184+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1184+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1200+   0,   16);
		API_polar::template xo <  16>(s,    1184+   0, 1184+  16,            1184+   0,   16);
		API_polar::template xo <  32>(s,    1152+   0, 1152+  32,            1152+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1152+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1216+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1216+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1220+   0,    4);
		API_polar::template xo <   4>(s,    1216+   0, 1216+   4,            1216+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1216+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1224+   0,    8);
		API_polar::template xo <   8>(s,    1216+   0, 1216+   8,            1216+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1216+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1232+   0,   16);
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
		API_polar::template g0 <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template g0 <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1292+   0,    4);
		API_polar::template xo0<   4>(s,    1288+   4,                       1288+   0,    4);
		API_polar::template xo0<   8>(s,    1280+   8,                       1280+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1280+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1296+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1296+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1300+   0,    4);
		API_polar::template xo <   4>(s,    1296+   0, 1296+   4,            1296+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1296+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1304+   0,    8);
		API_polar::template xo <   8>(s,    1296+   0, 1296+   8,            1296+   0,    8);
		API_polar::template xo <  16>(s,    1280+   0, 1280+  16,            1280+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1280+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1312+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1312+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1316+   0,    4);
		API_polar::template xo <   4>(s,    1312+   0, 1312+   4,            1312+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1312+   0, 4064+  16,    8);
		API_polar::template spc<   8>(s, l, 4080+   0,                       1320+   0,    8);
		API_polar::template xo <   8>(s,    1312+   0, 1312+   8,            1312+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1312+   0, 4032+  32,   16);
		API_polar::template spc<  16>(s, l, 4064+   0,                       1328+   0,   16);
		API_polar::template xo <  16>(s,    1312+   0, 1312+  16,            1312+   0,   16);
		API_polar::template xo <  32>(s,    1280+   0, 1280+  32,            1280+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1280+   0, 3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1344+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1344+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1348+   0,    4);
		API_polar::template xo <   4>(s,    1344+   0, 1344+   4,            1344+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1344+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1352+   0,    8);
		API_polar::template xo <   8>(s,    1344+   0, 1344+   8,            1344+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1344+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                       1360+   0,   16);
		API_polar::template xo <  16>(s,    1344+   0, 1344+  16,            1344+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1344+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                       1376+   0,   32);
		API_polar::template xo <  32>(s,    1344+   0, 1344+  32,            1344+   0,   32);
		API_polar::template xo <  64>(s,    1280+   0, 1280+  64,            1280+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1280+   0, 3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template g0 <   2>(   l, 4088+   0, 4088+   2,            4088+   4,    2);
		API_polar::template h  <   2>(s, l, 4092+   0,                       1410+   0,    2);
		API_polar::template xo0<   2>(s,    1408+   2,                       1408+   0,    2);
		API_polar::template g  <   4>(s, l, 4080+   0, 4080+   4, 1408+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1412+   0,    4);
		API_polar::template xo <   4>(s,    1408+   0, 1408+   4,            1408+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1408+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1416+   0,    8);
		API_polar::template xo <   8>(s,    1408+   0, 1408+   8,            1408+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1408+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                       1424+   0,   16);
		API_polar::template xo <  16>(s,    1408+   0, 1408+  16,            1408+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1408+   0, 3968+  64,   32);
		API_polar::template h  <  32>(s, l, 4032+   0,                       1440+   0,   32);
		API_polar::template xo <  32>(s,    1408+   0, 1408+  32,            1408+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1408+   0, 3840+ 128,   64);
		API_polar::template h  <  64>(s, l, 3968+   0,                       1472+   0,   64);
		API_polar::template xo <  64>(s,    1408+   0, 1408+  64,            1408+   0,   64);
		API_polar::template xo < 128>(s,    1280+   0, 1280+ 128,            1280+   0,  128);
		API_polar::template xo < 256>(s,    1024+   0, 1024+ 256,            1024+   0,  256);
		API_polar::template g  < 512>(s, l, 2048+   0, 2048+ 512, 1024+   0, 2048+1024,  512);
		API_polar::template f  < 256>(   l, 3072+   0, 3072+ 256,            3072+ 512,  256);
		API_polar::template f  < 128>(   l, 3584+   0, 3584+ 128,            3584+ 256,  128);
		API_polar::template f  <  64>(   l, 3840+   0, 3840+  64,            3840+ 128,   64);
		API_polar::template f  <  32>(   l, 3968+   0, 3968+  32,            3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template rep<   8>(s, l, 4080+   0,                       1536+   0,    8);
		API_polar::template gr <   8>(s, l, 4064+   0, 4064+   8, 1536+   0, 4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1544+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1544+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1548+   0,    4);
		API_polar::template xo <   4>(s,    1544+   0, 1544+   4,            1544+   0,    4);
		API_polar::template xo <   8>(s,    1536+   0, 1536+   8,            1536+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1536+   0, 4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1552+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1552+   0, 4080+   8,    4);
		API_polar::template spc<   4>(s, l, 4088+   0,                       1556+   0,    4);
		API_polar::template xo <   4>(s,    1552+   0, 1552+   4,            1552+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1552+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1560+   0,    8);
		API_polar::template xo <   8>(s,    1552+   0, 1552+   8,            1552+   0,    8);
		API_polar::template xo <  16>(s,    1536+   0, 1536+  16,            1536+   0,   16);
		API_polar::template g  <  32>(s, l, 3968+   0, 3968+  32, 1536+   0, 3968+  64,   32);
		API_polar::template f  <  16>(   l, 4032+   0, 4032+  16,            4032+  32,   16);
		API_polar::template f  <   8>(   l, 4064+   0, 4064+   8,            4064+  16,    8);
		API_polar::template f  <   4>(   l, 4080+   0, 4080+   4,            4080+   8,    4);
		API_polar::template rep<   4>(s, l, 4088+   0,                       1568+   0,    4);
		API_polar::template gr <   4>(s, l, 4080+   0, 4080+   4, 1568+   0, 4080+   8,    4);
		API_polar::template h  <   4>(s, l, 4088+   0,                       1572+   0,    4);
		API_polar::template xo <   4>(s,    1568+   0, 1568+   4,            1568+   0,    4);
		API_polar::template g  <   8>(s, l, 4064+   0, 4064+   8, 1568+   0, 4064+  16,    8);
		API_polar::template h  <   8>(s, l, 4080+   0,                       1576+   0,    8);
		API_polar::template xo <   8>(s,    1568+   0, 1568+   8,            1568+   0,    8);
		API_polar::template g  <  16>(s, l, 4032+   0, 4032+  16, 1568+   0, 4032+  32,   16);
		API_polar::template h  <  16>(s, l, 4064+   0,                       1584+   0,   16);
		API_polar::template xo <  16>(s,    1568+   0, 1568+  16,            1568+   0,   16);
		API_polar::template xo <  32>(s,    1536+   0, 1536+  32,            1536+   0,   32);
		API_polar::template g  <  64>(s, l, 3840+   0, 3840+  64, 1536+   0, 3840+ 128,   64);
		API_polar::template spc<  64>(s, l, 3968+   0,                       1600+   0,   64);
		API_polar::template xo <  64>(s,    1536+   0, 1536+  64,            1536+   0,   64);
		API_polar::template g  < 128>(s, l, 3584+   0, 3584+ 128, 1536+   0, 3584+ 256,  128);
		API_polar::template spc< 128>(s, l, 3840+   0,                       1664+   0,  128);
		API_polar::template xo < 128>(s,    1536+   0, 1536+ 128,            1536+   0,  128);
		API_polar::template g  < 256>(s, l, 3072+   0, 3072+ 256, 1536+   0, 3072+ 512,  256);
		API_polar::template spc< 256>(s, l, 3584+   0,                       1792+   0,  256);
		API_polar::template xo < 256>(s,    1536+   0, 1536+ 256,            1536+   0,  256);
		API_polar::template xo < 512>(s,    1024+   0, 1024+ 512,            1024+   0,  512);
		API_polar::template xo <1024>(s,       0+   0,    0+1024,               0+   0, 1024);
	}
};
}
}
#endif
