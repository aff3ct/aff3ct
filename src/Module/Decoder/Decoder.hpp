#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <chrono>
#include <string>
#include <vector>

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
	Decoder(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder();

	int get_K() const;

	int get_N() const;

	/*!
	 * \brief Gets the number of frames absorbed by the SIMD instructions.
	 *
	 * \return the number of frames absorbed by the SIMD instructions.
	 */
	int get_simd_inter_frame_level() const;

	int get_n_dec_waves() const;

	virtual void reset();
};
}
}

#endif /* DECODER_HPP_ */
