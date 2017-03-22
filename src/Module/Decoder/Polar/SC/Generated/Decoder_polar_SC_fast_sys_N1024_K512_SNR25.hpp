#ifndef DECODER_POLAR_SC_FAST_SYS_N1024_K512_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N1024_K512_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
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
		API_polar::template g0 < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template g0 <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template g0 <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        220+   0,   4);
		API_polar::template xo0<  4>(s,     216+   4,                        216+   0,   4);
		API_polar::template xo0<  8>(s,     208+   8,                        208+   0,   8);
		API_polar::template xo0< 16>(s,     192+  16,                        192+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  192+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template g0 <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        232+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  232+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        236+   0,   4);
		API_polar::template xo <  4>(s,     232+   0,  232+   4,             232+   0,   4);
		API_polar::template xo0<  8>(s,     224+   8,                        224+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  224+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        240+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  240+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        244+   0,   4);
		API_polar::template xo <  4>(s,     240+   0,  240+   4,             240+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  240+   0, 2016+  16,   8);
		API_polar::template spc<  8>(s, l, 2032+   0,                        248+   0,   8);
		API_polar::template xo <  8>(s,     240+   0,  240+   8,             240+   0,   8);
		API_polar::template xo < 16>(s,     224+   0,  224+  16,             224+   0,  16);
		API_polar::template xo < 32>(s,     192+   0,  192+  32,             192+   0,  32);
		API_polar::template xo < 64>(s,     128+   0,  128+  64,             128+   0,  64);
		API_polar::template xo <128>(s,       0+   0,    0+ 128,               0+   0, 128);
		API_polar::template g  <256>(s, l, 1024+   0, 1024+ 256,    0+   0, 1024+ 512, 256);
		API_polar::template f  <128>(   l, 1536+   0, 1536+ 128,            1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template g0 < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template g0 < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template g0 <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        312+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  312+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        316+   0,   4);
		API_polar::template xo <  4>(s,     312+   0,  312+   4,             312+   0,   4);
		API_polar::template xo0<  8>(s,     304+   8,                        304+   0,   8);
		API_polar::template xo0< 16>(s,     288+  16,                        288+   0,  16);
		API_polar::template xo0< 32>(s,     256+  32,                        256+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  256+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template g0 < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template rep<  8>(s, l, 2032+   0,                        336+   0,   8);
		API_polar::template gr <  8>(s, l, 2016+   0, 2016+   8,  336+   0, 2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        344+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  344+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        348+   0,   4);
		API_polar::template xo <  4>(s,     344+   0,  344+   4,             344+   0,   4);
		API_polar::template xo <  8>(s,     336+   0,  336+   8,             336+   0,   8);
		API_polar::template xo0< 16>(s,     320+  16,                        320+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  320+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template rep<  8>(s, l, 2032+   0,                        352+   0,   8);
		API_polar::template gr <  8>(s, l, 2016+   0, 2016+   8,  352+   0, 2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        360+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  360+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        364+   0,   4);
		API_polar::template xo <  4>(s,     360+   0,  360+   4,             360+   0,   4);
		API_polar::template xo <  8>(s,     352+   0,  352+   8,             352+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  352+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template g0 <  2>(   l, 2040+   0, 2040+   2,            2040+   4,   2);
		API_polar::template h  <  2>(s, l, 2044+   0,                        370+   0,   2);
		API_polar::template xo0<  2>(s,     368+   2,                        368+   0,   2);
		API_polar::template g  <  4>(s, l, 2032+   0, 2032+   4,  368+   0, 2032+   8,   4);
		API_polar::template h  <  4>(s, l, 2040+   0,                        372+   0,   4);
		API_polar::template xo <  4>(s,     368+   0,  368+   4,             368+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  368+   0, 2016+  16,   8);
		API_polar::template h  <  8>(s, l, 2032+   0,                        376+   0,   8);
		API_polar::template xo <  8>(s,     368+   0,  368+   8,             368+   0,   8);
		API_polar::template xo < 16>(s,     352+   0,  352+  16,             352+   0,  16);
		API_polar::template xo < 32>(s,     320+   0,  320+  32,             320+   0,  32);
		API_polar::template xo < 64>(s,     256+   0,  256+  64,             256+   0,  64);
		API_polar::template g  <128>(s, l, 1536+   0, 1536+ 128,  256+   0, 1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template rep< 16>(s, l, 2016+   0,                        384+   0,  16);
		API_polar::template gr < 16>(s, l, 1984+   0, 1984+  16,  384+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template rep<  8>(s, l, 2032+   0,                        400+   0,   8);
		API_polar::template gr <  8>(s, l, 2016+   0, 2016+   8,  400+   0, 2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        408+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  408+   0, 2032+   8,   4);
		API_polar::template h  <  4>(s, l, 2040+   0,                        412+   0,   4);
		API_polar::template xo <  4>(s,     408+   0,  408+   4,             408+   0,   4);
		API_polar::template xo <  8>(s,     400+   0,  400+   8,             400+   0,   8);
		API_polar::template xo < 16>(s,     384+   0,  384+  16,             384+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  384+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template g0 <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template g0 <  2>(   l, 2040+   0, 2040+   2,            2040+   4,   2);
		API_polar::template h  <  2>(s, l, 2044+   0,                        422+   0,   2);
		API_polar::template xo0<  2>(s,     420+   2,                        420+   0,   2);
		API_polar::template xo0<  4>(s,     416+   4,                        416+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  416+   0, 2016+  16,   8);
		API_polar::template spc<  8>(s, l, 2032+   0,                        424+   0,   8);
		API_polar::template xo <  8>(s,     416+   0,  416+   8,             416+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  416+   0, 1984+  32,  16);
		API_polar::template spc< 16>(s, l, 2016+   0,                        432+   0,  16);
		API_polar::template xo < 16>(s,     416+   0,  416+  16,             416+   0,  16);
		API_polar::template xo < 32>(s,     384+   0,  384+  32,             384+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  384+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        448+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  448+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        452+   0,   4);
		API_polar::template xo <  4>(s,     448+   0,  448+   4,             448+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  448+   0, 2016+  16,   8);
		API_polar::template spc<  8>(s, l, 2032+   0,                        456+   0,   8);
		API_polar::template xo <  8>(s,     448+   0,  448+   8,             448+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  448+   0, 1984+  32,  16);
		API_polar::template spc< 16>(s, l, 2016+   0,                        464+   0,  16);
		API_polar::template xo < 16>(s,     448+   0,  448+  16,             448+   0,  16);
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
		API_polar::template g0 < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template rep< 16>(s, l, 2016+   0,                        544+   0,  16);
		API_polar::template gr < 16>(s, l, 1984+   0, 1984+  16,  544+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template rep<  8>(s, l, 2032+   0,                        560+   0,   8);
		API_polar::template gr <  8>(s, l, 2016+   0, 2016+   8,  560+   0, 2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        568+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  568+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        572+   0,   4);
		API_polar::template xo <  4>(s,     568+   0,  568+   4,             568+   0,   4);
		API_polar::template xo <  8>(s,     560+   0,  560+   8,             560+   0,   8);
		API_polar::template xo < 16>(s,     544+   0,  544+  16,             544+   0,  16);
		API_polar::template xo0< 32>(s,     512+  32,                        512+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  512+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template rep< 16>(s, l, 2016+   0,                        576+   0,  16);
		API_polar::template gr < 16>(s, l, 1984+   0, 1984+  16,  576+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template g0 <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template g0 <  2>(   l, 2040+   0, 2040+   2,            2040+   4,   2);
		API_polar::template h  <  2>(s, l, 2044+   0,                        598+   0,   2);
		API_polar::template xo0<  2>(s,     596+   2,                        596+   0,   2);
		API_polar::template xo0<  4>(s,     592+   4,                        592+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  592+   0, 2016+  16,   8);
		API_polar::template spc<  8>(s, l, 2032+   0,                        600+   0,   8);
		API_polar::template xo <  8>(s,     592+   0,  592+   8,             592+   0,   8);
		API_polar::template xo < 16>(s,     576+   0,  576+  16,             576+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  576+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        608+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  608+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        612+   0,   4);
		API_polar::template xo <  4>(s,     608+   0,  608+   4,             608+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  608+   0, 2016+  16,   8);
		API_polar::template spc<  8>(s, l, 2032+   0,                        616+   0,   8);
		API_polar::template xo <  8>(s,     608+   0,  608+   8,             608+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  608+   0, 1984+  32,  16);
		API_polar::template spc< 16>(s, l, 2016+   0,                        624+   0,  16);
		API_polar::template xo < 16>(s,     608+   0,  608+  16,             608+   0,  16);
		API_polar::template xo < 32>(s,     576+   0,  576+  32,             576+   0,  32);
		API_polar::template xo < 64>(s,     512+   0,  512+  64,             512+   0,  64);
		API_polar::template g  <128>(s, l, 1536+   0, 1536+ 128,  512+   0, 1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template g0 <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template g0 <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        652+   0,   4);
		API_polar::template xo0<  4>(s,     648+   4,                        648+   0,   4);
		API_polar::template xo0<  8>(s,     640+   8,                        640+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  640+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        656+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  656+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        660+   0,   4);
		API_polar::template xo <  4>(s,     656+   0,  656+   4,             656+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  656+   0, 2016+  16,   8);
		API_polar::template spc<  8>(s, l, 2032+   0,                        664+   0,   8);
		API_polar::template xo <  8>(s,     656+   0,  656+   8,             656+   0,   8);
		API_polar::template xo < 16>(s,     640+   0,  640+  16,             640+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  640+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        672+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  672+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        676+   0,   4);
		API_polar::template xo <  4>(s,     672+   0,  672+   4,             672+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  672+   0, 2016+  16,   8);
		API_polar::template spc<  8>(s, l, 2032+   0,                        680+   0,   8);
		API_polar::template xo <  8>(s,     672+   0,  672+   8,             672+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  672+   0, 1984+  32,  16);
		API_polar::template spc< 16>(s, l, 2016+   0,                        688+   0,  16);
		API_polar::template xo < 16>(s,     672+   0,  672+  16,             672+   0,  16);
		API_polar::template xo < 32>(s,     640+   0,  640+  32,             640+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  640+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        704+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  704+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        708+   0,   4);
		API_polar::template xo <  4>(s,     704+   0,  704+   4,             704+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  704+   0, 2016+  16,   8);
		API_polar::template h  <  8>(s, l, 2032+   0,                        712+   0,   8);
		API_polar::template xo <  8>(s,     704+   0,  704+   8,             704+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  704+   0, 1984+  32,  16);
		API_polar::template h  < 16>(s, l, 2016+   0,                        720+   0,  16);
		API_polar::template xo < 16>(s,     704+   0,  704+  16,             704+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  704+   0, 1920+  64,  32);
		API_polar::template h  < 32>(s, l, 1984+   0,                        736+   0,  32);
		API_polar::template xo < 32>(s,     704+   0,  704+  32,             704+   0,  32);
		API_polar::template xo < 64>(s,     640+   0,  640+  64,             640+   0,  64);
		API_polar::template xo <128>(s,     512+   0,  512+ 128,             512+   0, 128);
		API_polar::template g  <256>(s, l, 1024+   0, 1024+ 256,  512+   0, 1024+ 512, 256);
		API_polar::template f  <128>(   l, 1536+   0, 1536+ 128,            1536+ 256, 128);
		API_polar::template f  < 64>(   l, 1792+   0, 1792+  64,            1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template rep<  8>(s, l, 2032+   0,                        768+   0,   8);
		API_polar::template gr <  8>(s, l, 2016+   0, 2016+   8,  768+   0, 2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        776+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  776+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        780+   0,   4);
		API_polar::template xo <  4>(s,     776+   0,  776+   4,             776+   0,   4);
		API_polar::template xo <  8>(s,     768+   0,  768+   8,             768+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  768+   0, 1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        784+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  784+   0, 2032+   8,   4);
		API_polar::template spc<  4>(s, l, 2040+   0,                        788+   0,   4);
		API_polar::template xo <  4>(s,     784+   0,  784+   4,             784+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  784+   0, 2016+  16,   8);
		API_polar::template spc<  8>(s, l, 2032+   0,                        792+   0,   8);
		API_polar::template xo <  8>(s,     784+   0,  784+   8,             784+   0,   8);
		API_polar::template xo < 16>(s,     768+   0,  768+  16,             768+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  768+   0, 1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template rep<  4>(s, l, 2040+   0,                        800+   0,   4);
		API_polar::template gr <  4>(s, l, 2032+   0, 2032+   4,  800+   0, 2032+   8,   4);
		API_polar::template h  <  4>(s, l, 2040+   0,                        804+   0,   4);
		API_polar::template xo <  4>(s,     800+   0,  800+   4,             800+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  800+   0, 2016+  16,   8);
		API_polar::template h  <  8>(s, l, 2032+   0,                        808+   0,   8);
		API_polar::template xo <  8>(s,     800+   0,  800+   8,             800+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  800+   0, 1984+  32,  16);
		API_polar::template h  < 16>(s, l, 2016+   0,                        816+   0,  16);
		API_polar::template xo < 16>(s,     800+   0,  800+  16,             800+   0,  16);
		API_polar::template xo < 32>(s,     768+   0,  768+  32,             768+   0,  32);
		API_polar::template g  < 64>(s, l, 1792+   0, 1792+  64,  768+   0, 1792+ 128,  64);
		API_polar::template f  < 32>(   l, 1920+   0, 1920+  32,            1920+  64,  32);
		API_polar::template f  < 16>(   l, 1984+   0, 1984+  16,            1984+  32,  16);
		API_polar::template f  <  8>(   l, 2016+   0, 2016+   8,            2016+  16,   8);
		API_polar::template f  <  4>(   l, 2032+   0, 2032+   4,            2032+   8,   4);
		API_polar::template g0 <  2>(   l, 2040+   0, 2040+   2,            2040+   4,   2);
		API_polar::template h  <  2>(s, l, 2044+   0,                        834+   0,   2);
		API_polar::template xo0<  2>(s,     832+   2,                        832+   0,   2);
		API_polar::template g  <  4>(s, l, 2032+   0, 2032+   4,  832+   0, 2032+   8,   4);
		API_polar::template h  <  4>(s, l, 2040+   0,                        836+   0,   4);
		API_polar::template xo <  4>(s,     832+   0,  832+   4,             832+   0,   4);
		API_polar::template g  <  8>(s, l, 2016+   0, 2016+   8,  832+   0, 2016+  16,   8);
		API_polar::template h  <  8>(s, l, 2032+   0,                        840+   0,   8);
		API_polar::template xo <  8>(s,     832+   0,  832+   8,             832+   0,   8);
		API_polar::template g  < 16>(s, l, 1984+   0, 1984+  16,  832+   0, 1984+  32,  16);
		API_polar::template h  < 16>(s, l, 2016+   0,                        848+   0,  16);
		API_polar::template xo < 16>(s,     832+   0,  832+  16,             832+   0,  16);
		API_polar::template g  < 32>(s, l, 1920+   0, 1920+  32,  832+   0, 1920+  64,  32);
		API_polar::template h  < 32>(s, l, 1984+   0,                        864+   0,  32);
		API_polar::template xo < 32>(s,     832+   0,  832+  32,             832+   0,  32);
		API_polar::template xo < 64>(s,     768+   0,  768+  64,             768+   0,  64);
		API_polar::template g  <128>(s, l, 1536+   0, 1536+ 128,  768+   0, 1536+ 256, 128);
		API_polar::template spc<128>(s, l, 1792+   0,                        896+   0, 128);
		API_polar::template xo <128>(s,     768+   0,  768+ 128,             768+   0, 128);
		API_polar::template xo <256>(s,     512+   0,  512+ 256,             512+   0, 256);
		API_polar::template xo <512>(s,       0+   0,    0+ 512,               0+   0, 512);
	}
};
}
}
#endif
