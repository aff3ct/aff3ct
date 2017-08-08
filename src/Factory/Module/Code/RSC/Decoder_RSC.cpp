#include <mipp.h>

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

#include "Decoder_RSC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_RSC::name   = "Decoder RSC";
const std::string aff3ct::factory::Decoder_RSC::prefix = "dec";

template <typename B, typename Q, typename QD, tools::proto_max<Q> MAX1, tools::proto_max<QD> MAX2>
module::Decoder_SISO_SIHO<B,Q>* Decoder_RSC
::_build_seq(const parameters                    &dec_par,
             const std::vector<std::vector<int>> &trellis,
                   std::ostream                  &stream,
             const int                            n_ite)
{
	if (dec_par.type == "BCJR")
	{
		     if (dec_par.implem == "STD"         ) return new module::Decoder_RSC_BCJR_seq_std             <B,Q,QD,MAX1,MAX2>(dec_par.K, trellis,        dec_par.buffered,         dec_par.n_frames);
		else if (dec_par.implem == "GENERIC"     ) return new module::Decoder_RSC_BCJR_seq_generic_std     <B,Q,QD,MAX1,MAX2>(dec_par.K, trellis,        dec_par.buffered,         dec_par.n_frames);
		else if (dec_par.implem == "GENERIC_JSON") return new module::Decoder_RSC_BCJR_seq_generic_std_json<B,Q,QD,MAX1,MAX2>(dec_par.K, trellis, n_ite, dec_par.buffered, stream, dec_par.n_frames);
		else if (dec_par.implem == "FAST"        ) return new module::Decoder_RSC_BCJR_seq_fast            <B,Q,QD,MAX1,MAX2>(dec_par.K, trellis,        dec_par.buffered,         dec_par.n_frames);
		else if (dec_par.implem == "VERY_FAST"   ) return new module::Decoder_RSC_BCJR_seq_very_fast       <B,Q,QD,MAX1,MAX2>(dec_par.K, trellis,        dec_par.buffered,         dec_par.n_frames);
		else if (dec_par.implem == "SCAN"        ) return new module::Decoder_RSC_BCJR_seq_scan            <B,Q,QD          >(dec_par.K, trellis,        dec_par.buffered,         dec_par.n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, typename QD, tools::proto_max_i<Q> MAX>
module::Decoder_SISO_SIHO<B,Q>* Decoder_RSC
::_build_simd(const parameters                    &dec_par,
              const std::vector<std::vector<int>> &trellis)
{
	if (dec_par.type == "BCJR" && dec_par.simd_strategy == "INTER")
	{
		     if (dec_par.implem == "STD"      ) return new module::Decoder_RSC_BCJR_inter_std      <B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames);
		else if (dec_par.implem == "FAST"     ) return new module::Decoder_RSC_BCJR_inter_fast     <B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames);
		else if (dec_par.implem == "VERY_FAST") return new module::Decoder_RSC_BCJR_inter_very_fast<B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames);
	}

	if (dec_par.type == "BCJR" && dec_par.simd_strategy == "INTRA")
	{
		if (dec_par.implem == "STD")
		{
			switch (mipp::nElReg<Q>())
			{
				case 8: return new module::Decoder_RSC_BCJR_intra_std<B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames);
				default:
					break;
			}
		}
		else if (dec_par.implem == "FAST")
		{
#ifdef __AVX__
			switch (mipp::nElReg<Q>())
			{
				case 8:  return new module::Decoder_RSC_BCJR_intra_fast             <B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames); break;
				case 16: return new module::Decoder_RSC_BCJR_inter_intra_fast_x2_AVX<B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames); break;
				case 32: return new module::Decoder_RSC_BCJR_inter_intra_fast_x4_AVX<B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames); break;
				default:
					break;
			}
#else /* NEON and SSE */
			switch (mipp::nElReg<Q>())
			{
				case 8:  return new module::Decoder_RSC_BCJR_intra_fast             <B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames); break;
				case 16: return new module::Decoder_RSC_BCJR_inter_intra_fast_x2_SSE<B,Q,MAX>(dec_par.K, trellis, dec_par.buffered, dec_par.n_frames); break;
				default:
					break;
			}
