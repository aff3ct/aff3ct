#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <string>

namespace aff3ct
{
namespace tools
{
class exception : public std::exception
{
private:
	std::string message;

public:
	exception() throw();

	exception(const std::string message) throw();

	exception(const std::string filename,
	          const int line_num,
	          const std::string funcname,
	          const std::string message) throw();

	virtual ~exception() throw();

	virtual const char* what() const throw();

private:
	static std::string get_back_trace();
};
}
}

#endif /* EXCEPTION_HPP_ */
