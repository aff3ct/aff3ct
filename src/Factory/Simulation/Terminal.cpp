#include "Terminal.hpp"

using namespace aff3ct::factory;

void Terminal::build_args(arg_map &req_args, arg_map &opt_args)
{
	opt_args[{"term-no"}] =
		{"",
		 "disable reporting for each iteration."};

	opt_args[{"term-freq"}] =
		{"positive_int",
		 "display frequency in ms (refresh time step for each iteration, 0 = disable display refresh)."};
}

void Terminal::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	if(ar.exist_arg({"term-no"  })) params.disabled  = true;
	if(ar.exist_arg({"term-freq"})) params.frequency = std::chrono::milliseconds(ar.get_arg_int({"term-freq"}));
}

void Terminal::group_args(arg_grp& ar)
{
	ar.push_back({"term", "Terminal parameter(s)"});
}

void Terminal::header(params_list& head_ter, const parameters& params)
{
}
