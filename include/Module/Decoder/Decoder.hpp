/*!
 * \file
 * \brief Class module::Decoder.
 */
#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <cstdint>
#include <cstddef>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace dec
	{
		enum class tsk : size_t { decode_hiho, decode_hiho_cw, decode_siho, decode_siho_cw, decode_siso, SIZE };

		namespace sck
		{
			enum class decode_hiho    : size_t { Y_N,  V_K , SIZE };
			enum class decode_hiho_cw : size_t { Y_N,  V_N , SIZE };
			enum class decode_siho    : size_t { Y_N,  V_K , SIZE };
			enum class decode_siho_cw : size_t { Y_N,  V_N , SIZE };
			enum class decode_siso    : size_t { Y_N1, Y_N2, SIZE };
		}

		namespace tm
		{
			enum class decode_hiho    : size_t { load, decode, store       , SIZE };
			enum class decode_hiho_cw : size_t { load, decode, store       , SIZE };
			enum class decode_siho    : size_t { load, decode, store, total, SIZE };
			enum class decode_siho_cw : size_t { load, decode, store, total, SIZE };
		}
	}

class Decoder : public Module
{
protected:
	const int n_inter_frame_rest;

	const int  K;                      /*!< Number of information bits in one frame */
	const int  N;                      /*!< Size of one frame (= number of bits in one frame) */
	const int  simd_inter_frame_level; /*!< Number of frames absorbed by the SIMD instructions. */
	const int  n_dec_waves;
	      bool auto_reset;

public:
	Decoder(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder() = default;

	virtual Decoder* clone() const;

	int get_K() const;

	int get_N() const;

	/*!
	 * \brief Gets the number of frames absorbed by the SIMD instructions.
	 *
	 * \return the number of frames absorbed by the SIMD instructions.
	 */
	int get_simd_inter_frame_level() const;

	int get_n_dec_waves() const;

	bool is_auto_reset() const;
	void set_auto_reset(const bool enable_auto_reset);

	void reset(const int frame_id = -1);

protected:
	virtual void _reset(const int frame_id);
};
}
}

#endif /* DECODER_HPP_ */
