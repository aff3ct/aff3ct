#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/LDPC/BP/Flooding/SPA/Decoder_LDPC_BP_flooding_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/LSPA/Decoder_LDPC_BP_flooding_log_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/ONMS/Decoder_LDPC_BP_flooding_offset_normalize_min_sum.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_A.hpp"
#include "Module/Decoder/LDPC/BP/Layered/SPA/Decoder_LDPC_BP_layered_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/LSPA/Decoder_LDPC_BP_layered_log_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/ONMS/Decoder_LDPC_BP_layered_offset_normalize_min_sum.hpp"
#include "Module/Decoder/LDPC/BP/Layered/ONMS/Decoder_LDPC_BP_layered_ONMS_inter.hpp"

#include "Decoder_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_LDPC::name   = "Decoder LDPC";
const std::string aff3ct::factory::Decoder_LDPC::prefix = "dec";

template <typename B, typename R>
module::Decoder_SISO<B,R>* Decoder_LDPC
::build(const parameters& params, const tools::Sparse_matrix &H, const std::vector<unsigned> &info_bits_pos)
{
	if ((params.type == "BP" || params.type == "BP_FLOODING") && params.simd_strategy.empty())
	{
		     if (params.implem == "ONMS") return new module::Decoder_LDPC_BP_flooding_ONMS     <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos, params.norm_factor, params.offset, params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "GALA") return new module::Decoder_LDPC_BP_flooding_GALA     <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "SPA" ) return new module::Decoder_LDPC_BP_flooding_SPA      <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "LSPA") return new module::Decoder_LDPC_BP_flooding_LSPA     <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
	}
	else if (params.type == "BP_LAYERED" && params.simd_strategy.empty())
	{
		     if (params.implem == "ONMS") return new module::Decoder_LDPC_BP_layered_ONMS      <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos, params.norm_factor, params.offset, params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "SPA" ) return new module::Decoder_LDPC_BP_layered_SPA       <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
		else if (params.implem == "LSPA") return new module::Decoder_LDPC_BP_layered_LSPA      <B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos,                                    params.enable_syndrome, params.syndrome_depth, params.n_frames);
	}
	else if (params.type == "BP_LAYERED" && params.simd_strategy == "INTER")
	{
		     if (params.implem == "ONMS") return new module::Decoder_LDPC_BP_layered_ONMS_inter<B,R>(params.K, params.N_cw, params.n_ite, H, info_bits_pos, params.norm_factor, params.offset, params.enable_syndrome, params.syndrome_depth, params.n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_LDPC
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args);

	req_args[{p+"-h-path"}] =
		{"string",
		 "path to the H matrix (AList formated file)."};

	opt_args[{p+"-type", "D"}].push_back("BP, BP_FLOODING, BP_LAYERED");

	opt_args[{p+"-implem"}].push_back("ONMS, SPA, LSPA, GALA");

	opt_args[{p+"-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo decoder."};

	opt_args[{p+"-off"}] =
		{"float",
		 "offset used in the offset min-sum BP algorithm (works only with \"--dec-implem ONMS\")."};

	opt_args[{p+"-norm"}] =
		{"positive_float",
		 "normalization factor used in the normalized min-sum BP algorithm (works only with \"--dec-implem ONMS\")."};

	opt_args[{p+"-no-synd"}] =
		{"",
		 "disable the syndrome detection (disable the stop criterion in the LDPC decoders)."};

	opt_args[{p+"-synd-depth"}] =
		{"positive_int",
		 "successive number of iterations to validate the syndrome detection."};

	opt_args[{p+"-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTER"};
}

void Decoder_LDPC
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type   = "BP_FLOODING";
	params.implem = "SPA";

	Decoder::store_args(vals, params);

	if(exist(vals, {p+"-h-path"    })) params.H_alist_path    =           vals.at({p+"-h-path"    });
	if(exist(vals, {p+"-ite",   "i"})) params.n_ite           = std::stoi(vals.at({p+"-ite",   "i"}));
	if(exist(vals, {p+"-off"       })) params.offset          = std::stof(vals.at({p+"-off"       }));
	if(exist(vals, {p+"-norm"      })) params.norm_factor     = std::stof(vals.at({p+"-norm"      }));
	if(exist(vals, {p+"-synd-depth"})) params.syndrome_depth  = std::stoi(vals.at({p+"-synd-depth"}));
	if(exist(vals, {p+"-simd"      })) params.simd_strategy   =           vals.at({p+"-simd"      });
	if(exist(vals, {p+"-no-synd"   })) params.enable_syndrome = false;
}

void Decoder_LDPC
::make_header(params_list& head_dec, const parameters& params)
{
	Decoder::make_header(head_dec, params);

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
template aff3ct::module::Decoder_SISO<B_8 ,Q_8 >* aff3ct::factory::Decoder_LDPC::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
template aff3ct::module::Decoder_SISO<B_16,Q_16>* aff3ct::factory::Decoder_LDPC::build<B_16,Q_16>(const aff3ct::factory::Decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
template aff3ct::module::Decoder_SISO<B_32,Q_32>* aff3ct::factory::Decoder_LDPC::build<B_32,Q_32>(const aff3ct::factory::Decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
template aff3ct::module::Decoder_SISO<B_64,Q_64>* aff3ct::factory::Decoder_LDPC::build<B_64,Q_64>(const aff3ct::factory::Decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
#else
template aff3ct::module::Decoder_SISO<B,Q>* aff3ct::factory::Decoder_LDPC::build<B,Q>(const aff3ct::factory::Decoder_LDPC::parameters&, const aff3ct::tools::Sparse_matrix&, const std::vector<unsigned>&);
#endif
// ==================================================================================== explicit template instantiation
