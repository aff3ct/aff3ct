#include "Tools/Exception/exception.hpp"

#include "Terminal_EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Terminal_EXIT::name   = "Terminal EXIT";
const std::string aff3ct::factory::Terminal_EXIT::prefix = "ter";

Terminal_EXIT::parameters
::parameters(const std::string prefix)
: Terminal::parameters(Terminal_EXIT::name, prefix)
{
}

Terminal_EXIT::parameters
::~parameters()
{
}

Terminal_EXIT::parameters* Terminal_EXIT::parameters
::clone() const
{
	return new Terminal_EXIT::parameters(*this);
}

void Terminal_EXIT::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Terminal::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args[{p+"-cw-size", "N"}] =
		{"strictly_positive_int",
		 "number of bits in the codeword."};

	opt_args[{p+"-type"}] =
		{"string",
		 "select the terminal type you want.",
		 "STD"};
}

void Terminal_EXIT::parameters
::store(const arg_val_map &vals)
{
	Terminal::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-type"})) this->type = vals.at({p+"-type"});
}

void Terminal_EXIT::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));

	Terminal::parameters::get_headers(headers, full);
}

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

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Terminal_EXIT<B_32,R_32>* aff3ct::factory::Terminal_EXIT::parameters::build<B_32,R_32>(const aff3ct::module::Monitor_EXIT<B_32,R_32>&) const;
template aff3ct::tools::Terminal_EXIT<B_64,R_64>* aff3ct::factory::Terminal_EXIT::parameters::build<B_64,R_64>(const aff3ct::module::Monitor_EXIT<B_64,R_64>&) const;
template aff3ct::tools::Terminal_EXIT<B_32,R_32>* aff3ct::factory::Terminal_EXIT::build<B_32,R_32>(const aff3ct::factory::Terminal_EXIT::parameters&, const aff3ct::module::Monitor_EXIT<B_32,R_32>&);
template aff3ct::tools::Terminal_EXIT<B_64,R_64>* aff3ct::factory::Terminal_EXIT::build<B_64,R_64>(const aff3ct::factory::Terminal_EXIT::parameters&, const aff3ct::module::Monitor_EXIT<B_64,R_64>&);
#else
template aff3ct::tools::Terminal_EXIT<B,R>* aff3ct::factory::Terminal_EXIT::parameters::build<B,R>(const aff3ct::module::Monitor_EXIT<B,R>&) const;
template aff3ct::tools::Terminal_EXIT<B,R>* aff3ct::factory::Terminal_EXIT::build<B,R>(const aff3ct::factory::Terminal_EXIT::parameters&, const aff3ct::module::Monitor_EXIT<B,R>&);
#endif
