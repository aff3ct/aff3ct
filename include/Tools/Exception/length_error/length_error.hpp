/*!
 * \file
 * \brief Class tools::length_error.
 */
#ifndef LENGTH_ERROR_HPP_
#define LENGTH_ERROR_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class length_error : public exception
{
	static const std::string default_message;

public:
	length_error() throw();

	explicit length_error(const std::string &message) throw();

	length_error(const std::string &filename,
	             const int line_num,
	             const std::string &funcname = exception::empty_string,
	             const std::string &message = exception::empty_string) throw();

	virtual ~length_error() = default;
};
}
}

#endif /* LENGTH_ERROR_HPP_ */
