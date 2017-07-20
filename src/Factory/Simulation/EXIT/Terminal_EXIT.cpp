#include "Terminal_EXIT.hpp"

using namespace aff3ct::factory;

void Terminal_EXIT::build_args(arg_map &req_args, arg_map &opt_args)
{
	Terminal::build_args(req_args, opt_args);
}

void Terminal_EXIT::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	Terminal::store_args(ar, params);
}

void Terminal_EXIT::group_args(arg_grp& ar)
{
	Terminal::group_args(ar);
}

void Terminal_EXIT::header(params_list& head_ter, const parameters& params)
{
	Terminal::header(head_ter, params);
}
