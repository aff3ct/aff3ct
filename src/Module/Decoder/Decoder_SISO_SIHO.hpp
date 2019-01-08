/*!
 * \file
 * \brief A Decoder_SISO is both a "Soft Input Hard Output" decoder and a "Soft Input Soft Output" decoder.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_SISO_SIHO_HPP_
#define DECODER_SISO_SIHO_HPP_

#include "Decoder_SIHO.hpp"
#include "Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Decoder_SISO_SIHO
 *
 * \brief A Decoder_SISO_SIHO is both a "Soft Input Hard Output" decoder and a "Soft Input Soft Output" decoder.
 *
 * \tparam B: type of the bits in the decoder.
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the decoder.
 */
template <typename B = int, typename R = float>
class Decoder_SISO_SIHO : public Decoder_SIHO<B,R>, public Decoder_SISO<R>
{
public:
	inline Task&   operator[](const dec::tsk                 t) { return Module::operator[]((int)t);                                }
	inline Socket& operator[](const dec::sck::decode_siso    s) { return Module::operator[]((int)dec::tsk::decode_siso   )[(int)s]; }
	inline Socket& operator[](const dec::sck::decode_siho    s) { return Module::operator[]((int)dec::tsk::decode_siho   )[(int)s]; }
	inline Socket& operator[](const dec::sck::decode_siho_cw s) { return Module::operator[]((int)dec::tsk::decode_siho_cw)[(int)s]; }

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
	Decoder_SISO_SIHO(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SISO_SIHO() = default;
};
}
}
#include "Decoder_SISO_SIHO.hxx"

#endif /* DECODER_SISO_SIHO_HPP_ */
