#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo/Decoder_turbo_std.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_fast.hpp"

#include "Decoder_turbo.hpp"

namespace aff3ct
{
namespace factory
{
template <typename B, typename Q, class D1, class D2>
module::Decoder_turbo<B,Q>* Decoder_turbo
::build(const parameters<D1,D2>        &params,
        const module::Interleaver<int> &itl,
              module::SISO<Q>          &siso_n,
              module::SISO<Q>          &siso_i)
{
	if (params.type == "TURBO")
	{
		     if (params.implem == "STD" ) return new module::Decoder_turbo_std <B,Q>(params.K, params.N_cw, params.n_ite, itl, siso_n, siso_i, params.sub1.buffered);
		else if (params.implem == "FAST") return new module::Decoder_turbo_fast<B,Q>(params.K, params.N_cw, params.n_ite, itl, siso_n, siso_i, params.sub1.buffered);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <class D1, class D2>
void Decoder_turbo
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args, p);
	req_args.erase({p+"-cw-size", "N"});

	Interleaver::build_args(req_args, opt_args, "itl");
	req_args.erase({"itl-size"    });
	opt_args.erase({"itl-fra", "F"});

	opt_args[{p+"-type", "D"}].push_back("TURBO");

	opt_args[{p+"-implem"}].push_back("STD, FAST");

	opt_args[{p+"-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo."};

	opt_args[{p+"-sc"}] =
		{"",
		 "enables the self corrected decoder (requires \"--crc-type\")."};

	opt_args[{p+"-json"}] =
		{"",
		 "enable the json output trace."};

	Scaling_factor::build_args(req_args, opt_args, p+"-sf" );
	opt_args.erase({p+"-sf-ite"});

	Flip_and_check::build_args(req_args, opt_args, p+"-fnc");
	req_args.erase({p+"-fnc-size", "K"});
	opt_args.erase({p+"-fnc-fra",  "F"});
	opt_args.erase({p+"-fnc-ite",  "i"});

	if (std::is_same<D1,D2>())
	{
		D1::build_args(req_args, opt_args, p+"-sub");
		req_args.erase({p+"-sub-info-bits", "K"});
		req_args.erase({p+"-sub-cw-size",   "N"});
		opt_args.erase({p+"-sub-fra",       "F"});
	}
	else
	{
		D1::build_args(req_args, opt_args, p+"-sub1");
		D2::build_args(req_args, opt_args, p+"-sub2");

		req_args.erase({p+"-sub1-info-bits", "K"});
		req_args.erase({p+"-sub2-info-bits", "K"});
		req_args.erase({p+"-sub1-cw-size",   "N"});
		req_args.erase({p+"-sub2-cw-size",   "N"});
		opt_args.erase({p+"-sub1-fra",       "F"});
		opt_args.erase({p+"-sub2-fra",       "F"});
	}
}

template <class D1, class D2>
void Decoder_turbo
::store_args(const arg_val_map &vals, parameters<D1,D2> &params, const std::string p)
{
	params.type   = "TURBO";
	params.implem = "FAST";

	Decoder::store_args(vals, params, p);

	if(exist(vals, {p+"-ite", "i"})) params.n_ite          = std::stoi(vals.at({p+"-ite", "i"}));
	if(exist(vals, {p+"-sc"      })) params.self_corrected = true;
	if(exist(vals, {p+"-json"    })) params.enable_json    = true;

	if (params.sub1.standard == "LTE" && !exist(vals, {"itl-type"}))
		params.itl.type = "LTE";

	if (params.sub1.standard == "CCSDS" && !exist(vals, {"itl-type"}))
		params.itl.type = "CCSDS";

	params.sub1.K        = params.K;
	params.sub2.K        = params.K;
	params.sub1.n_frames = params.n_frames;
	params.sub2.n_frames = params.n_frames;

	if (std::is_same<D1,D2>())
	{
		D1::store_args(vals, params.sub1, p+"-sub");
		D2::store_args(vals, params.sub2, p+"-sub");
	}
	else
	{
		D1::store_args(vals, params.sub1, p+"-sub1");
		D2::store_args(vals, params.sub2, p+"-sub2");
	}

	if (params.enable_json)
	{
		params.sub1.implem        = "GENERIC_JSON";
		params.sub2.implem        = "GENERIC_JSON";
		params.sub1.simd_strategy = "";
		params.sub2.simd_strategy = "";
	}

	params.tail_length = params.sub1.tail_length + params.sub2.tail_length;
	params.N_cw        = params.sub1.N_cw + params.sub2.N_cw - params.K;
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

template <class D1, class D2>
void Decoder_turbo
::make_header(params_list& head_dec, params_list& head_itl, const parameters<D1,D2>& params, const bool full)
{
	Decoder    ::make_header(head_dec, params,     full);
	Interleaver::make_header(head_itl, params.itl, full);

	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));
	if (params.tail_length && full)
		head_dec.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));
	head_dec.push_back(std::make_pair("Enable json", ((params.enable_json) ? "on" : "off")));
	head_dec.push_back(std::make_pair("Self-corrected", ((params.self_corrected) ? "on" : "off")));

	Scaling_factor::make_header(head_dec, params.sf,  full);
	Flip_and_check::make_header(head_dec, params.fnc, full);

	if (std::is_same<D1,D2>())
	{
		params_list head_dec_sub1;
		D1::make_header(head_dec_sub1, params.sub1, full);
		for (auto p : head_dec_sub1) { p.first.insert(0, D1::name + ": "); head_dec.push_back(p); }
	}
	else
	{
		params_list head_dec_sub1, head_dec_sub2;
		D1::make_header(head_dec_sub1, params.sub1, full);
		D2::make_header(head_dec_sub2, params.sub2, full);
		for (auto p : head_dec_sub1) { p.first.insert(0, D1::name + ": "); head_dec.push_back(p); }
		for (auto p : head_dec_sub2) { p.first.insert(0, D2::name + ": "); head_dec.push_back(p); }
	}
}
}
}
