/*!
 * \file Decoder_SISO.hpp
 *
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
	 * \param K       : number of information bits in the frame.
	 * \param N       : size of one frame.
	 * \param n_frames: number of frames to process in the decoder.
	 * \param name    : decoder name.
	 */
	Decoder_SISO(const int K, const int N, const int n_frames = 1, std::string name = "Decoder_SISO") 
	: Decoder<B,R>(K, N, n_frames, name          ), 
	  SISO   <  R>(K, N, n_frames, name + "_siso") {}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SISO() {}

	/*!
	 * \brief Get the number of frames.
	 *
	 * \return the number of frames to process in the decoder.
	 */
	virtual int get_n_frames() const
	{
		assert((SISO<R>::n_frames) == (Decoder<B,R>::n_frames));
		return SISO<R>::n_frames;
	}
};

#endif /* DECODER_SISO_HPP_ */
