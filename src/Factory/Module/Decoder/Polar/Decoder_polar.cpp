#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive.hpp"
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive_sys.hpp"
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_fast_sys.hpp"
#include "Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive.hpp"
#include "Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive_sys.hpp"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive.hpp"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive_sys.hpp"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_fast_sys.hpp"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_MEM_fast_sys.hpp"
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA.hpp"
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA_sys.hpp"
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_fast_CA_sys.hpp"
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_MEM_fast_CA_sys.hpp"
#include "Module/Decoder/Polar/ASCL/Decoder_polar_ASCL_fast_CA_sys.hpp"
#include "Module/Decoder/Polar/ASCL/Decoder_polar_ASCL_MEM_fast_CA_sys.hpp"

//#define API_POLAR_DYNAMIC 1

#include "Tools/Code/Polar/API/API_polar_dynamic_seq.hpp"
#include "Tools/Code/Polar/API/API_polar_dynamic_intra.hpp"
#ifdef API_POLAR_DYNAMIC
#include "Tools/Code/Polar/API/API_polar_dynamic_inter.hpp"
#include "Tools/Code/Polar/API/API_polar_dynamic_inter_8bit_bitpacking.hpp"
#else
#include "Tools/Code/Polar/API/API_polar_static_seq.hpp"
#include "Tools/Code/Polar/API/API_polar_static_inter.hpp"
#include "Tools/Code/Polar/API/API_polar_static_inter_8bit_bitpacking.hpp"
#include "Tools/Code/Polar/API/API_polar_static_intra_8bit.hpp"
#include "Tools/Code/Polar/API/API_polar_static_intra_16bit.hpp"
#include "Tools/Code/Polar/API/API_polar_static_intra_32bit.hpp"
#endif

#include "Tools/Code/Polar/nodes_parser.h"

