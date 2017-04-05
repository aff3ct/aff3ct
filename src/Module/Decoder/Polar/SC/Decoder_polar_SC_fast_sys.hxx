#include <chrono>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Tools/Perf/Transpose/transpose_selector.h"
#include "Tools/Code/Polar/Pattern_parser_polar.hpp"

#include "Patterns/Pattern_SC_standard.hpp"
#include "Patterns/Pattern_SC_rate0.hpp"
#include "Patterns/Pattern_SC_rate1.hpp"
#include "Patterns/Pattern_SC_rep.hpp"
#include "Patterns/Pattern_SC_spc.hpp"
#include "Patterns/Pattern_SC_rate0_left.hpp"
#include "Patterns/Pattern_SC_rep_left.hpp"

#include "Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
constexpr int static_level = 6; // 2^6 = 64

template <typename B, typename R, class API_polar, int REV_D>
struct Decoder_polar_SC_fast_sys_static
{
	static void decode(std::vector<char> &pattern_types_per_id, mipp::vector<B> &s, mipp::vector<R> &l, 
	                   const int off_l, const int off_s, int &id)
	{
		constexpr int reverse_depth = REV_D;
		constexpr int n_elmts = 1 << reverse_depth;
		constexpr int n_elm_2 = n_elmts >> 1;

		const int my_id = id;
		const pattern_SC_type my_pattern_type = (pattern_SC_type)pattern_types_per_id[my_id];

		const bool is_terminal_pattern = (my_pattern_type == pattern_SC_type::RATE_0) ||
		                                 (my_pattern_type == pattern_SC_type::RATE_1) ||
		                                 (my_pattern_type == pattern_SC_type::REP)    ||
		                                 (my_pattern_type == pattern_SC_type::SPC);

		if (!is_terminal_pattern && reverse_depth)
		{
			// f
			switch (my_pattern_type)
			{
				case STANDARD: API_polar::template f<n_elm_2>(l, off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2); break;
				case REP_LEFT: API_polar::template f<n_elm_2>(l, off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2); break;
				default:
					break;
			}

			Decoder_polar_SC_fast_sys_static<B,R,API_polar,REV_D-1>
			::decode(pattern_types_per_id, s, l, off_l + n_elmts, off_s, ++id); // recursive call left

			// g
			switch (my_pattern_type)
			{
				case STANDARD:    API_polar::template g <n_elm_2>(s, l, off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2); break;
				case RATE_0_LEFT: API_polar::template g0<n_elm_2>(   l, off_l, off_l + n_elm_2,        off_l + n_elmts, n_elm_2); break;
				case REP_LEFT:    API_polar::template gr<n_elm_2>(s, l, off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2); break;
				default:
					break;
			}

			Decoder_polar_SC_fast_sys_static<B,R,API_polar,REV_D-1>
			::decode(pattern_types_per_id, s, l, off_l + n_elmts, off_s + n_elm_2, ++id); // recursive call right

			// xor
			switch (my_pattern_type)
			{
				case STANDARD:    API_polar::template xo <n_elm_2>(s, off_s, off_s + n_elm_2, off_s, n_elm_2); break;
				case RATE_0_LEFT: API_polar::template xo0<n_elm_2>(s,        off_s + n_elm_2, off_s, n_elm_2); break;
				case REP_LEFT:    API_polar::template xo <n_elm_2>(s, off_s, off_s + n_elm_2, off_s, n_elm_2); break;
				default:
					break;
			}
		}
		else
		{
			// h
			switch (my_pattern_type)
			{
				case RATE_1: API_polar::template h  <n_elmts>(s, l, off_l, off_s, n_elmts); break;
				case REP:    API_polar::template rep<n_elmts>(s, l, off_l, off_s, n_elmts); break;
				case SPC:    API_polar::template spc<n_elmts>(s, l, off_l, off_s, n_elmts); break;
				default:
					break;
			}
		}
	}
};

