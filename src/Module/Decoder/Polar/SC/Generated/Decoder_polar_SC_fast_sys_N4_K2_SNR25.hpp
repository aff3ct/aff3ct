#ifndef DECODER_POLAR_SC_FAST_SYS_N4_K2_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N4_K2_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Frozen_bits_4_2_25[4] = {
1, 1, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N4_K2_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N4_K2_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 4);
		assert(K == 2);
		
		auto i = 0;
		while (i < 4 && Frozen_bits_4_2_25[i] == frozen_bits[i]) i++;
		assert(i == 4);
	}

	virtual ~Decoder_polar_SC_fast_sys_N4_K2_SNR25()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template g0 <2>(   l, 0+0, 0+2,      0+4, 2);
		API_polar::template h  <2>(s, l, 4+0,           2+0, 2);
		API_polar::template xo0<2>(s,    0+2,           0+0, 2);
	}
};
}
}
#endif
