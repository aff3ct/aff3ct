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

template <typename B, typename Q, typename QD, proto_max<Q> MAX1, proto_max<QD> MAX2>
Decoder_SISO<B,Q>* Factory_decoder_RSC
::_build_seq(const parameters                      &dec_par,
             const Factory_encoder_RSC::parameters &enc_par,
             const std::vector<std::vector<int>>   &trellis,
                   std::ostream                    &stream,
             const int                              n_ite)
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

template <typename B, typename Q, typename QD, proto_max_i<Q> MAX>
Decoder_SISO<B,Q>* Factory_decoder_RSC
::_build_simd(const parameters                      &dec_par,
              const Factory_encoder_RSC::parameters &enc_par,
              const std::vector<std::vector<int>>   &trellis)
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
Decoder_SISO<B,Q>* Factory_decoder_RSC
::build(const parameters                      &dec_par,
        const Factory_encoder_RSC::parameters &enc_par,
        const std::vector<std::vector<int>>   &trellis,
              std::ostream                    &stream,
        const int                              n_ite)
{
	if (dec_par.simd_strategy.empty())
	{
		     if (dec_par.max == "MAX" ) return _build_seq<B,Q,QD,max       <Q>,max       <QD>>(dec_par, enc_par, trellis, stream, n_ite);
		else if (dec_par.max == "MAXS") return _build_seq<B,Q,QD,max_star  <Q>,max_star  <QD>>(dec_par, enc_par, trellis, stream, n_ite);
		else if (dec_par.max == "MAXL") return _build_seq<B,Q,QD,max_linear<Q>,max_linear<QD>>(dec_par, enc_par, trellis, stream, n_ite);
	}
	else
	{
		     if (dec_par.max == "MAX" ) return _build_simd<B,Q,QD,max_i       <Q>>(dec_par, enc_par, trellis);
		else if (dec_par.max == "MAXS") return _build_simd<B,Q,QD,max_star_i  <Q>>(dec_par, enc_par, trellis);
		else if (dec_par.max == "MAXL") return _build_simd<B,Q,QD,max_linear_i<Q>>(dec_par, enc_par, trellis);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_decoder_RSC
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder::build_args(req_args, opt_args);

	opt_args[{"dec-type", "D"}].push_back("BCJR, LTE, CCSDS");

	opt_args[{"dec-implem"}].push_back("GENERIC, STD, FAST, VERY_FAST");

	opt_args[{"dec-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};

	opt_args[{"dec-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXL, MAXS"};

	opt_args[{"dec-no-buff"}] =
		{"",
		 "does not suppose a buffered encoding."};

	opt_args[{"dec-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{"dec-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};
}

void Factory_decoder_RSC
::store_args(const Arguments_reader& ar, parameters &params)
{
	params.type   = "BCJR";
	params.implem = "GENERIC";

	Factory_decoder::store_args(ar, params);

	if(ar.exist_arg({"dec-simd"})) params.simd_strategy = ar.get_arg({"dec-simd"});
	if(ar.exist_arg({"dec-max" })) params.max = ar.get_arg({"dec-max"});
	if(ar.exist_arg({"dec-no-buff"})) params.buffered = false;
	if(ar.exist_arg({"dec-std"})) params.standard = ar.get_arg({"dec-std"});

	if (params.type == "LTE")
		params.poly = {013, 015};

	if (params.type == "CCSDS")
		params.poly = {023, 033};

	if (ar.exist_arg({"dec-poly"}))
	{
		auto poly_str = ar.get_arg({"dec-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}

	params.tail_length = (int)(2 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N_cw       += params.tail_length;
}

void Factory_decoder_RSC
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder::group_args(ar);
}

void Factory_decoder_RSC
::header(params_list& head_dec, const parameters& params)
{
	Factory_decoder::header(head_dec, params);

	if (params.tail_length)
		head_dec.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));

	head_dec.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));

	if (!params.standard.empty())
		head_dec.push_back(std::make_pair("Standard", params.standard));

	std::stringstream poly;
	poly << "{0" << std::oct << params.poly[0] << ",0" << std::oct << params.poly[1] << "}";
	head_dec.push_back(std::make_pair(std::string("Polynomials"), poly.str()));

	if (!params.simd_strategy.empty())
		head_dec.push_back(std::make_pair(std::string("SIMD strategy"), params.simd_strategy));

	head_dec.push_back(std::make_pair(std::string("Max type"), params.max));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_SISO<B_8 ,Q_8 >* aff3ct::tools::Factory_decoder_RSC::build<B_8 ,Q_8 ,QD_8 >(const aff3ct::tools::Factory_decoder_RSC::parameters&, const aff3ct::tools::Factory_encoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
template aff3ct::module::Decoder_SISO<B_16,Q_16>* aff3ct::tools::Factory_decoder_RSC::build<B_16,Q_16,QD_16>(const aff3ct::tools::Factory_decoder_RSC::parameters&, const aff3ct::tools::Factory_encoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
template aff3ct::module::Decoder_SISO<B_32,Q_32>* aff3ct::tools::Factory_decoder_RSC::build<B_32,Q_32,QD_32>(const aff3ct::tools::Factory_decoder_RSC::parameters&, const aff3ct::tools::Factory_encoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
template aff3ct::module::Decoder_SISO<B_64,Q_64>* aff3ct::tools::Factory_decoder_RSC::build<B_64,Q_64,QD_64>(const aff3ct::tools::Factory_decoder_RSC::parameters&, const aff3ct::tools::Factory_encoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
#else
template aff3ct::module::Decoder_SISO<B,Q>* aff3ct::tools::Factory_decoder_RSC::build<B,Q,QD>(const aff3ct::tools::Factory_decoder_RSC::parameters&, const aff3ct::tools::Factory_encoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
#endif
// ==================================================================================== explicit template instantiation
