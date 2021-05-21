/*!
 * \file
 * \brief Class tools::domain_error.
 */
#ifndef DOMAIN_ERROR_HPP_
#define DOMAIN_ERROR_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class domain_error : public exception
{
	static const std::string default_message;

public:
	domain_error() throw();

	explicit domain_error(const std::string &message) throw();

	domain_error(const std::string &filename,
	             const int line_num,
	             const std::string &funcname = exception::empty_string,
	             const std::string &message = exception::empty_string) throw();

	virtual ~domain_error() = default;
};
}
}

#endif /* DOMAIN_ERROR_HPP_ */
