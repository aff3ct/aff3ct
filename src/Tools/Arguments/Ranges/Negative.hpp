#ifndef ARGUMENT_RANGE_NEGATIVE_HPP_
#define ARGUMENT_RANGE_NEGATIVE_HPP_

#include <string>

#include "Max.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Negative_range : public Max_range<T>
{
public:
	explicit Negative_range()
	: Max_range<T>(0)
	{ }

	virtual ~Negative_range() {};

	virtual Negative_range* clone() const
	{
		return new Negative_range(*this);
	}

	virtual std::string get_title() const
	{
		return "negative";
	}
};

template <typename T = float> // then int are converted into floats in the max check function
Negative_range<T>* Negative()
{
	return new Negative_range<T>();
}

}
}

#endif /* ARGUMENT_RANGE_NEGATIVE_HPP_ */