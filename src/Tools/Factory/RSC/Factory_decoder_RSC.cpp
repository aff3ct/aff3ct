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

template <typename B, typename Q, typename QD>
template <proto_max<Q> MAX1, proto_max<QD> MAX2>
Decoder_SISO<B,Q>* Factory_decoder_RSC<B,Q,QD>
::_build_seq(const decoder_parameters_RSC                                  &dec_par,
             const typename Factory_encoder_RSC<B>::encoder_parameters_RSC &enc_par,
             const std::vector<std::vector<int>>                           &trellis,
                   std::ostream                                            &stream,
             const int                                                      n_ite)
{
	if (dec_par.type == "BCJR")
	{
		     if (dec_par.implem == "STD"         ) return new Decoder_RSC_BCJR_seq_std             <B,Q,QD,MAX1,MAX2>(dec_par.K, trellis,        enc_par.buffered,         dec_par.n_frames);
		else if (dec_par.implem == "GENERIC"     ) return new Decoder_RSC_BCJR_seq_generic_std     <B,Q,QD,MAX1,MAX2>(dec_par.K, trellis,        enc_par.buffered,         dec_par.n_frames);
		else if (dec_par.implem == "GENERIC_JSON") return new Decoder_RSC_BCJR_seq_generic_std_json<B,Q,QD,MAX1,MAX2>(dec_par.K, trellis, n_ite, enc_par.buffered, stream, dec_par.n_frames);
		else if (dec_par.implem == "FAST"        ) return new Decoder_RSC_BCJR_seq_fast            <B,Q,QD,MAX1,MAX2>(dec_par.K, trellis,        enc_par.buffered,         dec_par.n_frames);
		else if (dec_par.implem == "VERY_FAST"   ) return new Decoder_RSC_BCJR_seq_very_fast       <B,Q,QD,MAX1,MAX2>(dec_par.K, trellis,        enc_par.buffered,         dec_par.n_frames);
		else if (dec_par.implem == "SCAN"        ) return new Decoder_RSC_BCJR_seq_scan            <B,Q,QD          >(dec_par.K, trellis,        enc_par.buffered,         dec_par.n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, typename QD>
template <proto_max_i<Q> MAX>
Decoder_SISO<B,Q>* Factory_decoder_RSC<B,Q,QD>
::_build_simd(const decoder_parameters_RSC                                  &dec_par,
              const typename Factory_encoder_RSC<B>::encoder_parameters_RSC &enc_par,
              const std::vector<std::vector<int>>                           &trellis)
{
	if (dec_par.type == "BCJR" && dec_par.simd_strategy == "INTER")
	{
		     if (dec_par.implem == "STD"      ) return new Decoder_RSC_BCJR_inter_std      <B,Q,MAX>(dec_par.K, trellis, enc_par.buffered, dec_par.n_frames);
		else if (dec_par.implem == "FAST"     ) return new Decoder_RSC_BCJR_inter_fast     <B,Q,MAX>(dec_par.K, trellis, enc_par.buffered, dec_par.n_frames);
		else if (dec_par.implem == "VERY_FAST") return new Decoder_RSC_BCJR_inter_very_fast<B,Q,MAX>(dec_par.K, trellis, enc_par.buffered, dec_par.n_frames);
	}

	if (dec_par.type == "BCJR" && dec_par.simd_strategy == "INTRA")
	{
		if (dec_par.implem == "STD")
		{
			switch (mipp::nElReg<Q>())
			{
				case 8: return new Decoder_RSC_BCJR_intra_std<B,Q,MAX>(dec_par.K, trellis, enc_par.buffered, dec_par.n_frames);
				default:
					break;
			}
		}
		else if (dec_par.implem == "FAST")
		{
#ifdef __AVX__
			switch (mipp::nElReg<Q>())
			{
				case 8:  return new Decoder_RSC_BCJR_intra_fast             <B,Q,MAX>(dec_par.K, trellis, enc_par.buffered, dec_par.n_frames); break;
				case 16: return new Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,Q,MAX>(dec_par.K, trellis, enc_par.buffered, dec_par.n_frames); break;
				case 32: return new Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,Q,MAX>(dec_par.K, trellis, enc_par.buffered, dec_par.n_frames); break;
				default:
					break;
			}
#else /* NEON and SSE */
			switch (mipp::nElReg<Q>())
			{
				case 8:  return new Decoder_RSC_BCJR_intra_fast             <B,Q,MAX>(dec_par.K, trellis, enc_par.buffered, dec_par.n_frames); break;
				case 16: return new Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,Q,MAX>(dec_par.K, trellis, enc_par.buffered, dec_par.n_frames); break;
				default:
					break;
			}
#endif
		}
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, typename QD>
Decoder_SISO<B,Q>* Factory_decoder_RSC<B,Q,QD>
::build(const decoder_parameters_RSC                                  &dec_par,
        const typename Factory_encoder_RSC<B>::encoder_parameters_RSC &enc_par,
        const std::vector<std::vector<int>>                           &trellis,
              std::ostream                                            &stream,
        const int                                                      n_ite)
{
	if (dec_par.simd_strategy.empty())
	{
		     if (dec_par.max == "MAX" ) return _build_seq<max       <Q>,max       <QD>>(dec_par, enc_par, trellis, stream, n_ite);
		else if (dec_par.max == "MAXS") return _build_seq<max_star  <Q>,max_star  <QD>>(dec_par, enc_par, trellis, stream, n_ite);
		else if (dec_par.max == "MAXL") return _build_seq<max_linear<Q>,max_linear<QD>>(dec_par, enc_par, trellis, stream, n_ite);
	}
	else
	{
		     if (dec_par.max == "MAX" ) return _build_simd<max_i       <Q>>(dec_par, enc_par, trellis);
		else if (dec_par.max == "MAXS") return _build_simd<max_star_i  <Q>>(dec_par, enc_par, trellis);
		else if (dec_par.max == "MAXL") return _build_simd<max_linear_i<Q>>(dec_par, enc_par, trellis);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, typename QD>
void Factory_decoder_RSC<B,Q,QD>
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
}

template <typename B, typename Q, typename QD>
void Factory_decoder_RSC<B,Q,QD>
::store_args(const Arguments_reader& ar, decoder_parameters_RSC &params,
             const int K, const int N, const int n_frames, const bool activate_simd)
{
	params.type   = "BCJR";
	params.implem = "GENERIC";

	Factory_decoder_common::store_args(ar, params, K, N, n_frames);

	// ------------------------------------------------------------------------------------------------------- decoder
	if(ar.exist_arg({"dec-simd"})) params.simd_strategy = ar.get_arg({"dec-simd"});
	if(ar.exist_arg({"dec-max" })) params.max           = ar.get_arg({"dec-max" });

	if (params.simd_strategy == "INTER" && activate_simd)
		params.n_frames = mipp::nElReg<Q>();
	else if (params.simd_strategy == "INTRA" && activate_simd)
		params.n_frames = (int)std::ceil(mipp::nElReg<Q>() / 8.f);
}

template <typename B, typename Q, typename QD>
void Factory_decoder_RSC<B,Q,QD>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder_common::group_args(ar);
}

template <typename B, typename Q, typename QD>
void Factory_decoder_RSC<B,Q,QD>
::header(Header::params_list& head_dec, const decoder_parameters_RSC& params)
{
	Factory_decoder_common::header(head_dec, params);

	// ------------------------------------------------------------------------------------------------------- decoder
	if (!params.simd_strategy.empty())
		head_dec.push_back(std::make_pair(std::string("SIMD strategy"), params.simd_strategy));

	head_dec.push_back(std::make_pair(std::string("Max type"), params.max));
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
