#ifndef ARGUMENT_TYPE_LIMITED_HPP_
#define ARGUMENT_TYPE_LIMITED_HPP_

#include <string>
#include <stdexcept>

#include "Argument_type.hpp"
#include "../Ranges/Argument_range.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T>
class Argument_type_limited_T : public Argument_type
{
public:
	static const std::string ranges_description_separator;

private:
	std::vector<void*> ranges;

public:
	explicit Argument_type_limited_T(const std::string& title)
	: Argument_type(title)
	{}

	void* get_range(size_t pos) const
	{
		return ranges.at(pos);
	}

	std::vector<void*>& get_ranges()
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
	const std::vector<void*>& get_ranges() const
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
	explicit Argument_type_limited(const std::string& title, const Ranges*... new_ranges)
	: Argument_type_limited_T<T>(title)
	{
		add_ranges(new_ranges...);
	}

protected:
	explicit Argument_type_limited(const std::string& title)
	: Argument_type_limited_T<T>(title)
	{ }

public:
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

	template <typename... NewRanges>
	void add_ranges(NewRanges*... new_ranges)
	{
		_add_ranges<NewRanges...>(new_ranges...);
	}

protected:
	void check_ranges(const T& val) const
	{
		if (this->get_ranges().size())
			_check<Ranges...>(val);
	}

	template <typename... OtherRanges>
	Argument_type_limited<T,Ranges...,OtherRanges...>*
	clone_ranges(Argument_type_limited<T,Ranges...,OtherRanges...>* clone) const
	{
		clone->get_ranges().clear();

		_clone<Argument_type_limited<T,Ranges...,OtherRanges...>,Ranges...>(clone);

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
			throw std::out_of_range(std::string("Given position out of range when clearing : ")
			                                    + std::to_string(pos) + " on "
			                                    + std::to_string(this->get_ranges().size()) + "elements. title ="
			                                    + this->title);

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
			throw std::out_of_range(std::string("Given position out of range when checking : ")
			                                    + std::to_string(pos) + " on "
			                                    + std::to_string(this->get_ranges().size()) + "elements. title ="
			                                    + this->title);

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4244)
#endif
		static_cast<r*>(this->get_ranges()[pos])->check(val);
#ifdef _MSC_VER
#pragma warning( pop )
#endif
	}

	template <typename r, typename... R>
	typename std::enable_if<sizeof...(R) != 0,void>::type _check(const T& val, size_t pos = 0) const
	{
		_check<r   >(val,   pos);
		_check<R...>(val, ++pos);
	}

	template <typename C, typename r>
	void _clone(C* clone, size_t pos = 0) const
	{
		if (pos >= this->get_ranges().size())
			throw std::out_of_range(std::string("Given position out of range when cloning : ")
			                                    + std::to_string(pos) + " on "
			                                    + std::to_string(this->get_ranges().size()) + "elements. title ="
			                                    + this->title);

		clone->add_ranges(static_cast<r*>(this->get_ranges()[pos])->clone());
	}

	template <typename C, typename r, typename... R>
	typename std::enable_if<sizeof...(R) != 0,void>::type _clone(C* clone, size_t pos = 0) const
	{
		_clone<C,r   >(clone,   pos);
		_clone<C,R...>(clone, ++pos);
	}

	template <typename r>
	void _get_ranges_title(std::string& t, size_t pos = 0) const
	{
		if (pos >= this->get_ranges().size())
			throw std::out_of_range(std::string("Given position out of range when getting title : ")
			                                    + std::to_string(pos) + " on "
			                                    + std::to_string(this->get_ranges().size()) + "elements. title ="
			                                    + this->title);

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
	void _add_ranges(r* new_range)
	{
		if (new_range == nullptr)
			throw std::invalid_argument("The given argument range is a nullptr.");

		if (this->get_ranges().size() >= sizeof...(Ranges))
			throw std::out_of_range(std::string("Try to add a range but already at its maximum : ")
			                                    + std::to_string(this->get_ranges().size()) + "elements. title ="
			                                    + this->title);

		this->get_ranges().push_back((void*)new_range);
	}

	template <typename r, typename... R>
	typename std::enable_if<sizeof...(R) != 0,void>::type _add_ranges(r* new_range, R*... new_ranges)
	{
		_add_ranges(new_range    );
		_add_ranges(new_ranges...);
	}

	template <typename Friend, typename... FriendRange>
	friend class Argument_type_limited;
};

template <typename T>
class Argument_type_limited<T> : public Argument_type_limited_T<T>
{
public:
	explicit Argument_type_limited(const std::string& title)
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

	template <typename... OtherRanges>
	Argument_type_limited<T,OtherRanges...>*
	clone_ranges(Argument_type_limited<T,OtherRanges...>* clone) const
	{
		clone->get_ranges().clear();
		return clone;
	}

	void get_ranges_title(std::string& t) const
	{
	}
};

}
}

#include "Number/Real.hpp"
#include "Number/Integer.hpp"
#include "Number/Boolean.hpp"
#include "Container/Text.hpp"
#include "Container/List.hpp"
#include "File_system/File_system.hpp"


#endif /* ARGUMENT_TYPE_LIMITED_HPP_ */