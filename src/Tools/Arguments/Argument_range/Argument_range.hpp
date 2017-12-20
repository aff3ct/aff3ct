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

template <typename T>
class Argument_range
{
protected:
	std::string title;

public:
	Argument_range(const std::string& title)
	: title(title)
	{ }

	virtual ~Argument_range() {};

	virtual void check(const T&)    const = 0;
	virtual Argument_range<T>* clone() const = 0;

	virtual std::string get_title() const
	{
		return title;
	}
};

template <typename T>
class Min : public Argument_range<T>
{
	const T min_val;

public:
	Min(T m)
	: Argument_range<T>("minimum " + std::to_string(m)), min_val(m)
	{ }

	virtual ~Min() {};

	virtual Min<T>* clone() const
	{
		return new Min<T>(*this);
	}

	virtual void check(const T& val) const
	{
		if (val < min_val)
		{
			std::stringstream message;
			message << "shall be higher than " << min_val;
			throw std::runtime_error(message.str());
		}
	}
};

template <typename T>
class Max : public Argument_range<T>
{
	const T max_val;

public:
	Max(T m)
	: Argument_range<T>("maximum " + std::to_string(m)), max_val(m)
	{ }

	virtual ~Max() {};

	virtual Max<T>* clone() const
	{
		return new Max<T>(*this);
	}

	virtual void check(const T& val) const
	{
		if (val > max_val)
		{
			std::stringstream message;
			message << "shall be lower than " << max_val;
			throw std::runtime_error(message.str());
		}
	}
};



template <typename T>
class Set : public Argument_range<T>
{
protected:
	std::vector<T> options;

public:
	Set(const std::string& title, const std::vector<T>& options)
	: Argument_range<T>(title)
	{
		add_options(options);
	}

	virtual ~Set() {};

	void add_options(const std::vector<T>& new_options)
	{
		for (auto& o : new_options)
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
};

template <typename T>
class Including_set : public Set<T>
{
public:
	Including_set(const std::vector<T>& options)
	: Set<T>("including set", options)
	{ }

	virtual ~Including_set() {};

	virtual Including_set<T>* clone() const
	{
		return new Including_set<T>(*this);
	}

	virtual void check(const T& val) const
	{
		if (this->options.empty())
			return;

		auto it = std::find(this->options.begin(), this->options.end(), val);

		if (it == this->options.end())
			throw std::runtime_error("shall be in the set " + this->get_option_list());
	}
};

template <typename T>
class Excluding_set : public Set<T>
{
public:
	Excluding_set(const std::vector<T>& options)
	: Set<T>("excluding set", options)
	{ }

	virtual ~Excluding_set() {};

	virtual Excluding_set<T>* clone() const
	{
		return new Excluding_set<T>(*this);
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

}
}

#include "Non_zero.hpp"
#include "Negative.hpp"
#include "Positive.hpp"

#endif /* ARGUMENT_RANGE_HPP_ */