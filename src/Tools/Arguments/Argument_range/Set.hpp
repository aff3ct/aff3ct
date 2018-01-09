#ifndef ARGUMENT_SET_RANGE_HPP_
#define ARGUMENT_SET_RANGE_HPP_

#include "Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Set_range : public Argument_range
{
protected:
	std::vector<T> options;

public:
	Set_range(const std::string& title, const std::vector<T>& options)
	: Argument_range(title)
	{
		add_options(options);
	}

	virtual ~Set_range() {};

	void add_options(const std::vector<T>& new_options)
	{
		for (const auto& o : new_options)
			add_option(o);
	}

	void add_option(const T& o)
	{
		options.push_back(o);
	}

	virtual std::string get_title() const
	{
		return this->title + "=" + this->get_option_list();
	}

	virtual std::string get_option_list() const
	{
		std::stringstream options_list;
		options_list << "{";

		for (unsigned i = 0; i < options.size(); i++)
		{
			options_list << options[i];
			if (i < (options.size()-1))
				options_list << "|";
		}
		options_list << "}";

		return options_list.str();
	}

	virtual void check(const T& val) const = 0;
};



template <typename T>
class Including_set_range : public Set_range<T>
{
public:
	Including_set_range(const std::vector<T>& options)
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
			return;

		auto it = std::find(this->options.begin(), this->options.end(), val);

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




template <typename T>
class Excluding_set_range : public Set_range<T>
{
public:
	Excluding_set_range(const std::vector<T>& options)
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

		auto it = std::find(this->options.begin(), this->options.end(), val);

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