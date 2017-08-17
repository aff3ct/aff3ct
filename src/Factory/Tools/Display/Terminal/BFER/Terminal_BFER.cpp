#include "Terminal_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Terminal_BFER::name   = "Terminal BFER";
const std::string aff3ct::factory::Terminal_BFER::prefix = "ter";

template <typename B>
tools::Terminal_BFER<B>* Terminal_BFER
::build(const parameters &params, const module::Monitor<B> &monitor, const std::chrono::nanoseconds *d_decod_total)
{
	if (params.type == "STD") return new tools::Terminal_BFER<B>(params.K, params.N ? params.N : params.K, monitor, d_decod_total);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Terminal_BFER::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Terminal::build_args(req_args, opt_args, p);

	req_args[{p+"-info-bits", "K"}] =
		{"positive_int",
		 "number of information bits."};

	opt_args[{p+"-type"}] =
		{"string",
		 "select the terminal type you want.",
		 "STD"};

	opt_args[{p+"-cw-size", "N"}] =
		{"positive_int",
		 "number of bits in the codeword."};
}

void Terminal_BFER::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Terminal::store_args(vals, params, p);

	if(exist(vals, {p+"-info-bits", "K"})) params.K    = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-cw-size",   "N"})) params.N    = std::stoi(vals.at({p+"-cw-size",   "N"}));
	if(exist(vals, {p+"-type"          })) params.type =           vals.at({p+"-type"          });
}

void Terminal_BFER::make_header(params_list& head_ter, const parameters& params, const bool full)
{
	head_ter.push_back(std::make_pair("Type", params.type));
	if (full) head_ter.push_back(std::make_pair("Info bits (K)", std::to_string(params.K)));

	if (params.N && full)
		head_ter.push_back(std::make_pair("Codeword size (N)", std::to_string(params.N)));

	Terminal::make_header(head_ter, params, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Terminal_BFER<B_8 >* aff3ct::factory::Terminal_BFER::build<B_8 >(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B_8 >&, const std::chrono::nanoseconds*);
template aff3ct::tools::Terminal_BFER<B_16>* aff3ct::factory::Terminal_BFER::build<B_16>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B_16>&, const std::chrono::nanoseconds*);
template aff3ct::tools::Terminal_BFER<B_32>* aff3ct::factory::Terminal_BFER::build<B_32>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B_32>&, const std::chrono::nanoseconds*);
template aff3ct::tools::Terminal_BFER<B_64>* aff3ct::factory::Terminal_BFER::build<B_64>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B_64>&, const std::chrono::nanoseconds*);
#else
template aff3ct::tools::Terminal_BFER<B>* aff3ct::factory::Terminal_BFER::build<B>(const aff3ct::factory::Terminal_BFER::parameters&, const aff3ct::module::Monitor<B>&, const std::chrono::nanoseconds*);
#endif
