#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <sstream>
#include <string>
#include <cmath>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Module/Decoder/RSC/BCJR/Decoder_RSC_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
Decoder_RSC_BCJR<B,R>
::Decoder_RSC_BCJR(const int K,
                   const std::vector<std::vector<int>> &trellis,
                   const bool buffered_encoding)
: Decoder_SISO<B,R>(K, 2*(K + (int)std::log2(trellis[0].size()))),
  n_states((int)trellis[0].size()),
  n_ff((int)std::log2(n_states)),
  buffered_encoding(buffered_encoding),
  trellis(trellis),
  sys((K+n_ff) * this->get_n_frames_per_wave() + mipp::nElReg<R>()),
  par((K+n_ff) * this->get_n_frames_per_wave() + mipp::nElReg<R>()),
  ext( K       * this->get_n_frames_per_wave() + mipp::nElReg<R>()),
  s  ( K       * this->get_n_frames_per_wave() + mipp::nElReg<B>())
{
	const std::string name = "Decoder_RSC_BCJR";
	this->set_name(name);

	if (!tools::is_power_of_2(n_states))
	{
		std::stringstream message;
		message << "'n_states' has to be a power of 2 ('n_states' = " << n_states << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
void Decoder_RSC_BCJR<B,R>
::set_n_frames_per_wave(const size_t n_frames_per_wave)
{
	const auto old_n_frames_per_wave = this->get_n_frames_per_wave();
	if (old_n_frames_per_wave != n_frames_per_wave)
	{
		Decoder_SISO<B,R>::set_n_frames_per_wave(n_frames_per_wave);

		this->sys.resize((this->K+n_ff) * this->get_n_frames_per_wave() + mipp::nElReg<R>());
		this->par.resize((this->K+n_ff) * this->get_n_frames_per_wave() + mipp::nElReg<R>());
		this->ext.resize( this->K       * this->get_n_frames_per_wave() + mipp::nElReg<R>());
		this->s  .resize( this->K       * this->get_n_frames_per_wave() + mipp::nElReg<B>());
	}
}

template <typename B, typename R>
Decoder_RSC_BCJR<B,R>* Decoder_RSC_BCJR<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
int Decoder_RSC_BCJR<B,R>
::tail_length() const
{
	return 2 * n_ff;
}

template <typename B, typename R>
void Decoder_RSC_BCJR<B,R>
::_load(const R *Y_N)
{
	if (buffered_encoding)
	{
		const auto tail     = this->tail_length();
		const auto n_frames = this->get_n_frames_per_wave();

		if (n_frames == 1)
		{
			std::copy(Y_N + 0*this->K,          Y_N + 1*this->K + tail/2, sys.begin());
			std::copy(Y_N + 1*this->K + tail/2, Y_N + 2*this->K + tail/2, par.begin());
		}
		else
		{
			const auto frame_size = 2*this->K + tail;

			std::vector<const R*> frames(n_frames);
			for (size_t f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size;
			tools::Reorderer<R>::apply(frames, sys.data(), this->K + tail/2);

			for (size_t f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size + this->K + tail/2;
			tools::Reorderer<R>::apply(frames, par.data(), this->K + tail/2);
		}
	}
	else
	{
		const auto n_frames = this->get_n_frames_per_wave();

		// reordering
		for (auto i = 0; i < this->K + n_ff; i++)
		{
			for (size_t f = 0; f < n_frames; f++)
			{
				sys[(i*n_frames) +f] = Y_N[f*2*(this->K +n_ff) + i*2 +0];
				par[(i*n_frames) +f] = Y_N[f*2*(this->K +n_ff) + i*2 +1];
			}
		}
	}
}

template <typename B, typename R>
int Decoder_RSC_BCJR<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode_siso_alt(sys.data(), par.data(), ext.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	for (size_t i = 0; i < this->K * this->get_n_frames_per_wave(); i += mipp::nElReg<R>())
	{
		const auto r_post = mipp::Reg<R>(&ext[i]) + mipp::Reg<R>(&sys[i]);

		// s[i] = post[i] < 0;
		const auto r_s = mipp::cast<R,B>(r_post) >> (sizeof(B) * 8 - 1);

		r_s.store(&s[i]);
	}

	_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::total,  d_load + d_decod + d_store);

	return status;
}

template <typename B, typename R>
void Decoder_RSC_BCJR<B,R>
::_store(B *V_K) const
{
	if (this->get_n_frames_per_wave() == 1)
	{
		std::copy(s.begin(), s.begin() + this->K, V_K);
	}
	else // inter frame => output reordering
	{
		const auto n_frames = this->get_n_frames_per_wave();

		std::vector<B*> frames(n_frames);
		for (size_t f = 0; f < n_frames; f++)
			frames[f] = V_K + f*this->K;
		tools::Reorderer<B>::apply_rev(s.data(), frames, this->K);
	}
}
}
}
