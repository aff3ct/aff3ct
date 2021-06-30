/*!
 * \file
 * \brief Class module::Decoder_HIHO.
 */
#ifndef DECODER_HIHO_HPP_
#define DECODER_HIHO_HPP_

#include <memory>
#include <vector>
#include <cstdint>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Decoder_HIHO
 *
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \tparam B: type of the bits in the Decoder.
 *
 * The Decoder takes a soft input (real numbers) and return a hard output (bits).
 */
template <typename B = int>
class Decoder_HIHO : public Decoder
{
public:
	inline Task&   operator[](const dec::tsk                 t);
	inline Socket& operator[](const dec::sck::decode_hiho    s);
	inline Socket& operator[](const dec::sck::decode_hiho_cw s);

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K: number of information bits in the frame.
	 * \param N: size of one frame.
	 */
	Decoder_HIHO(const int K, const int N);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_HIHO() = default;

	virtual Decoder_HIHO<B>* clone() const;

	/*!
	 * \brief Task method that decodes the noisy frame.
	 *
	 * \param Y_N: a noisy frame.
	 * \param V_K: a decoded codeword (only the information bits).
	 */
	template <class A = std::allocator<B>>
	int decode_hiho(const std::vector<B,A>& Y_N, std::vector<int8_t,A>& CWD, std::vector<B,A>& V_K,
	                const int frame_id = -1, const bool managed_memory = true);

	template <class A = std::allocator<B>>
	int decode_hiho(const std::vector<B,A>& Y_N, std::vector<B,A>& V_K,
	                const int frame_id = -1, const bool managed_memory = true);

	int decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const int frame_id = -1, const bool managed_memory = true);

	int decode_hiho(const B *Y_N, B *V_K, const int frame_id = -1, const bool managed_memory = true);

	template <class A = std::allocator<B>>
	int decode_hiho_cw(const std::vector<B,A>& Y_N, std::vector<int8_t,A>& CWD, std::vector<B,A>& V_N,
	                   const int frame_id = -1, const bool managed_memory = true);

	template <class A = std::allocator<B>>
	int decode_hiho_cw(const std::vector<B,A>& Y_N, std::vector<B,A>& V_N,
	                   const int frame_id = -1, const bool managed_memory = true);

	int decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const int frame_id = -1, const bool managed_memory = true);

	int decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual int _decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	virtual int _decode_hiho(const B *Y_N,              B *V_K, const size_t frame_id);

	virtual int _decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);
	virtual int _decode_hiho_cw(const B *Y_N,              B *V_N, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Decoder_HIHO.hxx"
#endif

#endif /* DECODER_HIHO_HPP_ */
