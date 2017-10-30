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
	namespace dec
	{
		namespace tsk
		{
			enum list { decode_hiho, decode_hiho_cw, decode_siho, decode_siho_cw, decode_siso, SIZE };
		}

		namespace sck
		{
			namespace decode_hiho   { enum list { Y_N,  V_K , SIZE }; }
			namespace decode_hiho_cw{ enum list { Y_N,  V_N , SIZE }; }
			namespace decode_siho   { enum list { Y_N,  V_K , SIZE }; }
			namespace decode_siho_cw{ enum list { Y_N,  V_N , SIZE }; }
			namespace decode_siso   { enum list { Y_N1, Y_N2, SIZE }; }
		}

		namespace tm
		{
			namespace decode_hiho   { enum list { load, decode, store       , SIZE }; }
			namespace decode_hiho_cw{ enum list { load, decode, store       , SIZE }; }
			namespace decode_siho   { enum list { load, decode, store, total, SIZE }; }
			namespace decode_siho_cw{ enum list { load, decode, store, total, SIZE }; }
		}
	}

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

		this->tasks_with_nullptr.resize(dec::tsk::SIZE);
		for (size_t t = 0; t < dec::tsk::SIZE; t++)
			this->tasks_with_nullptr[t] = nullptr;
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

	virtual void reset() {};
};
}
}

#endif /* DECODER_HPP_ */
