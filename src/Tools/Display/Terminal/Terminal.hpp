/*!
 * \file
 * \brief The terminal display.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef TERMINAL_HPP_
#define TERMINAL_HPP_

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
	/*!
	 * \brief Constructor.
	 */
	Terminal()
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Terminal()
	{
	}

	/*!
	 * \brief Displays the terminal legend.
	 *
	 * \param stream: the stream to print the legend.
	 */
	virtual void legend(std::ostream &stream)
	{
	}

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
	virtual void final_report(std::ostream &stream) = 0;
};
}
}

#endif /* TERMINAL_HPP_ */
