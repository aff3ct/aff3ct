#ifndef FACTORY_TERMINAL_EXIT_HPP_
#define FACTORY_TERMINAL_EXIT_HPP_

#include <string>
#include <chrono>

#include "Tools/Arguments_reader.hpp"

#include "../Terminal.hpp"

namespace aff3ct
{
namespace factory
{
struct Terminal_EXIT : Terminal
{
	struct parameters : Terminal::parameters
	{
	};

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters& params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_ter, const parameters& params);
};
}
}

#endif /* FACTORY_TERMINAL_EXIT_HPP_ */
