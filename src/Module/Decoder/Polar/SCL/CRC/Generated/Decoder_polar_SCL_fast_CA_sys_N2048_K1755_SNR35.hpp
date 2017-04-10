#ifndef DECODER_POLAR_SCL_FAST_SYS_CA_N2048_K1755_SNR35_HPP_
#define DECODER_POLAR_SCL_FAST_SYS_CA_N2048_K1755_SNR35_HPP_

#include <cassert>

#include "../Decoder_polar_SCL_fast_CA_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Decoder_polar_SCL_fast_CA_sys_fb_2048_1755_35[2048] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
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
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
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
class Decoder_polar_SCL_fast_CA_sys_N2048_K1755_SNR35 : public Decoder_polar_SCL_fast_CA_sys<B, R, API_polar>
{
public:
	Decoder_polar_SCL_fast_CA_sys_N2048_K1755_SNR35(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, CRC<B>& crc, const int n_frames = 1)
	: Decoder_polar_SCL_fast_CA_sys<B, R, API_polar>(K, N, L, frozen_bits, crc, n_frames)
	{
		assert(N == 2048);
		assert(K == 1755);
		
		auto i = 0;
		while (i < 2048 && Decoder_polar_SCL_fast_CA_sys_fb_2048_1755_35[i] == frozen_bits[i]) i++;
		assert(i == 2048);
	}

	virtual ~Decoder_polar_SCL_fast_CA_sys_N2048_K1755_SNR35()
	{
	}

