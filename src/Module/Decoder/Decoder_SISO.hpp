/*!
 * \file
 * \brief A SISO (Soft Input Soft Output) is a type of decoder which takes a soft input and return a soft output.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_SISO_HPP_
#define DECODER_SISO_HPP_

#include <string>
#include <vector>

#include "Decoder.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Decoder_SISO
 *
 * \brief A Decoder_SISO (Soft Input Soft Output) is a type of decoder which takes a soft input and return a soft output.
 *
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the Decoder_SISO.
 *
 * Please use Decoder_SISO for inheritance (instead of Decoder_SISO).
 */
template <typename R = float>
class Decoder_SISO : virtual public Decoder
{
public:
	inline Task&   operator[](const dec::tsk              t) { return Module::operator[]((int)t);                             }
	inline Socket& operator[](const dec::sck::decode_siso s) { return Module::operator[]((int)dec::tsk::decode_siso)[(int)s]; }

private:
	std::vector<R> Y_N1;
	std::vector<R> Y_N2;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:                      number of information bits in the frame.
	 * \param N:                      size of one frame.
	 * \param n_frames:               number of frames to process in the Decoder_SISO.
	 * \param simd_inter_frame_level: number of frames absorbed by the SIMD instructions.
	 * \param name:                   decoder name.
	 */
	Decoder_SISO(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SISO() = default;

	/*!
	 * \brief Decodes a given noisy codeword. This prototype supposes that the encoded frame is systematic, can't be
	 *        used otherwise.
	 *
	 * \param sys: systematic LLRs (original noised information bits) from the channel.
	 * \param par: parity LLRs from the channel.
	 * \param ext: extrinsic information about the systematic bits.
	 */
	template <class A = std::allocator<R>>
	void decode_siso(const std::vector<R,A> &sys, const std::vector<R,A> &par, std::vector<R,A> &ext,
	                 const int n_frames = -1, const int frame_id = -1);

	virtual void decode_siso(const R *sys, const R *par, R *ext, const int n_frames = -1, const int frame_id = -1);

	/*!
	 * \brief Decodes a given noisy codeword.
	 *
	 * \param Y_N1: a completely noisy codeword from the channel.
	 * \param Y_N2: an extrinsic information about all the bits in the frame.
	 */
	template <class A = std::allocator<R>>
	void decode_siso(const std::vector<R,A> &Y_N1, std::vector<R,A> &Y_N2, const int frame_id = -1);

	virtual void decode_siso(const R *Y_N1, R *Y_N2, const int frame_id = -1);

	/*!
	 * \brief Gets the number of tail bits.
	 *
	 * \return the number of tail bits.
	 */
	virtual int tail_length() const;

protected:
	virtual void _decode_siso(const R *sys, const R *par, R *ext, const int frame_id);

	virtual void _decode_siso(const R *Y_N1, R *Y_N2, const int frame_id);
};
}
}
#include "Decoder_SISO.hxx"

#endif /* DECODER_SISO_HPP_ */
