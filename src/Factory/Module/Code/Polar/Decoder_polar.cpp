#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"

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
module::Decoder_SISO<B,Q>* Decoder_polar
::build_siso(const parameters& params, const mipp::vector<B> &frozen_bits)
{
	if (params.type == "SCAN" && params.systematic)
	{
		if (params.implem == "NAIVE") return new module::Decoder_polar_SCAN_naive_sys<B, Q, tools::init_LLR<Q>, tools::f_LLR<Q>, tools::v_LLR<Q>, tools::h_LLR<B,Q>>(params.K, params.N_cw, params.n_ite, frozen_bits, params.n_frames);
	}
	else if (params.type == "SCAN" && !params.systematic)
	{
		if (params.implem == "NAIVE") return new module::Decoder_polar_SCAN_naive    <B, Q, tools::init_LLR<Q>, tools::f_LLR<Q>, tools::v_LLR<Q>, tools::h_LLR<B,Q>>(params.K, params.N_cw, params.n_ite, frozen_bits, params.n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, class API_polar>
module::Decoder<B,Q>* Decoder_polar
::_build(const parameters& params, const mipp::vector<B> &frozen_bits, module::CRC<B> *crc)
{
	int idx_r0, idx_r1;
	auto polar_patterns = tools::nodes_parser(params.polar_nodes, idx_r0, idx_r1);

	if (!params.systematic) // non-systematic encoding
	{
		if (params.implem == "NAIVE")
		{
			if (crc == nullptr || crc->get_size() == 0)
			{
				     if (params.type == "SC"  ) return new module::Decoder_polar_SC_naive        <B,Q,                   tools::f_LLR<Q>,tools::g_LLR<B,Q>,tools::h_LLR<B,Q>>(params.K, params.N_cw,               frozen_bits,       params.n_frames);
				else if (params.type == "SCAN") return new module::Decoder_polar_SCAN_naive      <B,Q,tools::init_LLR<Q>,tools::f_LLR<Q>,tools::v_LLR<  Q>,tools::h_LLR<B,Q>>(params.K, params.N_cw, params.n_ite, frozen_bits,       params.n_frames);
				else if (params.type == "SCL" ) return new module::Decoder_polar_SCL_naive       <B,Q,                   tools::f_LLR<Q>,tools::g_LLR<B,Q>                  >(params.K, params.N_cw, params.L,     frozen_bits,       params.n_frames);
			}
			else
				     if (params.type == "SCL" ) return new module::Decoder_polar_SCL_naive_CA    <B,Q,                   tools::f_LLR<Q>,tools::g_LLR<B,Q>                  >(params.K, params.N_cw, params.L,     frozen_bits, *crc, params.n_frames);
		}
	}
	else // systematic encoding
	{
		if (params.implem == "NAIVE")
		{
			if (crc == nullptr || crc->get_size() == 0)
			{
				     if (params.type == "SC"  ) return new module::Decoder_polar_SC_naive_sys    <B,Q,                   tools::f_LLR<Q>,tools::g_LLR<B,Q>,tools::h_LLR<B,Q>>(params.K, params.N_cw,               frozen_bits,       params.n_frames);
				else if (params.type == "SCAN") return new module::Decoder_polar_SCAN_naive_sys  <B,Q,tools::init_LLR<Q>,tools::f_LLR<Q>,tools::v_LLR<  Q>,tools::h_LLR<B,Q>>(params.K, params.N_cw, params.n_ite, frozen_bits,       params.n_frames);
				else if (params.type == "SCL" ) return new module::Decoder_polar_SCL_naive_sys   <B,Q,                   tools::f_LLR<Q>,tools::g_LLR<B,Q>                  >(params.K, params.N_cw, params.L,     frozen_bits,       params.n_frames);
			}
			else
				     if (params.type == "SCL" ) return new module::Decoder_polar_SCL_naive_CA_sys<B,Q,                   tools::f_LLR<Q>,tools::g_LLR<B,Q>                  >(params.K, params.N_cw, params.L,     frozen_bits, *crc, params.n_frames);
		}
		else if (params.implem == "FAST")
		{
			if (crc == nullptr || crc->get_size() == 0)
			{
				     if (params.type == "SC"  ) return new module::Decoder_polar_SC_fast_sys<B, Q, API_polar>(params.K, params.N_cw, frozen_bits, polar_patterns, idx_r0, idx_r1, params.n_frames);
			}
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, class API_polar>
module::Decoder<B,Q>* Decoder_polar
::_build_scl_fast(const parameters& params, const mipp::vector<B> &frozen_bits, module::CRC<B> *crc)
{
	int idx_r0, idx_r1;
	auto polar_patterns = tools::nodes_parser(params.polar_nodes, idx_r0, idx_r1);

	if (params.implem == "FAST" && params.systematic)
	{
		if (crc != nullptr && crc->get_size() > 0)
		{
			     if (params.type == "ASCL"    ) return new module::Decoder_polar_ASCL_fast_CA_sys    <B, Q, API_polar>(params.K, params.N_cw, params.L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc, params.full_adaptive, params.n_frames);
			else if (params.type == "ASCL_MEM") return new module::Decoder_polar_ASCL_MEM_fast_CA_sys<B, Q, API_polar>(params.K, params.N_cw, params.L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc, params.full_adaptive, params.n_frames);
			else if (params.type == "SCL"     ) return new module::Decoder_polar_SCL_fast_CA_sys     <B, Q, API_polar>(params.K, params.N_cw, params.L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc,                       params.n_frames);
			else if (params.type == "SCL_MEM" ) return new module::Decoder_polar_SCL_MEM_fast_CA_sys <B, Q, API_polar>(params.K, params.N_cw, params.L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc,                       params.n_frames);
		}
		else
		{
			     if (params.type == "SCL"     ) return new module::Decoder_polar_SCL_fast_sys        <B, Q, API_polar>(params.K, params.N_cw, params.L, frozen_bits, polar_patterns, idx_r0, idx_r1,                             params.n_frames);
			else if (params.type == "SCL_MEM" ) return new module::Decoder_polar_SCL_MEM_fast_sys    <B, Q, API_polar>(params.K, params.N_cw, params.L, frozen_bits, polar_patterns, idx_r0, idx_r1,                             params.n_frames);
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Decoder<B,Q>* Decoder_polar
::build(const parameters& params, const mipp::vector<B> &frozen_bits, module::CRC<B> *crc)
{
	if (params.type.find("SCL") != std::string::npos && params.implem == "FAST")
	{
		if (params.simd_strategy == "INTRA")
		{
			if (typeid(B) == typeid(signed char))
			{
				using API_polar = tools::API_polar_dynamic_intra
				                  <B, Q, tools::f_LLR  <Q>, tools::g_LLR  <B,Q>, tools::g0_LLR  <Q>, tools::h_LLR  <B,Q>, tools::xo_STD  <B>,
				                         tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;

				return _build_scl_fast<B,Q,API_polar>(params, frozen_bits, crc);
			}
			else if (typeid(B) == typeid(short))
			{
				using API_polar = tools::API_polar_dynamic_intra
				                  <B, Q, tools::f_LLR  <Q>, tools::g_LLR  <B,Q>, tools::g0_LLR  <Q>, tools::h_LLR  <B,Q>, tools::xo_STD  <B>,
				                         tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;

				return _build_scl_fast<B,Q,API_polar>(params, frozen_bits, crc);
			}
			else if (typeid(B) == typeid(int))
			{
				using API_polar = tools::API_polar_dynamic_intra
				                  <B, Q, tools::f_LLR  <Q>, tools::g_LLR  <B,Q>, tools::g0_LLR  <Q>, tools::h_LLR  <B,Q>, tools::xo_STD  <B>,
				                         tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;

				return _build_scl_fast<B,Q,API_polar>(params, frozen_bits, crc);
			}
		}
		else if (params.simd_strategy.empty())
		{
			using API_polar = tools::API_polar_dynamic_seq
			                  <B, Q, tools::f_LLR<Q>, tools::g_LLR<B,Q>, tools::g0_LLR<Q>, tools::h_LLR<B,Q>, tools::xo_STD<B>>;

			return _build_scl_fast<B,Q,API_polar>(params, frozen_bits, crc);
		}
	}

	if (params.simd_strategy == "INTER" && params.type == "SC" && params.implem == "FAST")
	{
		if (typeid(B) == typeid(signed char))
		{
#ifdef ENABLE_BIT_PACKING
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_inter_8bit_bitpacking
			                  <B, Q, tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#else
			using API_polar = tools::API_polar_static_inter_8bit_bitpacking
			                  <B, Q, tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#endif
#else
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_inter
			                  <B, Q, tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#else
			using API_polar = tools::API_polar_static_inter
			                  <B, Q, tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#endif
#endif
			return _build<B,Q,API_polar>(params, frozen_bits, crc);
		}
		else
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_inter
			                  <B, Q, tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#else
			using API_polar = tools::API_polar_static_inter
			                  <B, Q, tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#endif
			return _build<B,Q,API_polar>(params, frozen_bits, crc);
		}
	}
	else if (params.simd_strategy == "INTRA" && params.implem == "FAST")
	{
		if (typeid(B) == typeid(signed char))
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_intra
			                  <B, Q, tools::f_LLR  <Q>, tools::g_LLR  <B,Q>, tools::g0_LLR  <Q>, tools::h_LLR  <B,Q>, tools::xo_STD  <B>,
			                         tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#else
			using API_polar = tools::API_polar_static_intra_8bit
			                  <B, Q, tools::f_LLR  <Q>, tools::g_LLR  <B,Q>, tools::g0_LLR  <Q>, tools::h_LLR  <B,Q>, tools::xo_STD  <B>,
			                         tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#endif
			return _build<B,Q,API_polar>(params, frozen_bits, crc);
		}
		else if (typeid(B) == typeid(short))
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_intra
			                  <B, Q, tools::f_LLR  <Q>, tools::g_LLR  <B,Q>, tools::g0_LLR  <Q>, tools::h_LLR  <B,Q>, tools::xo_STD  <B>,
			                         tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#else
			using API_polar = tools::API_polar_static_intra_16bit
			                  <B, Q, tools::f_LLR  <Q>, tools::g_LLR  <B,Q>, tools::g0_LLR  <Q>, tools::h_LLR  <B,Q>, tools::xo_STD  <B>,
			                         tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#endif
			return _build<B,Q,API_polar>(params, frozen_bits, crc);
		}
		else if (typeid(B) == typeid(int))
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = tools::API_polar_dynamic_intra
			                  <B, Q, tools::f_LLR  <Q>, tools::g_LLR  <B,Q>, tools::g0_LLR  <Q>, tools::h_LLR  <B,Q>, tools::xo_STD  <B>,
			                         tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#else
			using API_polar = tools::API_polar_static_intra_32bit
			                  <B, Q, tools::f_LLR  <Q>, tools::g_LLR  <B,Q>, tools::g0_LLR  <Q>, tools::h_LLR  <B,Q>, tools::xo_STD  <B>,
			                         tools::f_LLR_i<Q>, tools::g_LLR_i<B,Q>, tools::g0_LLR_i<Q>, tools::h_LLR_i<B,Q>, tools::xo_STD_i<B>>;
#endif
			return _build<B,Q,API_polar>(params, frozen_bits, crc);
		}
	}
	else if (params.simd_strategy.empty())
	{
#ifdef API_POLAR_DYNAMIC
		using API_polar = tools::API_polar_dynamic_seq
		                  <B, Q, tools::f_LLR<Q>, tools::g_LLR<B,Q>, tools::g0_LLR<Q>, tools::h_LLR<B,Q>, tools::xo_STD<B>>;
#else
		using API_polar = tools::API_polar_static_seq
		                  <B, Q, tools::f_LLR<Q>, tools::g_LLR<B,Q>, tools::g0_LLR<Q>, tools::h_LLR<B,Q>, tools::xo_STD<B>>;
#endif
		return _build<B,Q,API_polar>(params, frozen_bits, crc);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_polar
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args);

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
		 "the type of nodes you want to detect in the Polar tree (ex: {R0,R1,R0L,REP_2-8,REPL,SPC_4+})."};

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

	Decoder::store_args(vals, params);

	if(exist(vals, {p+"-ite",         "i"})) params.n_ite         = std::stoi(vals.at({p+"-ite",    "i"}));
	if(exist(vals, {p+"-lists",       "L"})) params.L             = std::stoi(vals.at({p+"-lists",  "L"}));
	if(exist(vals, {p+"-simd"            })) params.simd_strategy =           vals.at({p+"-simd"       });
	if(exist(vals, {p+"-polar-nodes"     })) params.polar_nodes   =           vals.at({p+"-polar-nodes"});
	if(exist(vals, {p+"-partial-adaptive"})) params.full_adaptive = false;

	// force 1 iteration max if not SCAN (and polar code)
	if (params.type != "SCAN") params.n_ite = 1;
}

void Decoder_polar
::make_header(params_list& head_dec, const parameters& params)
{
	Decoder::make_header(head_dec, params);

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
template aff3ct::module::Decoder_SISO<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar::build_siso<B_8 ,Q_8 >(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B_8 >&);
template aff3ct::module::Decoder_SISO<B_16,Q_16>* aff3ct::factory::Decoder_polar::build_siso<B_16,Q_16>(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B_16>&);
template aff3ct::module::Decoder_SISO<B_32,Q_32>* aff3ct::factory::Decoder_polar::build_siso<B_32,Q_32>(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B_32>&);
template aff3ct::module::Decoder_SISO<B_64,Q_64>* aff3ct::factory::Decoder_polar::build_siso<B_64,Q_64>(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B_64>&);
#else
template aff3ct::module::Decoder_SISO<B,Q>* aff3ct::factory::Decoder_polar::build_siso<B,Q>(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B>&);
#endif

#ifdef MULTI_PREC
template aff3ct::module::Decoder<B_8 ,Q_8 >* aff3ct::factory::Decoder_polar::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B_8 >&, module::CRC<B_8 >*);
template aff3ct::module::Decoder<B_16,Q_16>* aff3ct::factory::Decoder_polar::build<B_16,Q_16>(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B_16>&, module::CRC<B_16>*);
template aff3ct::module::Decoder<B_32,Q_32>* aff3ct::factory::Decoder_polar::build<B_32,Q_32>(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B_32>&, module::CRC<B_32>*);
template aff3ct::module::Decoder<B_64,Q_64>* aff3ct::factory::Decoder_polar::build<B_64,Q_64>(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B_64>&, module::CRC<B_64>*);
#else
template aff3ct::module::Decoder<B,Q>* aff3ct::factory::Decoder_polar::build<B,Q>(const aff3ct::factory::Decoder_polar::parameters&, const mipp::vector<B>&, module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation
