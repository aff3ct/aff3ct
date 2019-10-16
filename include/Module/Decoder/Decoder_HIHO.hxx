#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/Decoder_HIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
Decoder_HIHO<B>
::Decoder_HIHO(const int K, const int N, const int n_frames, const int simd_inter_frame_level)
: Decoder(K, N, n_frames, simd_inter_frame_level),
  Y_N    (this->n_inter_frame_rest ? this->simd_inter_frame_level * this->N : 0),
  V_KN   (this->n_inter_frame_rest ? this->simd_inter_frame_level * this->N : 0)
{
	const std::string name = "Decoder_HIHO";
	this->set_name(name);

	auto &p1 = this->create_task("decode_hiho", (int)dec::tsk::decode_hiho);
	auto &p1s_Y_N = this->template create_socket_in <B>(p1, "Y_N", this->N * this->n_frames);
	auto &p1s_V_K = this->template create_socket_out<B>(p1, "V_K", this->K * this->n_frames);
	this->create_codelet(p1, [this, &p1s_Y_N, &p1s_V_K]() -> int
	{
		this->decode_hiho(static_cast<B*>(p1s_Y_N.get_dataptr()),
		                  static_cast<B*>(p1s_V_K.get_dataptr()));

		return 0;
	});
	this->register_timer(p1, "load");
	this->register_timer(p1, "decode");
	this->register_timer(p1, "store");

	auto &p2 = this->create_task("decode_hiho_cw", (int)dec::tsk::decode_hiho_cw);
	auto &p2s_Y_N = this->template create_socket_in <B>(p2, "Y_N", this->N * this->n_frames);
	auto &p2s_V_N = this->template create_socket_out<B>(p2, "V_N", this->N * this->n_frames);
	this->create_codelet(p2, [this, &p2s_Y_N, &p2s_V_N]() -> int
	{
		this->decode_hiho_cw(static_cast<B*>(p2s_Y_N.get_dataptr()),
		                     static_cast<B*>(p2s_V_N.get_dataptr()));

		return 0;
	});
	this->register_timer(p2, "load");
	this->register_timer(p2, "decode");
	this->register_timer(p2, "store");
}

template <typename B>
template <class A>
void Decoder_HIHO<B>
::decode_hiho(const std::vector<B,A>& Y_N, std::vector<B,A>& V_K, const int frame_id)
{
	if (this->N * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->K * this->n_frames != (int)V_K.size())
	{
		std::stringstream message;
		message << "'V_K.size()' has to be equal to 'K' * 'n_frames' ('V_K.size()' = " << V_K.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->decode_hiho(Y_N.data(), V_K.data(), frame_id);
}

template <typename B>
void Decoder_HIHO<B>
::decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
	if (frame_id < 0 || this->simd_inter_frame_level == 1)
	{
		const auto w_start = (frame_id < 0) ? 0 : frame_id % this->n_dec_waves;
		const auto w_stop  = (frame_id < 0) ? this->n_dec_waves : w_start +1;

		auto w = 0;
		for (w = w_start; w < w_stop -1; w++)
			this->_decode_hiho(Y_N + w * this->N * this->simd_inter_frame_level,
			                   V_K + w * this->K * this->simd_inter_frame_level,
			                   w * this->simd_inter_frame_level);

		if (this->n_inter_frame_rest == 0)
			this->_decode_hiho(Y_N + w * this->N * this->simd_inter_frame_level,
			                   V_K + w * this->K * this->simd_inter_frame_level,
			                   w * this->simd_inter_frame_level);
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level * this->N;
			std::copy(Y_N + waves_off1,
			          Y_N + waves_off1 + this->n_inter_frame_rest * this->N,
			          this->Y_N.begin());

			this->_decode_hiho(this->Y_N.data(), this->V_KN.data(), w * this->simd_inter_frame_level);

			const auto waves_off2 = w * this->simd_inter_frame_level * this->K;
			std::copy(this->V_KN.begin(),
			          this->V_KN.begin() + this->n_inter_frame_rest * this->K,
			          V_K + waves_off2);
		}
	}
	else
	{
		const auto w = (frame_id % this->n_frames) / this->simd_inter_frame_level;
		const auto w_pos = frame_id % this->simd_inter_frame_level;

		// std::fill(this->Y_N.begin(), this->Y_N.end(), (B)0);
		std::copy(Y_N + ((frame_id % this->n_frames) + 0) * this->N,
		          Y_N + ((frame_id % this->n_frames) + 1) * this->N,
		          this->Y_N.begin() + w_pos * this->N);

		this->_decode_hiho(this->Y_N.data(), this->V_KN.data(), w * this->simd_inter_frame_level);

		std::copy(this->V_KN.begin() + (w_pos +0) * this->K,
		          this->V_KN.begin() + (w_pos +1) * this->K,
		          V_K + (frame_id % this->n_frames) * this->K);
	}
}

template <typename B>
template <class A>
void Decoder_HIHO<B>
::decode_hiho_cw(const std::vector<B,A>& Y_N, std::vector<B,A>& V_N, const int frame_id)
{
	if (this->N * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)V_N.size())
	{
		std::stringstream message;
		message << "'V_N.size()' has to be equal to 'N' * 'n_frames' ('V_N.size()' = " << V_N.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->decode_hiho_cw(Y_N.data(), V_N.data(), frame_id);
}

template <typename B>
void Decoder_HIHO<B>
::decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
	if (frame_id < 0 || this->simd_inter_frame_level == 1)
	{
		const auto w_start = (frame_id < 0) ? 0 : frame_id % this->n_dec_waves;
		const auto w_stop  = (frame_id < 0) ? this->n_dec_waves : w_start +1;

		auto w = 0;
		for (w = w_start; w < w_stop -1; w++)
			this->_decode_hiho_cw(Y_N + w * this->N * this->simd_inter_frame_level,
			                      V_N + w * this->N * this->simd_inter_frame_level,
			                      w * this->simd_inter_frame_level);

		if (this->n_inter_frame_rest == 0)
			this->_decode_hiho_cw(Y_N + w * this->N * this->simd_inter_frame_level,
			                      V_N + w * this->N * this->simd_inter_frame_level,
			                      w * this->simd_inter_frame_level);
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level * this->N;
			std::copy(Y_N + waves_off1,
			          Y_N + waves_off1 + this->n_inter_frame_rest * this->N,
			          this->Y_N.begin());

			this->_decode_hiho_cw(this->Y_N.data(), this->V_KN.data(), w * this->simd_inter_frame_level);

			const auto waves_off2 = w * this->simd_inter_frame_level * this->N;
			std::copy(this->V_KN.begin(),
			          this->V_KN.begin() + this->n_inter_frame_rest * this->N,
			          V_N + waves_off2);
		}
	}
	else
	{
		const auto w = (frame_id % this->n_frames) / this->simd_inter_frame_level;
		const auto w_pos = frame_id % this->simd_inter_frame_level;

		// std::fill(this->Y_N.begin(), this->Y_N.end(), (B)0);
		std::copy(Y_N + ((frame_id % this->n_frames) + 0) * this->N,
		          Y_N + ((frame_id % this->n_frames) + 1) * this->N,
		          this->Y_N.begin() + w_pos * this->N);

		this->_decode_hiho_cw(this->Y_N.data(), this->V_KN.data(), w * this->simd_inter_frame_level);

		std::copy(this->V_KN.begin() + (w_pos +0) * this->N,
		          this->V_KN.begin() + (w_pos +1) * this->N,
		          V_N + (frame_id % this->n_frames) * this->N);
	}
}

template <typename B>
void Decoder_HIHO<B>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
void Decoder_HIHO<B>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
