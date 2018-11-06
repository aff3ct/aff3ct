#ifndef ARGUMENT_INCLUDING_SET_RANGE_HPP_
#define ARGUMENT_INCLUDING_SET_RANGE_HPP_

#include "Set.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Including_set_range : public Set_range<T>
{
public:
	explicit Including_set_range(const std::vector<T>& options)
	: Set_range<T>("including set", options)
	{ }

	virtual ~Including_set_range() {};

	virtual Including_set_range<T>* clone() const
	{
		return new Including_set_range<T>(*this);
	}

	void check(const T& val) const
	{
		if (this->options.empty())
			throw std::runtime_error("accepts no argument");

		auto it = this->options.find(val);

		if (it == this->options.end())
			throw std::runtime_error("shall be in the set " + this->get_option_list());
	}
};

template <typename T, typename... O>
typename std::enable_if<!std::is_same<T, char const*>::value, Set_range<T>*>::type
Including_set(const T option, const O... options)
{
	return new Including_set_range<T>({option, options...});
}

template <typename T, typename... O>
typename std::enable_if<std::is_same<T, char const*>::value, Set_range<std::string>*>::type
Including_set(const T option, const O... options)
{
	std::vector<std::string> list = {option, options...};
	return new Including_set_range<std::string>(list);
}

template <typename T>
typename std::enable_if<!std::is_same<T, char const*>::value, Set_range<T>*>::type
Including_set(const std::vector<T>& options)
{
	return new Including_set_range<T>(options);
}

template <typename T>
typename std::enable_if<std::is_same<T, char const*>::value, Set_range<std::string>*>::type
Including_set(const std::vector<T>& options)
{
	std::vector<std::string> list(options.size());

	std::copy(options.begin(), options.end(), list.begin());

	return new Including_set_range<std::string>(list);
}

template <typename T>
Set_range<T>* Including_set()
{
	return new Including_set_range<T>({});
}

}
}

#endif