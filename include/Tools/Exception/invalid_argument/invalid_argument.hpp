/*!
 * \file
 * \brief Class tools::invalid_argument.
 */
#ifndef INVALID_ARGUMENT_HPP_
#define INVALID_ARGUMENT_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class invalid_argument : public exception
{
	static const std::string default_message;

public:
	invalid_argument() throw();

	explicit invalid_argument(const std::string &message) throw();

	invalid_argument(const std::string &filename,
	                 const int line_num,
	                 const std::string &funcname = exception::empty_string,
	                 const std::string &message = exception::empty_string) throw();

	virtual ~invalid_argument() = default;
};
}
}

#endif /* INVALID_ARGUMENT_HPP_ */
