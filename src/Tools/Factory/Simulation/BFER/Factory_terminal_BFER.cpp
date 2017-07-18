#include "Factory_terminal_BFER.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_terminal_BFER::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_terminal::build_args(req_args, opt_args);

	opt_args[{"term-type"}] =
		{"string",
		 "select the terminal type you want.",
		 "STD"};
}

void Factory_terminal_BFER::store_args(const Arguments_reader& ar, parameters &params)
{
	Factory_terminal::store_args(ar, params);

	if(ar.exist_arg({"term-type"})) params.type = ar.get_arg({"term-type"});
}

void Factory_terminal_BFER::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_terminal::group_args(ar);
}

void Factory_terminal_BFER::header(params_list& head_ter, const parameters& params)
{
	Factory_terminal::header(head_ter, params);
}
