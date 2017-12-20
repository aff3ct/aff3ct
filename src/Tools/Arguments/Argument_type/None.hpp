#ifndef ARGUMENT_TYPE_NONE_HPP_
#define ARGUMENT_TYPE_NONE_HPP_

#include <string>
#include <stdexcept>

#include "Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

class None : public Argument_type
{
public:
	None()
	: Argument_type("")
	{ }

	virtual ~None() {};

	virtual None* clone() const
	{
		return new None(*this);
	}

	virtual void check(const std::string& val) const
	{
	}
};

}
}
#endif /* ARGUMENT_TYPE_NONE_HPP_ */