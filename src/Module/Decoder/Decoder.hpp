#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <cmath>
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
class Decoder : public Module
{
protected:
	const int n_inter_frame_rest;

	const int K; /*!< Number of information bits in one frame */
	const int N; /*!< Size of one frame (= number of bits in one frame) */
	const int simd_inter_frame_level; /*!< Number of frames absorbed by the SIMD instructions. */
	const int n_dec_waves;

public:
	Decoder(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1,
	        std::string name = "Decoder")
	: Module(n_frames, name, "Decoder"),
	  n_inter_frame_rest(this->n_frames % simd_inter_frame_level),
	  K(K),
	  N(N),
	  simd_inter_frame_level(simd_inter_frame_level),
	  n_dec_waves((int)std::ceil((float)this->n_frames / (float)simd_inter_frame_level))
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
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder()
	{
	}

	int get_K() const
	{
		return this->K;
	}

	int get_N() const
	{
		return this->N;
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

	int get_n_dec_waves() const
	{
		return this->n_dec_waves;
	}
};
}
}

#endif /* DECODER_HPP_ */
