#ifndef ARGUMENT_FUNCTION_RANGE_HPP_
#define ARGUMENT_FUNCTION_RANGE_HPP_

#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T, typename F>
class Function_range : public Argument_range
{
	F f;

public:
	Function_range(const std::string& title, F f)
	: Argument_range(title), f(f)
	{ }

	virtual ~Function_range() {};

	virtual Function_range<T,F>* clone() const
	{
		return new Function_range<T,F>(*this);
	}

	template <typename V>
	void check(const V& val) const
	{
		f((T)val);
	}
};

template <typename T, typename F>
Function_range<T,F>* Function(const std::string& title, F f)
{
	return new Function_range<T,F>(title, f);
}

}
}

#endif