template <typename B, typename R, class API_polar>
struct Decoder_polar_SC_fast_sys_static<B,R,API_polar,0>
{
	static void decode(std::vector<char> &pattern_types_per_id, mipp::vector<B> &s, mipp::vector<R> &l, 
	                   const int off_l, const int off_s, int &id)
	{
		constexpr int reverse_depth = 0;
		constexpr int n_elmts = 1 << reverse_depth;

		const int my_id = id;
		const pattern_SC_type my_pattern_type = (pattern_SC_type)pattern_types_per_id[my_id];

		switch (my_pattern_type)
		{
			case RATE_1: API_polar::template h<n_elmts>(s, l, off_l, off_s, n_elmts); break;
			default:
				break;
		}
	}
};

template <typename B, typename R, class API_polar>
Decoder_polar_SC_fast_sys<B,R,API_polar>
::Decoder_polar_SC_fast_sys(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames,
                            const std::string name)
: Decoder<B,R>(K, N, n_frames, API_polar::get_n_frames(), name),
  m((int)std::log2(N)),
  l    (2 * N * this->simd_inter_frame_level + mipp::nElmtsPerRegister<R>()),
  s    (1 * N * this->simd_inter_frame_level + mipp::nElmtsPerRegister<B>()),
  s_bis(1 * N * this->simd_inter_frame_level + mipp::nElmtsPerRegister<B>()),
  frozen_bits(frozen_bits)
{
	static_assert(sizeof(B) == sizeof(R), "");

	if (!tools::is_power_of_2(this->N))
		throw std::invalid_argument("aff3ct::module::Decoder_polar_SC_fast_sys: \"N\" has to be positive a power "
		                            "of 2.");

	if (this->N != (int)frozen_bits.size())
		throw std::length_error("aff3ct::module::Decoder_polar_SC_fast_sys: \"frozen_bits.size()\" has to be equal to "
		                        "\"N\".");

	Pattern_SC_interface* pattern_SC_r0 = new Pattern_SC<pattern_SC_type::RATE_0>();
	Pattern_SC_interface* pattern_SC_r1 = new Pattern_SC<pattern_SC_type::RATE_1>();

	std::vector<Pattern_SC_interface*> patterns_SC;
	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::STANDARD   >());
	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::RATE_0_LEFT>());
	patterns_SC.push_back(pattern_SC_r0                                 );
	patterns_SC.push_back(pattern_SC_r1                                 );
	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::REP_LEFT   >());
	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::REP        >());
	patterns_SC.push_back(new Pattern_SC<pattern_SC_type::SPC        >());

	mipp::vector<int> fb_int32(N);
	for (unsigned i = 0; i < frozen_bits.size(); i++)
		fb_int32[i] = (int)frozen_bits[i];

	auto parser = new tools::Pattern_parser_polar(N, fb_int32, patterns_SC, *pattern_SC_r0, *pattern_SC_r1);
	pattern_types_per_id = parser->get_pattern_types_per_id();

	delete parser;
	for (unsigned i = 0; i < patterns_SC.size(); i++)
		delete patterns_SC[i];

	std::fill(s.begin(), s.end(), (B)0);
}

