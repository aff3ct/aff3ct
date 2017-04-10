#ifndef DECODER_POLAR_SCL_FAST_SYS_CA_N256_K64_SNR30_HPP_
#define DECODER_POLAR_SCL_FAST_SYS_CA_N256_K64_SNR30_HPP_

#include <cassert>

#include "../Decoder_polar_SCL_fast_CA_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Decoder_polar_SCL_fast_CA_sys_fb_256_64_30[256] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SCL_fast_CA_sys_N256_K64_SNR30 : public Decoder_polar_SCL_fast_CA_sys<B, R, API_polar>
{
public:
	Decoder_polar_SCL_fast_CA_sys_N256_K64_SNR30(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, CRC<B>& crc, const int n_frames = 1)
	: Decoder_polar_SCL_fast_CA_sys<B, R, API_polar>(K, N, L, frozen_bits, crc, n_frames)
	{
		assert(N == 256);
		assert(K == 64);
		
		auto i = 0;
		while (i < 256 && Decoder_polar_SCL_fast_CA_sys_fb_256_64_30[i] == frozen_bits[i]) i++;
		assert(i == 256);
	}

	virtual ~Decoder_polar_SCL_fast_CA_sys_N256_K64_SNR30()
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

		API_polar::template f<128>(y, y + 128, l[0].data(), 128);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 0, parent + 0 + 64, child + 0 + 128, 64);
		}
		this->template update_paths_r0<6, 64>(128, 0);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template g0<64>(parent + 0, parent + 0 + 64, child + 0 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 128, parent + 128 + 32, child + 128 + 64, 32);
		}
		this->template update_paths_rep<5, 32>(192, 64);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template gr<32>(parent + 128, parent + 128 + 32, s[path].data() + 64, child + 128 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 192, parent + 192 + 16, child + 192 + 32, 16);
		}
		this->template update_paths_rep<4, 16>(224, 96);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template gr<16>(parent + 192, parent + 192 + 16, s[path].data() + 96, child + 192 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 224, parent + 224 + 8, child + 224 + 16, 8);
		}
		this->template update_paths_rep<3, 8>(240, 112);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template gr<8>(parent + 224, parent + 224 + 8, s[path].data() + 112, child + 224 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 240, parent + 240 + 4, child + 240 + 8, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template f<2>(parent + 248, parent + 248 + 2, child + 248 + 4, 2);
		}
		this->template update_paths_r0<1, 2>(252, 120);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template g0<2>(parent + 248, parent + 248 + 2, child + 248 + 4, 2);
		}
		this->template update_paths_r1<1, 2>(252, 122);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<2>(s[this->paths[i]], 120 + 2, 120, 2);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g<4>(parent + 240, parent + 240 + 4, s[path].data() + 120, child + 240 + 8, 4);
		}
		this->template update_paths_r1<2, 4>(248, 124);
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
			API_polar::template xo0<64>(s[this->paths[i]], 0 + 64, 0, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path  = this->paths[i];
			const auto child = l[this->up_ref_array_idx(path, 8 -1)].data();
			API_polar::template g<128>(y, y + 128, s[path].data() + 0, child, 128);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][7   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 7 -1)].data();
			API_polar::template f<64>(parent + 0, parent + 0 + 64, child + 0 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 128, parent + 128 + 32, child + 128 + 64, 32);
		}
		this->template update_paths_rep<5, 32>(192, 128);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template gr<32>(parent + 128, parent + 128 + 32, s[path].data() + 128, child + 128 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 192, parent + 192 + 16, child + 192 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 224, parent + 224 + 8, child + 224 + 16, 8);
		}
		this->template update_paths_r0<3, 8>(240, 160);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g0<8>(parent + 224, parent + 224 + 8, child + 224 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 240, parent + 240 + 4, child + 240 + 8, 4);
		}
		this->template update_paths_r0<2, 4>(248, 168);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g0<4>(parent + 240, parent + 240 + 4, child + 240 + 8, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template f<2>(parent + 248, parent + 248 + 2, child + 248 + 4, 2);
		}
		this->template update_paths_r0<1, 2>(252, 172);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][2   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 2 -1)].data();
			API_polar::template g0<2>(parent + 248, parent + 248 + 2, child + 248 + 4, 2);
		}
		this->template update_paths_r1<1, 2>(252, 174);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<2>(s[this->paths[i]], 172 + 2, 172, 2);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<4>(s[this->paths[i]], 168 + 4, 168, 4);
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
			API_polar::template g<16>(parent + 192, parent + 192 + 16, s[path].data() + 160, child + 192 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 224, parent + 224 + 8, child + 224 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 240, parent + 240 + 4, child + 240 + 8, 4);
		}
		this->template update_paths_r0<2, 4>(248, 176);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template g0<4>(parent + 240, parent + 240 + 4, child + 240 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(248, 180);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<4>(s[this->paths[i]], 176 + 4, 176, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g<8>(parent + 224, parent + 224 + 8, s[path].data() + 176, child + 224 + 16, 8);
		}
		this->template update_paths_spc<3, 8>(240, 184);
		normalize_scl_metrics<R>(this->metrics, this->L);
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
			API_polar::template g<64>(parent + 0, parent + 0 + 64, s[path].data() + 128, child + 0 + 128, 64);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][6   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 6 -1)].data();
			API_polar::template f<32>(parent + 128, parent + 128 + 32, child + 128 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 192, parent + 192 + 16, child + 192 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 224, parent + 224 + 8, child + 224 + 16, 8);
		}
		this->template update_paths_r0<3, 8>(240, 192);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template g0<8>(parent + 224, parent + 224 + 8, child + 224 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 240, parent + 240 + 4, child + 240 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(248, 200);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 240, parent + 240 + 4, s[path].data() + 200, child + 240 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(248, 204);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo<4>(s[this->paths[i]], 200, 200 + 4, 200, 4);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			API_polar::template xo0<8>(s[this->paths[i]], 192 + 8, 192, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template g<16>(parent + 192, parent + 192 + 16, s[path].data() + 192, child + 192 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 224, parent + 224 + 8, child + 224 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 240, parent + 240 + 4, child + 240 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(248, 208);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 240, parent + 240 + 4, s[path].data() + 208, child + 240 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(248, 212);
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
			API_polar::template g<8>(parent + 224, parent + 224 + 8, s[path].data() + 208, child + 224 + 16, 8);
		}
		this->template update_paths_spc<3, 8>(240, 216);
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
			API_polar::template g<32>(parent + 128, parent + 128 + 32, s[path].data() + 192, child + 128 + 64, 32);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][5   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 5 -1)].data();
			API_polar::template f<16>(parent + 192, parent + 192 + 16, child + 192 + 32, 16);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][4   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 4 -1)].data();
			API_polar::template f<8>(parent + 224, parent + 224 + 8, child + 224 + 16, 8);
		}
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template f<4>(parent + 240, parent + 240 + 4, child + 240 + 8, 4);
		}
		this->template update_paths_rep<2, 4>(248, 224);
		normalize_scl_metrics<R>(this->metrics, this->L);
		for (auto i = 0; i < this->n_active_paths; i++) 
		{
			const auto path   = this->paths[i];
			const auto parent = l[this->path_2_array    [path][3   ]].data();
			const auto child  = l[this->up_ref_array_idx(path, 3 -1)].data();
			API_polar::template gr<4>(parent + 240, parent + 240 + 4, s[path].data() + 224, child + 240 + 8, 4);
		}
		this->template update_paths_spc<2, 4>(248, 228);
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
			API_polar::template g<8>(parent + 224, parent + 224 + 8, s[path].data() + 224, child + 224 + 16, 8);
		}
		this->template update_paths_spc<3, 8>(240, 232);
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
			API_polar::template g<16>(parent + 192, parent + 192 + 16, s[path].data() + 224, child + 192 + 32, 16);
		}
		this->template update_paths_r1<4, 16>(224, 240);
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
