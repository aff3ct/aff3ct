/*!
 * \file
 * \brief Class tools::Terminal_std.
 */
#ifndef TERMINAL_STD_HPP_
#define TERMINAL_STD_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <rang.hpp>

#include "Tools/Display/Reporter/Reporter.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"

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
	std::vector<tools::Reporter*> reporters;

public:
	/*!
	 * \brief Constructor.
	 */
	explicit Terminal_std(const std::vector<tools::Reporter*>& reporters);

	/*!
	 * \brief Constructor.
	 */
	explicit Terminal_std(const std::vector<std::unique_ptr<tools::Reporter>>& reporters);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Terminal_std() = default;

	/*!
	 * \brief Displays the terminal_std legend.
	 *
	 * \param stream: the stream to print the legend.
	 */
	void legend(std::ostream &stream = std::cout) const;

	std::vector<tools::Reporter*>& get_reporters();

protected:
	virtual void report(std::ostream &stream = std::cout, bool final = false);

	static unsigned extra_spaces   (const Reporter::title_t& text, const unsigned group_width);
	static unsigned extra_spaces   (const Reporter::group_t& group);
	static unsigned get_group_width(const Reporter::group_t& group);
};
}
}

#endif /* TERMINAL_STD_HPP_ */