#endif
		}
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q, typename QD>
module::Decoder_SISO_SIHO<B,Q>* Decoder_RSC
::build(const parameters                      &dec_par,
        const std::vector<std::vector<int>>   &trellis,
              std::ostream                    &stream,
        const int                              n_ite)
{
	if (dec_par.simd_strategy.empty())
	{
		     if (dec_par.max == "MAX" ) return _build_seq<B,Q,QD,tools::max       <Q>,tools::max       <QD>>(dec_par, trellis, stream, n_ite);
		else if (dec_par.max == "MAXS") return _build_seq<B,Q,QD,tools::max_star  <Q>,tools::max_star  <QD>>(dec_par, trellis, stream, n_ite);
		else if (dec_par.max == "MAXL") return _build_seq<B,Q,QD,tools::max_linear<Q>,tools::max_linear<QD>>(dec_par, trellis, stream, n_ite);
	}
	else
	{
		     if (dec_par.max == "MAX" ) return _build_simd<B,Q,QD,tools::max_i       <Q>>(dec_par, trellis);
		else if (dec_par.max == "MAXS") return _build_simd<B,Q,QD,tools::max_star_i  <Q>>(dec_par, trellis);
		else if (dec_par.max == "MAXL") return _build_simd<B,Q,QD,tools::max_linear_i<Q>>(dec_par, trellis);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_RSC
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args, p);
	req_args.erase({p+"-cw-size", "N"});

	opt_args[{p+"-type", "D"}].push_back("BCJR");

	opt_args[{p+"-implem"}].push_back("GENERIC, STD, FAST, VERY_FAST");

	opt_args[{p+"-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};

	opt_args[{p+"-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXL, MAXS"};

	opt_args[{p+"-no-buff"}] =
		{"",
		 "does not suppose a buffered encoding."};

	opt_args[{p+"-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{p+"-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};
}

void Decoder_RSC
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type   = "BCJR";
	params.implem = "STD";

	Decoder::store_args(vals, params, p);

	if(exist(vals, {p+"-simd"   })) params.simd_strategy = vals.at({p+"-simd"});
	if(exist(vals, {p+"-max"    })) params.max           = vals.at({p+"-max" });
	if(exist(vals, {p+"-std"    })) params.standard      = vals.at({p+"-std" });
	if(exist(vals, {p+"-no-buff"})) params.buffered      = false;

	if (params.standard == "LTE")
		params.poly = {013, 015};

	if (params.standard == "CCSDS")
		params.poly = {023, 033};

	if (exist(vals, {p+"-poly"}))
	{
		auto poly_str = vals.at({p+"-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}

	if (params.poly[0] != 013 || params.poly[1] != 015)
		params.implem = "GENERIC";

	params.tail_length = (int)(2 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N_cw        = 2 * params.K + params.tail_length;
	params.R           = (float)params.K / (float)params.N_cw;
}

void Decoder_RSC
::make_header(params_list& head_dec, const parameters& params, const bool full)
{
	Decoder::make_header(head_dec, params, full);

	if (params.tail_length && full)
		head_dec.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));

	if (full) head_dec.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));

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
template aff3ct::module::Decoder_SISO_SIHO<B_8 ,Q_8 >* aff3ct::factory::Decoder_RSC::build<B_8 ,Q_8 ,QD_8 >(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
template aff3ct::module::Decoder_SISO_SIHO<B_16,Q_16>* aff3ct::factory::Decoder_RSC::build<B_16,Q_16,QD_16>(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
template aff3ct::module::Decoder_SISO_SIHO<B_32,Q_32>* aff3ct::factory::Decoder_RSC::build<B_32,Q_32,QD_32>(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
template aff3ct::module::Decoder_SISO_SIHO<B_64,Q_64>* aff3ct::factory::Decoder_RSC::build<B_64,Q_64,QD_64>(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
#else
template aff3ct::module::Decoder_SISO_SIHO<B,Q>* aff3ct::factory::Decoder_RSC::build<B,Q,QD>(const aff3ct::factory::Decoder_RSC::parameters&, const std::vector<std::vector<int>>&, std::ostream&, const int);
#endif
// ==================================================================================== explicit template instantiation
