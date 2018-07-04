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
		enum class tsk : uint8_t { check_errors, check_mutual_info, get_mutual_info, SIZE };

		namespace sck
		{
			enum class check_errors      : uint8_t { U,    V             , SIZE };
			enum class check_mutual_info : uint8_t { bits, llrs_a, llrs_e, SIZE };
			enum class get_mutual_info   : uint8_t { X,    Y             , SIZE };
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

	const int K; /*!< Number of bits */
	const int N; /*!< Number of bits */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Registers the SIGINT (signal interrupt or ctrl+C) interruption.
	 *
	 * \param K: number of bits of information
	 * \param N: number of coded bits
	 */
	Monitor(const int K, const int N, int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Monitor();

	/*!
	 * \brief Gets the number of bits in a frame.
	 *
	 * \return the number of bits.
	 */
	int get_K() const;
	int get_N() const;

	virtual void reset();

	virtual void clear_callbacks();

	/*!
	 * \brief Tells if the user asked for stopping the current computations.
	 *
	 * \return true if the SIGINT (ctrl+c) is called.
	 */
	static bool is_interrupt();

	/*!
	 * \brief Tells if the user asked for stopping the whole simulation.
	 *
	 * \return true if the SIGINT (ctrl+c) is called twice.
	 */
	static bool is_over();

	/*!
	 * \brief Put Monitor<B,R>::interrupt and Monitor<B,R>::over to true.
	 */
	static void stop();

private:
	static void signal_interrupt_handler(int signal);
};
}
}

#endif /* MONITOR_HPP_ */
