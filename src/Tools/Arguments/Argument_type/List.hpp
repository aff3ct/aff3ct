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

template <class S = Generic_splitter, typename List_element_type = std::string>
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

	virtual List<S, List_element_type>* clone() const
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

}
}
#endif /* ARGUMENT_TYPE_LIST_HPP_ */