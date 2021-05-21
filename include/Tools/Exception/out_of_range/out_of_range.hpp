/*!
 * \file
 * \brief Class tools::out_of_range.
 */
#ifndef OUT_OF_RANGE_HPP_
#define OUT_OF_RANGE_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class out_of_range : public exception
{
	static const std::string default_message;

public:
	out_of_range() throw();

	explicit out_of_range(const std::string &message) throw();

	out_of_range(const std::string &filename,
	             const int line_num,
	             const std::string &funcname = exception::empty_string,
	             const std::string &message = exception::empty_string) throw();

	virtual ~out_of_range() = default;
};
}
}

#endif /* OUT_OF_RANGE_HPP_ */
