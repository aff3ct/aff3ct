#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_fast.hpp"

#include "Decoder_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B, typename Q>
module::Decoder_turbo<B,Q>* Decoder_turbo
::build(const parameters               &params,
        const module::Interleaver<int> &itl,
              module::SISO<Q>          &siso_n,
              module::SISO<Q>          &siso_i)
{
	std::string implem = std::is_same<B,int64_t>::value ? "STD" : "FAST";
	if (params.type == "TURBO")
	{
		     if (implem == "STD" ) return new module::Decoder_turbo_naive<B,Q>(params.K, params.N_cw, params.n_ite, itl, siso_n, siso_i, params.buffered);
		else if (implem == "FAST") return new module::Decoder_turbo_fast <B,Q>(params.K, params.N_cw, params.n_ite, itl, siso_n, siso_i, params.buffered);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_turbo
::build_args(arg_map &req_args, arg_map &opt_args)
{
	Decoder::build_args(req_args, opt_args);
	req_args.erase({"dec-cw-size", "N"});

	opt_args[{"dec-type", "D"}].push_back("BCJR");

	opt_args[{"dec-implem"}].push_back("GENERIC, STD, FAST, VERY_FAST");

	opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo."};

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

	opt_args[{"dec-no-buff"}] =
		{"",
		 "does not suppose a buffered encoding."};

	opt_args[{"dec-json"}] =
		{"",
		 "enable the json output trace."};

	opt_args[{"dec-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{"dec-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};

	Scaling_factor::build_args(req_args, opt_args);
	Flip_and_check::build_args(req_args, opt_args);
	req_args.erase({"dec-fnc-size", "K"});
}

void Decoder_turbo
::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	// for the RSC
	params.type   = "BCJR";
	params.implem = "STD";

	Decoder::store_args(ar, params);

	if(ar.exist_arg({"dec-ite", "i"})) params.n_ite          = ar.get_arg_int({"dec-ite", "i"});
	if(ar.exist_arg({"dec-simd"    })) params.simd_strategy  = ar.get_arg    ({"dec-simd"    });
	if(ar.exist_arg({"dec-max"     })) params.max            = ar.get_arg    ({"dec-max"     });
	if(ar.exist_arg({"dec-std"     })) params.standard       = ar.get_arg    ({"dec-std"     });
	if(ar.exist_arg({"dec-sc"      })) params.self_corrected = true;
	if(ar.exist_arg({"dec-json"    })) params.enable_json    = true;
	if(ar.exist_arg({"dec-no-buff" })) params.buffered       = false;

	if (params.enable_json)
	{
		params.type          = "BCJR";
		params.implem        = "GENERIC_JSON";
		params.simd_strategy = "";
	}

	if (params.standard == "LTE")
		params.poly = {013, 015};

	if (params.standard == "CCSDS")
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

	if (params.poly[0] != 013 || params.poly[1] != 015)
		params.implem = "GENERIC";

	params.tail_length = (int)(4 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N_cw        = 3 * params.K + params.tail_length;
	params.R           = (float)params.K / (float)params.N_cw;

	Scaling_factor::store_args(ar, params.sf);

	params.fnc.size     = params.K;
	params.fnc.n_frames = params.n_frames;
	Flip_and_check::store_args(ar, params.fnc);
}

void Decoder_turbo
::group_args(arg_grp& ar)
{
	Decoder::group_args(ar);
	Scaling_factor::group_args(ar);
	Flip_and_check::group_args(ar);
}

void Decoder_turbo
::header(params_list& head_dec, const parameters& params)
{
	Decoder::header(head_dec, params);

	if (!params.standard.empty())
		head_dec.push_back(std::make_pair("Standard", params.standard));
	std::stringstream poly;
	poly << "{0" << std::oct << params.poly[0] << ",0" << std::oct << params.poly[1] << "}";
	head_dec.push_back(std::make_pair(std::string("Polynomials"), poly.str()));
	if (!params.simd_strategy.empty())
		head_dec.push_back(std::make_pair("SIMD strategy", params.simd_strategy));
	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));
	if (params.tail_length)
		head_dec.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));
	head_dec.push_back(std::make_pair("Max type", params.max));
	head_dec.push_back(std::make_pair("Enable json", ((params.enable_json) ? "on" : "off")));
	head_dec.push_back(std::make_pair("Buffered", ((params.buffered) ? "on" : "off")));
	head_dec.push_back(std::make_pair("Self-corrected", ((params.self_corrected) ? "on" : "off")));

	Scaling_factor::header(head_dec, params.sf);
	Flip_and_check::header(head_dec, params.fnc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_turbo<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::SISO<Q_8 >&, aff3ct::module::SISO<Q_8 >&);
template aff3ct::module::Decoder_turbo<B_16,Q_16>* aff3ct::factory::Decoder_turbo::build<B_16,Q_16>(const aff3ct::factory::Decoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::SISO<Q_16>&, aff3ct::module::SISO<Q_16>&);
template aff3ct::module::Decoder_turbo<B_32,Q_32>* aff3ct::factory::Decoder_turbo::build<B_32,Q_32>(const aff3ct::factory::Decoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::SISO<Q_32>&, aff3ct::module::SISO<Q_32>&);
template aff3ct::module::Decoder_turbo<B_64,Q_64>* aff3ct::factory::Decoder_turbo::build<B_64,Q_64>(const aff3ct::factory::Decoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::SISO<Q_64>&, aff3ct::module::SISO<Q_64>&);
#else
template aff3ct::module::Decoder_turbo<B,Q>* aff3ct::factory::Decoder_turbo::build<B,Q>(const aff3ct::factory::Decoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::SISO<Q>&, aff3ct::module::SISO<Q>&);
#endif
// ==================================================================================== explicit template instantiation
