/*!
 * \file
 * \brief Monitors the simulated frames, tells if there is a frame errors and counts the number of bit errors.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include <functional>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{

/*!
 * \class Monitor
 *
 * \brief Monitors the simulated frames, tells if there is a frame errors and counts the number of bit errors.
 *
 * \tparam B: type of the bits in the frames to compare.
 * \tparam R: type of the samples in the channel frame.
 *
 * Please use Monitor for inheritance (instead of Monitor).
 */
class Monitor : public Module
{
public:
	/*!
	 * \brief Constructor.
	 *
	 * Registers the SIGINT (signal interrupt or ctrl+C) interruption.
	 *
	 * \param K: number of bits of information
	 * \param N: number of coded bits
	 */
	Monitor(int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Monitor() = default;

	virtual void reset();

	virtual void clear_callbacks();

	virtual void collect(const Monitor& m) = 0;
};
}
}

#endif /* MONITOR_HPP_ */
