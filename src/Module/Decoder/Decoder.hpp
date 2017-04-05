/*!
 * \file
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <chrono>
#include <string>
#include <vector>
#include <stdexcept>

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Decoder_i
 *
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \tparam B: type of the bits in the Decoder.
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the Decoder.
 *
 * The Decoder takes a soft input (real numbers) and return a hard output (bits).
 * Please use Decoder for inheritance (instead of Decoder_i).
 */
template <typename B = int, typename R = float>
class Decoder_i : public Module
{
private:
	const int n_dec_waves;
	const int n_inter_frame_rest;

	mipp::vector<R> Y_N;
	mipp::vector<B> V_K;

protected:
	const int K; /*!< Number of information bits in one frame */
	const int N; /*!< Size of one frame (= number of bits in one frame) */
	const int simd_inter_frame_level; /*!< Number of frames absorbed by the SIMD instructions. */

	std::chrono::nanoseconds d_load_total;
	std::chrono::nanoseconds d_decod_total;
	std::chrono::nanoseconds d_store_total;

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
	Decoder_i(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1,
	          std::string name = "Decoder_i")
	: Module(n_frames, name),
	  n_dec_waves((int)std::ceil((float)this->n_frames / (float)simd_inter_frame_level)),
	  n_inter_frame_rest(this->n_frames % simd_inter_frame_level),
	  Y_N(simd_inter_frame_level * N, (R)0),
	  V_K(simd_inter_frame_level * K, (B)0),
	  K(K), N(N), simd_inter_frame_level(simd_inter_frame_level)
	{
		if (K <= 0)
			throw std::invalid_argument("aff3ct::module::Decoder: \"K\" has to be greater than 0.");
		if (N <= 0)
			throw std::invalid_argument("aff3ct::module::Decoder: \"N\" has to be greater than 0.");
		if (simd_inter_frame_level <= 0)
			throw std::invalid_argument("aff3ct::module::Decoder: \"simd_inter_frame_level\" has to be greater "
			                            "than 0.");
		if (K > N)
			throw std::invalid_argument("aff3ct::module::Decoder: \"K\" has to be smaller than \"N\".");
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_i()
	{
	}

	/*!
	 * \brief Decodes the noisy frame.
	 *
	 * \param Y_N: a noisy frame.
	 * \param V_K: a decoded codeword (only the information bits).
	 */
	void hard_decode(const mipp::vector<R>& Y_N, mipp::vector<B>& V_K)
	{
		if (this->N * this->n_frames != (int)Y_N.size())
			throw std::length_error("aff3ct::module::Decoder: \"Y_N.size()\" has to be equal to "
			                        "\"N\" * \"n_frames\".");

		if (this->K * this->n_frames != (int)V_K.size())
			throw std::length_error("aff3ct::module::Decoder: \"V_K.size()\" has to be equal to "
			                        "\"K\" * \"n_frames\".");

		this->d_load_total  = std::chrono::nanoseconds(0);
		this->d_decod_total = std::chrono::nanoseconds(0);
		this->d_store_total = std::chrono::nanoseconds(0);

		this->_hard_decode(Y_N, V_K);
	}

	virtual void _hard_decode(const mipp::vector<R>& Y_N, mipp::vector<B>& V_K)
	{
		auto w = 0;
		for (w = 0; w < this->n_dec_waves -1; w++)
			this->_hard_decode_fbf(Y_N.data() + w * this->N * this->simd_inter_frame_level,
			                       V_K.data() + w * this->K * this->simd_inter_frame_level);

		if (this->n_inter_frame_rest == 0)
			this->_hard_decode_fbf(Y_N.data() + w * this->N * this->simd_inter_frame_level,
			                       V_K.data() + w * this->K * this->simd_inter_frame_level);
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level * this->N;
			std::copy(Y_N.begin() + waves_off1,
			          Y_N.begin() + waves_off1 + this->n_inter_frame_rest * this->N,
			          this->Y_N.begin());

			this->_hard_decode_fbf(this->Y_N.data(), this->V_K.data());

			const auto waves_off2 = w * this->simd_inter_frame_level * this->K;
			std::copy(this->V_K.begin(),
			          this->V_K.begin() + this->n_inter_frame_rest * this->K,
			          V_K.begin() + waves_off2);
		}
	}

	/*!
	 * \brief Gets the duration of the data loading in the decoding process.
	 *
	 * \return the duration of the data loading in the decoding process.
	 */
	std::chrono::nanoseconds get_load_duration() const
	{
		return this->d_load_total;
	}

	/*!
	 * \brief Gets the duration of the decoding process (without loads and stores).
	 *
	 * \return the duration of the decoding process (without loads and stores).
	 */
	std::chrono::nanoseconds get_decode_duration() const
	{
		return this->d_decod_total;
	}

	/*!
	 * \brief Gets the duration of the data storing in the decoding process.
	 *
	 * \return the duration of the data storing in the decoding process.
	 */
	std::chrono::nanoseconds get_store_duration() const
	{
		return this->d_store_total;
	}

	/*!
	 * \brief Gets the number of frames absorbed by the SIMD instructions.
	 *
	 * \return the number of frames absorbed by the SIMD instructions.
	 */
	int get_simd_inter_frame_level() const
	{
		return this->simd_inter_frame_level;
	}

protected:
	virtual void _hard_decode_fbf(const R *Y_N, B *V_K)
	{
		throw std::runtime_error("aff3ct::module::Decoder: \"_hard_decode_fbf\" is unimplemented.");
	}
};
}
}

#include "SC_Decoder.hpp"

#endif /* DECODER_HPP_ */
