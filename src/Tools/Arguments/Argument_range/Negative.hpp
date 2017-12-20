#ifndef ARGUMENT_RANGE_NEGATIVE_HPP_
#define ARGUMENT_RANGE_NEGATIVE_HPP_

#include <string>

#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Negative : public Max<T>
{
public:
	Negative()
	: Max<T>(0)
	{ }

	virtual ~Negative() {};

	virtual Negative* clone() const
	{
		return new Negative(*this);
	}

	virtual std::string get_title() const
	{
		return "negative";
	}
};

}
}

#endif /* ARGUMENT_RANGE_NEGATIVE_HPP_ */