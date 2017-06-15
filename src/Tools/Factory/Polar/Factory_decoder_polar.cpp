#include "Tools/Exceptions/Cannot_allocate.hpp"

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

#include "Factory_decoder_polar.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_SISO<B,R>* Factory_decoder_polar<B,R>
::build_siso(const std::string      type,
             const std::string      implem,
             const int              K,
             const int              N,
             const mipp::vector<B> &frozen_bits,
             const bool             sys_encoding,
             const int              n_ite,
             const int              n_frames)
{
	if (type == "SCAN" && sys_encoding)
		if (implem == "NAIVE") return new Decoder_polar_SCAN_naive_sys<B, R, init_LLR<R>, f_LLR<R>, v_LLR<R>, h_LLR<B,R>>(K, N, n_ite, frozen_bits, n_frames);

	throw Cannot_allocate("aff3ct::tools::Factory_decoder_polar: the factory could not allocate the object.");
}

template <typename B, typename R>
template <class API_polar>
Decoder<B,R>* Factory_decoder_polar<B,R>
::_build(const std::string      type,
         const std::string      implem,
         const int              K,
         const int              N,
         const mipp::vector<B> &frozen_bits,
         const bool             sys_encoding,
         const std::string      polar_nodes,
         const int              L,
         const int              n_ite,
               CRC<B>          *crc,
         const bool             full_adaptive,
         const int              n_frames)
{
	int idx_r0, idx_r1;
	auto polar_patterns = nodes_parser(polar_nodes, idx_r0, idx_r1);

	if (!sys_encoding) // non-systematic encoding
	{
		if (implem == "NAIVE")
		{
			if (crc == nullptr || crc->get_size() == 0)
			{
				     if (type == "SC"  ) return new Decoder_polar_SC_naive        <B,R,            f_LLR<R>,g_LLR<B,R>,h_LLR<B,R>>(K, N,        frozen_bits,       n_frames);
				else if (type == "SCAN") return new Decoder_polar_SCAN_naive      <B,R,init_LLR<R>,f_LLR<R>,v_LLR<  R>,h_LLR<B,R>>(K, N, n_ite, frozen_bits,       n_frames);
				else if (type == "SCL" ) return new Decoder_polar_SCL_naive       <B,R,            f_LLR<R>,g_LLR<B,R>           >(K, N, L,     frozen_bits,       n_frames);
			}
			else
				     if (type == "SCL" ) return new Decoder_polar_SCL_naive_CA    <B,R,            f_LLR<R>,g_LLR<B,R>           >(K, N, L,     frozen_bits, *crc, n_frames);
		}
	}
	else // systematic encoding
	{
		if (implem == "NAIVE")
		{
			if (crc == nullptr || crc->get_size() == 0)
			{
				     if (type == "SC"  ) return new Decoder_polar_SC_naive_sys    <B,R,            f_LLR<R>,g_LLR<B,R>,h_LLR<B,R>>(K, N,        frozen_bits,       n_frames);
				else if (type == "SCAN") return new Decoder_polar_SCAN_naive_sys  <B,R,init_LLR<R>,f_LLR<R>,v_LLR<  R>,h_LLR<B,R>>(K, N, n_ite, frozen_bits,       n_frames);
				else if (type == "SCL" ) return new Decoder_polar_SCL_naive_sys   <B,R,            f_LLR<R>,g_LLR<B,R>           >(K, N, L,     frozen_bits,       n_frames);
			}
			else
				     if (type == "SCL" ) return new Decoder_polar_SCL_naive_CA_sys<B,R,            f_LLR<R>,g_LLR<B,R>           >(K, N, L,     frozen_bits, *crc, n_frames);
		}
		else if (implem == "FAST")
		{
			if (crc == nullptr || crc->get_size() == 0)
			{
				     if (type == "SC"  ) return new Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, polar_patterns, idx_r0, idx_r1, n_frames);
			}
		}
	}

	throw Cannot_allocate("aff3ct::tools::Factory_decoder_polar: the factory could not allocate the object.");
}

