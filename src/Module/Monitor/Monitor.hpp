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
#include <csignal>
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
 * \class Monitor_i
 *
 * \brief Monitors the simulated frames, tells if there is a frame errors and counts the number of bit errors.
 *
 * \tparam B: type of the bits in the frames to compare.
 * \tparam R: type of the samples in the channel frame.
 *
 * Please use Monitor for inheritance (instead of Monitor_i).
 */
class Monitor_i : public Module
{
protected:
	static bool interrupt;                                                                                /*!< True if there is a SIGINT signal (ctrl+C). */
	static bool first_interrupt;                                                                          /*!< True if this is the first time that SIGIN is called. */
	static bool over;                                                                                     /*!< True if SIGINT is called twice in the Monitor_i::d_delta_interrupt time */
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_interrupt; /*!< Time point of the last call to SIGINT */

	const int size; /*!< Number of bits */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Registers the SIGINT (signal interrupt or ctrl+C) interruption.
	 *
	 * \param size: number of bits.
	 */
	Monitor_i(const int size, int n_frames = 1, const std::string name = "Monitor_i")
	: Module(n_frames, name, "Monitor"), size(size)
	{
		if (size <= 0)
		{
			std::stringstream message;
			message << "'size' has to be greater than 0 ('size' = " << size << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		Monitor_i::interrupt = false;

#ifndef ENABLE_MPI
		// Install a signal handler
		std::signal(SIGINT, Monitor_i::signal_interrupt_handler);
#endif
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Monitor_i()
	{
	}

	/*!
	 * \brief Gets the number of bits in a frame.
	 *
	 * \return the number of bits.
	 */
	int get_size() const
	{
		return size;
	}

	virtual void reset()
	{
		Monitor_i::interrupt = false;
		for (auto &t : this->tasks)
			t.second->reset_stats();
	}

	virtual void clear_callbacks()
	{
	}

	/*!
	 * \brief Tells if the user asked for stopping the current computations.
	 *
	 * \return true if the SIGINT (ctrl+c) is called.
	 */
	static bool is_interrupt()
	{
		return Monitor_i::interrupt;
	}

	/*!
	 * \brief Tells if the user asked for stopping the whole simulation.
	 *
	 * \return true if the SIGINT (ctrl+c) is called twice.
	 */
	static bool is_over()
	{
		return Monitor_i::over;
	}

	/*!
	 * \brief Put Monitor_i<B,R>::interrupt and Monitor_i<B,R>::over to true.
	 */
	static void stop()
	{
		Monitor_i::interrupt = true;
		Monitor_i::over      = true;
	}

private:
	static void signal_interrupt_handler(int signal)
	{
		auto t_now = std::chrono::steady_clock::now();
		if (!Monitor_i::first_interrupt)
		{
			auto d_delta_interrupt = t_now - Monitor_i::t_last_interrupt;
			if (d_delta_interrupt < std::chrono::milliseconds(500))
				Monitor_i::stop();
		}
		Monitor_i::t_last_interrupt  = t_now;

		Monitor_i::first_interrupt = false;
		Monitor_i::interrupt       = true;
	}
};
}
}

#include "SC_Monitor.hpp"

#endif /* MONITOR_HPP_ */
