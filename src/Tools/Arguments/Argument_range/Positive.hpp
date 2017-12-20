#ifndef ARGUMENT_RANGE_POSITIVE_HPP_
#define ARGUMENT_RANGE_POSITIVE_HPP_

#include <string>

#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Positive : public Min<T>
{
public:
	Positive()
	: Min<T>(0)
	{ }

	virtual ~Positive() {};

	virtual Positive* clone() const
	{
		return new Positive(*this);
	}

	virtual std::string get_title() const
	{
		return "positive";
	}
};

}
}

#endif /* ARGUMENT_RANGE_POSITIVE_HPP_ */