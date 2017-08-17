/*!
 * \file
 * \brief Determines the frozen bit positions in a frame.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef FROZENBITS_GENERATOR_HPP_
#define FROZENBITS_GENERATOR_HPP_

#include <sstream>
#include <vector>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Frozenbits_generator
 * \brief Determines the frozen bit positions in a frame.
 */
class Frozenbits_generator
{
protected:
	const int K; /*!< Number of information bits in the frame. */
	const int N; /*!< Codeword size (or frame size). */
	float sigma; /*!< The AWGN channel sigma value. */

	std::vector<uint32_t> best_channels; /*!< The best channels in a codeword sorted by descending order. */

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

	int get_K() const
	{
		return this->K;
	}

	int get_N() const
	{
		return this->N;
	}

	/*!
	 * \brief Sets the AWGN channel sigma value.
	 *
	 * \param sigma: the AWGN channel sigma value.
	 */
	void set_sigma(const float sigma)
	{
		this->sigma = sigma;
	}

	float get_sigma() const
	{
		return this->sigma;
	}

	/*!
	 * \brief Generates the frozen bits vector.
	 *
	 * \param frozen_bits: output vector of frozen bits.
	 */
	void generate(std::vector<bool> &frozen_bits)
	{
		if (frozen_bits.size() != (unsigned)N)
		{
			std::stringstream message;
			message << "'frozen_bits.size()' has to be equal to 'N' ('frozen_bits.size()' = " << frozen_bits.size()
			        << ", 'N' = " << N << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->evaluate();

		// init frozen_bits vector, true means frozen bits, false means information bits
		std::fill(frozen_bits.begin(), frozen_bits.end(), true);
		for (auto i = 0; i < K; i++) 
			frozen_bits[best_channels[i]] = false;
	}

	/*!
	 * \brief Gets the best channels (the most secured bits sorted by descending order).
	 *
	 * \return a vector of the best channels.
	 */
	const std::vector<uint32_t>& get_best_channels() const
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
}

#endif /* FROZENBITS_GENERATOR_HPP_ */
