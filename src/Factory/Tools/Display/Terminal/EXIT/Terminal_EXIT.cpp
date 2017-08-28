#include "Tools/Exception/exception.hpp"

#include "Terminal_EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Terminal_EXIT::name   = "Terminal EXIT";
const std::string aff3ct::factory::Terminal_EXIT::prefix = "ter";

template <typename B, typename R>
tools::Terminal_EXIT<B,R>* Terminal_EXIT::parameters
::build(const module::Monitor_EXIT<B,R> &monitor) const
{
	if (this->type == "STD") return new tools::Terminal_EXIT<B,R>(monitor);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
tools::Terminal_EXIT<B,R>* Terminal_EXIT
::build(const parameters &params, const module::Monitor_EXIT<B,R> &monitor)
{
	return params.build<B,R>(monitor);
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

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Terminal_EXIT<B_8 ,R_8 >* aff3ct::factory::Terminal_EXIT::parameters::build<B_8 ,R_8 >(const aff3ct::module::Monitor_EXIT<B_8 ,R_8 >&) const;
template aff3ct::tools::Terminal_EXIT<B_16,R_16>* aff3ct::factory::Terminal_EXIT::parameters::build<B_16,R_16>(const aff3ct::module::Monitor_EXIT<B_16,R_16>&) const;
template aff3ct::tools::Terminal_EXIT<B_32,R_32>* aff3ct::factory::Terminal_EXIT::parameters::build<B_32,R_32>(const aff3ct::module::Monitor_EXIT<B_32,R_32>&) const;
template aff3ct::tools::Terminal_EXIT<B_64,R_64>* aff3ct::factory::Terminal_EXIT::parameters::build<B_64,R_64>(const aff3ct::module::Monitor_EXIT<B_64,R_64>&) const;
template aff3ct::tools::Terminal_EXIT<B_8 ,R_8 >* aff3ct::factory::Terminal_EXIT::build<B_8 ,R_8 >(const aff3ct::factory::Terminal_EXIT::parameters&, const aff3ct::module::Monitor_EXIT<B_8 ,R_8 >&);
template aff3ct::tools::Terminal_EXIT<B_16,R_16>* aff3ct::factory::Terminal_EXIT::build<B_16,R_16>(const aff3ct::factory::Terminal_EXIT::parameters&, const aff3ct::module::Monitor_EXIT<B_16,R_16>&);
template aff3ct::tools::Terminal_EXIT<B_32,R_32>* aff3ct::factory::Terminal_EXIT::build<B_32,R_32>(const aff3ct::factory::Terminal_EXIT::parameters&, const aff3ct::module::Monitor_EXIT<B_32,R_32>&);
template aff3ct::tools::Terminal_EXIT<B_64,R_64>* aff3ct::factory::Terminal_EXIT::build<B_64,R_64>(const aff3ct::factory::Terminal_EXIT::parameters&, const aff3ct::module::Monitor_EXIT<B_64,R_64>&);
#else
template aff3ct::tools::Terminal_EXIT<B,R>* aff3ct::factory::Terminal_EXIT::parameters::build<B,R>(const aff3ct::module::Monitor_EXIT<B,R>&) const;
template aff3ct::tools::Terminal_EXIT<B,R>* aff3ct::factory::Terminal_EXIT::build<B,R>(const aff3ct::factory::Terminal_EXIT::parameters&, const aff3ct::module::Monitor_EXIT<B,R>&);
#endif
