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
	struct parameters
	{
		std::chrono::milliseconds frequency = std::chrono::milliseconds(500);
		bool                      disabled  = false;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_ter, const parameters& params);
};

struct Factory_terminal_BFER : Factory_terminal
{
	struct parameters : Factory_terminal::parameters
	{
		std::string type = "STD";
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_ter, const parameters& params);
};

struct Factory_terminal_EXIT : Factory_terminal::Factory_terminal
{
	struct parameters : Factory_terminal::parameters
	{
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_ter, const parameters& params);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_TERMINAL_HPP_ */
