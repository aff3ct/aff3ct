#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Math/utils.h"
#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Tools/Perf/Transpose/transpose_selector.h"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_rep_left.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_spc.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_std.hpp"
#include "Tools/Code/Polar/fb_extract.h"
#include "Tools/Code/Polar/fb_assert.h"
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
constexpr int static_level = 6; // 2^6 = 64

template <typename B, typename R, class API_polar, int REV_D>
struct Decoder_polar_SC_fast_sys_static
{
	static void decode(const tools::Pattern_polar_parser &polar_patterns, mipp::vector<B> &s,
	                   mipp::vector<R> &l, const int off_l, const int off_s, int &node_id)
	{
		constexpr int reverse_depth = REV_D;
		constexpr int n_elmts = 1 << reverse_depth;
		constexpr int n_elm_2 = n_elmts >> 1;

		const tools::polar_node_t node_type = polar_patterns.get_node_type(node_id);

		const bool is_terminal_pattern = (node_type == tools::polar_node_t::RATE_0) ||
		                                 (node_type == tools::polar_node_t::RATE_1) ||
		                                 (node_type == tools::polar_node_t::REP)    ||
		                                 (node_type == tools::polar_node_t::SPC);

		if (!is_terminal_pattern && reverse_depth)
		{
			// f
			switch (node_type)
			{
				case tools::polar_node_t::STANDARD: API_polar::template f<n_elm_2>(l, off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2); break;
				case tools::polar_node_t::REP_LEFT: API_polar::template f<n_elm_2>(l, off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2); break;
				default:
					break;
			}

			Decoder_polar_SC_fast_sys_static<B,R,API_polar,REV_D-1>
			::decode(polar_patterns, s, l, off_l + n_elmts, off_s, ++node_id); // recursive call left

			// g
			switch (node_type)
			{
				case tools::polar_node_t::STANDARD:    API_polar::template g <n_elm_2>(s, l, off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2); break;
				case tools::polar_node_t::RATE_0_LEFT: API_polar::template g0<n_elm_2>(   l, off_l, off_l + n_elm_2,        off_l + n_elmts, n_elm_2); break;
				case tools::polar_node_t::REP_LEFT:    API_polar::template gr<n_elm_2>(s, l, off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2); break;
				default:
					break;
			}

			Decoder_polar_SC_fast_sys_static<B,R,API_polar,REV_D-1>
			::decode(polar_patterns, s, l, off_l + n_elmts, off_s + n_elm_2, ++node_id); // recursive call right

			// xor
			switch (node_type)
			{
				case tools::polar_node_t::STANDARD:    API_polar::template xo <n_elm_2>(s, off_s, off_s + n_elm_2, off_s, n_elm_2); break;
				case tools::polar_node_t::RATE_0_LEFT: API_polar::template xo0<n_elm_2>(s,        off_s + n_elm_2, off_s, n_elm_2); break;
				case tools::polar_node_t::REP_LEFT:    API_polar::template xo <n_elm_2>(s, off_s, off_s + n_elm_2, off_s, n_elm_2); break;
				default:
					break;
			}
		}
		else
		{
			// h
			switch (node_type)
			{
				case tools::polar_node_t::RATE_0: API_polar::template h0 <n_elmts>(s,           off_s, n_elmts); break;
				case tools::polar_node_t::RATE_1: API_polar::template h  <n_elmts>(s, l, off_l, off_s, n_elmts); break;
				case tools::polar_node_t::REP:    API_polar::template rep<n_elmts>(s, l, off_l, off_s, n_elmts); break;
				case tools::polar_node_t::SPC:    API_polar::template spc<n_elmts>(s, l, off_l, off_s, n_elmts); break;
				default:
					break;
			}
		}
	}
};

