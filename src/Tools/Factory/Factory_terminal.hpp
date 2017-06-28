#ifndef FACTORY_TERMINAL_HPP_
#define FACTORY_TERMINAL_HPP_

#include <string>
#include <chrono>

#include "Tools/Arguments_reader.hpp"
#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_terminal : Factory
{
	struct terminal_params
	{
		std::chrono::milliseconds frequency = std::chrono::milliseconds(500);
		bool                      disabled  = false;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, terminal_params& params);
	static void group_args(Arguments_reader::arg_grp& ar);
};

struct Factory_terminal_BFER : Factory_terminal
{
	struct terminal_params_BFER : terminal_params
	{
		std::string type = "STD";
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, terminal_params_BFER& params);
	static void group_args(Arguments_reader::arg_grp& ar);
};

struct Factory_terminal_EXIT : Factory_terminal
{
	struct terminal_params_EXIT : terminal_params
	{
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, terminal_params_EXIT& params);
	static void group_args(Arguments_reader::arg_grp& ar);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_TERMINAL_HPP_ */