#include "Decoder_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_polar::name   = "Decoder Polar";
const std::string aff3ct::factory::Decoder_polar::prefix = "dec";

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_polar::parameters
::build_siso(const std::vector<bool> &frozen_bits) const
{
	if (this->type == "SCAN" && this->systematic)
	{
		if (this->implem == "NAIVE") return new module::Decoder_polar_SCAN_naive_sys<B, Q, tools::f_LLR<Q>, tools::v_LLR<Q>, tools::h_LLR<B,Q>>(this->K, this->N_cw, this->n_ite, frozen_bits, this->n_frames);
	}
	else if (this->type == "SCAN" && !this->systematic)
	{
		if (this->implem == "NAIVE") return new module::Decoder_polar_SCAN_naive    <B, Q, tools::f_LLR<Q>, tools::v_LLR<Q>, tools::h_LLR<B,Q>>(this->K, this->N_cw, this->n_ite, frozen_bits, this->n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, class API_polar>
module::Decoder_SIHO<B,Q>* Decoder_polar::parameters
::_build(const std::vector<bool> &frozen_bits, module::CRC<B> *crc) const
{
	int idx_r0, idx_r1;
	auto polar_patterns = tools::nodes_parser(this->polar_nodes, idx_r0, idx_r1);

	if (!this->systematic) // non-systematic encoding
	{
		if (this->implem == "NAIVE")
		{
			if (crc == nullptr || crc->get_size() == 0)
			{
				     if (this->type == "SC"  ) return new module::Decoder_polar_SC_naive        <B,Q,tools::f_LLR<Q>,tools::g_LLR<B,Q>,tools::h_LLR<B,Q>>(this->K, this->N_cw,              frozen_bits,       this->n_frames);
				else if (this->type == "SCAN") return new module::Decoder_polar_SCAN_naive      <B,Q,tools::f_LLR<Q>,tools::v_LLR<  Q>,tools::h_LLR<B,Q>>(this->K, this->N_cw, this->n_ite, frozen_bits,       this->n_frames);
				else if (this->type == "SCL" ) return new module::Decoder_polar_SCL_naive       <B,Q,tools::f_LLR<Q>,tools::g_LLR<B,Q>                  >(this->K, this->N_cw, this->L,     frozen_bits,       this->n_frames);
			}
			else
				     if (this->type == "SCL" ) return new module::Decoder_polar_SCL_naive_CA    <B,Q,tools::f_LLR<Q>,tools::g_LLR<B,Q>                  >(this->K, this->N_cw, this->L,     frozen_bits, *crc, this->n_frames);
		}
	}
	else // systematic encoding
	{
		if (this->implem == "NAIVE")
		{
			if (crc == nullptr || crc->get_size() == 0)
			{
				     if (this->type == "SC"  ) return new module::Decoder_polar_SC_naive_sys    <B,Q,tools::f_LLR<Q>,tools::g_LLR<B,Q>,tools::h_LLR<B,Q>>(this->K, this->N_cw,              frozen_bits,       this->n_frames);
				else if (this->type == "SCAN") return new module::Decoder_polar_SCAN_naive_sys  <B,Q,tools::f_LLR<Q>,tools::v_LLR<  Q>,tools::h_LLR<B,Q>>(this->K, this->N_cw, this->n_ite, frozen_bits,       this->n_frames);
				else if (this->type == "SCL" ) return new module::Decoder_polar_SCL_naive_sys   <B,Q,tools::f_LLR<Q>,tools::g_LLR<B,Q>                  >(this->K, this->N_cw, this->L,     frozen_bits,       this->n_frames);
			}
			else
				     if (this->type == "SCL" ) return new module::Decoder_polar_SCL_naive_CA_sys<B,Q,tools::f_LLR<Q>,tools::g_LLR<B,Q>                  >(this->K, this->N_cw, this->L,     frozen_bits, *crc, this->n_frames);
		}
		else if (this->implem == "FAST")
		{
			if (crc == nullptr || crc->get_size() == 0)
			{
				     if (this->type == "SC"  ) return new module::Decoder_polar_SC_fast_sys<B, Q, API_polar>(this->K, this->N_cw, frozen_bits, polar_patterns, idx_r0, idx_r1, this->n_frames);
			}
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, class API_polar>
module::Decoder_SIHO<B,Q>* Decoder_polar::parameters
::_build_scl_fast(const std::vector<bool> &frozen_bits, module::CRC<B> *crc) const
{
	int idx_r0, idx_r1;
	auto polar_patterns = tools::nodes_parser(this->polar_nodes, idx_r0, idx_r1);

	if (this->implem == "FAST" && this->systematic)
	{
		if (crc != nullptr && crc->get_size() > 0)
		{
			     if (this->type == "ASCL"    ) return new module::Decoder_polar_ASCL_fast_CA_sys    <B, Q, API_polar>(this->K, this->N_cw, this->L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc, this->full_adaptive, this->n_frames);
			else if (this->type == "ASCL_MEM") return new module::Decoder_polar_ASCL_MEM_fast_CA_sys<B, Q, API_polar>(this->K, this->N_cw, this->L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc, this->full_adaptive, this->n_frames);
			else if (this->type == "SCL"     ) return new module::Decoder_polar_SCL_fast_CA_sys     <B, Q, API_polar>(this->K, this->N_cw, this->L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc,                      this->n_frames);
			else if (this->type == "SCL_MEM" ) return new module::Decoder_polar_SCL_MEM_fast_CA_sys <B, Q, API_polar>(this->K, this->N_cw, this->L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc,                      this->n_frames);
		}
		else
		{
			     if (this->type == "SCL"     ) return new module::Decoder_polar_SCL_fast_sys        <B, Q, API_polar>(this->K, this->N_cw, this->L, frozen_bits, polar_patterns, idx_r0, idx_r1,                            this->n_frames);
			else if (this->type == "SCL_MEM" ) return new module::Decoder_polar_SCL_MEM_fast_sys    <B, Q, API_polar>(this->K, this->N_cw, this->L, frozen_bits, polar_patterns, idx_r0, idx_r1,                            this->n_frames);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_polar::parameters
::build(const std::vector<bool> &frozen_bits, module::CRC<B> *crc) const
{
	if (this->type.find("SCL") != std::string::npos && this->implem == "FAST")
	{
		if (this->simd_strategy == "INTRA")
		{
			if (typeid(B) == typeid(signed char))
			{
				return _build_scl_fast<B,Q,tools::API_polar_dynamic_intra<B,Q>>(frozen_bits, crc);
			}
			else if (typeid(B) == typeid(short))
			{
				return _build_scl_fast<B,Q,tools::API_polar_dynamic_intra<B,Q>>(frozen_bits, crc);
			}
			else if (typeid(B) == typeid(int))
			{
				return _build_scl_fast<B,Q,tools::API_polar_dynamic_intra<B,Q>>(frozen_bits, crc);
			}
		}
		else if (this->simd_strategy.empty())
		{
			return _build_scl_fast<B,Q,tools::API_polar_dynamic_seq<B,Q>>(frozen_bits, crc);
		}
	}

	if (this->simd_strategy == "INTER" && this->type == "SC" && this->implem == "FAST")
	{
		if (typeid(B) == typeid(signed char))
		{
#ifdef ENABLE_BIT_PACKING
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_inter_8bit_bitpacking<B,Q>;
#else
			using API_polar = tools::API_polar_static_inter_8bit_bitpacking<B,Q>;
#endif
#else
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_inter<B,Q>;
#else
			using API_polar = tools::API_polar_static_inter<B,Q>;
#endif
#endif
			return _build<B,Q,API_polar>(frozen_bits, crc);
		}
		else
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_inter<B,Q>;
#else
			using API_polar = tools::API_polar_static_inter<B,Q>;
#endif
			return _build<B,Q,API_polar>(frozen_bits, crc);
		}
	}
	else if (this->simd_strategy == "INTRA" && this->implem == "FAST")
	{
		if (typeid(B) == typeid(signed char))
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_intra<B,Q>;
#else
			using API_polar = tools::API_polar_static_intra_8bit<B,Q>;
#endif
			return _build<B,Q,API_polar>(frozen_bits, crc);
		}
		else if (typeid(B) == typeid(short))
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_intra<B,Q>;
#else
			using API_polar = tools::API_polar_static_intra_16bit<B,Q>;
#endif
			return _build<B,Q,API_polar>(frozen_bits, crc);
		}
		else if (typeid(B) == typeid(int))
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_intra<B,Q>;
#else
			using API_polar = tools::API_polar_static_intra_32bit<B,Q>;
#endif
			return _build<B,Q,API_polar>(frozen_bits, crc);
		}
	}
	else if (this->simd_strategy.empty())
	{
#ifdef API_POLAR_DYNAMIC
		using API_polar = tools::API_polar_dynamic_seq<B,Q>;
#else
		using API_polar = tools::API_polar_static_seq<B,Q>;
#endif
		return _build<B,Q,API_polar>(frozen_bits, crc);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder_SISO_SIHO<B,Q>* Decoder_polar
::build_siso(const parameters& params, const std::vector<bool> &frozen_bits)
{
	return params.template build_siso<B,Q>(frozen_bits);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Decoder_polar
::build(const parameters& params, const std::vector<bool> &frozen_bits, module::CRC<B> *crc)
{
	return params.template build<B,Q>(frozen_bits, crc);
}

void Decoder_polar
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args, p);

	opt_args[{p+"-type", "D"}].push_back("SC, SCL, SCL_MEM, ASCL, ASCL_MEM, SCAN");

	opt_args[{p+"-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the SCAN decoder."};

	opt_args[{p+"-lists", "L"}] =
		{"positive_int",
		 "maximal number of paths in the SCL decoder."};

	opt_args[{p+"-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};

	opt_args[{p+"-polar-nodes"}] =
		{"string",
		 "the type of nodes you want to detect in the Polar tree (ex: \"{R0,R1,R0L,REP_2-8,REPL,SPC_4+}\")."};

	opt_args[{p+"-partial-adaptive"}] =
		{"",
		 "enable the partial adaptive mode for the ASCL decoder (by default full adaptive is selected)."};

	opt_args[{p+"-no-sys"}] =
		{"",
		 "does not suppose a systematic encoding."};
}

void Decoder_polar
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type   = "SC";
	params.implem = "FAST";

	Decoder::store_args(vals, params, p);

	if(exist(vals, {p+"-ite",         "i"})) params.n_ite         = std::stoi(vals.at({p+"-ite",    "i"}));
	if(exist(vals, {p+"-lists",       "L"})) params.L             = std::stoi(vals.at({p+"-lists",  "L"}));
	if(exist(vals, {p+"-simd"            })) params.simd_strategy =           vals.at({p+"-simd"       });
	if(exist(vals, {p+"-polar-nodes"     })) params.polar_nodes   =           vals.at({p+"-polar-nodes"});
	if(exist(vals, {p+"-partial-adaptive"})) params.full_adaptive = false;

	// force 1 iteration max if not SCAN (and polar code)
	if (params.type != "SCAN") params.n_ite = 1;
}

void Decoder_polar
::make_header(params_list& head_dec, const parameters& params, const bool full)
{
	Decoder::make_header(head_dec, params, full);

	if (!params.simd_strategy.empty())
		head_dec.push_back(std::make_pair("SIMD strategy", params.simd_strategy));

	if (params.type == "SCAN")
		head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));

	if (params.type == "SCL" || params.type == "SCL_MEM")
		head_dec.push_back(std::make_pair("Num. of lists (L)", std::to_string(params.L)));

	if (params.type == "ASCL" || params.type == "ASCL_MEM")
	{
		auto adaptative_mode = params.full_adaptive ? "full" : "partial";
		head_dec.push_back(std::make_pair("Max num. of lists (L)", std::to_string(params.L)));
		head_dec.push_back(std::make_pair("Adaptative mode", adaptative_mode));
	}

	if ((params.type == "SC"      ||
	     params.type == "SCL"     ||
	     params.type == "ASCL"    ||
	     params.type == "SCL_MEM" ||
	     params.type == "ASCL_MEM") && params.implem == "FAST")
		head_dec.push_back(std::make_pair("Polar node types", params.polar_nodes));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar::parameters::build_siso<B_8 ,Q_8 >(const std::vector<bool>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_polar::parameters::build_siso<B_16,Q_16>(const std::vector<bool>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_polar::parameters::build_siso<B_32,Q_32>(const std::vector<bool>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_polar::parameters::build_siso<B_64,Q_64>(const std::vector<bool>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar::build_siso<B_8 ,Q_8 >(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&);
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_polar::build_siso<B_16,Q_16>(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&);
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_polar::build_siso<B_32,Q_32>(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&);
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_polar::build_siso<B_64,Q_64>(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&);
#else
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_polar::parameters::build_siso<B,Q>(const std::vector<bool>&) const;
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_polar::build_siso<B,Q>(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&);
#endif

#ifdef MULTI_PREC
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar::parameters::build<B_8 ,Q_8 >(const std::vector<bool>&, module::CRC<B_8 >*) const;
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_polar::parameters::build<B_16,Q_16>(const std::vector<bool>&, module::CRC<B_16>*) const;
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_polar::parameters::build<B_32,Q_32>(const std::vector<bool>&, module::CRC<B_32>*) const;
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_polar::parameters::build<B_64,Q_64>(const std::vector<bool>&, module::CRC<B_64>*) const;
template aff3ct::module::Decoder_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&, module::CRC<B_8 >*);
template aff3ct::module::Decoder_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_polar::build<B_16,Q_16>(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&, module::CRC<B_16>*);
template aff3ct::module::Decoder_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_polar::build<B_32,Q_32>(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&, module::CRC<B_32>*);
template aff3ct::module::Decoder_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_polar::build<B_64,Q_64>(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&, module::CRC<B_64>*);
#else
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_polar::parameters::build<B,Q>(const std::vector<bool>&, module::CRC<B>*) const;
template aff3ct::module::Decoder_SIHO<B,Q>* aff3ct::factory::Decoder_polar::build<B,Q>(const aff3ct::factory::Decoder_polar::parameters&, const std::vector<bool>&, module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation
