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
	namespace mnt
	{
		namespace tsk
		{
			enum list { check_errors, check_mutual_info, SIZE };
		}

		namespace sck
		{
			namespace check_errors      { enum list { U,    V             , SIZE }; }
			namespace check_mutual_info { enum list { bits, llrs_a, llrs_e, SIZE }; }
		}
	}

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
protected:
	static bool interrupt;                                                                                /*!< True if there is a SIGINT signal (ctrl+C). */
	static bool first_interrupt;                                                                          /*!< True if this is the first time that SIGIN is called. */
	static int  interrupt_cnt;                                                                            /*!< The number of gor interrupt signal. */
	static bool over;                                                                                     /*!< True if SIGINT is called twice in the Monitor::d_delta_interrupt time */
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
	Monitor(const int size, int n_frames = 1)
	: Module(n_frames), size(size)
	{
		const std::string name = "Monitor";
		this->set_name(name);
		this->set_short_name(name);

		if (size <= 0)
		{
			std::stringstream message;
			message << "'size' has to be greater than 0 ('size' = " << size << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		Monitor::interrupt = false;

#ifndef ENABLE_MPI
		// Install a signal handler
		std::signal(SIGINT, Monitor::signal_interrupt_handler);
#endif

		this->tasks_with_nullptr.resize(mnt::tsk::SIZE);
		for (size_t t = 0; t < mnt::tsk::SIZE; t++)
			this->tasks_with_nullptr[t] = nullptr;
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Monitor()
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
		Monitor::interrupt = false;
		Monitor::interrupt_cnt = 0;
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
		return Monitor::interrupt;
	}

	/*!
	 * \brief Tells if the user asked for stopping the whole simulation.
	 *
	 * \return true if the SIGINT (ctrl+c) is called twice.
	 */
	static bool is_over()
	{
		return Monitor::over;
	}

	/*!
	 * \brief Put Monitor<B,R>::interrupt and Monitor<B,R>::over to true.
	 */
	static void stop()
	{
		Monitor::interrupt = true;
		Monitor::over      = true;
	}

private:
	static void signal_interrupt_handler(int signal)
	{
		Monitor::interrupt_cnt++;

		auto t_now = std::chrono::steady_clock::now();
		if (!Monitor::first_interrupt)
		{
			auto d_delta_interrupt = t_now - Monitor::t_last_interrupt;
			if (d_delta_interrupt < std::chrono::milliseconds(500))
				Monitor::stop();

			if (d_delta_interrupt < std::chrono::milliseconds(2000))
			{
				if (Monitor::interrupt_cnt >= 4)
					std::exit(EXIT_FAILURE);
			}
			else
				Monitor::interrupt_cnt = 1;
		}
		Monitor::t_last_interrupt  = t_now;

		Monitor::first_interrupt = false;
		Monitor::interrupt       = true;
	}
};
}
}

#endif /* MONITOR_HPP_ */