template <typename B, typename R, class API_polar>
Decoder_polar_SC_fast_sys<B,R,API_polar>
::~Decoder_polar_SC_fast_sys()
{
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
::_hard_decode_fbf(const R *Y_N, B *V_K)
{
	if (m < static_level)
		throw std::runtime_error("aff3ct::module::Decoder_polar_SC_fast_sys: \"m\" has to be equal or greater than "
		                         "\"static_level\".");

	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	int first_id = 0, off_l = 0, off_s = 0;
	this->recursive_decode(off_l, off_s, m, first_id);
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SC_fast_sys<B,R,API_polar>
::recursive_decode(const int off_l, const int off_s, const int reverse_depth, int &id)
{
	if (reverse_depth == static_level)
	{
		// use the static version of the decoder
		Decoder_polar_SC_fast_sys_static<B,R,API_polar,static_level>
		::decode(pattern_types_per_id, this->s, this->l, off_l, off_s, id);
	}
	else
	{
		const int n_elmts = 1 << reverse_depth;
		const int n_elm_2 = n_elmts >> 1;
		const int my_id = id;
		const pattern_SC_type my_pattern_type = (pattern_SC_type)pattern_types_per_id[my_id];

		const bool is_terminal_pattern = (my_pattern_type == pattern_SC_type::RATE_0) ||
		                                 (my_pattern_type == pattern_SC_type::RATE_1) ||
		                                 (my_pattern_type == pattern_SC_type::REP)    ||
		                                 (my_pattern_type == pattern_SC_type::SPC);

		if (!is_terminal_pattern && reverse_depth)
		{
			// f
			switch (my_pattern_type)
			{
				case STANDARD: API_polar::f(l, off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2); break;
				case REP_LEFT: API_polar::f(l, off_l, off_l + n_elm_2, off_l + n_elmts, n_elm_2); break;
				default:
					break;
			}

			this->recursive_decode(off_l + n_elmts, off_s, reverse_depth -1, ++id); // recursive call left

			// g
			switch (my_pattern_type)
			{
				case STANDARD:    API_polar::g (s, l, off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2); break;
				case RATE_0_LEFT: API_polar::g0(   l, off_l, off_l + n_elm_2,        off_l + n_elmts, n_elm_2); break;
				case REP_LEFT:    API_polar::gr(s, l, off_l, off_l + n_elm_2, off_s, off_l + n_elmts, n_elm_2); break;
				default:
					break;
			}

			this->recursive_decode(off_l + n_elmts, off_s + n_elm_2, reverse_depth -1, ++id); // recursive call right

			// xor
			switch (my_pattern_type)
			{
				case STANDARD:    API_polar::xo (s, off_s, off_s + n_elm_2, off_s, n_elm_2); break;
				case RATE_0_LEFT: API_polar::xo0(s,        off_s + n_elm_2, off_s, n_elm_2); break;
				case REP_LEFT:    API_polar::xo (s, off_s, off_s + n_elm_2, off_s, n_elm_2); break;
				default:
					break;
			}
		}
		else
		{
			// h
			switch (my_pattern_type)
			{
				case RATE_1: API_polar::h  (s, l, off_l, off_s, n_elmts); break;
				case REP:    API_polar::rep(s, l, off_l, off_s, n_elmts); break;
				case SPC:    API_polar::spc(s, l, off_l, off_s, n_elmts); break;
				default:
					break;
			}
		}
	}
}

template <typename B, typename R, class API_polar>
void Decoder_polar_SC_fast_sys<B,R,API_polar>
::_store(B *V_K) const
{
	constexpr int n_frames = API_polar::get_n_frames();

	if (n_frames == 1)
	{
		auto k = 0;
		for (auto j = 0; j < this->N; j++)
			if (!frozen_bits[j])
				V_K[k++] =  s[j] ? 1 : 0;
	}
	else
	{
		bool fast_deinterleave = false;
#if defined(ENABLE_BIT_PACKING)
		if (typeid(B) == typeid(signed char))
		{
			if (!(fast_deinterleave = tools::char_itranspose((signed char*)s.data(),
			                                                 (signed char*)s_bis.data(),
			                                                 (int)this->N)))
			{

				throw std::runtime_error("aff3ct::module::Decoder_polar_SC_fast_sys: unsupported \"N\" value for "
				                         "itransposition (N have to be greater or equal to 128 for SSE/NEON or to "
				                         "256 for AVX)");
			}
			else
			{
				// bit unpacking
				auto idx = n_frames * this->K -1;
				for (auto i = n_frames * this->N -1; i > 0; i -= this->N)
				for (unsigned j = (unsigned) 0; j < (unsigned) this->N; j += sizeof(B) * 8)
				{
					unsigned char packed_vals = (unsigned char) s_bis[(i -j) / (sizeof(B) * 8)];
					for (auto k = 0; k < 8; k++)
						if (!frozen_bits[(this->N -1 -j) -k])
							V_K[idx--] = ((packed_vals >> (7-k)) & 0x01);
				}
			}
		}
#endif
		if (!fast_deinterleave)
		{
			// transpose without bit packing (vectorized)
			std::vector<B*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = (B*)(s_bis.data() + f*this->N);
			tools::Reorderer_static<B,n_frames>::apply_rev(s.data(), frames, this->N);

			for (auto i = 0; i < n_frames; i++)
			{
				auto k = 0;
				for (auto j = 0; j < this->N; j++)
					if (!frozen_bits[j])
						V_K[i * this->K + (k++)] = s_bis[i * this->N + j] ? 1 : 0;
			}
		}
	}
}
}
}
