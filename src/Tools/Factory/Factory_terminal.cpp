#include "Factory_terminal.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_terminal::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ------------------------------------------------------------------------------------------------------ terminal
	opt_args[{"term-no"}] =
		{"",
		 "disable reporting for each iteration."};

	opt_args[{"term-freq"}] =
		{"positive_int",
		 "display frequency in ms (refresh time step for each iteration, 0 = disable display refresh)."};
}

void Factory_terminal::store_args(const Arguments_reader& ar, parameters &params)
{
	// ------------------------------------------------------------------------------------------------------ terminal
	if(ar.exist_arg({"term-no"  })) params.disabled  = true;
	if(ar.exist_arg({"term-freq"})) params.frequency = std::chrono::milliseconds(ar.get_arg_int({"term-freq"}));
}

void Factory_terminal::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"term", "Terminal parameter(s)"});
}

void Factory_terminal::header(params_list& head_ter, const parameters& params)
{

}




void Factory_terminal_BFER::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_terminal::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------ terminal
	opt_args[{"term-type"}] =
		{"string",
		 "select the terminal type you want.",
		 "STD, LEGACY"};
}

void Factory_terminal_BFER::store_args(const Arguments_reader& ar, parameters &params)
{
	Factory_terminal::store_args(ar, params);

	// ------------------------------------------------------------------------------------------------------ terminal
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
