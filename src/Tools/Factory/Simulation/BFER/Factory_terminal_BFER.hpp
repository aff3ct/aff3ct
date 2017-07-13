#ifndef FACTORY_TERMINAL_BFER_HPP_
#define FACTORY_TERMINAL_BFER_HPP_

#include <string>
#include <chrono>

#include "Tools/Arguments_reader.hpp"

#include "../Factory_terminal.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_terminal_BFER : Factory_terminal
{
	struct parameters : Factory_terminal::parameters
	{
		std::string type = "STD";
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_ter, const parameters& params);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_TERMINAL_HPP_ */
