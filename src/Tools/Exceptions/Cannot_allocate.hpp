#ifndef CANNOT_ALLOCATE_HPP_
#define CANNOT_ALLOCATE_HPP_

#include <exception>
#include <string>

namespace aff3ct
{
namespace tools
{
class Cannot_allocate : public std::exception
{
private:
	std::string message;

public:
	Cannot_allocate()
	{
	}

	Cannot_allocate(std::string message) : message(message)
	{
	}

	virtual ~Cannot_allocate() throw()
	{
	}

	virtual const char* what() const throw()
	{
		return message.c_str();
	}
};
}
}

#endif /* CANNOT_ALLOCATE_HPP_ */
