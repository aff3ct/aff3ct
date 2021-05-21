/*!
 * \file
 * \brief Class tools::runtime_error.
 */
#ifndef RUNTIME_ERROR_HPP_
#define RUNTIME_ERROR_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class runtime_error : public exception
{
	static const std::string default_message;

public:
	runtime_error() throw();

	explicit runtime_error(const std::string &message) throw();

	runtime_error(const std::string &filename,
	              const int line_num,
	              const std::string &funcname = exception::empty_string,
	              const std::string &message = exception::empty_string) throw();

	virtual ~runtime_error() = default;
};
}
}

#endif /* RUNTIME_ERROR_HPP_ */
