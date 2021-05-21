/*!
 * \file
 * \brief Class tools::waiting_canceled.
 */
#ifndef WAITING_CANCELED_HPP_
#define WAITING_CANCELED_HPP_

#include <string>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
class waiting_canceled : public exception
{
	static const std::string default_message;

public:
	waiting_canceled() throw();

	explicit waiting_canceled(const std::string &message) throw();

	waiting_canceled(const std::string &filename,
	                    const int line_num,
	                    const std::string &funcname = exception::empty_string,
	                    const std::string &message = exception::empty_string) throw();

	virtual ~waiting_canceled() = default;
};
}
}

#endif /* WAITING_CANCELED_HPP_ */
