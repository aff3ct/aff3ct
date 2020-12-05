#include <algorithm>
#include <string>

#include "Module/Decoder/Polar/ASCL/Decoder_polar_ASCL_fast_CA_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, class API_polar>
Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::Decoder_polar_ASCL_fast_CA_sys(const int& K, const int& N, const int& L_max, const std::vector<bool>& frozen_bits,
                                 const CRC<B>& crc, const bool is_full_adaptive)
: Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>(K, N, L_max, frozen_bits, crc),
  sc_decoder(new Decoder_polar_SC_fast_sys<B,R,API_polar>(K, N, frozen_bits)),
  L_max(L_max), is_full_adaptive(is_full_adaptive)
{
	const std::string name = "Decoder_polar_ASCL_fast_CA_sys";
	this->set_name(name);
}

template <typename B, typename R, class API_polar>
Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::Decoder_polar_ASCL_fast_CA_sys(const int& K, const int& N, const int& L_max, const std::vector<bool>& frozen_bits,
                                 const std::vector<tools::Pattern_polar_i*> &polar_patterns,
                                 const int idx_r0, const int idx_r1,
                                 const CRC<B>& crc, const bool is_full_adaptive)
: Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>(K, N, L_max, frozen_bits, polar_patterns, idx_r0, idx_r1, crc),
  sc_decoder(new Decoder_polar_SC_fast_sys<B,R,API_polar>(K, N, frozen_bits)),
  L_max(L_max), is_full_adaptive(is_full_adaptive)
{
	const std::string name = "Decoder_polar_ASCL_fast_CA_sys";
	this->set_name(name);
}

template <typename B, typename R, class API_polar>
Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>* Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::clone() const
{
	auto m = new Decoder_polar_ASCL_fast_CA_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::deep_copy(const Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar> &m)
{
	Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>::deep_copy(m);
	if (m.sc_decoder != nullptr) this->sc_decoder.reset(m.sc_decoder->clone());
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::set_frozen_bits(const std::vector<bool>& frozen_bits)
{
	Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>::set_frozen_bits(frozen_bits);
	sc_decoder->set_frozen_bits(frozen_bits);
}

template <typename B, typename R, class API_polar>
const std::vector<bool>& Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::get_frozen_bits() const
{
	return this->frozen_bits;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::_decode(const R *Y_N, B *V_K, const size_t frame_id)
{
	this->L = 1;
	sc_decoder->_decode_siho(Y_N, V_K, frame_id);

	// check the CRC
	auto crc_decode_result = this->crc->check(V_K, frame_id);

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
			while (!this->select_best_path(frame_id) && this->L < L_max);
		}
		else // partial adaptive mode
		{
			int first_node_id = 0, off_l = 0, off_s = 0;

			this->L = this->L_max;
			this->init_buffers();
			this->recursive_decode(Y_N, off_l, off_s, this->m, first_node_id);
			this->select_best_path(frame_id);
		}
	}
}

template <typename B, typename R, class API_polar>
int Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
//	auto t_decod = std::chrono::steady_clock::now();
	this->_decode(Y_N, V_K, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now();
	if (this->L > 1)
		Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>::_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	return 0;
}

template <typename B, typename R, class API_polar>
int Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
//	auto t_decod = std::chrono::steady_clock::now();
	this->_decode(Y_N, V_N, frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now();
	if (this->L > 1)
		Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>::_store_cw(V_N);
	else
		sc_decoder->_store_cw(V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	return 0;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_ASCL_fast_CA_sys<B,R,API_polar>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Decoder_polar_SCL_fast_CA_sys<B,R,API_polar>::set_n_frames(n_frames);
		sc_decoder->set_n_frames(n_frames);
	}
}

}
}
