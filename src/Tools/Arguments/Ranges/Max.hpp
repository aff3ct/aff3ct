#ifndef ARGUMENT_MAX_RANGE_HPP_
#define ARGUMENT_MAX_RANGE_HPP_

#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Max_range : public Argument_range
{
	const T max_val;

public:
	explicit Max_range(const T& m)
	: Argument_range("maximum " + std::to_string(m)), max_val(m)
	{ }

	virtual ~Max_range() {};

	virtual Max_range<T>* clone() const
	{
		return new Max_range<T>(*this);
	}

	template <typename V>
	void check(const V& val) const
	{
		if ((T)val > max_val)
		{
			std::stringstream message;
			message << "shall be lower than " << max_val;
			throw std::runtime_error(message.str());
		}
	}
};

template <typename T>
Max_range<T>* Max(const T& max_val)
{
	return new Max_range<T>(max_val);
}

}
}

#endif