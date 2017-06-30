#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_fast.hpp"

#include "Factory_decoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_turbo<B,R>* Factory_decoder_turbo<B,R>
::build(const std::string       type,
        const std::string       implem,
        const int               K,
        const int               N,
        const int               n_ite,
        const Interleaver<int> &itl,
              SISO<R>          &siso_n,
              SISO<R>          &siso_i,
        const bool              buffered)
{
	if (type == "TURBO")
	{
		     if (implem == "STD" ) return new Decoder_turbo_naive<B,R>(K, N, n_ite, itl, siso_n, siso_i, buffered);
		else if (implem == "FAST") return new Decoder_turbo_fast <B,R>(K, N, n_ite, itl, siso_n, siso_i, buffered);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Factory_decoder_turbo<B,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder_common::build_args(req_args, opt_args);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- decoder
	opt_args[{"dec-type", "D"}].push_back("BCJR, LTE, CCSDS"             );

	opt_args[{"dec-implem"   }].push_back("GENERIC, STD, FAST, VERY_FAST");

	opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo "};

	opt_args[{"dec-sf"}] =
		{"string",
		 "scaling factor type.",
		 "LTE, LTE_VEC, ARRAY"};

	opt_args[{"dec-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};

	opt_args[{"dec-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXS, MAXL"};

	opt_args[{"dec-sc"}] =
		{"",
		 "enables the self corrected decoder (requires \"--crc-type\")."};

	opt_args[{"dec-fnc"}] =
		{"",
		 "enables the flip and check decoder (requires \"--crc-type\")."};

	opt_args[{"dec-fnc-q"}] =
		{"positive_int",
		 "set the search's space for the fnc algorithm."};

	opt_args[{"dec-fnc-ite-m"}] =
		{"positive_int",
		 "set first iteration at which the fnc is used."};

	opt_args[{"dec-fnc-ite-M"}] =
		{"positive_int",
		 "set last iteration at which the fnc is used."};

	opt_args[{"dec-fnc-ite-s"}] =
		{"positive_int",
		 "set iteration step for the fnc algorithm."};

	// ---------------------------------------------------------------------------------------------------------- code
	opt_args[{"cde-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};
}

template <typename B, typename Q>
void Factory_decoder_turbo<B,Q>
::store_args(const Arguments_reader& ar, decoder_parameters_turbo &params, const int seed)
{
	params.type   = "BCJR";
	params.implem = "FAST";

	Factory_decoder_common::store_args(ar, params);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::store_args(ar, params.itl, seed);

	// ------------------------------------------------------------------------------------------------------- decoder
	if(ar.exist_arg({"dec-ite", "i" })) params.n_ite          = ar.get_arg_int({"dec-ite", "i" });
	if(ar.exist_arg({"dec-sf"       })) params.scaling_factor = ar.get_arg    ({"dec-sf"       });
	if(ar.exist_arg({"dec-simd"     })) params.simd_strategy  = ar.get_arg    ({"dec-simd"     });
	if(ar.exist_arg({"dec-max"      })) params.max            = ar.get_arg    ({"dec-max"      });

	if(ar.exist_arg({"dec-sc"       })) params.self_corrected = true;
	if(ar.exist_arg({"dec-fnc"      })) params.fnc            = true;
	if(ar.exist_arg({"dec-fnc-q"    })) params.fnc_q          = ar.get_arg_int({"dec-fnc-q"    });
	if(ar.exist_arg({"dec-fnc-ite-s"})) params.fnc_ite_step   = ar.get_arg_int({"dec-fnc-ite-s"});
	if(ar.exist_arg({"dec-fnc-ite-m"})) params.fnc_ite_min    = ar.get_arg_int({"dec-fnc-ite-m"});
	if(ar.exist_arg({"dec-fnc-ite-M"})) params.fnc_ite_max    = ar.get_arg_int({"dec-fnc-ite-M"});
	else                                params.fnc_ite_max    = params.n_ite;

//	if (params.simd_strategy == "INTER" && !ar.exist_arg({"sim-inter-lvl"}))
//		params.inter_frame_level = mipp::nElReg<Q>();
//	else if (params.simd_strategy == "INTRA" && !ar.exist_arg({"sim-inter-lvl"}))
//		params.inter_frame_level = (int)std::ceil(mipp::nElReg<Q>() / 8.f);

	if (params.type == "LTE")
	{
		params.type = "BCJR";
		params.poly = {013, 015};
		if (!ar.exist_arg({"itl-type"}))
			params.itl.type = "LTE";
	}

	if (params.type == "CCSDS")
	{
		params.type = "BCJR";
		params.poly = {023, 033};
		if (!ar.exist_arg({"itl-type"}))
			params.itl.type = "CCSDS";
	}

	if (!(params.poly[0] == 013 && params.poly[1] == 015)) // if not LTE BCJR
	{
		params.type          = "BCJR";
		params.implem        = "GENERIC";
		params.simd_strategy = "";
	}

//	if (!params.json_path.empty())
//	{
//		params.type          = "BCJR";
//		params.implem        = "GENERIC_JSON";
//		params.simd_strategy = "";
//	}

//	params.tail_length = (int)(4 * std::floor(std::log2((float)std::max(params.poly[0],
//																				   params.poly[1]))));
//	params.N += params.tail_length;
//	params.N_code = 3 * params.K + params.tail_length;

	// ---------------------------------------------------------------------------------------------------------- code
	if (ar.exist_arg({"cde-poly"}))
	{
		auto poly_str = ar.get_arg({"cde-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}
}

template <typename B, typename Q>
void Factory_decoder_turbo<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder_common::group_args(ar);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::group_args(ar);
}

template <typename B, typename Q>
void Factory_decoder_turbo<B,Q>
::header(Header::params_list& head_dec, Header::params_list& head_cde, Header::params_list& head_itl,
         const decoder_parameters_turbo& params, bool crc_activated)
{
	Factory_decoder_common::header(head_dec, params);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::header(head_itl, params.itl);

	// ------------------------------------------------------------------------------------------------------- decoder
	if (!params.simd_strategy.empty())
		head_dec.push_back(std::make_pair("SIMD strategy", params.simd_strategy));

	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));
	head_dec.push_back(std::make_pair("Scaling factor",         params.scaling_factor       ));
	head_dec.push_back(std::make_pair("Max type",               params.max                  ));

	if (crc_activated)
	{
		head_dec.push_back(std::make_pair("Self-corrected",       ((params.self_corrected) ? "on" : "off")));
		head_dec.push_back(std::make_pair("Flip aNd Check (FNC)", ((params.fnc           ) ? "on" : "off")));

		if (params.fnc)
		{
			head_dec.push_back(std::make_pair("FNC q"       ,  std::to_string(params.fnc_q)));
			head_dec.push_back(std::make_pair("FNC ite min" ,  std::to_string(params.fnc_ite_min)));
			head_dec.push_back(std::make_pair("FNC ite max" ,  std::to_string(params.fnc_ite_max)));
			head_dec.push_back(std::make_pair("FNC ite step",  std::to_string(params.fnc_ite_step)));
		}
	}

	// ---------------------------------------------------------------------------------------------------------- code
	std::stringstream poly;
	poly << "{0" << std::oct << params.poly[0] << ",0" << std::oct << params.poly[1] << "}";
	head_cde.push_back(std::make_pair(std::string("Polynomials"), poly.str()));
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_turbo<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_turbo<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_turbo<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_turbo<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
