/*!
 * \file
 * \brief Class tools::processing_aborted.
 */
#ifndef PROCESSING_ABORTED_HPP_
#define PROCESSING_ABORTED_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class processing_aborted : public exception
{
	static const std::string default_message;

public:
	processing_aborted() throw();

	explicit processing_aborted(const std::string &message) throw();

	processing_aborted(const std::string &filename,
	                   const int line_num,
	                   const std::string &funcname = exception::empty_string,
	                   const std::string &message = exception::empty_string) throw();

	virtual ~processing_aborted() = default;
};
}
}

#endif /* PROCESSING_ABORTED_HPP_ */
