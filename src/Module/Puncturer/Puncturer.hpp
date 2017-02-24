/*!
 * \file
 * \brief Punctures a codeword to match a frame size.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef PUNCTURER_HPP_
#define PUNCTURER_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

namespace aff3ct
{
/*!
 * \class Puncturer_i
 *
 * \brief Punctures a codeword to match a frame size.
 *
 * \tparam B: type of the bits in the frames.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the Puncturer.
 *
 * Please use Puncturer for inheritance (instead of Puncturer_i)
 */
template <typename B, typename Q>
class Puncturer_i : public Module
{
protected:
	const int K;      /*!< Number of information bits in one frame */
	const int N;      /*!< Size of one frame (= number of bits in one frame) */
	const int N_code; /*!< Real size of the codeword (Puncturer_i::N_code >= Puncturer_i::N) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param N:        size of one frame.
	 * \param N_code:   real size of the codeword (Puncturer_i::N_code >= Puncturer_i::N).
	 * \param n_frames: number of frames to process in the Puncturer.
	 * \param name:     Puncturer's name.
	 */
	Puncturer_i(const int K, const int N, const int N_code, const int n_frames = 1, 
	                    const std::string name = "Puncturer_i")
	: Module(n_frames, name), K(K), N(N), N_code(N_code)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Puncturer_i() {}

	/*!
	 * \brief Punctures a codeword.
	 *
	 * \param X_N1: a complete/valid codeword..
	 * \param X_N2: a punctured codeword (corresponding to the frame size).
	 */
	virtual void   puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const = 0;

	/*!
	 * \brief Depunctures a codeword.
	 *
	 * \param Y_N1: a noised and punctured codeword (corresponding to the frame size).
	 * \param Y_N2: a noised and complete/valid codeword.
	 */
	virtual void depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const = 0;
};

#include "SC_Puncturer.hpp"
}

#endif /* PUNCTURER_HPP_ */
