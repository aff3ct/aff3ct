#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_fast.hpp"

#include "Factory_decoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Decoder_turbo<B,Q>* Factory_decoder_turbo
::build(const parameters       &params,
        const Interleaver<int> &itl,
              SISO<Q>          &siso_n,
              SISO<Q>          &siso_i,
        const bool              buffered)
{
	std::string implem = std::is_same<B,int64_t>::value ? "STD" : "FAST";
	if (params.type == "TURBO")
	{
		     if (implem == "STD" ) return new Decoder_turbo_naive<B,Q>(params.K, params.N, params.n_ite, itl, siso_n, siso_i, buffered);
		else if (implem == "FAST") return new Decoder_turbo_fast <B,Q>(params.K, params.N, params.n_ite, itl, siso_n, siso_i, buffered);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_decoder_turbo
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------ scaling factor
	Factory_scaling_factor::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- decoder
	opt_args[{"dec-type", "D"}].push_back("BCJR");

	opt_args[{"dec-implem"   }].push_back("GENERIC, STD, FAST, VERY_FAST");

	opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo "};

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

	// ------------------------------------------------------------------------------------------------ flip_and_check
	Factory_flip_and_check::build_args(req_args, opt_args);
}

void Factory_decoder_turbo
::store_args(const Arguments_reader& ar, parameters &params,
             const int K, const int N, const int n_frames,
             const bool activate_simd, const bool activate_json)
{
	// for the RSC
	params.type   = "BCJR";
	params.implem = "GENERIC";

	Factory_decoder::store_args(ar, params, K, N, n_frames);

	// ------------------------------------------------------------------------------------------------ scaling factor
	Factory_scaling_factor::store_args(ar, params.scaling_factor);

	// ------------------------------------------------------------------------------------------------------- decoder
	if(ar.exist_arg({"dec-ite", "i" })) params.n_ite          = ar.get_arg_int({"dec-ite", "i" });
	if(ar.exist_arg({"dec-simd"     })) params.simd_strategy  = ar.get_arg    ({"dec-simd"     });
	if(ar.exist_arg({"dec-max"      })) params.max            = ar.get_arg    ({"dec-max"      });

	if(ar.exist_arg({"dec-sc"       })) params.self_corrected = true;

//	if (params.simd_strategy == "INTER" && activate_simd)
//		params.n_frames = mipp::nElReg<Q>();
//	else if (params.simd_strategy == "INTRA" && activate_simd)
//		params.n_frames = (int)std::ceil(mipp::nElReg<Q>() / 8.f);

	if (activate_json)
	{
		params.type          = "BCJR";
		params.implem        = "GENERIC_JSON";
		params.simd_strategy = "";
	}

	// ------------------------------------------------------------------------------------------------ flip_and_check
	Factory_flip_and_check::store_args(ar, params.flip_and_check, params.n_ite, params.N, params.n_ite);
}

void Factory_decoder_turbo
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder::group_args(ar);

	// ------------------------------------------------------------------------------------------------ scaling factor
	Factory_scaling_factor::group_args(ar);

	// ------------------------------------------------------------------------------------------------ flip_and_check
	Factory_flip_and_check::group_args(ar);
}

void Factory_decoder_turbo
::header(Header::params_list& head_dec, const parameters& params, bool crc_activated)
{
	Factory_decoder::header(head_dec, params);

	// ------------------------------------------------------------------------------------------------ scaling factor
	Factory_scaling_factor::header(head_dec, params.scaling_factor);

	// ------------------------------------------------------------------------------------------------------- decoder
	if (!params.simd_strategy.empty())
		head_dec.push_back(std::make_pair("SIMD strategy", params.simd_strategy));

	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));
	head_dec.push_back(std::make_pair("Max type",               params.max                  ));

	if (crc_activated)
	{
		head_dec.push_back(std::make_pair("Self-corrected",       ((params.self_corrected) ? "on" : "off")));

		// -------------------------------------------------------------------------------------------- flip_and_check
		Factory_flip_and_check::header(head_dec, params.flip_and_check);
	}

}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_turbo<B_8 ,Q_8 >* aff3ct::tools::Factory_decoder_turbo::build<B_8 ,Q_8 >(const aff3ct::tools::Factory_decoder_turbo::parameters&, const Interleaver<int>&, SISO<Q_8 >&, SISO<Q_8 >&, const bool);
template aff3ct::module::Decoder_turbo<B_16,Q_16>* aff3ct::tools::Factory_decoder_turbo::build<B_16,Q_16>(const aff3ct::tools::Factory_decoder_turbo::parameters&, const Interleaver<int>&, SISO<Q_16>&, SISO<Q_16>&, const bool);
template aff3ct::module::Decoder_turbo<B_32,Q_32>* aff3ct::tools::Factory_decoder_turbo::build<B_32,Q_32>(const aff3ct::tools::Factory_decoder_turbo::parameters&, const Interleaver<int>&, SISO<Q_32>&, SISO<Q_32>&, const bool);
template aff3ct::module::Decoder_turbo<B_64,Q_64>* aff3ct::tools::Factory_decoder_turbo::build<B_64,Q_64>(const aff3ct::tools::Factory_decoder_turbo::parameters&, const Interleaver<int>&, SISO<Q_64>&, SISO<Q_64>&, const bool);
#else
template aff3ct::module::Decoder_turbo<B,Q>* aff3ct::tools::Factory_decoder_turbo::build<B,Q>(const aff3ct::tools::Factory_decoder_turbo::parameters&, const Interleaver<int>&, SISO<Q>&, SISO<Q>&, const bool);
#endif
// ==================================================================================== explicit template instantiation
