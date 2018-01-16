#ifndef ARGUMENT_FUNCTION_RANGE_HPP_
#define ARGUMENT_FUNCTION_RANGE_HPP_

#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename F>
class Function_range : public Argument_range
{
public:
	explicit Function_range(const std::string& title)
	: Argument_range(title)
	{ }

	virtual ~Function_range() {};

	virtual Function_range<F>* clone() const
	{
		return new Function_range<F>(*this);
	}

	template <typename V>
	void check(const V& val) const
	{
		F::check(val);
	}
};

template <typename F>
Function_range<F>* Function(const std::string& title)
{
	return new Function_range<F>(title);
}

}
}

#endif