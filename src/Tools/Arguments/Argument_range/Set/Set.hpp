#ifndef ARGUMENT_SET_RANGE_HPP_
#define ARGUMENT_SET_RANGE_HPP_

#include <set>

#include "../Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Set_range : public Argument_range
{
protected:
	std::set<T> options;

public:
	explicit Set_range(const std::string& title, const std::vector<T>& options)
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
		options.emplace(o);
	}

	virtual std::string get_title() const
	{
		return this->title + "=" + this->get_option_list();
	}

	virtual std::string get_option_list() const
	{
		std::stringstream options_list;
		options_list << "{";

		for (auto it = options.begin(); it != options.end(); it++)
		{
			options_list << *it;
			if (it != --(options.end()))
				options_list << "|";
		}
		options_list << "}";

		return options_list.str();
	}

	virtual void check(const T& val) const = 0;
};

}
}

#include "Including_set.hpp"
#include "Excluding_set.hpp"
#include "Example_set.hpp"

#endif