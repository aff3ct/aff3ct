#include "Tools/Exception/exception.hpp"

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

#include "Factory_decoder_RSC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R, typename RD>
template <proto_max<R> MAX1, proto_max<RD> MAX2>
Decoder_SISO<B,R>* Factory_decoder_RSC<B,R,RD>
::_build_seq(const std::string                    type,
             const std::string                    implem,
             const int                            K,
             const std::vector<std::vector<int>> &trellis,
             const bool                           buffered,
                   std::ostream                  &stream,
             const int                            n_ite,
             const int                            n_frames)
{
	if (type == "BCJR")
	{
		     if (implem == "STD"         ) return new Decoder_RSC_BCJR_seq_std             <B,R,RD,MAX1,MAX2>(K, trellis,        buffered,         n_frames);
		else if (implem == "GENERIC"     ) return new Decoder_RSC_BCJR_seq_generic_std     <B,R,RD,MAX1,MAX2>(K, trellis,        buffered,         n_frames);
		else if (implem == "GENERIC_JSON") return new Decoder_RSC_BCJR_seq_generic_std_json<B,R,RD,MAX1,MAX2>(K, trellis, n_ite, buffered, stream, n_frames);
		else if (implem == "FAST"        ) return new Decoder_RSC_BCJR_seq_fast            <B,R,RD,MAX1,MAX2>(K, trellis,        buffered,         n_frames);
		else if (implem == "VERY_FAST"   ) return new Decoder_RSC_BCJR_seq_very_fast       <B,R,RD,MAX1,MAX2>(K, trellis,        buffered,         n_frames);
		else if (implem == "SCAN"        ) return new Decoder_RSC_BCJR_seq_scan            <B,R,RD          >(K, trellis,        buffered,         n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename RD>
template <proto_max_i<R> MAX>
Decoder_SISO<B,R>* Factory_decoder_RSC<B,R,RD>
::_build_simd(const std::string                    type,
              const std::string                    implem,
              const int                            K,
              const std::vector<std::vector<int>> &trellis,
              const std::string                    simd_strategy,
              const bool                           buffered,
              const int                            n_frames)
{
	if (type == "BCJR" && simd_strategy == "INTER")
	{
		     if (implem == "STD"      ) return new Decoder_RSC_BCJR_inter_std      <B,R,MAX>(K, trellis, buffered, n_frames);
		else if (implem == "FAST"     ) return new Decoder_RSC_BCJR_inter_fast     <B,R,MAX>(K, trellis, buffered, n_frames);
		else if (implem == "VERY_FAST") return new Decoder_RSC_BCJR_inter_very_fast<B,R,MAX>(K, trellis, buffered, n_frames);
	}

	if (type == "BCJR" && simd_strategy == "INTRA")
	{
		if (implem == "STD")
		{
			switch (mipp::nElReg<R>())
			{
				case 8: return new Decoder_RSC_BCJR_intra_std<B,R,MAX>(K, trellis, buffered, n_frames);
				default:
					break;
			}
		}
		else if (implem == "FAST")
		{
#ifdef __AVX__
			switch (mipp::nElReg<R>())
			{
				case 8:  return new Decoder_RSC_BCJR_intra_fast             <B,R,MAX>(K, trellis, buffered, n_frames); break;
				case 16: return new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,R,MAX>(K, trellis, buffered, n_frames); break;
				case 32: return new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,R,MAX>(K, trellis, buffered, n_frames); break;
				default:
					break;
			}
#else /* NEON and SSE */
			switch (mipp::nElReg<R>())
			{
				case 8:  return new Decoder_RSC_BCJR_intra_fast             <B,R,MAX>(K, trellis, buffered, n_frames); break;
				case 16: return new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,R,MAX>(K, trellis, buffered, n_frames); break;
				default:
					break;
			}
#endif
		}
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename RD>
Decoder_SISO<B,R>* Factory_decoder_RSC<B,R,RD>
::build(const std::string                    type,
        const std::string                    implem,
        const int                            K,
        const std::vector<std::vector<int>> &trellis,
        const std::string                    max_type,
        const std::string                    simd_strategy,
        const bool                           buffered,
              std::ostream                  &stream,
        const int                            n_ite,
        const int                            n_frames)
{
	if (simd_strategy.empty())
	{
		     if (max_type == "MAX" ) return _build_seq<max       <R>,max       <RD>>(type, implem, K, trellis, buffered, stream, n_ite, n_frames);
		else if (max_type == "MAXS") return _build_seq<max_star  <R>,max_star  <RD>>(type, implem, K, trellis, buffered, stream, n_ite, n_frames);
		else if (max_type == "MAXL") return _build_seq<max_linear<R>,max_linear<RD>>(type, implem, K, trellis, buffered, stream, n_ite, n_frames);
	}
	else
	{
		     if (max_type == "MAX" ) return _build_simd<max_i       <R>>(type, implem, K, trellis, simd_strategy, buffered, n_frames);
		else if (max_type == "MAXS") return _build_simd<max_star_i  <R>>(type, implem, K, trellis, simd_strategy, buffered, n_frames);
		else if (max_type == "MAXL") return _build_simd<max_linear_i<R>>(type, implem, K, trellis, simd_strategy, buffered, n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_RSC<B_8,Q_8,QD_8>;
template struct aff3ct::tools::Factory_decoder_RSC<B_16,Q_16,QD_16>;
template struct aff3ct::tools::Factory_decoder_RSC<B_32,Q_32,QD_32>;
template struct aff3ct::tools::Factory_decoder_RSC<B_64,Q_64,QD_64>;
#else
template struct aff3ct::tools::Factory_decoder_RSC<B,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
