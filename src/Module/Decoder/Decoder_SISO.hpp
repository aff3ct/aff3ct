/*!
 * \file
 * \brief A Decoder_SISO is both a "Soft Input Hard Output" decoder and a "Soft Input Soft Output" decoder.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_SISO_HPP_
#define DECODER_SISO_HPP_

#include <cassert>

#include "Decoder.hpp"
#include "SISO.hpp"

namespace aff3ct
{
/*!
 * \class Decoder_SISO
 *
 * \brief A Decoder_SISO is both a "Soft Input Hard Output" decoder and a "Soft Input Soft Output" decoder.
 *
 * \tparam B: type of the bits in the decoder.
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the decoder.
 */
template <typename B, typename R>
class Decoder_SISO : public Decoder<B,R>, public SISO<R>
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
	Decoder_SISO(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1,
	             std::string name = "Decoder_SISO")
	: Decoder<B,R>(K, N, n_frames, simd_inter_frame_level, name          ),
	  SISO   <  R>(K, N, n_frames, simd_inter_frame_level, name + "_siso") {}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SISO() {}

	/*!
	 * \brief Gets the number of frames.
	 *
	 * \return the number of frames to process in the decoder.
	 */
	virtual int get_n_frames() const
	{
		assert((SISO<R>::n_frames) == (Decoder<B,R>::n_frames));
		return SISO<R>::n_frames;
	}

	/*!
	 * \brief Sets the number of frames.
	 *
	 * \param n_frames: the number of frames to process in the decoder.
	 */
	void set_n_frames(const int n_frames)
	{
		Decoder<B,R>::set_n_frames(n_frames);
		SISO   <  R>::set_n_frames(n_frames);
	}

	int get_simd_inter_frame_level() const
	{
		assert((SISO<R>::simd_inter_frame_level_siso) == (Decoder<B,R>::simd_inter_frame_level));
		return SISO<R>::simd_inter_frame_level_siso;
	}
};
}

#endif /* DECODER_SISO_HPP_ */