template <typename B, typename R, class API_polar>
struct Decoder_polar_SC_fast_sys_static<B,R,API_polar,0>
{
	static void decode(const tools::Pattern_polar_parser &polar_patterns, mipp::vector<B> &s, mipp::vector<R> &l,
	                   const int off_l, const int off_s, int &node_id)
	{
		constexpr int reverse_depth = 0;
		constexpr int n_elmts = 1 << reverse_depth;

		const tools::polar_node_t node_t = polar_patterns.get_node_type(node_id);

		switch (node_t)
		{
			case tools::polar_node_t::RATE_0: API_polar::template h0<n_elmts>(s,           off_s, n_elmts); break;
			case tools::polar_node_t::RATE_1: API_polar::template h <n_elmts>(s, l, off_l, off_s, n_elmts); break;
			default:
				break;
		}
	}
};

template <typename B, typename R, class API_polar>
Decoder_polar_SC_fast_sys<B,R,API_polar>
::Decoder_polar_SC_fast_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits)
: Decoder_SIHO<B,R>(K, N),
  m                ((int)std::log2(N)),
  l                (2 * N * API_polar::get_n_frames() + mipp::nElReg<R>()   ),
  s                (1 * N * API_polar::get_n_frames() + mipp::nElReg<B>(), 0),
  s_bis            (1 * N * API_polar::get_n_frames() + mipp::nElReg<B>()   ),
  frozen_bits      (frozen_bits),
  polar_patterns   (frozen_bits,
                    {new tools::Pattern_polar_std,
                     new tools::Pattern_polar_r0_left,
                     new tools::Pattern_polar_r0,
                     new tools::Pattern_polar_r1,
                     new tools::Pattern_polar_rep_left,
                     new tools::Pattern_polar_rep,
                     new tools::Pattern_polar_spc},
                    2,
                    3,
                    true)
{
	const std::string name = "Decoder_polar_SC_fast_sys";
	this->set_name(name);
	this->set_n_frames_per_wave(API_polar::get_n_frames());

	static_assert(sizeof(B) == sizeof(R), "");

	if (!tools::is_power_of_2(this->N))
	{
		std::stringstream message;
		message << "'N' has to be a power of 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != (int)frozen_bits.size())
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ('frozen_bits.size()' = " << frozen_bits.size()
		        << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto k = 0; for (auto i = 0; i < this->N; i++) if (frozen_bits[i] == 0) k++;
	if (this->K != k)
	{
		std::stringstream message;
		message << "The number of information bits in the frozen_bits is invalid ('K' = " << K << ", 'k' = "
		        << k << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, class API_polar>
Decoder_polar_SC_fast_sys<B,R,API_polar>
::Decoder_polar_SC_fast_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits,
                            const std::vector<tools::Pattern_polar_i*> &polar_patterns,
                            const int idx_r0, const int idx_r1)
: Decoder_SIHO<B,R>(K, N),
  m                ((int)std::log2(N)),
  l                (2 * N * API_polar::get_n_frames() + mipp::nElReg<R>()   ),
  s                (1 * N * API_polar::get_n_frames() + mipp::nElReg<B>(), 0),
  s_bis            (1 * N * API_polar::get_n_frames() + mipp::nElReg<B>()   ),
  frozen_bits      (frozen_bits),
  polar_patterns   (frozen_bits, polar_patterns, idx_r0, idx_r1)
{
	const std::string name = "Decoder_polar_SC_fast_sys";
	this->set_name(name);
	this->set_n_frames_per_wave(API_polar::get_n_frames());

	static_assert(sizeof(B) == sizeof(R), "");

	if (!tools::is_power_of_2(this->N))
	{
		std::stringstream message;
		message << "'N' has to be a power of 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != (int)frozen_bits.size())
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ('frozen_bits.size()' = " << frozen_bits.size()
		        << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto k = 0; for (auto i = 0; i < this->N; i++) if (frozen_bits[i] == 0) k++;
	if (this->K != k)
	{
		std::stringstream message;
		message << "The number of information bits in the frozen_bits is invalid ('K' = " << K << ", 'k' = "
		        << k << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	this->set_frozen_bits(frozen_bits);
}

template <typename B, typename R, class API_polar>
Decoder_polar_SC_fast_sys<B,R,API_polar>* Decoder_polar_SC_fast_sys<B,R,API_polar>
::clone() const
{
	auto m = new Decoder_polar_SC_fast_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SC_fast_sys<B,R,API_polar>
::set_frozen_bits(const std::vector<bool>& fb)
{
	aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
	std::copy(fb.begin(), fb.end(), this->frozen_bits.begin());
	polar_patterns.set_frozen_bits(this->frozen_bits);
}

template <typename B, typename R, class API_polar>
const std::vector<bool>& Decoder_polar_SC_fast_sys<B,R,API_polar>
::get_frozen_bits() const
{
	return this->frozen_bits;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SC_fast_sys<B,R,API_polar>
::_load(const R *Y_N)
{
	constexpr int n_frames = API_polar::get_n_frames();

	if (n_frames == 1)
		std::copy(Y_N, Y_N + this->N, l.begin());
	else
	{
		bool fast_interleave = false;
		if (typeid(B) == typeid(signed char))
			fast_interleave = tools::char_transpose((signed char*)Y_N, (signed char*)l.data(), (int)this->N);

		if (!fast_interleave)
		{
			std::vector<const R*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*this->N;
			tools::Reorderer_static<R,n_frames>::apply(frames, l.data(), this->N);
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SC_fast_sys<B,R,API_polar>
::_decode()
{
	if (m < static_level)
	{
		std::stringstream message;
		message << "'m' has to be equal or greater than 'static_level' ('m' = " << m << ", 'static_level' = "
		        << static_level << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	int first_id = 0, off_l = 0, off_s = 0;
	this->recursive_decode(off_l, off_s, m, first_id);
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SC_fast_sys<B,R,API_polar>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
	if (!API_polar::isAligned(Y_N))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'Y_N' is misaligned memory.");

	if (!API_polar::isAligned(V_K))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'V_K' is misaligned memory.");

//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode();
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::total,  d_load + d_decod + d_store);

	return 0;
}

template <typename B, typename R, class API_polar>
int Decoder_polar_SC_fast_sys<B,R,API_polar>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
	if (!API_polar::isAligned(Y_N))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'Y_N' is misaligned memory.");

	if (!API_polar::isAligned(V_N))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'V_N' is misaligned memory.");

//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode();
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store_cw(V_N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::total,  d_load + d_decod + d_store);

	return 0;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SC_fast_sys<B,R,API_polar>
::recursive_decode(const int off_l, const int off_s, const int reverse_depth, int &node_id)
{
	if (reverse_depth == static_level)
	{
		// use the static version of the decoder
		Decoder_polar_SC_fast_sys_static<B,R,API_polar,static_level>
		::decode(polar_patterns, this->s, this->l, off_l, off_s, node_id);
	}
	else
	{
		const int n_elmts = 1 << reverse_depth;
		const int n_elm_2 = n_elmts >> 1;
		const auto node_type = polar_patterns.get_node_type(node_id);

		const bool is_terminal_pattern = (node_type == tools::polar_node_t::RATE_0) ||
		                                 (node_type == tools::polar_node_t::RATE_1) ||
		                                 (node_type == tools::polar_node_t::REP)    ||
		                                 (node_type == tools::polar_node_t::SPC);

		if (!is_terminal_pattern && reverse_depth)
		{
			// f
			switch (node_type)
			{
				case tools::polar_node_t::STANDARD: API_polar::f(l, off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2); break;
				case tools::polar_node_t::REP_LEFT: API_polar::f(l, off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2); break;
				default:
					break;
			}

			this->recursive_decode(off_l + n_elmts, off_s, reverse_depth -1, ++node_id); // recursive call left

			// g
			switch (node_type)
			{
				case tools::polar_node_t::STANDARD:    API_polar::g (s, l, off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2); break;
				case tools::polar_node_t::RATE_0_LEFT: API_polar::g0(   l, off_l, off_l + n_elm_2,        off_l + n_elmts, n_elm_2); break;
				case tools::polar_node_t::REP_LEFT:    API_polar::gr(s, l, off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2); break;
				default:
					break;
			}

			this->recursive_decode(off_l + n_elmts, off_s + n_elm_2, reverse_depth -1, ++node_id); // recursive call right

			// xor
			switch (node_type)
			{
				case tools::polar_node_t::STANDARD:    API_polar::xo (s, off_s, off_s + n_elm_2, off_s, n_elm_2); break;
				case tools::polar_node_t::RATE_0_LEFT: API_polar::xo0(s,        off_s + n_elm_2, off_s, n_elm_2); break;
				case tools::polar_node_t::REP_LEFT:    API_polar::xo (s, off_s, off_s + n_elm_2, off_s, n_elm_2); break;
				default:
					break;
			}
		}
		else
		{
			// h
			switch (node_type)
			{
				case tools::polar_node_t::RATE_0: API_polar::h0 (s,           off_s, n_elmts); break;
				case tools::polar_node_t::RATE_1: API_polar::h  (s, l, off_l, off_s, n_elmts); break;
				case tools::polar_node_t::REP:    API_polar::rep(s, l, off_l, off_s, n_elmts); break;
				case tools::polar_node_t::SPC:    API_polar::spc(s, l, off_l, off_s, n_elmts); break;
				default:
					break;
			}
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SC_fast_sys<B,R,API_polar>
::_store(B *V_K)
{
	constexpr int n_frames = API_polar::get_n_frames();

	if (n_frames == 1)
		tools::fb_extract(this->polar_patterns.get_leaves_pattern_types(), this->s.data(), V_K);
	else
	{
		bool fast_deinterleave = false;
#if defined(AFF3CT_POLAR_BIT_PACKING)
		if (typeid(B) == typeid(signed char))
		{
			fast_deinterleave = tools::char_itranspose((signed char*)s.data(),
			                                           (signed char*)s_bis.data(),
			                                           (int)this->N);
			if (!fast_deinterleave)
			{
				std::stringstream message;
				message << "Inverse transposition only supports NEON, SSE4.1 and AVX2 instruction sets and the "
				           "frame size 'N' has to be greater than 128 for NEON/SSE4.1 and greater than 256 for AVX2 "
				           "('N' = " << this->N << "). "
				           "To ensure the portability please do not compile with the -DAFF3CT_POLAR_BIT_PACKING "
				           "definition.";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			tools::Bit_packer::unpack(this->s_bis.data(), (unsigned char*)this->s.data(), this->N, n_frames);
		}
#endif
		if (!fast_deinterleave)
		{
			tools::fb_extract<B,n_frames>(this->polar_patterns.get_leaves_pattern_types(),
			                              this->s.data(), this->s_bis.data());

			// transpose without bit packing (vectorized)
			std::vector<B*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = (B*)(V_K + f*this->K);
			tools::Reorderer_static<B,n_frames>::apply_rev(s_bis.data(), frames, this->K);
		}
		else
			for (auto f = 0; f < n_frames; f++)
				tools::fb_extract(this->polar_patterns.get_leaves_pattern_types(), this->s.data() + f * this->N,
				                                                                   V_K            + f * this->K);
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SC_fast_sys<B,R,API_polar>
::_store_cw(B *V_N)
{
	constexpr int n_frames = API_polar::get_n_frames();

	if (n_frames == 1)
		std::copy(this->s.begin(), this->s.begin() + this->N, V_N);
	else
	{
		bool fast_deinterleave = false;
#if defined(AFF3CT_POLAR_BIT_PACKING)
		if (typeid(B) == typeid(signed char))
		{
			fast_deinterleave = tools::char_itranspose((signed char*)s.data(),
			                                           (signed char*)s_bis.data(),
			                                           (int)this->N);
			if (!fast_deinterleave)
			{
				std::stringstream message;
				message << "Inverse transposition only supports NEON, SSE4.1 and AVX2 instruction sets and the "
				           "frame size 'N' has to be greater than 128 for NEON/SSE4.1 and greater than 256 for AVX2 "
				           "('N' = " << this->N << "). "
				           "To ensure the portability please do not compile with the -DAFF3CT_POLAR_BIT_PACKING "
				           "definition.";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
			tools::Bit_packer::unpack(this->s_bis.data(), (unsigned char*)V_N, this->N, n_frames);
		}
#endif
		if (!fast_deinterleave)
		{
			// transpose without bit packing (vectorized)
			std::vector<B*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = (B*)(V_N + f*this->N);
			tools::Reorderer_static<B,n_frames>::apply_rev(this->s.data(), frames, this->N);
		}
	}
}
}
}
