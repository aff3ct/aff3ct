#ifndef ARGUMENT_TOOLS_HPP_
#define ARGUMENT_TOOLS_HPP_

#include <typeinfo>
#include <stdexcept>
#include "Argument_map.hpp"
#include "Argument_type_basics.hpp"
#include "Argument_range_basics.hpp"

namespace aff3ct
{
namespace tools
{
	template <typename T>
	void add_options(tools::Argument_type_limited<T>& atl, const std::vector<T>& new_options, const size_t set_pos = 0)
	{
		try
		{
			auto& arg_range = dynamic_cast<tools::Set<T>&>(*atl.get_ranges().at(set_pos));
			arg_range.add_options(new_options);
		}
		catch(std::exception& e)
		{
			std::string message = "The Argument Argument_type_limited<";
			message += typeid(T).name();
			message += "> object '" + atl.get_title() + "' has no class child Set<" + typeid(int).name() + ">";
			throw std::runtime_error(message);
		}
	}

	template <typename T>
	void add_options(Argument_type& at, const std::vector<T>& new_options, const size_t set_pos = 0)
	{
		try
		{
			auto& limited = dynamic_cast<tools::Argument_type_limited<T>&>(at);
			add_options(limited, new_options, set_pos);
		}
		catch(std::exception& e)
		{
			std::string message = "The Argument type object '" + at.get_title() + "' has no class child "
			                    + "Argument_type_limited<" + typeid(T).name() + ">";
			throw std::runtime_error(message);
		}
	}

	template <typename T>
	void add_options(Argument_info& ai, const std::vector<T>& new_options, const size_t set_pos = 0)
	{
		add_options(*ai.type, new_options, set_pos);
	}

	template <typename T>
	void add_options(Argument_info* ai, const std::vector<T>& new_options, const size_t set_pos = 0)
	{
		add_options(*ai->type, new_options, set_pos);
	}
}
}

#endif /* ARGUMENT_TOOLS_HPP_ */
