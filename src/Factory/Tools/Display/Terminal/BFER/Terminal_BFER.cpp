#include "Terminal_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B>
tools::Terminal_BFER<B>* Terminal_BFER
::build(const parameters &params, const module::Monitor<B> &monitor, const std::chrono::nanoseconds *d_decod_total)
{
	if (params.type == "STD") return new tools::Terminal_BFER<B>(params.K, params.N ? params.N : params.K, monitor, d_decod_total);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Terminal_BFER::build_args(arg_map &req_args, arg_map &opt_args)
{
	Terminal::build_args(req_args, opt_args);

	req_args[{"term-info-bits", "K"}] =
		{"positive_int",
		 "number of information bits."};

	opt_args[{"term-type"}] =
		{"string",
		 "select the terminal type you want.",
		 "STD"};

	opt_args[{"term-cw-size", "N"}] =
		{"positive_int",
		 "number of bits in the codeword."};
}

void Terminal_BFER::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	Terminal::store_args(ar, params);

	if(ar.exist_arg({"term-info-bits", "K"})) params.K    = ar.get_arg_int({"term-info-bits", "K"});
	if(ar.exist_arg({"term-cw-size",   "N"})) params.N    = ar.get_arg_int({"term-cw-size",   "N"});
	if(ar.exist_arg({"term-type"          })) params.type = ar.get_arg    ({"term-type"          });
}

void Terminal_BFER::group_args(arg_grp& ar)
{
	Terminal::group_args(ar);
}

void Terminal_BFER::header(params_list& head_ter, const parameters& params)
{
	head_ter.push_back(std::make_pair("Type", params.type));
	head_ter.push_back(std::make_pair("Info bits (K)", std::to_string(params.K)));

	if (params.N)
		head_ter.push_back(std::make_pair("Codeword size (N)", std::to_string(params.N)));

	Terminal::header(head_ter, params);
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
