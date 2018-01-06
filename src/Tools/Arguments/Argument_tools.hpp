#ifndef ARGUMENT_TOOLS_HPP_
#define ARGUMENT_TOOLS_HPP_

#include <typeinfo>
#include <stdexcept>
#include "Argument_map.hpp"

namespace aff3ct
{
namespace tools
{
	template <typename T, typename... Options>
	void add_options(tools::Argument_type_limited<T>& atl, const size_t set_pos, Options... new_options)
	{
		try
		{
			auto& arg_range = dynamic_cast<tools::Set_range<T>&>(*atl.get_ranges().at(set_pos));
			arg_range.add_options(new_options...);
		}
		catch(std::exception& e)
		{
			std::string message = "The Argument Argument_type_limited<";
			message += typeid(T).name();
			message += "> object '" + atl.get_title() + "' has no class child Set_range<" + typeid(int).name() + ">";
			throw std::runtime_error(message);
		}
	}

	template <typename T, typename... Options>
	void add_options(Argument_type& at, const size_t set_pos, T new_option, Options... new_options)
	{
		tools::Argument_type_limited<T>* limited;

		try
		{
			limited = dynamic_cast<tools::Argument_type_limited<T>*>(&at);
		}
		catch(std::exception& e)
		{
			std::string message = "The Argument type object '" + at.get_title() + "' has no class child "
			                    + "Argument_type_limited<" + typeid(T).name() + ">";
			throw std::runtime_error(message);
		}

		add_options<T>(*limited, set_pos, new_option, new_options...);
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
