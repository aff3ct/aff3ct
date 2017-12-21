#ifndef ARGUMENT_TYPE_LIST_HPP_
#define ARGUMENT_TYPE_LIST_HPP_

#include <string>
#include <stdexcept>
#include <iostream>
#include <type_traits>

#include "Argument_type.hpp"
#include "../Splitter/Splitter.hpp"

namespace aff3ct
{
namespace tools
{

template <class S = Generic_splitter, typename T = std::string>
class List : public Argument_type_limited<std::vector<T>>
{
protected:
	Argument_type* val_type;

public:
	List(Argument_type* val_type, const std::vector<Argument_range<std::vector<T>>*>& ranges = {})
	: Argument_type_limited<std::vector<T>>("list of " + val_type->get_short_title(), ranges), val_type(val_type)
	{ }

	virtual ~List()
	{
		if (val_type != nullptr) delete val_type;
	};

	virtual List<S, T>* clone() const
	{
		return new List(val_type->clone());
	}

	virtual void check(const std::string& val) const
	{
		// seperate values:
		auto list_vals = this->get_list(val);

		for (const auto& v : list_vals)
			val_type->check(v);

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

	// static std::vector<std::string> seperate(const std::string& val)
	// {
	// 	std::vector<std::string> list_vals;

	// 	const std::string head  = "{([";
	// 	const std::string queue = "})]";
	// 	const std::string separator = ";,.|";

	// 	// try first to find a head
	// 	size_t pos = val.find_first_of(head);

	// 	if (pos == std::string::npos)
	// 		pos = 0;
	// 	else
	// 		pos++;

	// 	while (pos < val.size())
	// 	{
	// 		// try to find any separator to get values
	// 		size_t found_pos = val.find_first_of(separator, pos);

	// 		if (found_pos == std::string::npos)
	// 		{
	// 			// try find a queue character
	// 			found_pos = val.find_first_of(queue, pos);

	// 			if (found_pos != std::string::npos)
	// 				list_vals.push_back(val.substr(pos, found_pos - pos));
	// 			else
	// 				list_vals.push_back(val.substr(pos));

	// 			break;
	// 		}

	// 		list_vals.push_back(val.substr(pos, found_pos - pos));


	// 		pos = found_pos + 1;
	// 	}

	// 	return list_vals;
	// }

	// virtual void* get_val(const std::string& val) const
	// {
	// 	auto* p_v = new std::vector<T>();

	// 	auto v = get_list(val);

	// 	p_v->swap(v);

	// 	return (void*)p_v;
	// }

	static std::vector<T> get_list(const std::string& val)
	{
		static_assert(std::is_same<T, std::string>::value || std::is_same<T, float>::value || std::is_same<T, int>::value || std::is_same<T, bool>::value,
		              "The get_list function is available only with a std::string, a float, a int or a bool type.");

		auto list = S::split(val);

		std::vector<T> list_T(list.size());

		void * p_val = nullptr;

		for(unsigned i = 0; i < list.size(); i++)
		{
			if (std::is_same<T, float>::value)
				p_val = (void*)(new float(std::stof(list[i])));

			else if (std::is_same<T, int>::value)
				p_val = (void*)(new int(std::stoi(list[i])));

			else if (std::is_same<T, bool>::value)
				p_val = (void*)(new bool(std::stoi(list[i])));

			else if (std::is_same<T, std::string>::value)
				p_val = (void*)(new std::string(list[i]));

			else
				p_val = nullptr;


			if (p_val == nullptr)
				throw std::runtime_error("Couldn't convert value.");

			list_T[i] = *(T*)p_val;

			delete (T*)p_val;
		}

		return list_T;
	}

};

}
}
#endif /* ARGUMENT_TYPE_LIST_HPP_ */