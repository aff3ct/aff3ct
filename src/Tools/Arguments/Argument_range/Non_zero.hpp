#ifndef ARGUMENT_RANGE_NON_ZERO_HPP_
#define ARGUMENT_RANGE_NON_ZERO_HPP_

#include <string>

#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Non_zero : public Excluding_set<T>
{
public:
	Non_zero()
	: Excluding_set<T>({0})
	{ }

	virtual ~Non_zero() {};

	virtual Non_zero* clone() const
	{
		return new Non_zero(*this);
	}

	virtual std::string get_title() const
	{
		return "non-zero";
	}
};

}
}

#endif /* ARGUMENT_RANGE_NON_ZERO_HPP_ */