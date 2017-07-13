#include <type_traits>

#include "Tools/Exception/exception.hpp"

#include "Module/Quantizer/Standard/Quantizer_standard.hpp"
#include "Module/Quantizer/Fast/Quantizer_fast.hpp"
#include "Module/Quantizer/Tricky/Quantizer_tricky.hpp"
#include "Module/Quantizer/NO/Quantizer_NO.hpp"

#include "Factory_quantizer.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R, typename Q>
Quantizer<R,Q>* Factory_quantizer
::build(const parameters& params, const float sigma)
{
	     if (params.type == "STD"     ) return new Quantizer_standard<R,Q>(params.size, params.n_decimals, params.n_bits,        params.n_frames);
	else if (params.type == "STD_FAST") return new Quantizer_fast    <R,Q>(params.size, params.n_decimals, params.n_bits,        params.n_frames);
	else if (params.type == "TRICKY"  ) return new Quantizer_tricky  <R,Q>(params.size, params.range,      params.n_bits, sigma, params.n_frames);
	else if (params.type == "NO"      ) return new Quantizer_NO      <R,Q>(params.size,                                          params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_quantizer
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ----------------------------------------------------------------------------------------------------- quantizer
	opt_args[{"qnt-type"}] =
		{"string",
		 "type of the quantizer to use in the simulation.",
		 "STD, STD_FAST, TRICKY"};

	opt_args[{"qnt-dec"}] =
		{"positive_int",
		 "the position of the fixed point in the quantified representation."};

	opt_args[{"qnt-bits"}] =
		{"positive_int",
		 "the number of bits used for the quantizer."};

	opt_args[{"qnt-range"}] =
		{"positive_float",
		 "the min/max bound for the tricky quantizer."};
}

void Factory_quantizer
::store_args(const Arguments_reader& ar, parameters &params,
	                       const int size, const int n_frames)
{
	params.size     = size;
	params.n_frames = n_frames;

	// ----------------------------------------------------------------------------------------------------- quantizer
	if(ar.exist_arg({"qnt-type" })) params.type       = ar.get_arg      ({"qnt-type" });
	if(ar.exist_arg({"qnt-dec"  })) params.n_decimals = ar.get_arg_int  ({"qnt-dec"  });
	if(ar.exist_arg({"qnt-bits" })) params.n_bits     = ar.get_arg_int  ({"qnt-bits" });
	if(ar.exist_arg({"qnt-range"})) params.range      = ar.get_arg_float({"qnt-range"});
}

void Factory_quantizer
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"qnt", "Quantizer parameter(s)"});
}

void Factory_quantizer
::header(params_list& head_qua, const parameters& params)
{
	// ----------------------------------------------------------------------------------------------------- quantizer
	std::string quantif = "unused";
	if (params.type == "TRICKY")
		quantif = "{"+std::to_string(params.n_bits)+", "+std::to_string(params.range)+"f}";
	else if (params.type == "STD" || params.type == "STD_FAST")
		quantif = "{"+std::to_string(params.n_bits)+", "+std::to_string(params.n_decimals)+"}";

	head_qua.push_back(std::make_pair("Type"               , params.type));
	head_qua.push_back(std::make_pair("Fixed-point config.", quantif    ));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Quantizer<R_8 ,Q_8 >* aff3ct::tools::Factory_quantizer::build<R_8 ,Q_8 >(const aff3ct::tools::Factory_quantizer::parameters&, const float);
template aff3ct::module::Quantizer<R_16,Q_16>* aff3ct::tools::Factory_quantizer::build<R_16,Q_16>(const aff3ct::tools::Factory_quantizer::parameters&, const float);
template aff3ct::module::Quantizer<R_32,Q_32>* aff3ct::tools::Factory_quantizer::build<R_32,Q_32>(const aff3ct::tools::Factory_quantizer::parameters&, const float);
template aff3ct::module::Quantizer<R_64,Q_64>* aff3ct::tools::Factory_quantizer::build<R_64,Q_64>(const aff3ct::tools::Factory_quantizer::parameters&, const float);
#else
template aff3ct::module::Quantizer<R,Q>* aff3ct::tools::Factory_quantizer::build<R,Q>(const aff3ct::tools::Factory_quantizer::parameters&, const float);
#endif
// ==================================================================================== explicit template instantiation
