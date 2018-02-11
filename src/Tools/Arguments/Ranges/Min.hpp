#ifndef ARGUMENT_MIN_RANGE_HPP_
#define ARGUMENT_MIN_RANGE_HPP_

#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Min_range : public Argument_range
{
	const T min_val;

public:
	explicit Min_range(const T& m)
	: Argument_range("minimum " + std::to_string(m)), min_val(m)
	{ }

	virtual ~Min_range() {};

	virtual Min_range<T>* clone() const
	{
		return new Min_range<T>(*this);
	}

	template <typename V>
	void check(const V& val) const
	{
		if ((T)val < min_val)
		{
			std::stringstream message;
			message << "shall be higher than " << min_val;
			throw std::runtime_error(message.str());
		}
	}
};

template <typename T>
Min_range<T>* Min(const T& min_val)
{
	return new Min_range<T>(min_val);
}

}
}

#endif