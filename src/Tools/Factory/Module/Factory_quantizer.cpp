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
::build(const parameters& params)
{
	     if (params.type == "STD"     ) return new Quantizer_standard<R,Q>(params.size, params.n_decimals, params.n_bits,               params.n_frames);
	else if (params.type == "STD_FAST") return new Quantizer_fast    <R,Q>(params.size, params.n_decimals, params.n_bits,               params.n_frames);
	else if (params.type == "TRICKY"  ) return new Quantizer_tricky  <R,Q>(params.size, params.range,      params.n_bits, params.sigma, params.n_frames);
	else if (params.type == "NO"      ) return new Quantizer_NO      <R,Q>(params.size,                                                 params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_quantizer
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	req_args[{"qnt-size", "N"}] =
		{"positive_int",
		 "number of real to quantize."};

	opt_args[{"qnt-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

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

	opt_args[{"qnt-sigma"}] =
		{"posive_float",
		 "noise variance value."};
}

void Factory_quantizer
::store_args(const Arguments_reader& ar, parameters &params)
{
	params.size = ar.get_arg_int({"qnt-size", "N"});
	if(ar.exist_arg({"qnt-fra", "F"})) params.n_frames = ar.get_arg_int({"qnt-fra", "F"});
	if(ar.exist_arg({"qnt-type"})) params.type = ar.get_arg({"qnt-type"});
	if(ar.exist_arg({"qnt-dec"})) params.n_decimals = ar.get_arg_int({"qnt-dec"});
	if(ar.exist_arg({"qnt-bits"})) params.n_bits = ar.get_arg_int({"qnt-bits"});
	if(ar.exist_arg({"qnt-range"})) params.range = ar.get_arg_float({"qnt-range"});
	if(ar.exist_arg({"qnt-sigma"})) params.sigma = ar.get_arg_float({"qnt-sigma"});
}

void Factory_quantizer
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"qnt", "Quantizer parameter(s)"});
}

void Factory_quantizer
::header(params_list& head_gnt, const parameters& params)
{
	// ----------------------------------------------------------------------------------------------------- quantizer
	std::string quantif = "unused";
	if (params.type == "TRICKY")
		quantif = "{"+std::to_string(params.n_bits)+", "+std::to_string(params.range)+"f}";
	else if (params.type == "STD" || params.type == "STD_FAST")
		quantif = "{"+std::to_string(params.n_bits)+", "+std::to_string(params.n_decimals)+"}";

	head_gnt.push_back(std::make_pair("Type", params.type));
	head_gnt.push_back(std::make_pair("Frame size (N)", std::to_string(params.size)));
	head_gnt.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
	head_gnt.push_back(std::make_pair("Fixed-point config.", quantif));
	if (params.sigma != -1.f)
		head_gnt.push_back(std::make_pair("Sigma value", std::to_string(params.sigma)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Quantizer<R_8 ,Q_8 >* aff3ct::tools::Factory_quantizer::build<R_8 ,Q_8 >(const aff3ct::tools::Factory_quantizer::parameters&);
template aff3ct::module::Quantizer<R_16,Q_16>* aff3ct::tools::Factory_quantizer::build<R_16,Q_16>(const aff3ct::tools::Factory_quantizer::parameters&);
template aff3ct::module::Quantizer<R_32,Q_32>* aff3ct::tools::Factory_quantizer::build<R_32,Q_32>(const aff3ct::tools::Factory_quantizer::parameters&);
template aff3ct::module::Quantizer<R_64,Q_64>* aff3ct::tools::Factory_quantizer::build<R_64,Q_64>(const aff3ct::tools::Factory_quantizer::parameters&);
#else
template aff3ct::module::Quantizer<R,Q>* aff3ct::tools::Factory_quantizer::build<R,Q>(const aff3ct::tools::Factory_quantizer::parameters&);
#endif
// ==================================================================================== explicit template instantiation
