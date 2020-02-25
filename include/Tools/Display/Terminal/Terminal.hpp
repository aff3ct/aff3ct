/*!
 * \file
 * \brief Class tools::Terminal.
 */
#ifndef TERMINAL_HPP_
#define TERMINAL_HPP_

#include <condition_variable>
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>

#include "Tools/Display/Reporter/Reporter.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Terminal
 *
 * \brief The terminal display.
 */
class Terminal
{
private:
	std::thread term_thread;
	std::mutex mutex_terminal;
	std::condition_variable cond_terminal;
	bool stop_terminal;

	static bool interrupt;                                                      /*!< True if there is a SIGINT signal (ctrl+C). */
	static bool first_interrupt;                                                /*!< True if this is the first time that SIGIN is called. */
	static int  interrupt_cnt;                                                  /*!< The number of gor interrupt signal. */
	static bool over;                                                           /*!< True if SIGINT is called twice in the Terminal::d_delta_interrupt time */
	static std::chrono::time_point<std::chrono::steady_clock> t_last_interrupt; /*!< Time point of the last call to SIGINT */

protected:
	std::chrono::time_point<std::chrono::steady_clock> t_term;
	uint8_t real_time_state;

public:
	/*!
	 * \brief Constructor.
	 */
	Terminal();

	/*!
	 * \brief Destructor.
	 */
	virtual ~Terminal();

	/*!
	 * \brief Displays the terminal legend.
	 *
	 * \param stream: the stream to print the legend.
	 */
	virtual void legend(std::ostream &stream = std::cout) const = 0;

	/*!
	 * \brief Temporary report.
	 *
	 * \param stream: the stream to print the report.
	 */
	void temp_report(std::ostream &stream = std::cout);

	/*!
	 * \brief Final report.
	 *
	 * \param stream: the stream to print the report.
	 */
	void final_report(std::ostream &stream = std::cout);

	void start_temp_report(const std::chrono::milliseconds freq = std::chrono::milliseconds(500));

	void stop_temp_report();

	static void init();

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
	 * \brief Put Terminal::interrupt and Terminal::over to true.
	 */
	static void stop();

	/*!
	 * \brief Put Terminal::interrupt and Terminal::over to false.
	 */
	static void reset();

protected:
	virtual void report(std::ostream &stream = std::cout, bool final = false) = 0;

private:
	static void start_thread_terminal(Terminal *terminal, const std::chrono::milliseconds freq);

	static void signal_interrupt_handler(int signal);
};
}
}

#endif /* TERMINAL_HPP_ */
