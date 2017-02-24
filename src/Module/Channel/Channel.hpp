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
#include <algorithm>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

namespace aff3ct
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
template <typename R>
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
	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N) = 0;

	/*!
	 * \brief Adds the noise to a perfectly clear signal.
	 *
	 * \param X_N: a perfectly clear message.
	 * \param Y_N: a noisy signal.
	 * \param H_N: the channel gains.
	 */
	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N, mipp::vector<R>& H_N)
	{
		this->add_noise(X_N, Y_N);
		std::fill(H_N.begin(), H_N.end(), (R)1);
	}
};

#include "SC_Channel.hpp"
}

#endif /* CHANNEL_HPP_ */
