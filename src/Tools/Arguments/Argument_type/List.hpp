#ifndef ARGUMENT_TYPE_LIST_HPP_
#define ARGUMENT_TYPE_LIST_HPP_

#include <string>
#include <sstream>
#include <stdexcept>
#include <type_traits>

#include "Argument_type.hpp"
#include "../Splitter/Splitter.hpp"

namespace aff3ct
{
namespace tools
{

template <typename List_element_type = std::string, class S = Generic_splitter>
class List : public Argument_type_limited<std::vector<List_element_type>>
{
protected:
	Argument_type* val_type;

public:
	List(Argument_type* val_type, const std::vector<Argument_range<std::vector<List_element_type>>*>& ranges = {})
	: Argument_type_limited<std::vector<List_element_type>>("list of " + val_type->get_short_title(), ranges), val_type(val_type)
	{ }

	virtual ~List()
	{
		if (val_type != nullptr) delete val_type;
	};

	virtual List<List_element_type, S>* clone() const
	{
		return new List(val_type->clone());
	}

	virtual void check(const std::string& val) const
	{
		// separate values:
		auto list = S::split(val);

		unsigned i = 0;
		try
		{
			for (; i < list.size(); i++)
				val_type->check(list[i]);
		}
		catch(std::exception& e)
		{
			std::stringstream message;
			message << "has the element " << i << " (" << list[i] << ") not respecting the rules: " << e.what();

			throw std::runtime_error(message.str());
		}

		auto list_vals = this->convert(list);

		this->check_ranges(list_vals);

	}

	virtual const std::string get_title() const
	{
		auto t = "list of (" + val_type->get_title() + ")";

		auto rt = this->get_ranges_title();
		if (rt.size()) // then add ranges titles to the argument title
		{
			t += Argument_type::title_description_separator;
			t += rt;
		}

		return t;
	}

	virtual std::vector<List_element_type> convert(const std::string& val) const
	{
		return this->convert(S::split(val));
	}

	virtual std::vector<List_element_type> convert(const std::vector<std::string>& list) const
	{
		std::vector<List_element_type> list_T(list.size());

		void * p_val = nullptr;

		for(unsigned i = 0; i < list.size(); i++)
		{
			p_val = val_type->get_val(list[i]);

			if (p_val == nullptr)
				throw std::runtime_error("Couldn't convert value.");

			list_T[i] = *(List_element_type*)p_val;

			delete (List_element_type*)p_val;
		}

		return list_T;
	}

};

template <typename List_element_type = std::string, class S1 = Generic_splitter, class S2 = String_splitter>
class List2D : public List<std::vector<List_element_type>, S1>
{
public:
	List2D(Argument_type* val_type, const std::vector<Argument_range<std::vector<std::vector<List_element_type>>>*>& ranges1 = {}, const std::vector<Argument_range<std::vector<List_element_type>>*>& ranges2 = {})
	: List<std::vector<List_element_type>, S1>(new List<List_element_type, S2>(val_type, ranges2), ranges1)
	{
	}
};


// template <typename List_element_type, class S, class... MD_S>
// struct unpack2vectors
// {
// 	using type = std::vector<typename std::conditional<sizeof...(MD_S) == 0,
// 	                                                   List_element_type,
// 	                                                   unpack2vectors<List_element_type, MD_S...>
// 	                                                  >::type
// 	                        >;
// };

// template <typename List_element_type = std::string, class S = Generic_splitter, class... MD_S>
// class MD_List : public List<typename unpack2vectors<List_element_type, S, MD_S...>::type>
// {
// public:
// 	MD_List(Argument_type* val_type, const std::vector<Argument_range<std::vector<List_element_type>>*>& ranges = {})
// 	: List<List_element_type, S>(val_type, ranges)
// 	{
// 	}

// 	template <class... MD_Argument_ranges>
// 	MD_List(Argument_type* val_type, const std::vector<Argument_range<std::vector<List_element_type>>*>&& ranges, const MD_Argument_ranges&&... MD_ranges)
// 	: List<typename unpack2vectors<List_element_type, S, MD_S...>::type, S>(new MD_List<List_element_type, MD_S...>(val_type, MD_ranges...), ranges)
// 	{
// 		static_assert(sizeof...(MD_S) == sizeof...(MD_Argument_ranges), "There shall be the same number of ranges elements than Splitter");
// 	}
// };

}
}
#endif /* ARGUMENT_TYPE_LIST_HPP_ */