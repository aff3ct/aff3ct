#ifndef ARGUMENT_TOOLS_HPP_
#define ARGUMENT_TOOLS_HPP_

#include <typeinfo>
#include <stdexcept>
#include <type_traits>
#include <iostream>

#include "Maps/Argument_map_info.hpp"
#include "Maps/Argument_map_group.hpp"
#include "Maps/Argument_map_value.hpp"

namespace aff3ct
{
namespace tools
{

/*******************************************************************************
 *                ADD RANGE TO AN ARGUMENT TYPE
 ******************************************************************************/

	template <typename CurrentType, typename... Ranges>
	void add_ranges(Argument_type** at, Ranges*... new_ranges)
	{
		CurrentType* old_arg_type;

		try
		{
			old_arg_type = dynamic_cast<CurrentType*>(*at);
		}
		catch(std::exception& e)
		{
			std::string message = "The Argument type object '" + (*at)->get_title() + "' is not parent of "
			                    + " the given CurrentType \"" + typeid(CurrentType).name() + "\"";
			throw std::runtime_error(message);
		}

		*at = old_arg_type->clone(new_ranges...);

		delete old_arg_type;
	}

	template <typename CurrentType, typename... Ranges>
	void add_ranges(Argument_info& ai, Ranges*... new_ranges)
	{
		add_ranges<CurrentType>(&(ai.type), new_ranges...);
	}

	template <typename CurrentType, typename... Ranges>
	void add_ranges(Argument_info* ai, Ranges*... new_ranges)
	{
		add_ranges<CurrentType>(&(ai->type), new_ranges...);
	}


/*******************************************************************************
 *                ADD OPTIONS TO A SET RANGE
 ******************************************************************************/

	template <typename T, typename... Options>
	void add_options(tools::Argument_type_limited_T<T>& atl, const size_t set_pos, Options... new_options)
	{
		auto* set_ptr = (&atl)->get_range(set_pos);

		try
		{
			auto* arg_range = static_cast<tools::Set_range<T>*>(set_ptr);
			arg_range->add_options({new_options...});
		}
		catch(std::exception&)
		{
			std::string message = "The Argument Argument_type_limited_T<";
			message += typeid(T).name();
			message += "> object '" + atl.get_title() + "' has no range Set_range<" + typeid(T).name() + ">";
			throw std::runtime_error(message);
		}
	}

	template <typename T, typename... Options>
	void _add_options(Argument_type& at, const size_t set_pos, Options... new_options)
	{
		tools::Argument_type_limited_T<T>* limited;

		try
		{
			limited = dynamic_cast<tools::Argument_type_limited_T<T>*>(&at);
		}
		catch(std::exception&)
		{
			std::string message = "The Argument type object '" + at.get_title() + "' has no class child "
			                    + "Argument_type_limited_T<" + typeid(T).name() + ">";
			throw std::runtime_error(message);
		}

		add_options(*limited, set_pos, new_options...);
	}

	template <typename T, typename... Options>
	typename std::enable_if<!std::is_same<T, char const*>::value,void>::type
	add_options(Argument_type& at, const size_t set_pos, T new_option, Options... new_options)
	{
		_add_options<T, T, Options...>(at, set_pos, new_option, new_options...);
	}

	template <typename T, typename... Options>
	typename std::enable_if<std::is_same<T, char const*>::value,void>::type
	add_options(Argument_type& at, const size_t set_pos, T new_option, Options... new_options)
	{
		_add_options<std::string, T, Options...>(at, set_pos, new_option, new_options...);
	}

	template <typename... Options>
	void add_options(Argument_info& ai, const size_t set_pos, Options... new_options)
	{
		add_options(*ai.type, set_pos, new_options...);
	}

	template <typename... Options>
	void add_options(Argument_info* ai, const size_t set_pos, Options... new_options)
	{
		add_options(*ai->type, set_pos, new_options...);
	}
}
}

#endif /* ARGUMENT_TOOLS_HPP_ */
