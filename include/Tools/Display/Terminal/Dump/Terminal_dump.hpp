/*!
 * \file
 * \brief Class tools::Terminal_dump.
 */
#ifndef TERMINAL_DUMP_HPP_
#define TERMINAL_DUMP_HPP_

#include <iostream>
#include <vector>

#include "Tools/Display/Reporter/Reporter.hpp"
#include "Tools/Display/Terminal/Standard/Terminal_std.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Terminal_dump
 *
 * \brief The Terminal_dump display.
 */
class Terminal_dump : public Terminal_std
{
public:
	/*!
	 * \brief Constructor.
	 */
	explicit Terminal_dump(const std::vector<tools::Reporter*>& reporters);

	/*!
	 * \brief Constructor.
	 */
	explicit Terminal_dump(const std::vector<std::unique_ptr<tools::Reporter>>& reporters);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Terminal_dump() = default;

protected:
	virtual void report(std::ostream &stream = std::cout, bool final = false);
};
}
}

#endif /* TERMINAL_DUMP_HPP_ */
