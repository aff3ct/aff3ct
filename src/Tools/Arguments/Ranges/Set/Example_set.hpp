#ifndef ARGUMENT_EXAMPLE_SET_RANGE_HPP_
#define ARGUMENT_EXAMPLE_SET_RANGE_HPP_

#include "Set.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Example_set_range : public Set_range<T>
{
public:
	explicit Example_set_range(const std::vector<T>& options)
	: Set_range<T>("example set", options)
	{ }

	virtual ~Example_set_range() {};

	virtual Example_set_range<T>* clone() const
	{
		return new Example_set_range<T>(*this);
	}

	void check(const T& val) const
	{
		// do nothing, options are just examples
	}
};

template <typename T, typename... O>
typename std::enable_if<!std::is_same<T, char const*>::value, Set_range<T>*>::type
Example_set(const T option, const O... options)
{
	return new Example_set_range<T>({option, options...});
}

template <typename T, typename... O>
typename std::enable_if<std::is_same<T, char const*>::value, Set_range<std::string>*>::type
Example_set(const T option, const O... options)
{
	std::vector<std::string> list = {option, options...};
	return new Example_set_range<std::string>(list);
}

template <typename T>
typename std::enable_if<!std::is_same<T, char const*>::value, Set_range<T>*>::type
Example_set(const std::vector<T>& options)
{
	return new Example_set_range<T>(options);
}

template <typename T>
typename std::enable_if<std::is_same<T, char const*>::value, Set_range<std::string>*>::type
Example_set(const std::vector<T>& options)
{
	std::vector<std::string> list(options.size());

	std::copy(options.begin(), options.end(), list.begin());

	return new Example_set_range<std::string>(list);
}

template <typename T>
Set_range<T>* Example_set()
{
	return new Example_set_range<T>({});
}

}
}

#endif