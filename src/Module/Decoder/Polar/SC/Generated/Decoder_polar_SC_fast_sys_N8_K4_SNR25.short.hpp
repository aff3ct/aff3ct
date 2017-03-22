#ifndef DECODER_POLAR_SC_FAST_SYS_N8_K4_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N8_K4_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Frozen_bits_8_4_25[8] = {
1, 1, 1, 0, 1, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N8_K4_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N8_K4_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 8);
		assert(K == 4);
		
		auto i = 0;
		while (i < 8 && Frozen_bits_8_4_25[i] == frozen_bits[i]) i++;
		assert(i == 8);
	}

	virtual ~Decoder_polar_SC_fast_sys_N8_K4_SNR25()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <4>(   l,  0+ 0,  0+ 4,         0+ 8, 4);
		API_polar::template rep<4>(s, l,  8+ 0,                0+ 0, 4);
		API_polar::template gr <4>(s, l,  0+ 0,  0+ 4,  0+ 0,  0+ 8, 4);
		API_polar::template spc<4>(s, l,  8+ 0,                4+ 0, 4);
		API_polar::template xo <4>(s,     0+ 0,  0+ 4,         0+ 0, 4);
	}
};
}
}
#endif
