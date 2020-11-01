/*!
 * \file
 * \brief Class module::Decoder.
 */
#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <cstdint>
#include <cstddef>

#include "Tools/Interface/Interface_set_seed.hpp"
#include "Tools/Interface/Interface_reset.hpp"
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
			enum class decode_hiho    : size_t { Y_N,  V_K , status };
			enum class decode_hiho_cw : size_t { Y_N,  V_N , status };
			enum class decode_siho    : size_t { Y_N,  V_K , status };
			enum class decode_siho_cw : size_t { Y_N,  V_N , status };
			enum class decode_siso    : size_t { Y_N1, Y_N2, status };
		}

		namespace tm
		{
			enum class decode_hiho    : size_t { load, decode, store       , SIZE };
			enum class decode_hiho_cw : size_t { load, decode, store       , SIZE };
			enum class decode_siho    : size_t { load, decode, store, total, SIZE };
			enum class decode_siho_cw : size_t { load, decode, store, total, SIZE };
		}
	}

class Decoder : public Module, public tools::Interface_set_seed, public tools::Interface_reset
{
protected:
	      int  n_inter_frame_rest;

	const int  K;                      /*!< Number of information bits in one frame */
	const int  N;                      /*!< Size of one frame (= number of bits in one frame) */
	const int  simd_inter_frame_level; /*!< Number of frames absorbed by the SIMD instructions. */
	      int  n_dec_waves;
	      bool auto_reset;
	const int  mask;

public:
	Decoder(const int K, const int N, const int simd_inter_frame_level = 1);

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

	void reset(const int frame_id);
	void reset();

	virtual void set_seed(const int seed);

	virtual void set_n_frames(const int n_frames);

protected:
	virtual void _reset(const int frame_id);
};
}
}

#endif /* DECODER_HPP_ */
