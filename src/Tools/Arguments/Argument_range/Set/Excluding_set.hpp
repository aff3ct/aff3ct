#ifndef ARGUMENT_EXCLUDING_SET_RANGE_HPP_
#define ARGUMENT_EXCLUDING_SET_RANGE_HPP_

#include "Set.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Excluding_set_range : public Set_range<T>
{
public:
	explicit Excluding_set_range(const std::vector<T>& options)
	: Set_range<T>("excluding set", options)
	{ }

	virtual ~Excluding_set_range() {};

	virtual Excluding_set_range<T>* clone() const
	{
		return new Excluding_set_range<T>(*this);
	}

	virtual void check(const T& val) const
	{
		if (this->options.empty())
			return;

		auto it = this->options.find(val);

		if (it != this->options.end())
			throw std::runtime_error("shall NOT be in the set " + this->get_option_list());
	}
};

template <typename T, typename... O>
typename std::enable_if<!std::is_same<T, char const*>::value, Set_range<T>*>::type
Excluding_set(const T option, const O... options)
{
	return new Excluding_set_range<T>({option, options...});
}

template <typename T, typename... O>
typename std::enable_if<std::is_same<T, char const*>::value, Set_range<std::string>*>::type
Excluding_set(const T option, const O... options)
{
	std::vector<std::string> list = {option, options...};
	return new Excluding_set_range<std::string>(list);
}

template <typename T>
typename std::enable_if<!std::is_same<T, char const*>::value, Set_range<T>*>::type
Excluding_set(const std::vector<T>& options)
{
	return new Excluding_set_range<T>(options);
}

template <typename T>
typename std::enable_if<std::is_same<T, char const*>::value, Set_range<std::string>*>::type
Excluding_set(const std::vector<T>& options)
{
	std::vector<std::string> list(options.size());

	std::copy(options.begin(), options.end(), list.begin());

	return new Excluding_set_range<std::string>(list);
}


template <typename T>
Set_range<T>* Excluding_set()
{
	return new Excluding_set_range<T>({});
}

}
}

#endif