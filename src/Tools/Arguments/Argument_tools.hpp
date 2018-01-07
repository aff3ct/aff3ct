#ifndef ARGUMENT_TOOLS_HPP_
#define ARGUMENT_TOOLS_HPP_

#include <typeinfo>
#include <stdexcept>
#include <type_traits>
#include <iostream>
#include "Argument_map.hpp"

namespace aff3ct
{
namespace tools
{
	template <typename T, typename... Options>
	void add_options(tools::Argument_type_limited_T<T>& atl, const size_t set_pos, Options... new_options)
	{
		auto* set_ptr = (&atl)->get_range(set_pos);

		try
		{
			auto* arg_range = static_cast<tools::Set_range<T>*>(set_ptr);
			arg_range->add_options({new_options...});
		}
		catch(std::exception& e)
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
		catch(std::exception& e)
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
