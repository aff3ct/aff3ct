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

#include "Tools/Display/rang_format/rang_format.h"

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
	static const char        line_separator;
	static const std::string col_separator;
	static const std::string group_separator;
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
	using title_t        = std::pair<std::string, std::string>;
	using group_title_t  = title_t;
	using column_title_t = std::vector<title_t>;
	using group_t        = std::pair<group_title_t, column_title_t>;

	std::vector<group_t> cols_groups;


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

protected:
	static unsigned extra_spaces(const title_t& text, const unsigned group_width);
	static unsigned extra_spaces(const group_t& group);
	static unsigned get_group_width(const group_t& group);

private:
	static void start_thread_terminal(Terminal *terminal, const std::chrono::milliseconds freq);
};
}
}

#endif /* TERMINAL_HPP_ */
