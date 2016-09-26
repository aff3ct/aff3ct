#ifndef DECODER_POLAR_SC_FAST_SYS_N8_K7_SNR40_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N8_K7_SNR40_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

static const char Frozen_bits_8_7_40[8] = {
1, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N8_K7_SNR40 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N8_K7_SNR40(const int& K, const int& N, const mipp::vector<B>& frozen_bits)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits)
	{
		assert(N == 8);
		assert(K == 7);
		
		auto i = 0;
		while (i < 8 && Frozen_bits_8_7_40[i] == frozen_bits[i]) i++;
		assert(i == 8);
	}

	virtual ~Decoder_polar_SC_fast_sys_N8_K7_SNR40()
	{
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<8>(s, l,  0+ 0,                0+ 0, 8);
	}
};
#endif
