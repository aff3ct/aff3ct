/*!
 * \file
 * \brief Class tools::logic_error.
 */
#ifndef LOGIC_ERROR_HPP_
#define LOGIC_ERROR_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class logic_error : public exception
{
	static const std::string default_message;

public:
	logic_error() throw();

	explicit logic_error(const std::string &message) throw();

	logic_error(const std::string &filename,
	            const int line_num,
	            const std::string &funcname = exception::empty_string,
	            const std::string &message = exception::empty_string) throw();

	virtual ~logic_error() = default;
};
}
}

#endif /* LOGIC_ERROR_HPP_ */
