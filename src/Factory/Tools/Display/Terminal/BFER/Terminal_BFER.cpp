#include "Terminal_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Terminal_BFER::name   = "Terminal BFER";
const std::string aff3ct::factory::Terminal_BFER::prefix = "ter";

template <typename B>
tools::Terminal_BFER<B>* Terminal_BFER::parameters
::build(const module::Monitor<B> &monitor) const
{
	if (this->type == "STD") return new tools::Terminal_BFER<B>(monitor);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
tools::Terminal_BFER<B>* Terminal_BFER
::build(const parameters &params, const module::Monitor<B> &monitor)
{
	return params.template build<B>(monitor);
}

void Terminal_BFER::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Terminal::build_args(req_args, opt_args, p);

	opt_args[{p+"-type"}] =
		{"string",
		 "select the terminal type you want.",
		 "STD"};
}

void Terminal_BFER::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Terminal::store_args(vals, params, p);

	if(exist(vals, {p+"-type"})) params.type = vals.at({p+"-type"});
}

void Terminal_BFER::make_header(params_list& head_ter, const parameters& params, const bool full)
{
	head_ter.push_back(std::make_pair("Type", params.type));

	Terminal::make_header(head_ter, params, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Terminal_BFER<B_8 >* aff3ct::factory::Terminal_BFER::parameters::build<B_8 >(const aff3ct::module::Monitor<B_8 >&) const;
template aff3ct::tools::Terminal_BFER<B_16>* aff3ct::factory::Terminal_BFER::parameters::build<B_16>(const aff3ct::module::Monitor<B_16>&) const;
template aff3ct::tools::Terminal_BFER<B_32>* aff3ct::factory::Terminal_BFER::parameters::build<B_32>(const aff3ct::module::Monitor<B_32>&) const;
template aff3ct::tools::Terminal_BFER<B_64>* aff3ct::factory::Terminal_BFER::parameters::build<B_64>(const aff3ct::module::Monitor<B_64>&) const;
template aff3ct::tools::Terminal_BFER<B_8 >* aff3ct::factory::Terminal_BFER::build<B_8 >(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B_8 >&);
template aff3ct::tools::Terminal_BFER<B_16>* aff3ct::factory::Terminal_BFER::build<B_16>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B_16>&);
template aff3ct::tools::Terminal_BFER<B_32>* aff3ct::factory::Terminal_BFER::build<B_32>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B_32>&);
template aff3ct::tools::Terminal_BFER<B_64>* aff3ct::factory::Terminal_BFER::build<B_64>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B_64>&);
#else
template aff3ct::tools::Terminal_BFER<B>* aff3ct::factory::Terminal_BFER::parameters::build<B>(const aff3ct::module::Monitor<B>&) const;
template aff3ct::tools::Terminal_BFER<B>* aff3ct::factory::Terminal_BFER::build<B>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B>&);
#endif
