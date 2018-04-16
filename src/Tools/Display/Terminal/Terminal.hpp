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
#include <vector>
#include <utility>
#include <string>
#include <rang.hpp>

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
public:
	static const char        col_separator;
	static const char        line_separator;
	static const std::string comment_tag;
	static const std::string spaced_scol_separator;
	static const std::string spaced_dcol_separator;
	static const std::string data_tag;
	static const rang::style legend_style;
	static const rang::style report_style;
	static const int         column_width;


private:
	std::thread term_thread;
	std::mutex mutex_terminal;
	std::condition_variable cond_terminal;
	bool stop_terminal;

protected:
	// vector of pairs {group title, columns titles}
	// group title is a pair {first line, second line}
	// columns titles is a vector of pair {first line, second line}
	std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::pair<std::string, std::string>>>> cols_groups;


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

	static std::string get_time_format(float secondes);

private:
	static void start_thread_terminal(Terminal *terminal, const std::chrono::milliseconds freq);
};
}
}

#endif /* TERMINAL_HPP_ */
