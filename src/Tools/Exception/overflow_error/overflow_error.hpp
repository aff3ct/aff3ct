#ifndef OVERFLOW_ERROR_HPP_
#define OVERFLOW_ERROR_HPP_

#include "../exception.hpp"

namespace aff3ct
{
namespace tools
{
class overflow_error : public exception
{
	static const std::string default_message;

public:
	overflow_error() throw();

	explicit overflow_error(const std::string &message) throw();

	overflow_error(const std::string &filename,
	               const int line_num,
	               const std::string &funcname = exception::empty_string,
	               const std::string &message = exception::empty_string) throw();

	virtual ~overflow_error() throw() = default;
};
}
}

#endif /* OVERFLOW_ERROR_HPP_ */
