#include "Terminal_BFER.hpp"

using namespace aff3ct::factory;

void Terminal_BFER::build_args(arg_map &req_args, arg_map &opt_args)
{
	Terminal::build_args(req_args, opt_args);

	opt_args[{"term-type"}] =
		{"string",
		 "select the terminal type you want.",
		 "STD"};
}

void Terminal_BFER::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	Terminal::store_args(ar, params);

	if(ar.exist_arg({"term-type"})) params.type = ar.get_arg({"term-type"});
}

void Terminal_BFER::group_args(arg_grp& ar)
{
	Terminal::group_args(ar);
}

void Terminal_BFER::header(params_list& head_ter, const parameters& params)
{
	Terminal::header(head_ter, params);
}