	void _hard_decode(const R *Y_N, B *V_K)
	{
		using namespace tools;

		auto t_decod = std::chrono::steady_clock::now();
		this->init_buffers();

		auto  y = Y_N;
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f<1024>(y, y + 1024, l[0].data(), 1024);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][10   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 10 -1)].data();
			API_polar::template f<512>(parent + 0, parent + 0 + 512, child + 0 + 1024, 512);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][9   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 9 -1)].data();
			API_polar::template f<256>(parent + 1024, parent + 1024 + 256, child + 1024 + 512, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template f<128>(parent + 1536, parent + 1536 + 128, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		this->template update_paths_rep<6, 64>(1920, 0);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template gr<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 0, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		this->template update_paths_rep<5, 32>(1984, 64);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template gr<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 64, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		this->template update_paths_r0<3, 8>(2032, 96);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g0<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_r0<2, 4>(2040, 104);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g0<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template f<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r0<1, 2>(2044, 108);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template g0<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r1<1, 2>(2044, 110);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<2>(s[this->paths[i]], 108 + 2, 108, 2);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<4>(s[this->paths[i]], 104 + 4, 104, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<8>(s[this->paths[i]], 96 + 8, 96, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 96, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_r0<2, 4>(2040, 112);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g0<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 116);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<4>(s[this->paths[i]], 112 + 4, 112, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 112, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 120);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 120, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 124);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 120, 120 + 4, 120, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 112, 112 + 8, 112, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 96, 96 + 16, 96, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 64, 64 + 32, 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 0, 0 + 64, 0, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template g<128>(parent + 1536, parent + 1536 + 128, s[path].data() + 0, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		this->template update_paths_r0<4, 16>(2016, 128);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g0<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		this->template update_paths_r0<3, 8>(2032, 144);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g0<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_r0<2, 4>(2040, 152);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g0<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 156);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<4>(s[this->paths[i]], 152 + 4, 152, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<8>(s[this->paths[i]], 144 + 8, 144, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<16>(s[this->paths[i]], 128 + 16, 128, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 128, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		this->template update_paths_r0<3, 8>(2032, 160);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g0<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 168);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 168, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 172);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 168, 168 + 4, 168, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<8>(s[this->paths[i]], 160 + 8, 160, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 160, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 176);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 176, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 180);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 176, 176 + 4, 176, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 176, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 184);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 184, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 188);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 184, 184 + 4, 184, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 176, 176 + 8, 176, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 160, 160 + 16, 160, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 128, 128 + 32, 128, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 128, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		this->template update_paths_rep<3, 8>(2032, 192);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template gr<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 192, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 200);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 200, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 204);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 200, 200 + 4, 200, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 192, 192 + 8, 192, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 192, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template f<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r0<1, 2>(2044, 208);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template g0<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r1<1, 2>(2044, 210);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<2>(s[this->paths[i]], 208 + 2, 208, 2);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 208, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 212);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 208, 208 + 4, 208, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 208, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 216);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 208, 208 + 8, 208, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 192, 192 + 16, 192, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 192, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 224);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 224, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 228);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 224, 224 + 4, 224, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 224, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 232);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 224, 224 + 8, 224, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 224, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 240);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 224, 224 + 16, 224, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 192, 192 + 32, 192, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 128, 128 + 64, 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<128>(s[this->paths[i]], 0, 0 + 128, 0, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][9   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 9 -1)].data();
			API_polar::template g<256>(parent + 1024, parent + 1024 + 256, s[path].data() + 0, child + 1024 + 512, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template f<128>(parent + 1536, parent + 1536 + 128, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		this->template update_paths_r0<4, 16>(2016, 256);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g0<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		this->template update_paths_rep<3, 8>(2032, 272);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template gr<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 272, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 280);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 280, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 284);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 280, 280 + 4, 280, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 272, 272 + 8, 272, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<16>(s[this->paths[i]], 256 + 16, 256, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 256, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		this->template update_paths_rep<3, 8>(2032, 288);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template gr<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 288, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 296);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 296, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 300);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 296, 296 + 4, 296, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 288, 288 + 8, 288, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 288, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 304);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 304, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 308);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 304, 304 + 4, 304, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 304, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 312);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 304, 304 + 8, 304, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 288, 288 + 16, 288, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 256, 256 + 32, 256, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 256, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_r0<2, 4>(2040, 320);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g0<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template f<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r0<1, 2>(2044, 324);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template g0<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r1<1, 2>(2044, 326);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<2>(s[this->paths[i]], 324 + 2, 324, 2);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<4>(s[this->paths[i]], 320 + 4, 320, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 320, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 328);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 328, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 332);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 328, 328 + 4, 328, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 320, 320 + 8, 320, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 320, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 336);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 336, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 340);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 336, 336 + 4, 336, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 336, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 344);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 336, 336 + 8, 336, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 320, 320 + 16, 320, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 320, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 352);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 352, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 356);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 352, 352 + 4, 352, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 352, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 360);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 352, 352 + 8, 352, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 352, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 368);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 352, 352 + 16, 352, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 320, 320 + 32, 320, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 256, 256 + 64, 256, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template g<128>(parent + 1536, parent + 1536 + 128, s[path].data() + 256, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 384);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 384, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 388);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 384, 384 + 4, 384, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 384, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 392);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 392, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 396);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 392, 392 + 4, 392, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 384, 384 + 8, 384, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 384, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 400);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 400, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 404);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 400, 400 + 4, 400, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 400, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 408);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 400, 400 + 8, 400, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 384, 384 + 16, 384, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 384, child + 1920 + 64, 32);
		}
		this->template update_paths_r1<5, 32>(1984, 416);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 384, 384 + 32, 384, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 384, child + 1792 + 128, 64);
		}
		this->template update_paths_r1<6, 64>(1920, 448);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 384, 384 + 64, 384, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<128>(s[this->paths[i]], 256, 256 + 128, 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<256>(s[this->paths[i]], 0, 0 + 256, 0, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][10   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 10 -1)].data();
			API_polar::template g<512>(parent + 0, parent + 0 + 512, s[path].data() + 0, child + 0 + 1024, 512);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][9   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 9 -1)].data();
			API_polar::template f<256>(parent + 1024, parent + 1024 + 256, child + 1024 + 512, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template f<128>(parent + 1536, parent + 1536 + 128, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		this->template update_paths_rep<4, 16>(2016, 512);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template gr<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 512, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_r0<2, 4>(2040, 528);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g0<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template f<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r0<1, 2>(2044, 532);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template g0<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r1<1, 2>(2044, 534);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<2>(s[this->paths[i]], 532 + 2, 532, 2);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<4>(s[this->paths[i]], 528 + 4, 528, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 528, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 536);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 536, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 540);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 536, 536 + 4, 536, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 528, 528 + 8, 528, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 512, 512 + 16, 512, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 512, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_r0<2, 4>(2040, 544);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g0<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 548);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<4>(s[this->paths[i]], 544 + 4, 544, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 544, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 552);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 552, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 556);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 552, 552 + 4, 552, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 544, 544 + 8, 544, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 544, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 560);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 560, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 564);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 560, 560 + 4, 560, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 560, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 568);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 560, 560 + 8, 560, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 544, 544 + 16, 544, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 512, 512 + 32, 512, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 512, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 576);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 576, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 580);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 576, 576 + 4, 576, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 576, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 584);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 584, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 588);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 584, 584 + 4, 584, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 576, 576 + 8, 576, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 576, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 592);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 576, 576 + 16, 576, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 576, child + 1920 + 64, 32);
		}
		this->template update_paths_r1<5, 32>(1984, 608);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 576, 576 + 32, 576, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 512, 512 + 64, 512, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template g<128>(parent + 1536, parent + 1536 + 128, s[path].data() + 512, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 640);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 640, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 644);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 640, 640 + 4, 640, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 640, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 648);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 640, 640 + 8, 640, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 640, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 656);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 640, 640 + 16, 640, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 640, child + 1920 + 64, 32);
		}
		this->template update_paths_r1<5, 32>(1984, 672);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 640, 640 + 32, 640, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 640, child + 1792 + 128, 64);
		}
		this->template update_paths_r1<6, 64>(1920, 704);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 640, 640 + 64, 640, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<128>(s[this->paths[i]], 512, 512 + 128, 512, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][9   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 9 -1)].data();
			API_polar::template g<256>(parent + 1024, parent + 1024 + 256, s[path].data() + 512, child + 1024 + 512, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template f<128>(parent + 1536, parent + 1536 + 128, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template f<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r0<1, 2>(2044, 768);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template g0<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r1<1, 2>(2044, 770);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<2>(s[this->paths[i]], 768 + 2, 768, 2);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 768, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 772);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 768, 768 + 4, 768, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 768, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 776);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 768, 768 + 8, 768, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 768, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 784);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 768, 768 + 16, 768, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 768, child + 1920 + 64, 32);
		}
		this->template update_paths_r1<5, 32>(1984, 800);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 768, 768 + 32, 768, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 768, child + 1792 + 128, 64);
		}
		this->template update_paths_r1<6, 64>(1920, 832);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 768, 768 + 64, 768, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template g<128>(parent + 1536, parent + 1536 + 128, s[path].data() + 768, child + 1536 + 256, 128);
		}
		this->template update_paths_r1<7, 128>(1792, 896);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<128>(s[this->paths[i]], 768, 768 + 128, 768, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<256>(s[this->paths[i]], 512, 512 + 256, 512, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<512>(s[this->paths[i]], 0, 0 + 512, 0, 512);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path  = this->paths[i];
			const auto child = l[this->up_ref_array_idx(path, 11 -1)].data();
			API_polar::template g<1024>(y, y + 1024, s[path].data() + 0, child, 1024);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][10   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 10 -1)].data();
			API_polar::template f<512>(parent + 0, parent + 0 + 512, child + 0 + 1024, 512);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][9   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 9 -1)].data();
			API_polar::template f<256>(parent + 1024, parent + 1024 + 256, child + 1024 + 512, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template f<128>(parent + 1536, parent + 1536 + 128, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		this->template update_paths_r0<3, 8>(2032, 1024);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g0<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 1032);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 1032, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 1036);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 1032, 1032 + 4, 1032, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<8>(s[this->paths[i]], 1024 + 8, 1024, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 1024, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 1040);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 1040, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 1044);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 1040, 1040 + 4, 1040, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 1040, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 1048);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 1048, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 1052);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 1048, 1048 + 4, 1048, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 1040, 1040 + 8, 1040, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 1024, 1024 + 16, 1024, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 1024, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(2040, 1056);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 1056, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 1060);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 1056, 1056 + 4, 1056, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 1056, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 1064);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 1056, 1056 + 8, 1056, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 1056, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 1072);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 1056, 1056 + 16, 1056, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 1024, 1024 + 32, 1024, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 1024, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template f<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r0<1, 2>(2044, 1088);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template g0<2>(parent + 2040, parent + 2040 + 2, child + 2040 + 4, 2);
		}
		this->template update_paths_r1<1, 2>(2044, 1090);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<2>(s[this->paths[i]], 1088 + 2, 1088, 2);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 1088, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 1092);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 1088, 1088 + 4, 1088, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 1088, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 1096);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 1088, 1088 + 8, 1088, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 1088, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 1104);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 1088, 1088 + 16, 1088, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 1088, child + 1920 + 64, 32);
		}
		this->template update_paths_r1<5, 32>(1984, 1120);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 1088, 1088 + 32, 1088, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 1024, 1024 + 64, 1024, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template g<128>(parent + 1536, parent + 1536 + 128, s[path].data() + 1024, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 1152);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 1152, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 1156);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 1152, 1152 + 4, 1152, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 1152, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 1160);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 1152, 1152 + 8, 1152, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 1152, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 1168);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 1152, 1152 + 16, 1152, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 1152, child + 1920 + 64, 32);
		}
		this->template update_paths_r1<5, 32>(1984, 1184);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 1152, 1152 + 32, 1152, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 1152, child + 1792 + 128, 64);
		}
		this->template update_paths_r1<6, 64>(1920, 1216);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 1152, 1152 + 64, 1152, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<128>(s[this->paths[i]], 1024, 1024 + 128, 1024, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][9   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 9 -1)].data();
			API_polar::template g<256>(parent + 1024, parent + 1024 + 256, s[path].data() + 1024, child + 1024 + 512, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template f<128>(parent + 1536, parent + 1536 + 128, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 1280);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 1280, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 1284);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 1280, 1280 + 4, 1280, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 1280, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 1288);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 1280, 1280 + 8, 1280, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 1280, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 1296);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 1280, 1280 + 16, 1280, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 1280, child + 1920 + 64, 32);
		}
		this->template update_paths_r1<5, 32>(1984, 1312);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 1280, 1280 + 32, 1280, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 1280, child + 1792 + 128, 64);
		}
		this->template update_paths_r1<6, 64>(1920, 1344);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 1280, 1280 + 64, 1280, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template g<128>(parent + 1536, parent + 1536 + 128, s[path].data() + 1280, child + 1536 + 256, 128);
		}
		this->template update_paths_r1<7, 128>(1792, 1408);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<128>(s[this->paths[i]], 1280, 1280 + 128, 1280, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<256>(s[this->paths[i]], 1024, 1024 + 256, 1024, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][10   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 10 -1)].data();
			API_polar::template g<512>(parent + 0, parent + 0 + 512, s[path].data() + 1024, child + 0 + 1024, 512);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][9   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 9 -1)].data();
			API_polar::template f<256>(parent + 1024, parent + 1024 + 256, child + 1024 + 512, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template f<128>(parent + 1536, parent + 1536 + 128, child + 1536 + 256, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 1792, parent + 1792 + 64, child + 1792 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 1920, parent + 1920 + 32, child + 1920 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 1984, parent + 1984 + 16, child + 1984 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 2016, parent + 2016 + 8, child + 2016 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 2032, parent + 2032 + 4, child + 2032 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(2040, 1536);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 2032, parent + 2032 + 4, s[path].data() + 1536, child + 2032 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(2040, 1540);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 1536, 1536 + 4, 1536, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 2016, parent + 2016 + 8, s[path].data() + 1536, child + 2016 + 16, 8);
		}
		this->template update_paths_r1<3, 8>(2032, 1544);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<8>(s[this->paths[i]], 1536, 1536 + 8, 1536, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 1984, parent + 1984 + 16, s[path].data() + 1536, child + 1984 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(2016, 1552);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<16>(s[this->paths[i]], 1536, 1536 + 16, 1536, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template g<32>(parent + 1920, parent + 1920 + 32, s[path].data() + 1536, child + 1920 + 64, 32);
		}
		this->template update_paths_r1<5, 32>(1984, 1568);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<32>(s[this->paths[i]], 1536, 1536 + 32, 1536, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g<64>(parent + 1792, parent + 1792 + 64, s[path].data() + 1536, child + 1792 + 128, 64);
		}
		this->template update_paths_r1<6, 64>(1920, 1600);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<64>(s[this->paths[i]], 1536, 1536 + 64, 1536, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][8   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template g<128>(parent + 1536, parent + 1536 + 128, s[path].data() + 1536, child + 1536 + 256, 128);
		}
		this->template update_paths_r1<7, 128>(1792, 1664);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<128>(s[this->paths[i]], 1536, 1536 + 128, 1536, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][9   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 9 -1)].data();
			API_polar::template g<256>(parent + 1024, parent + 1024 + 256, s[path].data() + 1536, child + 1024 + 512, 256);
		}
		this->template update_paths_r1<8, 256>(1536, 1792);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<256>(s[this->paths[i]], 1536, 1536 + 256, 1536, 256);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<512>(s[this->paths[i]], 1024, 1024 + 512, 1024, 512);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<1024>(s[this->paths[i]], 0, 0 + 1024, 0, 1024);
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
