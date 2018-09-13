/*!
 * \file
 * \brief The terminal_std display.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef TERMINAL_STD_HPP_
#define TERMINAL_STD_HPP_

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <utility>
#include <string>

#include "../Terminal.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Display/Reporter/Reporter.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Terminal_std
 *
 * \brief The terminal_std display.
 */
class Terminal_std : public Terminal
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

protected:
	std::vector<Reporter*>& reporters;

public:
	/*!
	 * \brief Constructor.
	 */
	explicit Terminal_std(std::vector<Reporter*>& reporters);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Terminal_std();

	/*!
	 * \brief Displays the terminal_std legend.
	 *
	 * \param stream: the stream to print the legend.
	 */
	void legend(std::ostream &stream = std::cout) const;

protected:
	virtual void report(std::ostream &stream = std::cout, bool final = false);

	static unsigned extra_spaces   (const Reporter::title_t& text, const unsigned group_width);
	static unsigned extra_spaces   (const Reporter::group_t& group);
	static unsigned get_group_width(const Reporter::group_t& group);

private:
	static void start_thread_terminal(Terminal_std *terminal_std, const std::chrono::milliseconds freq);

	static void signal_interrupt_handler(int signal);
};
}
}

#endif /* TERMINAL_STD_HPP_ */
