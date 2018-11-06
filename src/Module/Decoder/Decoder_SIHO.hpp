/*!
 * \file
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_SIHO_HPP_
#define DECODER_SIHO_HPP_

#include <chrono>
#include <string>
#include <vector>

#include "Decoder.hpp"

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
 * Please use Decoder for inheritance (instead of Decoder_SIHO).
 */
template <typename B = int, typename R = float>
class Decoder_SIHO : virtual public Decoder
{
public:
	inline Task&   operator[](const dec::tsk                 t) { return Module::operator[]((int)t);                                }
	inline Socket& operator[](const dec::sck::decode_siho    s) { return Module::operator[]((int)dec::tsk::decode_siho   )[(int)s]; }
	inline Socket& operator[](const dec::sck::decode_siho_cw s) { return Module::operator[]((int)dec::tsk::decode_siho_cw)[(int)s]; }

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
	 * \param name:                   Decoder's name.
	 */
	Decoder_SIHO(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SIHO() = default;

	/*!
	 * \brief Decodes the noisy frame.
	 *
	 * \param Y_N: a noisy frame.
	 * \param V_K: a decoded codeword (only the information bits).
	 */
	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	void decode_siho(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_K, const int frame_id = -1);

	virtual void decode_siho(const R *Y_N, B *V_K, const int frame_id = -1);

	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	void decode_siho_cw(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_N, const int frame_id = -1);

	virtual void decode_siho_cw(const R *Y_N, B *V_N, const int frame_id = -1);

protected:
	virtual void _decode_siho(const R *Y_N, B *V_K, const int frame_id);

	virtual void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);
};
}
}
#include "Decoder_SIHO.hxx"

#endif /* DECODER_SIHO_HPP_ */
