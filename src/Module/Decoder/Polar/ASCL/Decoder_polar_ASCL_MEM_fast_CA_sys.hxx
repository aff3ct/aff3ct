#include <stdexcept>

#include "Decoder_polar_ASCL_MEM_fast_CA_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, class API_polar>
Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::Decoder_polar_ASCL_MEM_fast_CA_sys(const int& K, const int& N, const int& L_max, const mipp::vector<B>& frozen_bits,
                                     CRC<B>& crc, const bool is_full_adaptive, const int n_frames, const std::string name)
: Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>(K, N, L_max, frozen_bits, crc, n_frames, name),
  sc_decoder                                      (K, N       , frozen_bits,      n_frames, name),
  L_max(L_max), is_full_adaptive(is_full_adaptive)
{
	if (!tools::is_power_of_2(this->N))
		throw std::invalid_argument("aff3ct::module::Decoder_polar_ASCL_MEM_fast_CA_sys: \"N\" has to be a power "
		                            "of 2.");

	if (this->N != (int)frozen_bits.size())
		throw std::length_error("aff3ct::module::Decoder_polar_ASCL_MEM_fast_CA_sys: \"frozen_bits.size()\" has to be "
		                        "equal to \"N\".");

	if (this->L_max <= 0 || !tools::is_power_of_2(this->L_max))
		throw std::invalid_argument("aff3ct::module::Decoder_polar_ASCL_MEM_fast_CA_sys: \"L_max\" has to be positive "
		                            "and a power of 2.");
}

template <typename B, typename R, class API_polar>
Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::Decoder_polar_ASCL_MEM_fast_CA_sys(const int& K, const int& N, const int& L_max, const mipp::vector<B>& frozen_bits,
                                     const std::vector<tools::Pattern_polar_i*> polar_patterns,
                                     const int idx_r0, const int idx_r1,
                                     CRC<B>& crc, const bool is_full_adaptive, const int n_frames, const std::string name)
: Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>(K, N, L_max, frozen_bits, polar_patterns, idx_r0, idx_r1, crc, n_frames, name),
  sc_decoder                                      (K, N       , frozen_bits,                                      n_frames, name),
  L_max(L_max), is_full_adaptive(is_full_adaptive)
{
	if (!tools::is_power_of_2(this->N))
		throw std::invalid_argument("aff3ct::module::Decoder_polar_ASCL_MEM_fast_CA_sys: \"N\" has to be a power "
		                            "of 2.");

	if (this->N != (int)frozen_bits.size())
		throw std::length_error("aff3ct::module::Decoder_polar_ASCL_MEM_fast_CA_sys: \"frozen_bits.size()\" has to be "
		                        "equal to \"N\".");

	if (this->L_max <= 0 || !tools::is_power_of_2(this->L_max))
		throw std::invalid_argument("aff3ct::module::Decoder_polar_ASCL_MEM_fast_CA_sys: \"L_max\" has to be positive "
		                            "and a power of 2.");
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::_hard_decode_fbf(const R *Y_N, B *V_K)
{
	sc_decoder.d_load_total  = std::chrono::nanoseconds(0);
	sc_decoder.d_decod_total = std::chrono::nanoseconds(0);
	sc_decoder.d_store_total = std::chrono::nanoseconds(0);

	this->L = 1;
	sc_decoder._hard_decode_fbf(Y_N, V_K);

	this->d_load_total  += sc_decoder.d_load_total;
	this->d_decod_total += sc_decoder.d_decod_total;
	this->d_store_total += sc_decoder.d_store_total;

	auto t_decod = std::chrono::steady_clock::now();
	// check the CRC
	auto crc_decode_result = this->crc._check(V_K, this->get_simd_inter_frame_level());

	// delete the path if the CRC result is negative
	if (!crc_decode_result && L_max > 1)
	{
		if (is_full_adaptive)
		{
			do
			{
				int first_node_id = 0, off_l = 0, off_s = 0;

				this->L <<= 1;
				this->init_buffers();
				this->recursive_decode(Y_N, off_l, off_s, this->m, first_node_id);
			}
			while (!this->select_best_path() && this->L < L_max);
		}
		else // partial adaptive mode
		{
			int first_node_id = 0, off_l = 0, off_s = 0;

			this->L = this->L_max;
			this->init_buffers();
			this->recursive_decode(Y_N, off_l, off_s, this->m, first_node_id);
		}
	}
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now();
	if (this->L > 1)
		Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>::_store(V_K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}
}
}
