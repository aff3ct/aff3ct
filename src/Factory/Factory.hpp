/*!
 * \file
 * \brief Class builder.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <iostream>

#include "Tools/Display/bash_tools.h"

namespace aff3ct
{
namespace factory
{
using params_list = std::vector<std::pair<std::string,std::string>>;
using arg_map = std::map<std::vector<std::string>, std::vector<std::string>>;
using arg_grp = std::vector<std::vector<std::string>>;

struct Header
{
	static void print_parameters(std::string grp_name, params_list params, int max_n_chars,
	                             std::ostream& stream = std::cout)
	{
		stream << "# * " << tools::style(tools::style(grp_name, tools::Style::BOLD), tools::Style::UNDERLINED) << " ";
		for (auto i = 0; i < 46 - (int)grp_name.length(); i++) std::cout << "-";
		stream << std::endl;

		for (auto i = 0; i < (int)params.size(); i++)
		{
			stream << "#    ** " << tools::style(params[i].first, tools::Style::BOLD);
			for (auto j = 0; j < max_n_chars - (int)params[i].first.length(); j++) stream << " ";
			stream << " = " << params[i].second << std::endl;
		}
	}

	static void compute_max_n_chars(const params_list& params, int& max_n_chars)
	{
		for (unsigned i = 0; i < params.size(); i++)
			max_n_chars = std::max(max_n_chars, (int)params[i].first.length());
	}
};

/*!
 * \class Factory
 *
 * \brief Class builder.
 *
 * A factory is a structure dedicated to the object creation. The common usage is to call the build method which returns
 * a new allocated object.
 */
struct Factory {};
}
}

#endif /* FACTORY_HPP */
