/*!
 * \file
 * \brief Class module::Decoder_SISO.
 */
#ifndef DECODER_SISO_HPP_
#define DECODER_SISO_HPP_

#include <memory>
#include <vector>
#include <cstdint>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

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
 */
template <typename B = int, typename R = float>
class Decoder_SISO : public Decoder_SIHO<B,R>
{
public:
	inline Task&   operator[](const dec::tsk                  t);
	inline Socket& operator[](const dec::sck::decode_hiho     s);
	inline Socket& operator[](const dec::sck::decode_hiho_cw  s);
	inline Socket& operator[](const dec::sck::decode_siho     s);
	inline Socket& operator[](const dec::sck::decode_siho_cw  s);
	inline Socket& operator[](const dec::sck::decode_siso     s);
	inline Socket& operator[](const dec::sck::decode_siso_alt s);

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K: number of information bits in the frame.
	 * \param N: size of one frame.
	 */
	Decoder_SISO(const int K, const int N);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SISO() = default;

	virtual Decoder_SISO<B,R>* clone() const;

	/*!
	 * \brief Task method that decodes a given noisy codeword.
	 *
	 * \param Y_N1: a completely noisy codeword from the channel.
	 * \param Y_N2: an extrinsic information about all the bits in the frame.
	 */
	template <class A = std::allocator<R>>
	int decode_siso(const std::vector<R,A> &Y_N1, std::vector<int8_t,A> &CWD, std::vector<R,A> &Y_N2,
	                const int frame_id = -1, const bool managed_memory = true);

	template <class A = std::allocator<R>>
	int decode_siso(const std::vector<R,A> &Y_N1, std::vector<R,A> &Y_N2,
	                const int frame_id = -1, const bool managed_memory = true);

	int decode_siso(const R *Y_N1, int8_t *CWD, R *Y_N2, const int frame_id = -1, const bool managed_memory = true);

	int decode_siso(const R *Y_N1, R *Y_N2, const int frame_id = -1, const bool managed_memory = true);

	/*!
	 * \brief Decodes a given noisy codeword. This prototype supposes that the encoded frame is systematic, can't be
	 *        used otherwise.
	 *
	 * \param sys: systematic LLRs (original noised information bits) from the channel.
	 * \param par: parity LLRs from the channel.
	 * \param ext: extrinsic information about the systematic bits.
	 */
	// template <class A = std::allocator<R>>
	// int decode_siso(const std::vector<R,A> &sys, const std::vector<R,A> &par, std::vector<R,A> &ext,
	//                  const int n_frames = -1, const int frame_id = -1);

	// virtual int decode_siso(const R *sys, const R *par, R *ext, const int n_frames = -1, const int frame_id = -1);

	template <class A = std::allocator<R>>
	int decode_siso_alt(const std::vector<R,A> &sys, const std::vector<R,A> &par, std::vector<int8_t,A> &CWD,
	                    std::vector<R,A> &ext, const int frame_id = -1, const bool managed_memory = true);

	template <class A = std::allocator<R>>
	int decode_siso_alt(const std::vector<R,A> &sys, const std::vector<R,A> &par, std::vector<R,A> &ext,
	                    const int frame_id = -1, const bool managed_memory = true);

	int decode_siso_alt(const R *sys, const R *par, int8_t *CWD, R *ext, const int frame_id = -1,
	                    const bool managed_memory = true);


	int decode_siso_alt(const R *sys, const R *par, R *ext, const int frame_id = -1,
	                    const bool managed_memory = true);

	/*!
	 * \brief Gets the number of tail bits.
	 *
	 * \return the number of tail bits.
	 */
	virtual int tail_length() const;

protected:
	virtual int _decode_siso(const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id);
	virtual int _decode_siso(const R *Y_N1,              R *Y_N2, const size_t frame_id);

	virtual int _decode_siso_alt(const R *sys, const R *par, int8_t *CWD, R *ext, const size_t frame_id);
	virtual int _decode_siso_alt(const R *sys, const R *par,              R *ext, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Decoder_SISO.hxx"
#endif

#endif /* DECODER_SISO_HPP_ */
