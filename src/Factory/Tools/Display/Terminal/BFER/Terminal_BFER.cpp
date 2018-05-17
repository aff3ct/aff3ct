#include "Terminal_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Terminal_BFER_name   = "Terminal BFER";
const std::string aff3ct::factory::Terminal_BFER_prefix = "ter";

Terminal_BFER::parameters
::parameters(const std::string &prefix)
: Terminal::parameters(Terminal_BFER_name, prefix)
{
}

Terminal_BFER::parameters
::~parameters()
{
}

Terminal_BFER::parameters* Terminal_BFER::parameters
::clone() const
{
	return new Terminal_BFER::parameters(*this);
}

void Terminal_BFER::parameters
::get_description(tools::Argument_map_info &args) const
{
	Terminal::parameters::get_description(args);

	auto p = this->get_prefix();

	args.add(
		{p+"-type"},
		tools::Text(tools::Including_set("STD")),
		"select the terminal type you want.");
}

void Terminal_BFER::parameters
::store(const tools::Argument_map_value &vals)
{
	Terminal::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-type"})) this->type = vals.at({p+"-type"});
}

void Terminal_BFER::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));

	Terminal::parameters::get_headers(headers, full);
}

template <typename B, typename R>
tools::Terminal_BFER<B,R>* Terminal_BFER::parameters
::build(const module::Monitor_BFER<B,R> &monitor, const bool display_mutinfo) const
{
	if (this->type == "STD") return new tools::Terminal_BFER<B,R>(monitor, display_mutinfo);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
tools::Terminal_BFER<B,R>* Terminal_BFER
::build(const parameters &params, const module::Monitor_BFER<B,R> &monitor, const bool display_mutinfo)
{
	return params.template build<B,R>(monitor, display_mutinfo);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Terminal_BFER<B_8 ,R_8 >* aff3ct::factory::Terminal_BFER::parameters::build<B_8 ,R_8 >(const aff3ct::module::Monitor_BFER<B_8 ,R_8 >&, const bool) const;
template aff3ct::tools::Terminal_BFER<B_16,R_16>* aff3ct::factory::Terminal_BFER::parameters::build<B_16,R_16>(const aff3ct::module::Monitor_BFER<B_16,R_16>&, const bool) const;
template aff3ct::tools::Terminal_BFER<B_32,R_32>* aff3ct::factory::Terminal_BFER::parameters::build<B_32,R_32>(const aff3ct::module::Monitor_BFER<B_32,R_32>&, const bool) const;
template aff3ct::tools::Terminal_BFER<B_64,R_64>* aff3ct::factory::Terminal_BFER::parameters::build<B_64,R_64>(const aff3ct::module::Monitor_BFER<B_64,R_64>&, const bool) const;
template aff3ct::tools::Terminal_BFER<B_8 ,R_8 >* aff3ct::factory::Terminal_BFER::build<B_8 ,R_8 >(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor_BFER<B_8 ,R_8 >&, const bool);
template aff3ct::tools::Terminal_BFER<B_16,R_16>* aff3ct::factory::Terminal_BFER::build<B_16,R_16>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor_BFER<B_16,R_16>&, const bool);
template aff3ct::tools::Terminal_BFER<B_32,R_32>* aff3ct::factory::Terminal_BFER::build<B_32,R_32>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor_BFER<B_32,R_32>&, const bool);
template aff3ct::tools::Terminal_BFER<B_64,R_64>* aff3ct::factory::Terminal_BFER::build<B_64,R_64>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor_BFER<B_64,R_64>&, const bool);
#else
template aff3ct::tools::Terminal_BFER<B,R>* aff3ct::factory::Terminal_BFER::parameters::build<B,R>(const aff3ct::module::Monitor_BFER<B,R>&, const bool) const;
template aff3ct::tools::Terminal_BFER<B,R>* aff3ct::factory::Terminal_BFER::build<B,R>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor_BFER<B,R>&, const bool);
#endif
