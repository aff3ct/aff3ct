/*!
 * \file
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_HIHO_HPP_
#define DECODER_HIHO_HPP_

#include <chrono>
#include <string>
#include <vector>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Decoder_SIHO_i
 *
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \tparam B: type of the bits in the Decoder.
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the Decoder.
 *
 * The Decoder takes a soft input (real numbers) and return a hard output (bits).
 * Please use Decoder for inheritance (instead of Decoder_HIHO_i).
 */
template <typename B = int>
class Decoder_HIHO_i : public Module
{
private:
	const int n_inter_frame_rest;

	std::vector<B> Y_N;
	std::vector<B> V_KN;

protected:
	const int K_hiho; /*!< Number of information bits in one frame */
	const int N_hiho; /*!< Size of one frame (= number of bits in one frame) */
	const int simd_inter_frame_level_hiho; /*!< Number of frames absorbed by the SIMD instructions. */
	const int n_dec_waves_hiho;

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
	Decoder_HIHO_i(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1,
	               std::string name = "Decoder_HIHO_i")
	: Module(n_frames, name, "Decoder_HIHO"),
	  n_inter_frame_rest(this->n_frames % simd_inter_frame_level),
	  Y_N (n_inter_frame_rest ? simd_inter_frame_level * N : 0),
	  V_KN(n_inter_frame_rest ? simd_inter_frame_level * N : 0),
	  K_hiho(K),
	  N_hiho(N),
	  simd_inter_frame_level_hiho(simd_inter_frame_level),
	  n_dec_waves_hiho((int)std::ceil((float)this->n_frames / (float)simd_inter_frame_level))
	{
		if (K <= 0)
		{
			std::stringstream message;
			message << "'K' has to be greater than 0 ('K' = " << K << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (N <= 0)
		{
			std::stringstream message;
			message << "'N' has to be greater than 0 ('N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (simd_inter_frame_level <= 0)
		{
			std::stringstream message;
			message << "'simd_inter_frame_level' has to be greater than 0 ('simd_inter_frame_level' = "
			        << simd_inter_frame_level << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (K > N)
		{
			std::stringstream message;
			message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		auto &p1 = this->create_process("decode_hiho");
		this->template create_socket_in <B>(p1, "Y_N", this->N_hiho * this->n_frames);
		this->template create_socket_out<B>(p1, "V_K", this->K_hiho * this->n_frames);
		this->create_codelet(p1, [&]() -> int
		{
			this->decode_hiho(static_cast<B*>(p1["Y_N"].get_dataptr()),
			                  static_cast<B*>(p1["V_K"].get_dataptr()));

			return 0;
		});
		this->register_duration(p1, "load");
		this->register_duration(p1, "decode");
		this->register_duration(p1, "store");

		auto &p2 = this->create_process("decode_hiho_coded");
		this->template create_socket_in <B>(p2, "Y_N", this->N_hiho * this->n_frames);
		this->template create_socket_out<B>(p2, "V_N", this->N_hiho * this->n_frames);
		this->create_codelet(p2, [&]() -> int
		{
			this->decode_hiho_coded(static_cast<B*>(p2["Y_N"].get_dataptr()),
			                        static_cast<B*>(p2["V_N"].get_dataptr()));

			return 0;
		});
		this->register_duration(p2, "load");
		this->register_duration(p2, "decode");
		this->register_duration(p2, "store");
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_HIHO_i()
	{
	}

	int get_K() const
	{
		return this->K_hiho;
	}

	int get_N() const
	{
		return this->N_hiho;
	}

	/*!
	 * \brief Gets the number of frames absorbed by the SIMD instructions.
	 *
	 * \return the number of frames absorbed by the SIMD instructions.
	 */
	int get_simd_inter_frame_level() const
	{
		return this->simd_inter_frame_level_hiho;
	}

	int get_n_dec_waves() const
	{
		return this->n_dec_waves_hiho;
	}

	/*!
	 * \brief Decodes the noisy frame.
	 *
	 * \param Y_N: a noisy frame.
	 * \param V_K: a decoded codeword (only the information bits).
	 */
	template <class A = std::allocator<B>>
	void decode_hiho(const std::vector<B,A>& Y_N, std::vector<B,A>& V_K)
	{
		if (this->N_hiho * this->n_frames != (int)Y_N.size())
		{
			std::stringstream message;
			message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
			        << ", 'N' = " << this->N_hiho << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->K_hiho * this->n_frames != (int)V_K.size())
		{
			std::stringstream message;
			message << "'V_K.size()' has to be equal to 'K' * 'n_frames' ('V_K.size()' = " << V_K.size()
			        << ", 'K' = " << this->K_hiho << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->decode_hiho(Y_N.data(), V_K.data());
	}

	virtual void decode_hiho(const B *Y_N, B *V_K)
	{
		auto w = 0;
		for (w = 0; w < this->n_dec_waves_hiho -1; w++)
			this->_decode_hiho(Y_N + w * this->N_hiho * this->simd_inter_frame_level_hiho,
			                   V_K + w * this->K_hiho * this->simd_inter_frame_level_hiho,
			                   w * simd_inter_frame_level_hiho);

		if (this->n_inter_frame_rest == 0)
			this->_decode_hiho(Y_N + w * this->N_hiho * this->simd_inter_frame_level_hiho,
			                   V_K + w * this->K_hiho * this->simd_inter_frame_level_hiho,
			                   w * simd_inter_frame_level_hiho);
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level_hiho * this->N_hiho;
			std::copy(Y_N + waves_off1,
			          Y_N + waves_off1 + this->n_inter_frame_rest * this->N_hiho,
			          this->Y_N.begin());

			this->_decode_hiho(this->Y_N.data(), this->V_KN.data(), w * simd_inter_frame_level_hiho);

			const auto waves_off2 = w * this->simd_inter_frame_level_hiho * this->K_hiho;
			std::copy(this->V_KN.begin(),
			          this->V_KN.begin() + this->n_inter_frame_rest * this->K_hiho,
			          V_K + waves_off2);
		}
	}

	template <class A = std::allocator<B>>
	void decode_hiho_coded(const std::vector<B,A>& Y_N, std::vector<B,A>& V_N)
	{
		if (this->N_hiho * this->n_frames != (int)Y_N.size())
		{
			std::stringstream message;
			message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
			        << ", 'N' = " << this->N_hiho << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_hiho * this->n_frames != (int)V_N.size())
		{
			std::stringstream message;
			message << "'V_N.size()' has to be equal to 'N' * 'n_frames' ('V_N.size()' = " << V_N.size()
			        << ", 'N' = " << this->N_hiho << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->decode_hiho_coded(Y_N.data(), V_N.data());
	}

	virtual void decode_hiho_coded(const B *Y_N, B *V_N)
	{
		auto w = 0;
		for (w = 0; w < this->n_dec_waves_hiho -1; w++)
			this->_decode_hiho_coded(Y_N + w * this->N_hiho * this->simd_inter_frame_level_hiho,
			                         V_N + w * this->N_hiho * this->simd_inter_frame_level_hiho,
			                         w * simd_inter_frame_level_hiho);

		if (this->n_inter_frame_rest == 0)
			this->_decode_hiho_coded(Y_N + w * this->N_hiho * this->simd_inter_frame_level_hiho,
			                         V_N + w * this->N_hiho * this->simd_inter_frame_level_hiho,
			                         w * simd_inter_frame_level_hiho);
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level_hiho * this->N_hiho;
			std::copy(Y_N + waves_off1,
			          Y_N + waves_off1 + this->n_inter_frame_rest * this->N_hiho,
			          this->Y_N.begin());

			this->_decode_hiho_coded(this->Y_N.data(), this->V_KN.data(), w * simd_inter_frame_level_hiho);

			const auto waves_off2 = w * this->simd_inter_frame_level_hiho * this->N_hiho;
			std::copy(this->V_KN.begin(),
			          this->V_KN.begin() + this->n_inter_frame_rest * this->N_hiho,
			          V_N + waves_off2);
		}
	}

protected:
	virtual void _decode_hiho(const B *Y_N, B *V_K, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _decode_hiho_coded(const B *Y_N, B *V_N, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};
}
}

#include "SC_Decoder_HIHO.hpp"

#endif /* DECODER_HIHO_HPP_ */
