/*!
 * \file
 * \brief Class module::Decoder_SIHO.
 */
#ifndef DECODER_SIHO_HPP_
#define DECODER_SIHO_HPP_

#include <memory>
#include <vector>
#include <cstdint>

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
protected:
	std::vector<R> Y_N;

public:
	inline Task&   operator[](const dec::tsk                 t);
	inline Socket& operator[](const dec::sck::decode_hiho    s);
	inline Socket& operator[](const dec::sck::decode_hiho_cw s);
	inline Socket& operator[](const dec::sck::decode_siho    s);
	inline Socket& operator[](const dec::sck::decode_siho_cw s);

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K: number of information bits in the frame.
	 * \param N: size of one frame.
	 */
	Decoder_SIHO(const int K, const int N);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_SIHO() = default;

	virtual Decoder_SIHO<B,R>* clone() const;

	/*!
	 * \brief Task method that decodes the noisy frame.
	 *
	 * \param Y_N: a noisy frame.
	 * \param V_K: a decoded codeword (only the information bits).
	 */
	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	int decode_siho(const std::vector<R,AR>& Y_N, std::vector<int8_t,AB>& CWD, std::vector<B,AB>& V_K,
	                const int frame_id = -1, const bool managed_memory = true);

	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	int decode_siho(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_K,
	                const int frame_id = -1, const bool managed_memory = true);

	int decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const int frame_id = -1, const bool managed_memory = true);

	int decode_siho(const R *Y_N, B *V_K, const int frame_id = -1, const bool managed_memory = true);

	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	int decode_siho_cw(const std::vector<R,AR>& Y_N, std::vector<int8_t,AB>& CWD, std::vector<B,AB>& V_N,
	                   const int frame_id = -1, const bool managed_memory = true);

	template <class AR = std::allocator<R>, class AB = std::allocator<B>>
	int decode_siho_cw(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_N,
	                   const int frame_id = -1, const bool managed_memory = true);

	int decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const int frame_id = -1, const bool managed_memory = true);

	int decode_siho_cw(const R *Y_N, B *V_N, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void set_n_frames_per_wave(const size_t n_frames_per_wave);

	virtual int _decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	virtual int _decode_siho(const R *Y_N,              B *V_K, const size_t frame_id);

	virtual int _decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);
	virtual int _decode_siho_cw(const R *Y_N,              B *V_N, const size_t frame_id);

	virtual int _decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);

	virtual int _decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Decoder_SIHO.hxx"
#endif

#endif /* DECODER_SIHO_HPP_ */