template <typename B, typename R>
template <class API_polar>
Decoder<B,R>* Factory_decoder_polar<B,R>
::_build_scl_fast(const std::string      type,
                  const std::string      implem,
                  const int              K,
                  const int              N,
                  const mipp::vector<B> &frozen_bits,
                  const bool             sys_encoding,
                  const std::string      polar_nodes,
                  const int              L,
                        CRC<B>          *crc,
                  const bool             full_adaptive,
                  const int              n_frames)
{
	int idx_r0, idx_r1;
	auto polar_patterns = nodes_parser(polar_nodes, idx_r0, idx_r1);

	if (implem == "FAST" && sys_encoding)
	{
		if (crc != nullptr && crc->get_size() > 0)
		{
			     if (type == "ASCL"    ) return new Decoder_polar_ASCL_fast_CA_sys    <B, R, API_polar>(K, N, L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc, full_adaptive, n_frames);
			else if (type == "ASCL_MEM") return new Decoder_polar_ASCL_MEM_fast_CA_sys<B, R, API_polar>(K, N, L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc, full_adaptive, n_frames);
			else if (type == "SCL"     ) return new Decoder_polar_SCL_fast_CA_sys     <B, R, API_polar>(K, N, L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc,                n_frames);
			else if (type == "SCL_MEM" ) return new Decoder_polar_SCL_MEM_fast_CA_sys <B, R, API_polar>(K, N, L, frozen_bits, polar_patterns, idx_r0, idx_r1, *crc,                n_frames);
		}
		else
		{
			     if (type == "SCL"     ) return new Decoder_polar_SCL_fast_sys        <B, R, API_polar>(K, N, L, frozen_bits, polar_patterns, idx_r0, idx_r1,                      n_frames);
			else if (type == "SCL_MEM" ) return new Decoder_polar_SCL_MEM_fast_sys    <B, R, API_polar>(K, N, L, frozen_bits, polar_patterns, idx_r0, idx_r1,                      n_frames);
		}
	}

	throw Cannot_allocate("aff3ct::tools::Factory_decoder_polar: the factory could not allocate the object.");
}

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_polar<B,R>
::build(const std::string      type,
        const std::string      implem,
        const int              K,
        const int              N,
        const mipp::vector<B> &frozen_bits,
        const std::string      simd_strategy,
        const bool             sys_encoding,
        const std::string      polar_nodes,
        const int              L,
        const int              n_ite,
              CRC<B>          *crc,
        const bool             full_adaptive,
        const int              n_frames)
{
	if (type.find("SCL") != std::string::npos && implem == "FAST")
	{
		if (simd_strategy == "INTRA")
		{
			if (typeid(B) == typeid(signed char))
			{
				using API_polar = API_polar_dynamic_intra
				                  <B, R, f_LLR  <R>, g_LLR  <B,R>, g0_LLR  <R>, h_LLR  <B,R>, xo_STD  <B>,
				                         f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;

				return _build_scl_fast<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, crc, full_adaptive, n_frames);
			}
			else if (typeid(B) == typeid(short))
			{
				using API_polar = API_polar_dynamic_intra
				                  <B, R, f_LLR  <R>, g_LLR  <B,R>, g0_LLR  <R>, h_LLR  <B,R>, xo_STD  <B>,
				                         f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;

				return _build_scl_fast<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, crc, full_adaptive, n_frames);
			}
			else if (typeid(B) == typeid(int))
			{
				using API_polar = API_polar_dynamic_intra
				                  <B, R, f_LLR  <R>, g_LLR  <B,R>, g0_LLR  <R>, h_LLR  <B,R>, xo_STD  <B>,
				                         f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;

				return _build_scl_fast<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, crc, full_adaptive, n_frames);
			}
		}
		else if (simd_strategy.empty())
		{
			using API_polar = API_polar_dynamic_seq
			                  <B, R, f_LLR<R>, g_LLR<B,R>, g0_LLR<R>, h_LLR<B,R>, xo_STD<B>>;

			return _build_scl_fast<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, crc, full_adaptive, n_frames);
		}
	}

	if (simd_strategy == "INTER" && type == "SC" && implem == "FAST")
	{
		if (typeid(B) == typeid(signed char))
		{
#ifdef ENABLE_BIT_PACKING
#ifdef API_POLAR_DYNAMIC
			using API_polar = API_polar_dynamic_inter_8bit_bitpacking
			                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#else
			using API_polar = API_polar_static_inter_8bit_bitpacking
			                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#endif
#else
#ifdef API_POLAR_DYNAMIC
			using API_polar = API_polar_dynamic_inter
			                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#else
			using API_polar = API_polar_static_inter
			                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#endif
#endif
			return _build<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, n_ite, crc, full_adaptive, n_frames);
		}
		else
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = API_polar_dynamic_inter
			                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#else
			using API_polar = API_polar_static_inter
			                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#endif
			return _build<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, n_ite, crc, full_adaptive, n_frames);
		}
	}
	else if (simd_strategy == "INTRA" && implem == "FAST")
	{
		if (typeid(B) == typeid(signed char))
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = API_polar_dynamic_intra
			                  <B, R, f_LLR  <R>, g_LLR  <B,R>, g0_LLR  <R>, h_LLR  <B,R>, xo_STD  <B>,
			                         f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#else
			using API_polar = API_polar_static_intra_8bit
			                  <B, R, f_LLR  <R>, g_LLR  <B,R>, g0_LLR  <R>, h_LLR  <B,R>, xo_STD  <B>,
			                         f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#endif
			return _build<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, n_ite, crc, full_adaptive, n_frames);
		}
		else if (typeid(B) == typeid(short))
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = API_polar_dynamic_intra
			                  <B, R, f_LLR  <R>, g_LLR  <B,R>, g0_LLR  <R>, h_LLR  <B,R>, xo_STD  <B>,
			                         f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#else
			using API_polar = API_polar_static_intra_16bit
			                  <B, R, f_LLR  <R>, g_LLR  <B,R>, g0_LLR  <R>, h_LLR  <B,R>, xo_STD  <B>,
			                         f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#endif
			return _build<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, n_ite, crc, full_adaptive, n_frames);
		}
		else if (typeid(B) == typeid(int))
		{
#ifdef API_POLAR_DYNAMIC
			using API_polar = API_polar_dynamic_intra
			                  <B, R, f_LLR  <R>, g_LLR  <B,R>, g0_LLR  <R>, h_LLR  <B,R>, xo_STD  <B>,
			                         f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#else
			using API_polar = API_polar_static_intra_32bit
			                  <B, R, f_LLR  <R>, g_LLR  <B,R>, g0_LLR  <R>, h_LLR  <B,R>, xo_STD  <B>,
			                         f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#endif
			return _build<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, n_ite, crc, full_adaptive, n_frames);
		}
	}
	else if (simd_strategy.empty())
	{
#ifdef API_POLAR_DYNAMIC
		using API_polar = API_polar_dynamic_seq
		                  <B, R, f_LLR<R>, g_LLR<B,R>, g0_LLR<R>, h_LLR<B,R>, xo_STD<B>>;
#else
		using API_polar = API_polar_static_seq
		                  <B, R, f_LLR<R>, g_LLR<B,R>, g0_LLR<R>, h_LLR<B,R>, xo_STD<B>>;
#endif
		return _build<API_polar>(type, implem, K, N, frozen_bits, sys_encoding, polar_nodes, L, n_ite, crc, full_adaptive, n_frames);
	}

	throw Cannot_allocate("aff3ct::tools::Factory_decoder_polar: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_polar<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_polar<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_polar<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_polar<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
