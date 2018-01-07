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
class Argument_type_limited_T : public Argument_type
{
public:
	static const std::string ranges_description_separator;

private:
	std::vector<void*> ranges;

public:
	Argument_type_limited_T(const std::string& title)
	: Argument_type(title)
	{}

	void* get_range(size_t pos) const
	{
		return ranges.at(pos);
	}

	const std::vector<void*>& get_ranges() const
	{
		return ranges;
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
	std::vector<void*>& get_ranges()
	{
		return ranges;
	}
};

template <typename T>
const std::string Argument_type_limited_T<T>::ranges_description_separator = ", ";



template <typename T, typename... Ranges>
class Argument_type_limited : public Argument_type_limited_T<T>
{
public:
	Argument_type_limited(const std::string& title, const Ranges*... new_ranges)
	: Argument_type_limited_T<T>(title)
	{
		add_ranges(new_ranges...);
	}

	virtual ~Argument_type_limited()
	{
		_clear<Ranges...>();
	}

	virtual const std::string get_title() const
	{
		auto t = this->title;

		if (this->get_ranges().size()) // then add ranges titles to the argument title
		{
			t += Argument_type::title_description_separator;
			get_ranges_title(t);
		}

		return t;
	}

protected:
	void check_ranges(const T& val) const
	{
		if (this->get_ranges().size())
			_check<Ranges...>(val);
	}

	virtual Argument_type_limited<T,Ranges...>* clone_ranges(Argument_type_limited<T,Ranges...>* clone) const
	{
		clone->get_ranges().clear();

		_clone<Ranges...>(clone);

		return clone;
	}

	void get_ranges_title(std::string& t) const
	{
		_get_ranges_title<Ranges...>(t);
	}

private:
	template <typename r>
	void _clear(size_t pos = 0)
	{
		if (pos >= this->get_ranges().size())
			throw std::out_of_range(std::string("Given position out of range : ")
			                                    + std::to_string(pos) + " on "
			                                    + std::to_string(this->get_ranges().size()) + "elements.");

		auto* ptr = static_cast<r*>(this->get_ranges()[pos]);
		if (ptr != nullptr)
			delete ptr;
		ptr = nullptr;
	}

	template <typename r, typename... R>
	typename std::enable_if<sizeof...(R) != 0,void>::type _clear(size_t pos = 0)
	{
		_clear<r   >(  pos);
		_clear<R...>(++pos);
	}

	template <typename r>
	void _check(const T& val, size_t pos = 0) const
	{
		if (pos >= this->get_ranges().size())
			throw std::out_of_range(std::string("Given position out of range : ")
			                                    + std::to_string(pos) + " on "
			                                    + std::to_string(this->get_ranges().size()) + "elements.");

		static_cast<r*>(this->get_ranges()[pos])->check(val);
	}

	template <typename r, typename... R>
	typename std::enable_if<sizeof...(R) != 0,void>::type _check(const T& val, size_t pos = 0) const
	{
		_check<r   >(val,   pos);
		_check<R...>(val, ++pos);
	}

	template <typename r>
	void _clone(Argument_type_limited<T,Ranges...>* clone, size_t pos = 0) const
	{
		if (pos >= this->get_ranges().size())
			throw std::out_of_range(std::string("Given position out of range : ")
			                                    + std::to_string(pos) + " on "
			                                    + std::to_string(this->get_ranges().size()) + "elements.");

		clone->add_ranges(static_cast<r*>(this->get_ranges()[pos])->clone());
	}

	template <typename r, typename... R>
	typename std::enable_if<sizeof...(R) != 0,void>::type _clone(Argument_type_limited<T,Ranges...>* clone, size_t pos = 0) const
	{
		_clone<r   >(clone,   pos);
		_clone<R...>(clone, ++pos);
	}

	template <typename r>
	void _get_ranges_title(std::string& t, size_t pos = 0) const
	{
		if (pos >= this->get_ranges().size())
			throw std::out_of_range(std::string("Given position out of range : ")
			                                    + std::to_string(pos) + " on "
			                                    + std::to_string(this->get_ranges().size()) + "elements.");

		t += static_cast<r*>(this->get_ranges()[pos])->get_title();
	}

	template <typename r, typename... R>
	typename std::enable_if<sizeof...(R) != 0,void>::type _get_ranges_title(std::string& t, size_t pos = 0) const
	{
		_get_ranges_title<r   >(t,   pos);

		t += this->ranges_description_separator;

		_get_ranges_title<R...>(t, ++pos);
	}

	template <typename r>
	void add_ranges(const r* new_range)
	{
		if (new_range == nullptr)
			throw std::invalid_argument("The given argument range is a nullptr.");

		this->get_ranges().push_back((void*)new_range);
	}

	template <typename r, typename... R>
	typename std::enable_if<sizeof...(R) != 0,void>::type add_ranges(const r* new_range, const R*... new_ranges)
	{
		add_ranges(new_range    );
		add_ranges(new_ranges...);
	}
};

template <typename T>
class Argument_type_limited<T> : public Argument_type_limited_T<T>
{
public:
	Argument_type_limited(const std::string& title)
	: Argument_type_limited_T<T>(title)
	{
	}

	virtual ~Argument_type_limited()
	{
	}


protected:
	void check_ranges(const T& val) const
	{
	}

	virtual Argument_type_limited<T>* clone_ranges(Argument_type_limited<T>* clone) const
	{
		clone->get_ranges().clear();
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