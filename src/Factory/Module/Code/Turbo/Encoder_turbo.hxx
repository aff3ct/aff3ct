#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Encoder_turbo.hpp"

namespace aff3ct
{
namespace factory
{
template <typename B, class E1, class E2>
module::Encoder<B>* Encoder_turbo
::build(const parameters<E1,E2>        &params,
        const module::Interleaver<int> &itl,
              module::Encoder_sys<B>   *enc_n,
              module::Encoder_sys<B>   *enc_i)
{
	enc_i = (enc_i == nullptr) ? enc_n : enc_i;

	if (params.sub1.buffered)
	{
		if (params.type == "TURBO") return new module::Encoder_turbo       <B>(params.K, params.N_cw, itl, *enc_n, *enc_i, params.n_frames);
	}
	else if (enc_n == enc_i)
	{
		if (params.type == "TURBO") return new module::Encoder_turbo_legacy<B>(params.K, params.N_cw, itl, *enc_n,         params.n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <class E1, class E2>
void Encoder_turbo
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args, p);
	req_args.erase({p+"-cw-size", "N"});

	Interleaver::build_args(req_args, opt_args, "itl");
	req_args.erase({"itl-size"    });
	opt_args.erase({"itl-fra", "F"});

	opt_args[{p+"-type"}][2] += ", TURBO";

	opt_args[{p+"-json-path"}] =
		{"string",
		 "path to store the encoder and decoder traces formated in JSON."};

	if (std::is_same<E1,E2>())
	{
		E1::build_args(req_args, opt_args, p+"-sub");
		req_args.erase({p+"-sub-info-bits", "K"});
		req_args.erase({p+"-sub-cw-size",   "N"});
		opt_args.erase({p+"-sub-fra",       "F"});
		opt_args.erase({p+"-sub-seed",      "S"});
		opt_args.erase({p+"-sub-path"          });
	}
	else
	{
		E1::build_args(req_args, opt_args, p+"-sub1");
		E2::build_args(req_args, opt_args, p+"-sub2");

		req_args.erase({p+"-sub1-info-bits", "K"});
		req_args.erase({p+"-sub2-info-bits", "K"});
		req_args.erase({p+"-sub1-cw-size",   "N"});
		req_args.erase({p+"-sub2-cw-size",   "N"});
		opt_args.erase({p+"-sub1-fra",       "F"});
		opt_args.erase({p+"-sub2-fra",       "F"});
		opt_args.erase({p+"-sub1-seed",      "S"});
		opt_args.erase({p+"-sub2-seed",      "S"});
		opt_args.erase({p+"-sub1-path"          });
		opt_args.erase({p+"-sub2-path"          });
	}
}

template <class E1, class E2>
void Encoder_turbo
::store_args(const arg_val_map &vals, parameters<E1,E2> &params, const std::string p)
{
	params.type = "TURBO";

	Encoder::store_args(vals, params, p);

	if(exist(vals, {p+"-json-path"})) params.json_path = vals.at({p+"-json-path"});

	std::string standard = "";
	if (exist(vals, {p+"-sub-std"}) || exist(vals, {p+"-sub1-std"}))
		standard = std::is_same<E1,E2>() ? vals.at({p+"-sub-std"}) : vals.at({p+"-sub1-std"});

	if (standard == "LTE" && !exist(vals, {"itl-type"}))
		params.itl.type = "LTE";

	if (standard == "CCSDS" && !exist(vals, {"itl-type"}))
		params.itl.type = "CCSDS";

	params.sub1.K        = params.K;
	params.sub2.K        = params.K;
	params.sub1.n_frames = params.n_frames;
	params.sub2.n_frames = params.n_frames;
	params.sub1.seed     = params.seed;
	params.sub2.seed     = params.seed;

	if (std::is_same<E1,E2>())
	{
		E1::store_args(vals, params.sub1, p+"-sub");
		E2::store_args(vals, params.sub2, p+"-sub");
	}
	else
	{
		E1::store_args(vals, params.sub1, p+"-sub1");
		E2::store_args(vals, params.sub2, p+"-sub2");
	}

	if (!params.json_path.empty())
	{
		params.sub1.type += "_JSON";
		params.sub2.type += "_JSON";
	}

	params.tail_length = params.sub1.tail_length + params.sub2.tail_length;
	params.N_cw        = params.sub1.N_cw + params.sub2.N_cw - params.K;
	params.R           = (float)params.K / (float)params.N_cw;

	params.itl.size     = params.K;
	params.itl.n_frames = params.n_frames;
	Interleaver::store_args(vals, params.itl, "itl");
}

template <class E1, class E2>
void Encoder_turbo
::make_header(params_list& head_enc, params_list& head_itl, const parameters<E1,E2>& params, const bool full)
{
	Encoder    ::make_header(head_enc, params,     full);
	Interleaver::make_header(head_itl, params.itl, full);

	if (params.tail_length)
		head_enc.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));

	if (!params.json_path.empty())
		head_enc.push_back(std::make_pair("Path to the JSON file", params.json_path));

	if (std::is_same<E1,E2>())
	{
		params_list head_enc_sub1;
		E1::make_header(head_enc_sub1, params.sub1, full);
		for (auto p : head_enc_sub1) { p.first.insert(0, E1::name + ": "); head_enc.push_back(p); }
	}
	else
	{
		params_list head_enc_sub1, head_enc_sub2;
		E1::make_header(head_enc_sub1, params.sub1, full);
		E2::make_header(head_enc_sub2, params.sub2, full);
		for (auto p : head_enc_sub1) { p.first.insert(0, E1::name + ": "); head_enc.push_back(p); }
		for (auto p : head_enc_sub2) { p.first.insert(0, E2::name + ": "); head_enc.push_back(p); }
	}
}
}
}
