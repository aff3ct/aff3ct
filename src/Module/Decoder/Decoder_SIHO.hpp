/*!
 * \file
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_SIHO_HPP_
#define DECODER_SIHO_HPP_

#include <cmath>
#include <chrono>
#include <string>
#include <vector>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Decoder.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Decoder_SIHO
 *
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \tparam B: type of the bits in the Decoder.
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the Decoder.
 *
 * The Decoder takes a soft input (real numbers) and return a hard output (bits).
 * Please use Decoder for inheritance (instead of Decoder_SIHO).
 */
template <typename B = int, typename R = float>
class Decoder_SIHO : virtual public Decoder
{
private:
	std::vector<R> Y_N;
	std::vector<B> V_KN;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:                      number of information bits in the frame.
	 * \param N:                      size of one frame.
	 * \param n_frames:               number of frames to process in the Decoder.
	 * \param simd_inter_frame_level: number of frames absorbed by the SIMD instructions.
	 * \param name:                   Decoder's name.
	 */
	Decoder_SIHO(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1)
	: Decoder(K, N, n_frames, simd_inter_frame_level),
	  Y_N    (this->n_inter_frame_rest ? this->simd_inter_frame_level * this->N : 0),
	  V_KN   (this->n_inter_frame_rest ? this->simd_inter_frame_level * this->N : 0)
	{
		const std::string name = "Decoder_SIHO";
		this->set_name(name);
		
		auto &p1 = this->create_task("decode_siho", dec::tsk::decode_siho);
		auto &p1s_Y_N = this->template create_socket_in <R>(p1, "Y_N", this->N * this->n_frames);
		auto &p1s_V_K = this->template create_socket_out<B>(p1, "V_K", this->K * this->n_frames);
		this->create_codelet(p1, [this, &p1s_Y_N, &p1s_V_K]() -> int
		{
			this->decode_siho(static_cast<R*>(p1s_Y_N.get_dataptr()),
			                  static_cast<B*>(p1s_V_K.get_dataptr()));

			return 0;
		});
		this->register_timer(p1, "load");
		this->register_timer(p1, "decode");
		this->register_timer(p1, "store");
		this->register_timer(p1, "total");

		auto &p2 = this->create_task("decode_siho_cw", dec::tsk::decode_siho_cw);
		auto &p2s_Y_N = this->template create_socket_in <R>(p2, "Y_N", this->N * this->n_frames);
		auto &p2s_V_N = this->template create_socket_out<B>(p2, "V_N", this->N * this->n_frames);
		this->create_codelet(p2, [this, &p2s_Y_N, &p2s_V_N]() -> int
		{
			this->decode_siho_cw(static_cast<R*>(p2s_Y_N.get_dataptr()),
			                     static_cast<B*>(p2s_V_N.get_dataptr()));

			return 0;
		});
		this->register_timer(p2, "load");
		this->register_timer(p2, "decode");
		this->register_timer(p2, "store");
		this->register_timer(p2, "total");
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SIHO()
	{
	}

	/*!
	 * \brief Decodes the noisy frame.
	 *
	 * \param Y_N: a noisy frame.
	 * \param V_K: a decoded codeword (only the information bits).
	 */
	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	void decode_siho(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_K)
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

		this->decode_siho(Y_N.data(), V_K.data());
	}

	virtual void decode_siho(const R *Y_N, B *V_K)
	{
		auto w = 0;
		for (w = 0; w < this->n_dec_waves -1; w++)
			this->_decode_siho(Y_N + w * this->N * this->simd_inter_frame_level,
			                   V_K + w * this->K * this->simd_inter_frame_level,
			                   w * this->simd_inter_frame_level);

		if (this->n_inter_frame_rest == 0)
			this->_decode_siho(Y_N + w * this->N * this->simd_inter_frame_level,
			                   V_K + w * this->K * this->simd_inter_frame_level,
			                   w * this->simd_inter_frame_level);
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level * this->N;
			std::copy(Y_N + waves_off1,
			          Y_N + waves_off1 + this->n_inter_frame_rest * this->N,
			          this->Y_N.begin());

			this->_decode_siho(this->Y_N.data(), this->V_KN.data(), w * simd_inter_frame_level);

			const auto waves_off2 = w * this->simd_inter_frame_level * this->K;
			std::copy(this->V_KN.begin(),
			          this->V_KN.begin() + this->n_inter_frame_rest * this->K,
			          V_K + waves_off2);
		}
	}

	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	void decode_siho_cw(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_N)
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

		this->decode_siho_cw(Y_N.data(), V_N.data());
	}

	virtual void decode_siho_cw(const R *Y_N, B *V_N)
	{
		auto w = 0;
		for (w = 0; w < this->n_dec_waves -1; w++)
			this->_decode_siho_cw(Y_N + w * this->N * this->simd_inter_frame_level,
			                      V_N + w * this->N * this->simd_inter_frame_level,
			                      w * this->simd_inter_frame_level);

		if (this->n_inter_frame_rest == 0)
			this->_decode_siho_cw(Y_N + w * this->N * this->simd_inter_frame_level,
			                      V_N + w * this->N * this->simd_inter_frame_level,
			                      w * this->simd_inter_frame_level);
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level * this->N;
			std::copy(Y_N + waves_off1,
			          Y_N + waves_off1 + this->n_inter_frame_rest * this->N,
			          this->Y_N.begin());

			this->_decode_siho_cw(this->Y_N.data(), this->V_KN.data(), w * simd_inter_frame_level);

			const auto waves_off2 = w * this->simd_inter_frame_level * this->N;
			std::copy(this->V_KN.begin(),
			          this->V_KN.begin() + this->n_inter_frame_rest * this->N,
			          V_N + waves_off2);
		}
	}

protected:
	virtual void _decode_siho(const R *Y_N, B *V_K, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};
}
}

#endif /* DECODER_SIHO_HPP_ */
