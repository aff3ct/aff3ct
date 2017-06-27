#ifndef FACTORY_SIMULATION_HPP
#define FACTORY_SIMULATION_HPP

#include <string>
#include <iostream>

#include "Tools/Arguments_reader.hpp"
#include "Tools/params.h"
#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_simulation : public Factory
{
	static void build_args(tools::Arguments_reader::arg_map &req_args, tools::Arguments_reader::arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, tools::parameters &params);
	static void group_args(tools::Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_SIMULATION_HPP */
