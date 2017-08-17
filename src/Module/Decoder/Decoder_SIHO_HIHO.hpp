/*!
 * \file
 * \brief A Decoder_SISO is both a "Soft Input Hard Output" decoder and a "Hard Input Hard Output" decoder.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_SIHO_HIHO_HPP_
#define DECODER_SIHO_HIHO_HPP_

#include "Decoder_SIHO.hpp"
#include "Decoder_HIHO.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Decoder_SIHO_HIHO
 *
 * \brief A Decoder_SIHO_HIHO is both a "Soft Input Hard Output" decoder and a "Hard Input Hard Output" decoder.
 *
 * \tparam B: type of the bits in the decoder.
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the decoder.
 */
template <typename B = int, typename R = float>
class Decoder_SIHO_HIHO : public Decoder_SIHO<B,R>, public Decoder_HIHO<B>
{
public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:                      number of information bits in the frame.
	 * \param N:                      size of one frame.
	 * \param n_frames:               number of frames to process in the decoder.
	 * \param simd_inter_frame_level: number of frames absorbed by the SIMD instructions.
	 * \param name:                   decoder name.
	 */
	Decoder_SIHO_HIHO(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1,
	                  std::string name = "Decoder_SIHO_HIHO")
	: Decoder_SIHO<B,R>(K, N, n_frames, simd_inter_frame_level, name          ),
	  Decoder_HIHO<B  >(K, N, n_frames, simd_inter_frame_level, name + "_hiho")
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SIHO_HIHO() {}

	/*!
	 * \brief Gets the number of frames.
	 *
	 * \return the number of frames to process in the decoder.
	 */
	virtual int get_n_frames() const
	{
		return Decoder_SIHO<B,R>::n_frames;
	}

	int get_K() const
	{
		return Decoder_SIHO<B,R>::K;
	}

	int get_N() const
	{
		return Decoder_SIHO<B,R>::N;
	}

	/*!
	 * \brief Gets the number of frames absorbed by the SIMD instructions.
	 *
	 * \return the number of frames absorbed by the SIMD instructions.
	 */
	int get_simd_inter_frame_level() const
	{
		return Decoder_SIHO<B,R>::simd_inter_frame_level;
	}

	int get_n_dec_waves() const
	{
		return Decoder_SIHO<B,R>::n_dec_waves;
	}
};
}
}

#endif /* DECODER_SIHO_HIHO_HPP_ */
