#include "Factory_terminal_EXIT.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_terminal_EXIT::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_terminal::build_args(req_args, opt_args);
}

void Factory_terminal_EXIT::store_args(const Arguments_reader& ar, parameters &params)
{
	Factory_terminal::store_args(ar, params);
}

void Factory_terminal_EXIT::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_terminal::group_args(ar);
}

void Factory_terminal_EXIT::header(params_list& head_ter, const parameters& params)
{
	Factory_terminal::header(head_ter, params);
}
