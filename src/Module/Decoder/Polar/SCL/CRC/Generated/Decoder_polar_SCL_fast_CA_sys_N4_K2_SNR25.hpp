#ifndef DECODER_POLAR_SCL_FAST_SYS_CA_N4_K2_SNR25_HPP_
#define DECODER_POLAR_SCL_FAST_SYS_CA_N4_K2_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SCL_fast_CA_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Decoder_polar_SCL_fast_CA_sys_fb_4_2_25[4] = {
1, 1, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SCL_fast_CA_sys_N4_K2_SNR25 : public Decoder_polar_SCL_fast_CA_sys<B, R, API_polar>
{
public:
	Decoder_polar_SCL_fast_CA_sys_N4_K2_SNR25(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, CRC<B>& crc, const int n_frames = 1)
	: Decoder_polar_SCL_fast_CA_sys<B, R, API_polar>(K, N, L, frozen_bits, crc, n_frames)
	{
		assert(N == 4);
		assert(K == 2);
		
		auto i = 0;
		while (i < 4 && Decoder_polar_SCL_fast_CA_sys_fb_4_2_25[i] == frozen_bits[i]) i++;
		assert(i == 4);
	}

	virtual ~Decoder_polar_SCL_fast_CA_sys_N4_K2_SNR25()
	{
	}

	void _hard_decode(const R *Y_N, B *V_K, const int frame_id)
	{
		using namespace tools;

		auto t_decod = std::chrono::steady_clock::now();
		this->init_buffers();

		auto  y = Y_N;
		auto &l = this->l;
		auto &s = this->s;

		this->template update_paths_r0<1, 2>(0, 0);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path  = this->paths[i];
			const auto child = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template g0<2>(y, y + 2, child, 2);
		}
		this->template update_paths_r1<1, 2>(0, 2);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<2>(s[this->paths[i]], 0 + 2, 0, 2);
		}

		this->select_best_path();
		auto d_decod = std::chrono::steady_clock::now() - t_decod;

		auto t_store = std::chrono::steady_clock::now();
		this->_store(V_K);
		auto d_store = std::chrono::steady_clock::now() - t_store;

		this->d_decod_total += d_decod;
		this->d_store_total += d_store;
	}
};
}
}
#endif
