/*!
 * \file
 * \brief Encodes a vector of information bits (a message) and adds some redundancy (parity bits).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace enc
	{
		enum class tsk : uint8_t { encode, SIZE };

		namespace sck
		{
			enum class encode : uint8_t { U_K, X_N, SIZE };
		}
	}

/*!
 * \class Encoder
 *
 * \brief Encodes a vector of information bits (a message) and adds some redundancy (parity bits).
 *
 * \tparam B: type of the bits in the encoder.
 *
 * Please use Encoder for inheritance (instead of Encoder)
 */
template <typename B = int>
class Encoder : public Module
{
public:
	inline Task&   operator[](const enc::tsk         t) { return Module::operator[]((int)t);                        }
	inline Socket& operator[](const enc::sck::encode s) { return Module::operator[]((int)enc::tsk::encode)[(int)s]; }

protected:
	const int             K;             /*!< Number of information bits in one frame */
	const int             N;             /*!< Size of one frame (= number of bits in one frame) */
	      bool            sys;           /*!< Is the generated codeword systematic ? */
	      bool            memorizing;    /*!< If true, keep the last encoded frame(s) in memory */
	std::vector<uint32_t> info_bits_pos; /*!< Positions of the information bits in the codeword */

	std::vector<std::vector<B>> U_K_mem;
	std::vector<std::vector<B>> X_N_mem;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Encoder.
	 * \param name:     Encoder's name.
	 */
	Encoder(const int K, const int N, const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Encoder() = default;

	int get_K() const;

	int get_N() const;

	virtual bool is_sys() const;

	bool is_memorizing() const;

	void set_memorizing(const bool memorizing);

	const std::vector<B>& get_U_K(const int frame_id = 0) const;

	const std::vector<B>& get_X_N(const int frame_id = 0) const;

	/*!
	 * \brief Encodes a vector of information bits (a message).
	 *
	 * \param U_K: a vector of information bits (a message).
	 * \param X_N: an encoded frame with redundancy added (parity bits).
	 */
	template <class A = std::allocator<B>>
	void encode(const std::vector<B,A>& U_K, std::vector<B,A>& X_N, const int frame_id = -1);

	virtual void encode(const B *U_K, B *X_N, const int frame_id = -1);

	template <class A = std::allocator<B>>
	bool is_codeword(const std::vector<B,A>& X_N);

	virtual bool is_codeword(const B *X_N);

	virtual const std::vector<uint32_t>& get_info_bits_pos() const;

	/*!
	 * \brief Gets the number of tail bits.
	 *
	 * Some encoders need to add some additional bits in order to work. This is often the case for the cyclic codes.
	 * Other codes do not require this feature and so, the tail bits length is 0.
	 *
	 * \return the number of tail bits.
	 */
	virtual int tail_length() const;

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);

	void set_sys(const bool sys);
};
}
}
#include "Module/Encoder/Encoder.hxx"

#endif /* ENCODER_HPP_ */
