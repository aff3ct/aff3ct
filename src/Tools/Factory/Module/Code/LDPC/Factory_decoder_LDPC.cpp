#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/LDPC/BP/Flooding/SPA/Decoder_LDPC_BP_flooding_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/LSPA/Decoder_LDPC_BP_flooding_log_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/ONMS/Decoder_LDPC_BP_flooding_offset_normalize_min_sum.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_A.hpp"
#include "Module/Decoder/LDPC/BP/Layered/SPA/Decoder_LDPC_BP_layered_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/LSPA/Decoder_LDPC_BP_layered_log_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/ONMS/Decoder_LDPC_BP_layered_offset_normalize_min_sum.hpp"
#include "Module/Decoder/LDPC/BP/Layered/ONMS/Decoder_LDPC_BP_layered_ONMS_inter.hpp"

#include "Factory_decoder_LDPC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_SISO<B,R>* Factory_decoder_LDPC
::build(const parameters& params, const Sparse_matrix &H,
        const std::vector<unsigned> &info_bits_pos)
{
	if ((params.type == "BP" || params.type == "BP_FLOODING") && params.simd_strategy.empty())
	{
		     if (params.implem == "ONMS") return new Decoder_LDPC_BP_flooding_ONMS     <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos, params.norm_factor, params.offset, params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "GALA") return new Decoder_LDPC_BP_flooding_GALA     <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "SPA" ) return new Decoder_LDPC_BP_flooding_SPA      <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "LSPA") return new Decoder_LDPC_BP_flooding_LSPA     <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
	}
	else if (params.type == "BP_LAYERED" && params.simd_strategy.empty())
	{
		     if (params.implem == "ONMS") return new Decoder_LDPC_BP_layered_ONMS      <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos, params.norm_factor, params.offset, params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "SPA" ) return new Decoder_LDPC_BP_layered_SPA       <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "LSPA") return new Decoder_LDPC_BP_layered_LSPA      <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
	}
	else if (params.type == "BP_LAYERED" && params.simd_strategy == "INTER")
	{
		     if (params.implem == "ONMS") return new Decoder_LDPC_BP_layered_ONMS_inter<B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos, params.norm_factor, params.offset, params.enable_syndrome, params.syndrome_depth, params.n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_decoder_LDPC
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder::build_args(req_args, opt_args);

	req_args[{"dec-h-path"}] =
		{"string",
		 "path to the H matrix (AList formated file)."};

	opt_args[{"dec-type", "D"}].push_back("BP, BP_FLOODING, BP_LAYERED");

	opt_args[{"dec-implem"}].push_back("ONMS, SPA, LSPA, GALA");

	opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo decoder."};

	opt_args[{"dec-off"}] =
		{"float",
		 "offset used in the offset min-sum BP algorithm (works only with \"--dec-implem ONMS\")."};

	opt_args[{"dec-norm"}] =
		{"positive_float",
		 "normalization factor used in the normalized min-sum BP algorithm (works only with \"--dec-implem ONMS\")."};

	opt_args[{"dec-no-synd"}] =
		{"",
		 "disable the syndrome detection (disable the stop criterion in the LDPC decoders)."};

	opt_args[{"dec-synd-depth"}] =
		{"positive_int",
		 "successive number of iterations to validate the syndrome detection."};

	opt_args[{"dec-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTER"};
}

void Factory_decoder_LDPC
::store_args(const Arguments_reader& ar, parameters &params)
{
	params.type   = "BP_FLOODING";
	params.implem = "SPA";

	Factory_decoder::store_args(ar, params);

	if(ar.exist_arg({"dec-h-path"})) params.H_alist_path = ar.get_arg({"dec-h-path"});
	if(ar.exist_arg({"dec-ite","i"})) params.n_ite = ar.get_arg_int  ({"dec-ite", "i"});
	if(ar.exist_arg({"dec-off"})) params.offset = ar.get_arg_float({"dec-off"});
	if(ar.exist_arg({"dec-norm"})) params.norm_factor = ar.get_arg_float({"dec-norm"});
	if(ar.exist_arg({"dec-synd-depth"})) params.syndrome_depth = ar.get_arg_int({"dec-synd-depth"});
	if(ar.exist_arg({"dec-no-synd"})) params.enable_syndrome = false;
	if(ar.exist_arg({"dec-simd"})) params.simd_strategy = ar.get_arg({"dec-simd"});
}

void Factory_decoder_LDPC
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder::group_args(ar);
}

void Factory_decoder_LDPC
::header(params_list& head_dec, const parameters& params)
{
	Factory_decoder::header(head_dec, params);

	head_dec.push_back(std::make_pair("H matrix path", params.H_alist_path));

	if (!params.simd_strategy.empty())
		head_dec.push_back(std::make_pair("SIMD strategy", params.simd_strategy));

	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));

	if (params.implem == "ONMS")
	{
		head_dec.push_back(std::make_pair("Offset", std::to_string(params.offset)));
		head_dec.push_back(std::make_pair("Normalize factor", std::to_string(params.norm_factor)));
	}

	std::string syndrome = params.enable_syndrome ? "on" : "off";
	head_dec.push_back(std::make_pair("Stop criterion (syndrome)", syndrome));

	if (params.enable_syndrome)
		head_dec.push_back(std::make_pair("Stop criterion depth", std::to_string(params.syndrome_depth)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SISO<B_8 ,Q_8 >* aff3ct::tools::Factory_decoder_LDPC::build<B_8 ,Q_8 >(const aff3ct::tools::Factory_decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
template aff3ct::module::Decoder_SISO<B_16,Q_16>* aff3ct::tools::Factory_decoder_LDPC::build<B_16,Q_16>(const aff3ct::tools::Factory_decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
template aff3ct::module::Decoder_SISO<B_32,Q_32>* aff3ct::tools::Factory_decoder_LDPC::build<B_32,Q_32>(const aff3ct::tools::Factory_decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
template aff3ct::module::Decoder_SISO<B_64,Q_64>* aff3ct::tools::Factory_decoder_LDPC::build<B_64,Q_64>(const aff3ct::tools::Factory_decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
#else
template aff3ct::module::Decoder_SISO<B,Q>* aff3ct::tools::Factory_decoder_LDPC::build<B,Q>(const aff3ct::tools::Factory_decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
#endif
// ==================================================================================== explicit template instantiation
