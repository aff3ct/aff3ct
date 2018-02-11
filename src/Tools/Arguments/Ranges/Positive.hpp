#ifndef ARGUMENT_RANGE_POSITIVE_HPP_
#define ARGUMENT_RANGE_POSITIVE_HPP_

#include <string>

#include "Min.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = float>
class Positive_range : public Min_range<T>
{
public:
	Positive_range()
	: Min_range<T>(0)
	{ }

	virtual ~Positive_range() {};

	virtual Positive_range* clone() const
	{
		return new Positive_range(*this);
	}

	virtual std::string get_title() const
	{
		return "positive";
	}
};

template <typename T = float> // then int are converted into floats in the min check function
Positive_range<T>* Positive()
{
	return new Positive_range<T>();
}

}
}

#endif /* ARGUMENT_RANGE_POSITIVE_HPP_ */