#include "Decoder_polar_ASCL_MEM_fast_CA_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, class API_polar>
Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::Decoder_polar_ASCL_MEM_fast_CA_sys(const int& K, const int& N, const int& L_max, const mipp::vector<B>& frozen_bits,
                                     CRC<B>& crc, const int n_frames, const std::string name)
: Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>(K, N, L_max, frozen_bits, crc, n_frames, name),
  sc_decoder                                      (K, N       , frozen_bits,      n_frames, name),
  L_max(L_max), is_full_adaptive(true)
{
	assert(L_max > 0);
}

template <typename B, typename R, class API_polar>
Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::Decoder_polar_ASCL_MEM_fast_CA_sys(const int& K, const int& N, const int& L_max, const mipp::vector<B>& frozen_bits,
                                     const std::vector<tools::Pattern_polar_i*> polar_patterns,
                                     const int idx_r0, const int idx_r1,
                                     CRC<B>& crc, const int n_frames, const std::string name)
: Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>(K, N, L_max, frozen_bits, polar_patterns, idx_r0, idx_r1, crc, n_frames, name),
  sc_decoder                                      (K, N       , frozen_bits,                                      n_frames, name),
  L_max(L_max), is_full_adaptive(true)
{
	assert(L_max > 0);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::load(const mipp::vector<R>& Y_N)
{
	sc_decoder.load(Y_N);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::_hard_decode()
{
	this->L = 1;
	sc_decoder._hard_decode();

	// check the CRC
	auto crc_decode_result = this->crc_check(sc_decoder.s);

	// delete the path if the CRC result is negative
	if (!crc_decode_result && L_max > 1)
	{
		if (is_full_adaptive)
		{
			std::copy(sc_decoder.l.begin(), sc_decoder.l.begin() + this->N, this->Y_N.begin());
			do
			{
				int first_node_id = 0, off_l = 0, off_s = 0;

				this->L <<= 1;
				this->init_buffers();
				this->recursive_decode(off_l, off_s, this->m, first_node_id);
			}
			while (!this->select_best_path() && this->L < L_max);
		}
		else // pseudo adaptive mode
		{
			this->L = this->L_max;
			Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>::load(sc_decoder.l);
			Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>::_hard_decode();
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::store(mipp::vector<B>& V_K) const
{
	if (this->L == 1) sc_decoder.                      store(V_K);
	else Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>::store(V_K);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::store_fast(mipp::vector<B>& V) const
{
	if (this->L == 1) sc_decoder.                      store_fast(V);
	else Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>::store_fast(V);
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_MEM_fast_CA_sys<B,R,API_polar>
::unpack(mipp::vector<B>& V_N) const
{
	if (this->L == 1) sc_decoder.                      unpack(V_N);
	else Decoder_polar_SCL_MEM_fast_CA_sys<B,R,API_polar>::unpack(V_N);
}
}
}
