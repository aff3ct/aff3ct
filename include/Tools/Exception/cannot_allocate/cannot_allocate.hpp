/*!
 * \file
 * \brief Class tools::cannot_allocate.
 */
#ifndef CANNOT_ALLOCATE_HPP_
#define CANNOT_ALLOCATE_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class cannot_allocate : public exception
{
	static const std::string default_message;

public:
	cannot_allocate() throw();

	explicit cannot_allocate(const std::string &message) throw();

	cannot_allocate(const std::string &filename,
	                const int line_num,
	                const std::string &funcname = exception::empty_string,
	                const std::string &message = exception::empty_string) throw();

	virtual ~cannot_allocate() = default;
};
}
}

#endif /* CANNOT_ALLOCATE_HPP_ */
