/*!
 * \file
 * \brief Encodes a vector of information bits (a message) and adds some redundancy (parity bits).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef ENCODER_SYS_HPP_
#define ENCODER_SYS_HPP_

#include "Encoder.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Encoder_sys
 *
 * \brief Encodes a vector of information bits (a message) and adds some redundancy (parity bits).
 *        The encoding is systematic: the informations bits are included in the final encoded codeword.
 *
 * \tparam B: type of the bits in the encoder.
 */
template <typename B = int>
class Encoder_sys : public Encoder<B>
{
public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Encoder_sys.
	 * \param name:     Encoder_sys's name.
	 */
	Encoder_sys(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_sys") 
	: Encoder<B>(K, N, n_frames, name.c_str())
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Encoder_sys()
	{
	}

	/*!
	 * \brief Encodes a vector of information bits (a message).
	 *
	 * \param U_K: a vector of information bits (a message).
	 * \param par: a vector of the parity bits only (without the systematic bits), may contain the tail bits.
	 *             par = [par | tail bit sys | tail bits par]
	 */
	virtual void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par) = 0;
};
}
}

#endif /* ENCODER_SYS_HPP_ */
