#include "Module/Decoder/Polar/decoder_polar_functions.h"

#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive.hpp"
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive_sys.hpp"
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_fast_sys.hpp"
#include "Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive.hpp"
#include "Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive_sys.hpp"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive.hpp"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive_sys.hpp"
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA.hpp"
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA_sys.hpp"

// #define API_POLAR_DYNAMIC 1

#ifdef API_POLAR_DYNAMIC
#include "Module/Decoder/Polar/SC/API/API_polar_dynamic_seq.hpp"
#include "Module/Decoder/Polar/SC/API/API_polar_dynamic_inter.hpp"
#include "Module/Decoder/Polar/SC/API/API_polar_dynamic_inter_8bit_bitpacking.hpp"
#include "Module/Decoder/Polar/SC/API/API_polar_dynamic_intra.hpp"
#else
#include "Module/Decoder/Polar/SC/API/API_polar_static_seq.hpp"
#include "Module/Decoder/Polar/SC/API/API_polar_static_inter.hpp"
#include "Module/Decoder/Polar/SC/API/API_polar_static_inter_8bit_bitpacking.hpp"
#include "Module/Decoder/Polar/SC/API/API_polar_static_intra_8bit.hpp"
#include "Module/Decoder/Polar/SC/API/API_polar_static_intra_16bit.hpp"
#include "Module/Decoder/Polar/SC/API/API_polar_static_intra_32bit.hpp"
#endif

