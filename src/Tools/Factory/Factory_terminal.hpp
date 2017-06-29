#ifndef FACTORY_TERMINAL_HPP_
#define FACTORY_TERMINAL_HPP_

#include <string>
#include <chrono>

#include "Tools/Arguments_reader.hpp"
#include "Tools/Header.hpp"
#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_terminal : Factory
{
	struct terminal_parameters
	{
		std::chrono::milliseconds frequency = std::chrono::milliseconds(500);
		bool                      disabled  = false;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, terminal_parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_ter, const terminal_parameters& params);
};

struct Factory_terminal_BFER : Factory_terminal
{
	struct terminal_parameters_BFER : terminal_parameters
	{
		std::string type = "STD";
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, terminal_parameters_BFER& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_ter, const terminal_parameters_BFER& params);
};

struct Factory_terminal_EXIT : Factory_terminal
{
	struct terminal_parameters_EXIT : terminal_parameters
	{
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, terminal_parameters_EXIT& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_ter, const terminal_parameters_EXIT& params);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_TERMINAL_HPP_ */
