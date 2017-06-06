#ifndef DECODER_POLAR_SC_FAST_SYS_N8_K7_SNR40_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N8_K7_SNR40_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Decoder_polar_SC_fast_sys_fb_8_7_40[8] = {
1, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N8_K7_SNR40 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N8_K7_SNR40(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 8);
		assert(K == 7);
		
		auto i = 0;
		while (i < 8 && Decoder_polar_SC_fast_sys_fb_8_7_40[i] == frozen_bits[i]) i++;
		assert(i == 8);
	}

	virtual ~Decoder_polar_SC_fast_sys_N8_K7_SNR40()
	{
	}

	void _hard_decode(const R *Y_N, B *V_K, const int frame_id)
	{
		using namespace tools;

		auto t_load = std::chrono::steady_clock::now();
		this->_load(Y_N);
		auto d_load = std::chrono::steady_clock::now() - t_load;

		auto t_decod = std::chrono::steady_clock::now();
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<8>(s, l,  0+ 0,                0+ 0, 8);
		auto d_decod = std::chrono::steady_clock::now() - t_decod;

		auto t_store = std::chrono::steady_clock::now();
		this->_store(V_K);
		auto d_store = std::chrono::steady_clock::now() - t_store;

		this->d_load_total  += d_load;
		this->d_decod_total += d_decod;
		this->d_store_total += d_store;
	}
};
}
}
#endif
