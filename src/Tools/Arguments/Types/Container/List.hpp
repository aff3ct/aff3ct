#ifndef ARGUMENT_TYPE_LIST_HPP_
#define ARGUMENT_TYPE_LIST_HPP_

#include <string>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <tuple>

#include "../Argument_type_limited.hpp"
#include "../../utilities.hpp"
#include "../../Splitter/Splitter.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = std::string, class S = Generic_splitter, typename... Ranges>
class List_type : public Argument_type_limited<std::vector<T>,Ranges...>
{
protected:
	Argument_type* val_type;

public:
	template <typename r, typename... R>
	explicit List_type(Argument_type* val_type, const r* range, const R*... ranges)
	: Argument_type_limited<std::vector<T>,Ranges...>(generate_title(val_type), range, ranges...), val_type(val_type)
	{ }

	explicit List_type(Argument_type* val_type)
	: Argument_type_limited<std::vector<T>,Ranges...>(generate_title(val_type)), val_type(val_type)
	{ }

	virtual ~List_type()
	{
		if (val_type != nullptr) delete val_type;
	};

	virtual List_type<T, S, Ranges...>* clone() const
	{
		auto* clone = new List_type<T, S, Ranges...>(val_type);

		clone->val_type = val_type->clone();

		return dynamic_cast<List_type<T, S, Ranges...>*>(this->clone_ranges(clone));
	}

	template <typename... NewRanges>
	List_type<T, S, Ranges..., NewRanges...>*
	clone(NewRanges*... new_ranges)
	{
		auto* clone = new List_type<T, S, Ranges..., NewRanges...>(val_type);

		this->clone_ranges(clone);

		clone->add_ranges(new_ranges...);

		return clone;
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

		if (sizeof...(Ranges)) // then add ranges titles to the argument title
		{
			t += Argument_type::title_description_separator;
			this->get_ranges_title(t);
		}

		return t;
	}

	virtual std::vector<T> convert(const std::string& val) const
	{
		return this->convert(S::split(val));
	}

	virtual std::vector<T> convert(const std::vector<std::string>& list) const
	{
		std::vector<T> list_T(list.size());

		void * p_val = nullptr;

		for(unsigned i = 0; i < list.size(); i++)
		{
			p_val = val_type->get_val(list[i]);

			if (p_val == nullptr)
				throw std::runtime_error("Couldn't convert value.");

			list_T[i] = *(T*)p_val;

			delete (T*)p_val;
		}

		return list_T;
	}

	static std::string generate_title(Argument_type* val_type)
	{
		std::string str = "list of " + val_type->get_short_title();

		return str;
	}
};

template <typename T = std::string, class S = Generic_splitter, typename... Ranges>
List_type<T,S,Ranges...>* List(Argument_type* val_type, Ranges*... ranges)
{
	return new List_type<T,S,Ranges...>(val_type, ranges...);
}

template <typename T = std::string,
          class S1 = Generic_splitter, class S2 = String_splitter,
          typename... Ranges1, typename... Ranges2>
List_type<std::vector<T>, S1, Ranges1...>*
List2D(Argument_type* val_type,
      const std::tuple<Ranges1*...>& ranges1 = std::tuple<>(),
      const std::tuple<Ranges2*...>& ranges2 = std::tuple<>())
{
	Argument_type* listD2 = tools::apply_tuple(List<T,S2,Ranges2...>, std::tuple_cat(std::make_tuple(val_type), ranges2));
	return tools::apply_tuple(List<std::vector<T>,S1,Ranges1...>, std::tuple_cat(std::make_tuple(listD2), ranges1));
}

template <typename T = std::string,
          class S1 = Generic_splitter, class S2 = String_splitter,
          typename... Ranges1, typename... Ranges2>
List_type<std::vector<T>, S1, Ranges1...>*
List2D(Argument_type* val_type,
      std::tuple<Ranges1*...>&& ranges1 = std::tuple<>(),
      std::tuple<Ranges2*...>&& ranges2 = std::tuple<>())
{
	Argument_type* listD2 = tools::apply_tuple(List<T,S2,Ranges2...>, std::tuple_cat(std::make_tuple(val_type),
	                                                                                 std::forward<std::tuple<Ranges2*...>>(ranges2)));
	return tools::apply_tuple(List<std::vector<T>,S1,Ranges1...>, std::tuple_cat(std::make_tuple(listD2),
	                                                                             std::forward<std::tuple<Ranges1*...>>(ranges1)));
}

template <typename T = std::string, typename... Ranges1, typename... Ranges2>
List_type<std::vector<T>, Matlab_style_splitter::D1, Ranges1...>*
Matlab_vector(Argument_type* val_type,
              const std::tuple<Ranges1*...>& ranges1 = std::tuple<>(),
              const std::tuple<Ranges2*...>& ranges2 = std::tuple<>())
{
	return List2D<T, Matlab_style_splitter::D1, Matlab_style_splitter::D2>(val_type, ranges1, ranges2);
}

template <typename T = std::string, typename... Ranges1, typename... Ranges2>
List_type<std::vector<T>, Matlab_style_splitter::D1, Ranges1...>*
Matlab_vector(Argument_type* val_type,
              std::tuple<Ranges1*...>&& ranges1 = std::tuple<>(),
              std::tuple<Ranges2*...>&& ranges2 = std::tuple<>())
{
	return List2D<T, Matlab_style_splitter::D1, Matlab_style_splitter::D2>(val_type,
	                                                                       std::forward<std::tuple<Ranges1*...>>(ranges1),
	                                                                       std::forward<std::tuple<Ranges2*...>>(ranges2));
}

}
}
#endif /* ARGUMENT_TYPE_LIST_HPP_ */