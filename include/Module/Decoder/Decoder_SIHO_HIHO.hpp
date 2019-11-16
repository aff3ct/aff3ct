/*!
 * \file
 * \brief Class module::Decoder_SIHO_HIHO.
 */
#ifndef DECODER_SIHO_HIHO_HPP_
#define DECODER_SIHO_HIHO_HPP_

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_HIHO.hpp"

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
	inline Task&   operator[](const dec::tsk                 t);
	inline Socket& operator[](const dec::sck::decode_siho    s);
	inline Socket& operator[](const dec::sck::decode_siho_cw s);
	inline Socket& operator[](const dec::sck::decode_hiho    s);
	inline Socket& operator[](const dec::sck::decode_hiho_cw s);

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:                      number of information bits in the frame.
	 * \param N:                      size of one frame.
	 * \param n_frames:               number of frames to process in the decoder.
	 * \param simd_inter_frame_level: number of frames absorbed by the SIMD instructions.
	 */
	Decoder_SIHO_HIHO(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SIHO_HIHO() = default;

	virtual Decoder* clone() const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Decoder_SIHO_HIHO.hxx"
#endif

#endif /* DECODER_SIHO_HIHO_HPP_ */
