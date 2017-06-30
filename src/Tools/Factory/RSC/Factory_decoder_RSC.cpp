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

template <typename B, typename Q, typename RD>
void Factory_decoder_RSC<B,Q,RD>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder_common::build_args(req_args, opt_args);

	// ---------------------------------------------------------------------------------------------------------- code
	opt_args[{"dec-type", "D"}].push_back("BCJR, LTE, CCSDS"             );

	opt_args[{"dec-implem"   }].push_back("GENERIC, STD, FAST, VERY_FAST");

	opt_args[{"dec-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};

	opt_args[{"dec-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXL, MAXS"};

	opt_args[{"cde-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};
}

template <typename B, typename Q, typename RD>
void Factory_decoder_RSC<B,Q,RD>
::store_args(const Arguments_reader& ar, decoder_parameters_RSC &params)
{
	params.type   = "BCJR";
	params.implem = "GENERIC";

	Factory_decoder_common::store_args(ar, params);

	// ------------------------------------------------------------------------------------------------------- decoder
	if(ar.exist_arg({"dec-simd"   })) params.simd_strategy = ar.get_arg({"dec-simd"});
	if(ar.exist_arg({"dec-max"    })) params.max           = ar.get_arg({"dec-max" });

//	if (params.simd_strategy == "INTER" && !ar.exist_arg({"sim-inter-lvl"}))
//		params.inter_frame_level = mipp::nElReg<Q>();
//	else if (params.simd_strategy == "INTRA" && !ar.exist_arg({"sim-inter-lvl"}))
//		params.inter_frame_level = (int)std::ceil(mipp::nElReg<Q>() / 8.f);

	// ---------------------------------------------------------------------------------------------------------- code
//	params.tail_length = (int)(2 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
//	params.N += params.tail_length;
//	params.N_code = 2 * params.K + params.tail_length;

	if (ar.exist_arg({"cde-poly"}))
	{
		auto poly_str = ar.get_arg({"cde-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}

	if (params.type == "LTE")
	{
		params.type = "BCJR";
		params.poly = {013, 015};
	}

	if (params.type == "CCSDS")
	{
		params.type = "BCJR";
		params.poly = {023, 033};
	}

	if (!(params.poly[0] == 013 && params.poly[1] == 015)) // if not LTE BCJR
	{
		params.type          = "BCJR";
		params.implem        = "GENERIC";
		params.simd_strategy = "";
	}
}

template <typename B, typename Q, typename RD>
void Factory_decoder_RSC<B,Q,RD>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder_common::group_args(ar);
}

template <typename B, typename Q, typename RD>
void Factory_decoder_RSC<B,Q,RD>
::header(Header::params_list& head_dec, Header::params_list& head_cde, const decoder_parameters_RSC& params)
{
	Factory_decoder_common::header(head_dec, params);

	// ------------------------------------------------------------------------------------------------------- decoder
	if (!params.simd_strategy.empty())
		head_dec.push_back(std::make_pair(std::string("SIMD strategy"), params.simd_strategy));

	head_dec.push_back(std::make_pair(std::string("Max type"), params.max));

	// ---------------------------------------------------------------------------------------------------------- code
	std::stringstream poly;
	poly << "{0" << std::oct << params.poly[0] << ",0" << std::oct << params.poly[1] << "}";
	head_cde.push_back(std::make_pair(std::string("Polynomials"), poly.str()));
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
