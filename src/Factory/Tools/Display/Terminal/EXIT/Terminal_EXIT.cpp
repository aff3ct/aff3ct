#include "Tools/Exception/exception.hpp"

#include "Terminal_EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Terminal_EXIT::name   = "Terminal EXIT";
const std::string aff3ct::factory::Terminal_EXIT::prefix = "ter";

tools::Terminal_EXIT* Terminal_EXIT::parameters
::build(const int &cur_t, const int &trials, const double &I_A, const double &I_E) const
{
	if (this->type == "STD") return new tools::Terminal_EXIT(this->N, cur_t, trials, I_A, I_E);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

tools::Terminal_EXIT* Terminal_EXIT
::build(const parameters &params, const int &cur_t, const int &trials, const double &I_A, const double &I_E)
{
	return params.build(cur_t, trials, I_A, I_E);
}

void Terminal_EXIT::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Terminal::build_args(req_args, opt_args, p);

	req_args[{p+"-cw-size", "N"}] =
		{"positive_int",
		 "number of bits in the codeword."};

	opt_args[{p+"-type"}] =
		{"string",
		 "select the terminal type you want.",
		 "STD"};
}

void Terminal_EXIT::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Terminal::store_args(vals, params, p);

	if(exist(vals, {p+"-cw-size", "N"})) params.N    = std::stoi(vals.at({p+"-cw-size", "N"}));
	if(exist(vals, {p+"-type"        })) params.type =           vals.at({p+"-type"        });
}

void Terminal_EXIT::make_header(params_list& head_ter, const parameters& params, const bool full)
{
	head_ter.push_back(std::make_pair("Type", params.type));
	if (full) head_ter.push_back(std::make_pair("Codeword size (N)", std::to_string(params.N)));

	Terminal::make_header(head_ter, params, full);
}
