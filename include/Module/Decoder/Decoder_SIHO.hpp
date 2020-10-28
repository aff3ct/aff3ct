/*!
 * \file
 * \brief Class module::Decoder_SIHO.
 */
#ifndef DECODER_SIHO_HPP_
#define DECODER_SIHO_HPP_

#include <memory>
#include <vector>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Decoder/Decoder_HIHO.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Decoder_SIHO
 *
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \tparam B: type of the bits in the Decoder.
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the Decoder.
 *
 * The Decoder takes a soft input (real numbers) and return a hard output (bits).
 */
template <typename B = int, typename R = float>
class Decoder_SIHO : public Decoder_HIHO<B>
{
public:
	inline Task&   operator[](const dec::tsk                 t);
	inline Socket& operator[](const dec::sck::decode_hiho    s);
	inline Socket& operator[](const dec::sck::decode_hiho_cw s);
	inline Socket& operator[](const dec::sck::decode_siho    s);
	inline Socket& operator[](const dec::sck::decode_siho_cw s);

private:
	std::vector<R> Y_N;
	std::vector<B> V_KN;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:                      number of information bits in the frame.
	 * \param N:                      size of one frame.
	 * \param n_frames:               number of frames to process in the Decoder.
	 * \param simd_inter_frame_level: number of frames absorbed by the SIMD instructions.
	 */
	Decoder_SIHO(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SIHO() = default;

	virtual Decoder_SIHO<B,R>* clone() const;

	/*!
	 * \brief Decodes the noisy frame.
	 *
	 * \param Y_N: a noisy frame.
	 * \param V_K: a decoded codeword (only the information bits).
	 */
	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	int decode_siho(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_K, const int frame_id = -1);

	virtual int decode_siho(const R *Y_N, B *V_K, const int frame_id = -1);

	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	int decode_siho_cw(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_N, const int frame_id = -1);

	virtual int decode_siho_cw(const R *Y_N, B *V_N, const int frame_id = -1);

	virtual void set_n_frames(const int n_frames);

protected:
	virtual int _decode_siho(const R *Y_N, B *V_K, const int frame_id);

	virtual int _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);

	virtual int _decode_hiho(const B *Y_N, B *V_K, const int frame_id);

	virtual int _decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Decoder_SIHO.hxx"
#endif

#endif /* DECODER_SIHO_HPP_ */
