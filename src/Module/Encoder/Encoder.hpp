/*!
 * \file
 * \brief Encodes a vector of information bits (a message) and adds some redundancy (parity bits).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

/*!
 * \class Encoder_interface
 *
 * \brief Encodes a vector of information bits (a message) and adds some redundancy (parity bits).
 *
 * \tparam B: type of the bits in the encoder.
 *
 * Please use Encoder for inheritance (instead of Encoder_interface)
 */
template <typename B>
class Encoder_interface : public Module
{
protected:
	const int K; /*!< Number of information bits in one frame */
	const int N; /*!< Size of one frame (= number of bits in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Encoder.
	 * \param name:     Encoder's name.
	 */
	Encoder_interface(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_interface") 
	: Module(n_frames, name), K(K), N(N)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Encoder_interface()
	{
	}

	/*!
	 * \brief Encodes a vector of information bits (a message).
	 *
	 * \param U_K: a vector of information bits (a message).
	 * \param X_N: an encoded frame with redundancy added (parity bits).
	 */
	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;

	/*!
	 * \brief Gets the number of tail bits.
	 *
	 * Some encoders need to add some additional bits in order to work. This is often the case for the cyclic codes.
	 * Other codes do not require this feature and so, the tail bits length is 0.
	 *
	 * \return the number of tail bits.
	 */
	virtual int tail_length() const { return 0; }
};

#include "SC_Encoder.hpp"

#endif /* ENCODER_HPP_ */
