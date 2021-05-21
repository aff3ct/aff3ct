/*!
 * \file
 * \brief Class tools::unimplemented_error.
 */
#ifndef UNIMPLEMENTED_ERROR_HPP_
#define UNIMPLEMENTED_ERROR_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class unimplemented_error : public exception
{
	static const std::string default_message;

public:
	unimplemented_error() throw();

	explicit unimplemented_error(const std::string &message) throw();

	unimplemented_error(const std::string &filename,
	                    const int line_num,
	                    const std::string &funcname = exception::empty_string,
	                    const std::string &message = exception::empty_string) throw();

	virtual ~unimplemented_error() = default;
};
}
}

#endif /* UNIMPLEMENTED_ERROR_HPP_ */
