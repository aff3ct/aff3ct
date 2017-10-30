/*!
 * \file
 * \brief The terminal display.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef TERMINAL_HPP_
#define TERMINAL_HPP_

#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

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
	virtual void legend(std::ostream &stream);

	/*!
	 * \brief Temporary report.
	 *
	 * \param stream: the stream to print the report.
	 */
	virtual void temp_report (std::ostream &stream) = 0;

	/*!
	 * \brief Final report.
	 *
	 * \param stream: the stream to print the report.
	 */
	virtual void final_report(std::ostream &stream);

	void start_temp_report(const std::chrono::milliseconds freq = std::chrono::milliseconds(500));

	void stop_temp_report();

private:
	static void start_thread_terminal(Terminal *terminal, const std::chrono::milliseconds freq);
};
}
}

#endif /* TERMINAL_HPP_ */
