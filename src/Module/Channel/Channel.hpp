/*!
 * \file Channel.hpp
 *
 * \brief The Channel is the physical transmission medium.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

/*!
 * \class Channel_interface
 *
 * \brief The Channel is the physical transmission medium.
 *
 * \tparam R: type of the reals (floating-point representation) in the channel.
 *
 * Please use Channel for inheritance (instead of Channel_interface).
 */
template <typename R>
class Channel_interface : public Module
{
protected:
	const int N; /*!< Size of one frame (= number of bits in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N       : size of one frame.
	 * \param n_frames: number of frames to process in the channel.
	 * \param name    : channel name.
	 */
	Channel_interface(const int N, const int n_frames = 1, const std::string name = "Channel_interface")
	: Module(n_frames, name), N(N) {}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Channel_interface() {}

	/*!
	 * \brief Add the noise to a perfectly clear signal.
	 *
	 * \param X_N: the perfectly clear signal.
	 * \param Y_N: the noisy signal.
	 */
	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N) = 0;
};

#include "SC_Channel.hpp"

#endif /* CHANNEL_HPP_ */
