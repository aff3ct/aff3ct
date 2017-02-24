/*!
 * \file
 * \brief Determines the frozen bit positions in a frame.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef FROZENBITS_GENERATOR_HPP_
#define FROZENBITS_GENERATOR_HPP_

#include <cassert>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
/*!
 * \class Frozenbits_generator
 * \brief Determines the frozen bit positions in a frame.
 */
template <typename B>
class Frozenbits_generator
{
protected:
	const int K; /*!< Number of information bits in the frame. */
	const int N; /*!< Codeword size (or frame size). */
	float sigma; /*!< The AWGN channel sigma value. */

	std::vector<int> best_channels; /*!< The best channels in a codeword sorted by descending order. */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:     number of information bits in the frame.
	 * \param N:     codeword size (or frame size).
	 * \param sigma: the AWGN channel sigma value.
	 */
	Frozenbits_generator(const int K, const int N, const float sigma = 0.f) 
	: K(K), N(N), sigma(sigma), best_channels(N) {}
	
	/*!
	 * \brief Destructor.
	 */
	virtual ~Frozenbits_generator() {}

	/*!
	 * \brief Sets the AWGN channel sigma value.
	 *
	 * \param sigma: the AWGN channel sigma value.
	 */
	void set_sigma(const float sigma)
	{
		this->sigma = sigma;
	}

	/*!
	 * \brief Generates the frozen bits vector.
	 *
	 * \param frozen_bits: output vector of frozen bits.
	 */
	void generate(mipp::vector<B> &frozen_bits)
	{
		assert(frozen_bits.size() == (unsigned)N);

		this->evaluate();

		// init frozen_bits vector, true means frozen bits, false means information bits
		std::fill(frozen_bits.begin(), frozen_bits.end(), (B)1);
		for (auto i = 0; i < K; i++) 
			frozen_bits[best_channels[i]] = (B)0;
	}

	/*!
	 * \brief Gets the best channels (the most secured bits sorted by descending order).
	 *
	 * \return a vector of the best channels.
	 */
	const std::vector<int>& get_best_channels() const
	{
		return best_channels;
	}

protected:
	/*!
	 * \brief Evaluates the best channels.
	 *
	 * It is mandatory to compute the best channels to find the position of the frozen bits.
	 * This method fills the internal Frozenbits_generator::best_channels attribute.
	 */
	virtual void evaluate() = 0;
};
}

#endif /* FROZENBITS_GENERATOR_HPP_ */
