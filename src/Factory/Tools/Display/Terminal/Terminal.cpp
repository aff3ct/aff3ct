#include "Terminal.hpp"

using namespace aff3ct::factory;

const std::string aff3ct::factory::Terminal::name   = "Terminal";
const std::string aff3ct::factory::Terminal::prefix = "ter";

void Terminal::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	opt_args[{p+"-no"}] =
		{"",
		 "disable reporting for each iteration."};

	opt_args[{p+"-freq"}] =
		{"positive_int",
		 "display frequency in ms (refresh time step for each iteration, 0 = disable display refresh)."};
}

void Terminal::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p+"-no"  })) params.disabled  = true;
	if(exist(vals, {p+"-freq"})) params.frequency = std::chrono::milliseconds(std::stoi(vals.at({p+"-freq"})));
}

void Terminal::make_header(params_list& head_ter, const parameters& params)
{
	head_ter.push_back(std::make_pair("Enabled", params.disabled ? "no" : "yes"));
	head_ter.push_back(std::make_pair("Frequency (ms)", std::to_string(params.frequency.count())));
}
