#ifndef ARGUMENT_RANGE_NON_ZERO_HPP_
#define ARGUMENT_RANGE_NON_ZERO_HPP_

#include <string>

#include "Set/Set.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = float>
class Non_zero_range : public Excluding_set_range<T>
{
public:
	explicit Non_zero_range()
	: Excluding_set_range<T>({(T)0})
	{ }

	virtual ~Non_zero_range() {};

	virtual Non_zero_range* clone() const
	{
		return new Non_zero_range(*this);
	}

	virtual std::string get_title() const
	{
		return "non-zero";
	}
};

template <typename T = float> // then int are converted into floats in the excluding set check function
Non_zero_range<T>* Non_zero()
{
	return new Non_zero_range<T>();
}

}
}

#endif /* ARGUMENT_RANGE_NON_ZERO_HPP_ */