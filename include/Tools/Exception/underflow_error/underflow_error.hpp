/*!
 * \file
 * \brief Class tools::underflow_error.
 */
#ifndef UNDERFLOW_ERROR_HPP_
#define UNDERFLOW_ERROR_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class underflow_error : public exception
{
	static const std::string default_message;

public:
	underflow_error() throw();

	explicit underflow_error(const std::string &message) throw();

	underflow_error(const std::string &filename,
	                const int line_num,
	                const std::string &funcname = exception::empty_string,
	                const std::string &message = exception::empty_string) throw();

	virtual ~underflow_error() = default;
};
}
}

#endif /* UNDERFLOW_ERROR_HPP_ */
