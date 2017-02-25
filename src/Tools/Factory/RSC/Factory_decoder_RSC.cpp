#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_std.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_scan.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_very_fast.hpp"

#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std.hpp"
#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std_json.hpp"

#include "Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_std.hpp"
#include "Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_SSE.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_AVX.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x4_AVX.hpp"

#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_std.hpp"
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_very_fast.hpp"

#include "Tools/Math/max.h"

#include "Factory_decoder_RSC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R, typename RD>
SISO<R>* Factory_decoder_RSC<B,R,RD>
::build_siso(const parameters &params, const std::vector<std::vector<int>> &trellis, std::ostream &stream)
{
	SISO<R> *siso = nullptr;

	if (params.channel.domain == "LLR")
	{
		// build the decoder
		if (params.decoder.type == "BCJR")
		{
			if (params.decoder.implem == "STD")
			{
				if (params.decoder.simd_strategy == "INTER")
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
				else if (params.decoder.simd_strategy == "INTRA")
				{
					if (mipp::nElReg<R>() == 8)
					{
						if (params.decoder.max == "MAX")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
						else if (params.decoder.max == "MAXS")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
						else if (params.decoder.max == "MAXL")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					}
				}
				else if (params.decoder.simd_strategy.empty())
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
			}
			else if (params.decoder.implem == "GENERIC")
			{
				if (params.decoder.simd_strategy.empty())
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
			}
			else if (params.decoder.implem == "GENERIC_JSON")
			{
				if (params.decoder.simd_strategy.empty())
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.decoder.n_ite, params.encoder.buffered, stream, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.decoder.n_ite, params.encoder.buffered, stream, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.decoder.n_ite, params.encoder.buffered, stream, params.simulation.inter_frame_level);
				}
			}
			else if (params.decoder.implem == "FAST")
			{
				if (params.decoder.simd_strategy == "INTER")
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
				else if (params.decoder.simd_strategy == "INTRA")
				{
#ifdef __AVX__
					switch (mipp::nElReg<R>())
					{
						case 8:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						case 16:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						case 32:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						default:
							break;
					}
#else /* NEON and SSE */
					switch (mipp::nElReg<R>())
					{
						case 8:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						case 16:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						default:
							break;
					}
#endif
				}
				else if (params.decoder.simd_strategy.empty())
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
			}
			else if (params.decoder.implem == "VERY_FAST")
			{
				if (params.decoder.simd_strategy == "INTER")
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
				else if (params.decoder.simd_strategy == "INTRA")
					siso = nullptr;
				else if (params.decoder.simd_strategy.empty())
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
			}
			else if (params.decoder.implem == "SCAN")
			{
				if (params.decoder.simd_strategy == "INTER")
					siso = nullptr;
				else if (params.decoder.simd_strategy == "INTRA")
					siso = nullptr;
				else if (params.decoder.simd_strategy.empty() && params.decoder.max == "MAX")
					siso = new Decoder_RSC_BCJR_seq_scan<B,R,RD>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
			}
		}
	}

	return siso;
}

template <typename B, typename R, typename RD>
Decoder<B,R>* Factory_decoder_RSC<B,R,RD>
::build(const parameters &params, const std::vector<std::vector<int>> &trellis)
{
	Decoder<B,R> *siso = nullptr;

	if (params.channel.domain == "LLR")
	{
		// build the decoder
		if (params.decoder.type == "BCJR")
		{
			if (params.decoder.implem == "STD")
			{
				if (params.decoder.simd_strategy == "INTER")
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
				else if (params.decoder.simd_strategy == "INTRA")
				{
					if (mipp::nElReg<R>() == 8)
					{
						if (params.decoder.max == "MAX")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
						else if (params.decoder.max == "MAXS")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
						else if (params.decoder.max == "MAXL")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					}
				}
				else if (params.decoder.simd_strategy.empty())
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
			}
			else if (params.decoder.implem == "GENERIC")
			{
				if (params.decoder.simd_strategy.empty())
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
			}
			else if (params.decoder.implem == "FAST")
			{
				if (params.decoder.simd_strategy == "INTER")
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
				else if (params.decoder.simd_strategy == "INTRA")
				{
#ifdef __AVX__
					switch (mipp::nElReg<R>())
					{
						case 8:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						case 16:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						case 32:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						default:
							break;
					}
#else /* NEON and SSE */
					switch (mipp::nElReg<R>())
					{
						case 8:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						case 16:
							if (params.decoder.max == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							else if (params.decoder.max == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
							break;
						default:
							break;
					}
#endif
				}
				else if (params.decoder.simd_strategy.empty())
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
			}
			else if (params.decoder.implem == "VERY_FAST")
			{
				if (params.decoder.simd_strategy == "INTER")
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,max_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,max_star_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,max_linear_i<R>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
				else if (params.decoder.simd_strategy == "INTRA")
					siso = nullptr;
				else if (params.decoder.simd_strategy.empty())
				{
					if (params.decoder.max == "MAX")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
					else if (params.decoder.max == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
				}
			}
			else if (params.decoder.implem == "SCAN")
			{
				if (params.decoder.simd_strategy == "INTER")
					siso = nullptr;
				else if (params.decoder.simd_strategy == "INTRA")
					siso = nullptr;
				else if (params.decoder.simd_strategy.empty() && params.decoder.max == "MAX")
					siso = new Decoder_RSC_BCJR_seq_scan<B,R,RD>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
			}
		}
		else if (params.decoder.type == "BCJR_G" && params.code.type == "RSC"  )
		{
			if (params.decoder.max == "MAX")
				siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max<R>,max<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
			else if (params.decoder.max == "MAXS")
				siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max_star<R>,max_star<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
			else if (params.decoder.max == "MAXL")
				siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max_linear<R>,max_linear<RD>>(params.code.K, trellis, params.encoder.buffered, params.simulation.inter_frame_level);
		}
	}

	return siso;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_decoder_RSC<B_8,Q_8,QD_8>;
template struct Factory_decoder_RSC<B_16,Q_16,QD_16>;
template struct Factory_decoder_RSC<B_32,Q_32,QD_32>;
template struct Factory_decoder_RSC<B_64,Q_64,QD_64>;
#else
template struct Factory_decoder_RSC<B,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
