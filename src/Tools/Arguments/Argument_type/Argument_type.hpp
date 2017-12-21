#ifndef ARGUMENT_TYPE_HPP_
#define ARGUMENT_TYPE_HPP_

#include <string>
#include <stdexcept>

#include "../Argument_range/Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

class Argument_type
{
public:
	static const std::string title_description_separator;

protected:
	std::string title;

public:
	Argument_type(const std::string& title);

	virtual ~Argument_type() {};

	virtual void check(const std::string&) const = 0;
	virtual Argument_type* clone()         const = 0;

	virtual void* get_val(const std::string& val) const;

	virtual const std::string get_title() const;

	const std::string get_short_title() const;
};

template <typename T>
class Argument_type_limited : public Argument_type
{
private:
	std::vector<Argument_range<T>*> ranges;

public:
	using type = T;

public:
	Argument_type_limited(const std::string& title, const std::vector<Argument_range<T>*>& new_ranges = {})
	: Argument_type(title)
	{
		add_ranges(new_ranges);
	}

	virtual ~Argument_type_limited()
	{
		for (auto& r : ranges)
			if (r != nullptr)
				delete r;
	}

	void add_ranges(const std::vector<Argument_range<T>*>& new_ranges)
	{
		for (auto& r : new_ranges)
			add_range(r);
	}

	void add_range(const Argument_range<T>* r)
	{
		if (r == nullptr)
			throw std::invalid_argument("The given argument range is a nullptr.");

		ranges.push_back(const_cast<Argument_range<T>*>(r));
	}

	const std::vector<Argument_range<T>*>& get_ranges()
	{
		return ranges;
	}

	const std::string get_ranges_title() const
	{
		std::string t;

		// add ranges titles
		for (unsigned i = 0; i < ranges.size(); i++)
		{
			t += ranges[i]->get_title();
			if (i < (ranges.size()-1))
				t += ", ";
		}

		return t;
	}

	virtual const std::string get_title() const
	{
		auto t = this->title;

		if (ranges.size()) // then add ranges titles to the argument title
		{
			t += Argument_type::title_description_separator;
			t += this->get_ranges_title();
		}

		return t;
	}

	virtual T convert(const std::string& val) const
	{
		throw std::runtime_error("This method has not been instantiated for this class.");
	}

	virtual void* get_val(const std::string& val) const
	{
		T* p_val = new T(this->convert(val));

		return (void*)p_val;
	}


protected:
	virtual void check_ranges(const T& val) const
	{
		for (auto& r : ranges)
			r->check(val);
	}

	virtual Argument_type_limited<T>* clone_ranges(Argument_type_limited<T>* clone) const
	{
		clone->ranges.clear();

		for (auto& r : ranges)
			clone->add_range(r->clone());

		return clone;
	}
};

}
}

#include "None.hpp"
#include "Text.hpp"
#include "Real.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "List.hpp"

#endif /* ARGUMENT_TYPE_HPP_ */