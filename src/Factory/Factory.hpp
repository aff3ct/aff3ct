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
#include <vector>
#include <map>

#include "Tools/Display/bash_tools.h"

namespace aff3ct
{
namespace factory
{
using params_list = std::vector<std::pair<std::string,std::string>>;
using arg_map     = std::map<std::vector<std::string>, std::vector<std::string>>;
using arg_grp     = std::vector<std::vector<std::string>>;
using arg_val_map = std::map<std::vector<std::string>, std::string>;

bool exist(const arg_val_map &vals, const std::vector<std::string> &tags);

struct Header
{
	static void print_parameters(std::string grp_name, params_list params, int max_n_chars,
	                             std::ostream& stream = std::cout);

	static void compute_max_n_chars(const params_list& params, int& max_n_chars);
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
