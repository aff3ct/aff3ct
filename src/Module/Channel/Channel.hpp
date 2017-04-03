/*!
 * \file
 * \brief The Channel is the physical transmission medium.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Channel_i
 *
 * \brief The Channel is the physical transmission medium.
 *
 * \tparam R: type of the reals (floating-point representation) in the Channel.
 *
 * Please use Channel for inheritance (instead of Channel_i).
 */
template <typename R = float>
class Channel_i : public Module
{
protected:
	const int N; /*!< Size of one frame (= number of bits in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Channel.
	 * \param name:     Channel's name.
	 */
	Channel_i(const int N, const int n_frames = 1, const std::string name = "Channel_i")
	: Module(n_frames, name), N(N)
	{
		if (N <= 0)
			throw std::invalid_argument("aff3ct::module::Channel: \"N\" has to be greater than 0.");
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Channel_i()
	{
	}

	/*!
	 * \brief Adds the noise to a perfectly clear signal.
	 *
	 * \param X_N: a perfectly clear message.
	 * \param Y_N: a noisy signal.
	 */
	void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
	{
		if (X_N.size() != Y_N.size())
			throw std::length_error("aff3ct::module::Channel: \"X_N.size()\" has to be equal to \"Y_N.size()\".");
		if (this->N * this->n_frames != (int)X_N.size())
			throw std::length_error("aff3ct::module::Channel: \"X_N.size()\" has to be equal to \"N\" * \"n_frames\".");
		if (this->N * this->n_frames != (int)Y_N.size())
			throw std::length_error("aff3ct::module::Channel: \"Y_N.size()\" has to be equal to \"N\" * \"n_frames\".");

		this->_add_noise(X_N, Y_N);
	}

	/*!
	 * \brief Adds the noise to a perfectly clear signal.
	 *
	 * \param X_N: a perfectly clear message.
	 * \param Y_N: a noisy signal.
	 * \param H_N: the channel gains.
	 */
	void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N, mipp::vector<R>& H_N)
	{
		if (X_N.size() != Y_N.size() || Y_N.size() != H_N.size())
			throw std::length_error("aff3ct::module::Channel: \"X_N.size()\" has to be equal to \"Y_N.size()\" and "
			                        "\"H_N.size()\".");
		if (this->N * this->n_frames != (int)X_N.size())
			throw std::length_error("aff3ct::module::Channel: \"X_N.size()\" has to be equal to \"N\" * \"n_frames\".");
		if (this->N * this->n_frames != (int)Y_N.size())
			throw std::length_error("aff3ct::module::Channel: \"Y_N.size()\" has to be equal to \"N\" * \"n_frames\".");
		if (this->N * this->n_frames != (int)H_N.size())
			throw std::length_error("aff3ct::module::Channel: \"H_N.size()\" has to be equal to \"N\" * \"n_frames\".");

		this->_add_noise(X_N, Y_N, H_N);
	}

protected:
	virtual void _add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_add_noise_fbf(X_N.data() + f * this->N,
			                     Y_N.data() + f * this->N);
	}

	virtual void _add_noise_fbf(const R *X_N, R *Y_N)
	{
		throw std::runtime_error("aff3ct::module::Channel: \"_add_noise_fbf\" is unimplemented.");
	}

	virtual void _add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N, mipp::vector<R>& H_N)
	{
		this->_add_noise(X_N, Y_N);
		std::fill(H_N.begin(), H_N.end(), (R)1);
	}

	virtual void _add_noise_fbf(const R *X_N, R *Y_N, R *H_N)
	{
		throw std::runtime_error("aff3ct::module::Channel: \"_add_noise_fbf\" is unimplemented.");
	}
};
}
}

#include "SC_Channel.hpp"

#endif /* CHANNEL_HPP_ */
