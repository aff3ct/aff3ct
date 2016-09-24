/*!
 * \file
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

/*!
 * \class Decoder_interface
 *
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \tparam B: type of the bits in the Decoder.
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the Decoder.
 *
 * The Decoder takes a soft input (real numbers) and return a hard output (bits).
 * Please use Decoder for inheritance (instead of Decoder_interface).
 */
template <typename B, typename R>
class Decoder_interface : public Module
{
protected:
	const int K; /*!< Number of information bits in one frame */
	const int N; /*!< Size of one frame (= number of bits in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K       : number of information bits in the frame.
	 * \param N       : size of one frame.
	 * \param n_frames: number of frames to process in the Decoder.
	 * \param name    : Decoder's name.
	 */
	Decoder_interface(const int K, const int N, const int n_frames = 1, std::string name = "Decoder_interface") 
	: Module(n_frames, name), K(K), N(N) {}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_interface() {}

	/*!
	 * \brief Loads the noisy frame into the Decoder.
	 *
	 * \param Y_N: a noisy frame.
	 */
	virtual void load(const mipp::vector<R>& Y_N) = 0;

	/*!
	 * \brief Decodes the noisy frame (have to be called after the load method).
	 */
	virtual void decode() = 0;

	/*!
	 * \brief Stores the decoded information bits (have to be called after the decode method).
	 *
	 * \param V_K: an decoded codeword (only the information bits).
	 */
	virtual void store(mipp::vector<B>& V_K) const = 0;

	/*!
	 * \brief Stores the decoded codeword, may or may not contain the redundancy bits (parity) (should be called over
	 *        the standard store method).
	 *
	 * \param V: the decoded codeword.
	 */
	virtual void store_fast(mipp::vector<B>& V) const { store(V); }

	/*!
	 * \brief Can be called after the store_fast method if store_fast return the bits in a non-standard format. The
	 *        unpack method converts those bits into a standard format.
	 *
	 * \param V: the decoded and unpacked codeword.
	 */
	virtual void unpack(mipp::vector<B>& V) const {}
};

#include "SC_Decoder.hpp"

#endif /* DECODER_HPP_ */
