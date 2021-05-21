/*!
 * \file
 * \brief Class tools::range_error.
 */
#ifndef RANGE_ERROR_HPP_
#define RANGE_ERROR_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class range_error : public exception
{
	static const std::string default_message;

public:
	range_error() throw();

	explicit range_error(const std::string &message) throw();

	range_error(const std::string &filename,
	            const int line_num,
	            const std::string &funcname = exception::empty_string,
	            const std::string &message = exception::empty_string) throw();

	virtual ~range_error() = default;
};
}
}

#endif /* RANGE_ERROR_HPP_ */
