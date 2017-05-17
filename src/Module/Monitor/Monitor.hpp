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
#include <stdexcept>
#include <csignal>
#include <chrono>
#include <vector>
#include <string>
#include "Tools/Perf/MIPP/mipp.h"

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
template <typename B = int>
class Monitor_i : public Module
{
protected:
	static bool interrupt;                                                                                /*!< True if there is a SIGINT signal (ctrl+C). */
	static bool first_interrupt;                                                                          /*!< True if this is the first time that SIGIN is called. */
	static bool over;                                                                                     /*!< True if SIGINT is called twice in the Monitor_i::d_delta_interrupt time */
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_interrupt; /*!< Time point of the last call to SIGINT */
	static std::chrono::nanoseconds d_delta_interrupt;                                                    /*!< Delta time. */

	const int size; /*!< Number of bits */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Registers the SIGINT (signal interrupt or ctrl+C) interruption.
	 *
	 * \param size:     number of bits.
	 */
	Monitor_i(const int size, int n_frames = 1, const std::string name = "Monitor_i")
	: Module(n_frames, name), size(size)
	{
		if (this->size <= 0)
			throw std::invalid_argument("aff3ct::module::Monitor: \"size\" has to be greater than 0.");

		Monitor_i<B>::interrupt = false;
		Monitor_i<B>::d_delta_interrupt = std::chrono::nanoseconds(0);

#ifndef ENABLE_MPI
		// Install a signal handler
		std::signal(SIGINT, Monitor_i<B>::signal_interrupt_handler);
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

	/*!
	 * \brief Gets the number of bit errors.
	 *
	 * \return the number of bit errors.
	 */
	virtual unsigned long long get_n_be() const = 0;

	/*!
	 * \brief Gets the number of frame errors.
	 *
	 * \return the number of frame errors.
	 */
	virtual unsigned long long get_n_fe() const = 0;

	/*!
	 * \brief Gets the bit error rate.
	 *
	 * \return the bit error rate.
	 */
	virtual float get_ber() const = 0;

	/*!
	 * \brief Gets the frame error rate.
	 *
	 * \return the frame error rate.
	 */
	virtual float get_fer() const = 0;

	/*!
	 * \brief Gets the number of analyzed frames (analyzed in the Monitor_i::check_errors method).
	 *
	 * \return the number of analyzed frames.
	 */
	virtual unsigned long long get_n_analyzed_fra() const = 0;

	/*!
	 * \brief Gets the frame errors limit (maximal number of frame errors to simulate).
	 *
	 * \return the frame errors limit.
	 */
	virtual unsigned get_fe_limit() const = 0;

	/*!
	 * \brief Tells if the frame errors limit is achieved (in this case the current computations should stop).
	 *
	 * \return true if the frame errors limit is achieved.
	 */
	virtual bool fe_limit_achieved() = 0;

	/*!
	 * \brief Compares two messages and counts the number of frame errors and bit errors.
	 *
	 * Typically this method is called at the very end of a communication chain.
	 *
	 * \param U: the original message (from the Source or the CRC).
	 * \param V: the decoded message (from the Decoder).
	 */
	void check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V)
	{
		if ((int)U.size() != this->size * this->n_frames)
			throw std::length_error("aff3ct::module::Monitor: \"U.size()\" has to be equal to \"size * n_frames\".");

		if ((int)V.size() != this->size * this->n_frames)
			throw std::length_error("aff3ct::module::Monitor: \"V.size()\" has to be equal to \"size * n_frames\".");

		this->check_errors(U.data(), V.data());
	}

	virtual void check_errors(const B *U, const B *V)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_check_errors(U + f * this->size,
			                    V + f * this->size);
	}

	virtual void add_handler_fe               (std::function<void(int )> callback) = 0;
	virtual void add_handler_check            (std::function<void(void)> callback) = 0;
	virtual void add_handler_fe_limit_achieved(std::function<void(void)> callback) = 0;

	virtual void reset() = 0;

	/*!
	 * \brief Tells if the user asked for stopping the current computations.
	 *
	 * \return true if the SIGINT (ctrl+c) is called.
	 */
	static bool is_interrupt()
	{
		return Monitor_i<B>::interrupt;
	}

	/*!
	 * \brief Tells if the user asked for stopping the whole simulation.
	 *
	 * \return true if the SIGINT (ctrl+c) is called twice.
	 */
	static bool is_over()
	{
		return Monitor_i<B>::over;
	}

	/*!
	 * \brief Put Monitor_i<B,R>::interrupt and Monitor_i<B,R>::over to true.
	 */
	static void stop()
	{
		Monitor_i<B>::interrupt = true;
		Monitor_i<B>::over      = true;
	}

protected:
	virtual void _check_errors(const B *U, const B *V)
	{
		throw std::runtime_error("aff3ct::module::Monitor: \"_check_errors\" is unimplemented.");
	}

private:
	static void signal_interrupt_handler(int signal)
	{
		auto t_now = std::chrono::steady_clock::now();
		if (!Monitor_i<B>::first_interrupt)
		{
			Monitor_i<B>::d_delta_interrupt = t_now - Monitor_i<B>::t_last_interrupt;
			if (Monitor_i<B>::d_delta_interrupt < std::chrono::milliseconds(500))
				Monitor_i<B>::stop();
		}
		Monitor_i<B>::t_last_interrupt  = t_now;

		Monitor_i<B>::first_interrupt = false;
		Monitor_i<B>::interrupt       = true;
	}
};

template <typename B>
bool Monitor_i<B>::interrupt = false;

template <typename B>
bool Monitor_i<B>::first_interrupt = true;

template <typename B>
bool Monitor_i<B>::over = false;

template <typename B>
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> Monitor_i<B>::t_last_interrupt;

template <typename B>
std::chrono::nanoseconds Monitor_i<B>::d_delta_interrupt = std::chrono::nanoseconds(0);
}
}

#include "SC_Monitor.hpp"

#endif /* MONITOR_HPP_ */
