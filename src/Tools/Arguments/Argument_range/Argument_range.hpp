#ifndef ARGUMENT_RANGE_HPP_
#define ARGUMENT_RANGE_HPP_

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace aff3ct
{
namespace tools
{

class Argument_range
{
protected:
	std::string title;

public:
	Argument_range(const std::string& title)
	: title(title)
	{ }

	virtual ~Argument_range() {};

	virtual Argument_range* clone() const = 0;

	virtual std::string get_title() const
	{
		return title;
	}
};

template <typename T>
class Min_range : public Argument_range
{
	const T min_val;

public:
	Min_range(const T& m)
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



template <typename T>
class Max_range : public Argument_range
{
	const T max_val;

public:
	Max_range(const T& m)
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

	template <typename V>
	T adapt(const V& val) const
	{
		return (T)val;
	}
};

template <>
template <>
const char* Set_range<const char*>::template adapt<std::string>(const std::string& val) const;


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

	template <typename V>
	void check(const V& val) const
	{
		if (this->options.empty())
			return;

		T val_T = this->adapt(val);

		auto it = std::find(this->options.begin(), this->options.end(), val_T);

		if (it == this->options.end())
			throw std::runtime_error("shall be in the set " + this->get_option_list());
	}
};

template <typename T, typename... O>
Including_set_range<T>* Including_set(const T option, const O... options)
{
	return new Including_set_range<T>({option, options...});
}

template <typename T>
Including_set_range<T>* Including_set()
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

		T val_T = this->adapt(val);

		auto it = std::find(this->options.begin(), this->options.end(), val_T);

		if (it != this->options.end())
			throw std::runtime_error("shall NOT be in the set " + this->get_option_list());
	}
};

template <typename T, typename... O>
Excluding_set_range<T>* Excluding_set(const T option, const O... options)
{
	return new Excluding_set_range<T>({option, options...});
}

template <typename T>
Excluding_set_range<T>* Excluding_set()
{
	return new Excluding_set_range<T>({});
}


}
}

#include "Non_zero.hpp"
#include "Negative.hpp"
#include "Positive.hpp"
#include "Length.hpp"

#endif /* ARGUMENT_RANGE_HPP_ */