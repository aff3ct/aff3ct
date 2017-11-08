#ifndef ARGUMENT_RANGE_BASICS_HPP_
#define ARGUMENT_RANGE_BASICS_HPP_

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

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

#endif /* ARGUMENT_RANGE_BASICS_HPP_ */