#include "Factory_decoder_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
SISO<R>* Factory_decoder_polar<B,R>
::build_siso(const parameters &params, const mipp::vector<B> &frozen_bits)
{
	SISO<R> *siso = nullptr;

	// build the decoder
	if (params.encoder.systematic) // non-systematic encoding / decoding
		if (params.channel.domain == "LLR")
			if (params.decoder.type == "SCAN" && params.decoder.implem == "NAIVE")
				siso = new Decoder_polar_SCAN_naive_sys<B, R, init_LLR<R>, f_LLR<R>, v_LLR<R>, h_LLR<B,R>>(params.code.K, params.code.m, params.decoder.n_ite, frozen_bits, params.simulation.inter_frame_level);
	return siso;
}

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_polar<B,R>
::build(const parameters &params, const mipp::vector<B> &frozen_bits, CRC<B> *crc)
{
	Decoder<B,R> *decoder = nullptr;

	// build the decoder
	if (!params.encoder.systematic) // non-systematic encoding
	{
		if (params.channel.domain == "LR" && params.decoder.simd_strategy.empty())
		{
			if (params.decoder.type == "SC" && params.decoder.implem == "NAIVE")
				decoder = new Decoder_polar_SC_naive<B,R,f_LR<R>,g_LR<B,R>,h_LR<B,R>>(params.code.K, params.code.N_code, frozen_bits, params.simulation.inter_frame_level);
		}
		else if (params.channel.domain == "LLR" && params.decoder.simd_strategy.empty())
		{
			if (params.decoder.type == "SC" && params.decoder.implem == "NAIVE")
				decoder = new Decoder_polar_SC_naive <B,R,f_LLR<R>,g_LLR<B,R>,h_LLR<B,R>>(params.code.K, params.code.N_code, frozen_bits, params.simulation.inter_frame_level);
			if (params.decoder.type == "SCAN" && params.decoder.implem == "NAIVE")
				decoder = new Decoder_polar_SCAN_naive<B,R,init_LLR<R>,f_LLR<R>,v_LLR<R>,h_LLR<B,R>>(params.code.K, params.code.m, params.decoder.n_ite, frozen_bits, params.simulation.inter_frame_level);
			if (params.decoder.type == "SCL" && params.decoder.implem == "NAIVE" && params.crc.type.empty())
				decoder = new Decoder_polar_SCL_naive<B,R,f_LLR<R>,g_LLR<B,R>>(params.code.K, params.code.N_code, params.decoder.L, frozen_bits, params.simulation.inter_frame_level);
			if (params.decoder.type == "SCL" && params.decoder.implem == "NAIVE" && !params.crc.type.empty())
				decoder = new Decoder_polar_SCL_naive_CA<B,R,f_LLR<R>,g_LLR<B,R>>(params.code.K, params.code.N_code, params.decoder.L, frozen_bits, *crc, params.simulation.inter_frame_level);
		}
	}
	else // systematic encoding
	{
		if (params.channel.domain == "LR" && params.decoder.simd_strategy.empty())
		{
			if (params.decoder.type == "SC" && params.decoder.implem == "NAIVE")
				decoder = new Decoder_polar_SC_naive_sys<B, R, f_LR<R>, g_LR<B,R>, h_LR<B,R>>(params.code.K, params.code.N_code, frozen_bits, params.simulation.inter_frame_level);
		}
		else if (params.channel.domain == "LLR")
		{
			if (params.decoder.simd_strategy.empty())
			{
				if (params.decoder.type == "SC" && params.decoder.implem == "NAIVE")
					decoder = new Decoder_polar_SC_naive_sys<B, R, f_LLR<R>, g_LLR<B,R>, h_LLR<B,R>>(params.code.K, params.code.N_code, frozen_bits, params.simulation.inter_frame_level);
				if (params.decoder.type == "SCAN" && params.decoder.implem == "NAIVE")
					decoder = new Decoder_polar_SCAN_naive_sys<B, R, init_LLR<R>, f_LLR<R>, v_LLR<R>, h_LLR<B,R>>(params.code.K, params.code.m, params.decoder.n_ite, frozen_bits, params.simulation.inter_frame_level);
				if (params.decoder.type == "SCL" && params.decoder.implem == "NAIVE" && params.crc.type.empty())
					decoder = new Decoder_polar_SCL_naive_sys<B,R,f_LLR<R>,g_LLR<B,R>>(params.code.K, params.code.N_code, params.decoder.L, frozen_bits, params.simulation.inter_frame_level);
				if (params.decoder.type == "SCL" && params.decoder.implem == "NAIVE" && !params.crc.type.empty())
					decoder = new Decoder_polar_SCL_naive_CA_sys<B,R,f_LLR<R>,g_LLR<B,R>>(params.code.K, params.code.N_code, params.decoder.L, frozen_bits, *crc, params.simulation.inter_frame_level);
			}
		
			if (params.decoder.type == "SC" && params.decoder.implem == "FAST")
			{
				if (params.decoder.simd_strategy == "INTER")
				{
#if defined(ENABLE_BIT_PACKING)
					if (typeid(B) == typeid(signed char))
					{
#ifdef API_POLAR_DYNAMIC
						using API_polar = API_polar_dynamic_inter_8bit_bitpacking
						                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#else
						using API_polar = API_polar_static_inter_8bit_bitpacking
						                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#endif
						decoder = new Decoder_polar_SC_fast_sys<B, R, API_polar>(params.code.K, params.code.N_code,frozen_bits, params.simulation.inter_frame_level);
					}
#endif
					if (decoder == nullptr)
					{
#ifdef API_POLAR_DYNAMIC
						using API_polar = API_polar_dynamic_inter
						                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#else
						using API_polar = API_polar_static_inter
						                  <B, R, f_LLR_i<R>, g_LLR_i<B,R>, g0_LLR_i<R>, h_LLR_i<B,R>, xo_STD_i<B>>;
#endif
						decoder = new Decoder_polar_SC_fast_sys<B, R, API_polar>(params.code.K, params.code.N_code,frozen_bits, params.simulation.inter_frame_level);
					}
				}
				else if (params.decoder.simd_strategy == "INTRA")
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
						decoder = new Decoder_polar_SC_fast_sys<B, R, API_polar>(params.code.K, params.code.N_code,frozen_bits, params.simulation.inter_frame_level);
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
						decoder = new Decoder_polar_SC_fast_sys<B, R, API_polar>(params.code.K, params.code.N_code,frozen_bits, params.simulation.inter_frame_level);
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
						decoder = new Decoder_polar_SC_fast_sys<B, R, API_polar>(params.code.K, params.code.N_code,frozen_bits, params.simulation.inter_frame_level);
					}
				}
				else if (params.decoder.simd_strategy.empty())
				{
#ifdef API_POLAR_DYNAMIC
					using API_polar = API_polar_dynamic_seq
					                  <B, R, f_LLR<R>, g_LLR<B,R>, g0_LLR<R>, h_LLR<B,R>, xo_STD<B>>;
#else
					using API_polar = API_polar_static_seq
					                  <B, R, f_LLR<R>, g_LLR<B,R>, g0_LLR<R>, h_LLR<B,R>, xo_STD<B>>;
#endif
					decoder = new Decoder_polar_SC_fast_sys<B, R, API_polar>(params.code.K, params.code.N_code,frozen_bits, params.simulation.inter_frame_level);
				}
			}
		}
	}

	return decoder;
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
