#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_std.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_scan.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Seq/Decoder_RSC_BCJR_seq_very_fast.hpp"

#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std.hpp"

#include "Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_std.hpp"
#include "Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_SSE.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x2_AVX.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra_fast_x4_AVX.hpp"

#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_std.hpp"
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_fast.hpp"
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_very_fast.hpp"

#include "Factory_decoder_RSC.hpp"

template <typename B, typename R, typename RD>
SISO<R>* Factory_decoder_RSC<B,R,RD>
::build_siso(const t_code_param                  &code_params,
             const t_encoder_param               &enco_params,
             const t_channel_param               &chan_params,
             const t_decoder_param               &deco_params,
             const std::vector<std::vector<int>> &trellis)
{
	SISO<R> *siso = nullptr;

	if (chan_params.domain == "LLR")
	{
		// build the decoder
		if ((deco_params.algo == "LTE"   && code_params.type == "TURBO") || 
			(deco_params.algo == "CCSDS" && code_params.type == "TURBO") ||
			(deco_params.algo == "BCJR"  && code_params.type == "RSC"  ) ||
			(deco_params.algo == "BCJR4" && code_params.type == "RSC"  ))
		{
			if (deco_params.implem == "STD")
			{
				if (deco_params.simd_strategy == "INTER")
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
				}
				else if (deco_params.simd_strategy == "INTRA")
				{
					if (mipp::nElReg<R>() == 8)
					{
						if (deco_params.map == "MAX")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
						else if (deco_params.map == "MAXS")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
						else if (deco_params.map == "MAXL")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					}
				}
				else if (deco_params.simd_strategy.empty())
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,max_log_MAP<R>,max_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,log_MAP<R>,log_MAP<RD>>(code_params.K, trellis, enco_params.buffered); 
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,linear_log_MAP<R>,linear_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered); 
				}
			}
			else if (deco_params.implem == "GENERIC")
			{
				if (deco_params.simd_strategy.empty())
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max_log_MAP<R>,max_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,log_MAP<R>,log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,linear_log_MAP<R>,linear_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
				}
			}
			else if (deco_params.implem == "FAST")
			{
				if (deco_params.simd_strategy == "INTER")
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
				}
				else if (deco_params.simd_strategy == "INTRA")
				{
#ifdef __AVX__
					switch (mipp::nElReg<R>())
					{
						case 8:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						case 16:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						case 32:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						default:
							break;
					}
#else /* NEON and SSE */
					switch (mipp::nElReg<R>())
					{
						case 8:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						case 16:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						default:
							break;
					}
#endif
				}
				else if (deco_params.simd_strategy.empty())
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,max_log_MAP<R>,max_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,log_MAP<R>,log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,linear_log_MAP<R>,linear_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
				}
			}
			else if (deco_params.implem == "VERY_FAST")
			{
				if (deco_params.simd_strategy == "INTER")
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
				}
				else if (deco_params.simd_strategy == "INTRA")
					siso = nullptr;
				else if (deco_params.simd_strategy.empty())
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,max_log_MAP<R>,max_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,log_MAP<R>,log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,linear_log_MAP<R>,linear_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
				}
			}
			else if (deco_params.implem == "SCAN")
			{
				if (deco_params.simd_strategy == "INTER")
					siso = nullptr;
				else if (deco_params.simd_strategy == "INTRA")
					siso = nullptr;
				else if (deco_params.simd_strategy.empty() && deco_params.map == "MAX")	
					siso = new Decoder_RSC_BCJR_seq_scan<B,R,RD>(code_params.K, trellis, enco_params.buffered);
			}
		}
	}

	return siso;
}

template <typename B, typename R, typename RD>
Decoder<B,R>* Factory_decoder_RSC<B,R,RD>
::build(const t_code_param                  &code_params,
        const t_encoder_param               &enco_params,
        const t_channel_param               &chan_params,
        const t_decoder_param               &deco_params,
        const std::vector<std::vector<int>> &trellis)
{
	Decoder<B,R> *siso = nullptr;

	if (chan_params.domain == "LLR")
	{
		// build the decoder
		if ((deco_params.algo == "BCJR"  && code_params.type == "RSC") ||
			(deco_params.algo == "BCJR4" && code_params.type == "RSC"))
		{
			if (deco_params.implem == "STD")
			{
				if (deco_params.simd_strategy == "INTER")
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_std<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
				}
				else if (deco_params.simd_strategy == "INTRA")
				{
					if (mipp::nElReg<R>() == 8)
					{
						if (deco_params.map == "MAX")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
						else if (deco_params.map == "MAXS")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
						else if (deco_params.map == "MAXL")
							siso = new Decoder_RSC_BCJR_intra_std<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					}
				}
				else if (deco_params.simd_strategy.empty())
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,max_log_MAP<R>,max_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,log_MAP<R>,log_MAP<RD>>(code_params.K, trellis, enco_params.buffered); 
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_std<B,R,RD,linear_log_MAP<R>,linear_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered); 
				}
			}
			else if (deco_params.implem == "GENERIC")
			{
				if (deco_params.simd_strategy.empty())
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,max_log_MAP<R>,max_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,log_MAP<R>,log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_generic_std<B,R,RD,linear_log_MAP<R>,linear_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
				}
			}
			else if (deco_params.implem == "FAST")
			{
				if (deco_params.simd_strategy == "INTER")
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_fast<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
				}
				else if (deco_params.simd_strategy == "INTRA")
				{
#ifdef __AVX__
					switch (mipp::nElReg<R>())
					{
						case 8:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						case 16:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						case 32:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						default:
							break;
					}
#else /* NEON and SSE */
					switch (mipp::nElReg<R>())
					{
						case 8:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_intra_fast<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						case 16:
							if (deco_params.map == "MAX")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXS")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							else if (deco_params.map == "MAXL")
								siso = new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
							break;
						default:
							break;
					}
#endif
				}
				else if (deco_params.simd_strategy.empty())
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,max_log_MAP<R>,max_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,log_MAP<R>,log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_fast<B,R,RD,linear_log_MAP<R>,linear_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
				}
			}
			else if (deco_params.implem == "VERY_FAST")
			{
				if (deco_params.simd_strategy == "INTER")
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,max_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_inter_very_fast<B,R,linear_log_MAP_i<R>>(code_params.K, trellis, enco_params.buffered);
				}
				else if (deco_params.simd_strategy == "INTRA")
					siso = nullptr;
				else if (deco_params.simd_strategy.empty())
				{
					if (deco_params.map == "MAX")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,max_log_MAP<R>,max_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXS")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,log_MAP<R>,log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
					else if (deco_params.map == "MAXL")
						siso = new Decoder_RSC_BCJR_seq_very_fast<B,R,RD,linear_log_MAP<R>,linear_log_MAP<RD>>(code_params.K, trellis, enco_params.buffered);
				}
			}
			else if (deco_params.implem == "SCAN")
			{
				if (deco_params.simd_strategy == "INTER")
					siso = nullptr;
				else if (deco_params.simd_strategy == "INTRA")
					siso = nullptr;
				else if (deco_params.simd_strategy.empty() && deco_params.map == "MAX")	
					siso = new Decoder_RSC_BCJR_seq_scan<B,R,RD>(code_params.K, trellis, enco_params.buffered);
			}
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
