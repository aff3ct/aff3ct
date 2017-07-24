#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_fast.hpp"

#include "Decoder_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_turbo::name   = "Decoder Turbo";
const std::string aff3ct::factory::Decoder_turbo::prefix = "dec";

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
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args);
	req_args.erase({p+"-cw-size", "N"});

	Interleaver::build_args(req_args, opt_args, "itl");
	req_args.erase({"itl-size"    });
	opt_args.erase({"itl-fra", "F"});

	opt_args[{p+"-type", "D"}].push_back("BCJR");

	opt_args[{p+"-implem"}].push_back("GENERIC, STD, FAST, VERY_FAST");

	opt_args[{p+"-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo."};

	opt_args[{p+"-simd"}] =
		{"string",
		 "the SIMD strategy you want to use.",
		 "INTRA, INTER"};

	opt_args[{p+"-max"}] =
		{"string",
		 "the MAX implementation for the nodes.",
		 "MAX, MAXS, MAXL"};

	opt_args[{p+"-sc"}] =
		{"",
		 "enables the self corrected decoder (requires \"--crc-type\")."};

	opt_args[{p+"-no-buff"}] =
		{"",
		 "does not suppose a buffered encoding."};

	opt_args[{p+"-json"}] =
		{"",
		 "enable the json output trace."};

	opt_args[{p+"-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{p+"-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};

	Scaling_factor::build_args(req_args, opt_args, p+"-sf" );
	opt_args.erase({p+"-sf-ite"});

	Flip_and_check::build_args(req_args, opt_args, p+"-fnc");
	req_args.erase({p+"-fnc-size", "K"});
	opt_args.erase({p+"-fnc-fra",  "F"});
	opt_args.erase({p+"-fnc-ite",  "i"});
}

void Decoder_turbo
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	// for the RSC
	params.type   = "BCJR";
	params.implem = "STD";

	Decoder::store_args(vals, params);

	if(exist(vals, {p+"-ite", "i"})) params.n_ite          = std::stoi(vals.at({p+"-ite", "i"}));
	if(exist(vals, {p+"-simd"    })) params.simd_strategy  =           vals.at({p+"-simd"    });
	if(exist(vals, {p+"-max"     })) params.max            =           vals.at({p+"-max"     });
	if(exist(vals, {p+"-std"     })) params.standard       =           vals.at({p+"-std"     });
	if(exist(vals, {p+"-sc"      })) params.self_corrected = true;
	if(exist(vals, {p+"-json"    })) params.enable_json    = true;
	if(exist(vals, {p+"-no-buff" })) params.buffered       = false;

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

	params.tail_length = (int)(4 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N_cw        = 3 * params.K + params.tail_length;
	params.R           = (float)params.K / (float)params.N_cw;

	params.itl.size     = params.K;
	params.itl.n_frames = params.n_frames;
	Interleaver::store_args(vals, params.itl, "itl");

	params.sf.n_ite = params.n_ite;
	Scaling_factor::store_args(vals, params.sf, p+"-sf");

	params.fnc.size     = params.K;
	params.fnc.n_frames = params.n_frames;
	params.fnc.n_ite    = params.n_ite;
	Flip_and_check::store_args(vals, params.fnc, p+"-fnc");
}

void Decoder_turbo
::header(params_list& head_dec, params_list& head_itl, const parameters& params)
{
	Decoder    ::header(head_dec, params);
	Interleaver::header(head_itl, params.itl);

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
