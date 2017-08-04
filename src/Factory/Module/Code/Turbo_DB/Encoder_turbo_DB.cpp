#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"

#include "Encoder_turbo_DB.hpp"

const std::string aff3ct::factory::Encoder_turbo_DB::name   = "Encoder Turbo DB";
const std::string aff3ct::factory::Encoder_turbo_DB::prefix = "enc";

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B>
module::Encoder<B>* Encoder_turbo_DB
::build(const parameters                &params,
        const module::Interleaver<int>  &itl,
              module::Encoder_RSC_DB<B> &sub_enc)
{
	if (params.type == "TURBO_DB") return new module::Encoder_turbo_DB<B>(params.K, params.N_cw, itl, sub_enc, sub_enc, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Encoder_turbo_DB
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args, p);
	req_args.erase({p+"-cw-size", "N"});

	Interleaver::build_args(req_args, opt_args, "itl");
	req_args.erase({"itl-size"    });
	opt_args.erase({"itl-fra", "F"});

	opt_args[{p+"-type"}][2] += ", TURBO_DB";

	opt_args[{p+"-json-path"}] =
		{"string",
		 "path to store the encoder and decoder traces formated in JSON."};

	Encoder_RSC_DB::build_args(req_args, opt_args, p+"-sub");
	req_args.erase({p+"-sub-info-bits", "K"});
	req_args.erase({p+"-sub-cw-size",   "N"});
	opt_args.erase({p+"-sub-fra",       "F"});
	opt_args.erase({p+"-sub-seed",      "S"});
	opt_args.erase({p+"-sub-path"          });
	opt_args.erase({p+"-sub-no-buff"       });
}

void Encoder_turbo_DB
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "TURBO_DB";

	Encoder::store_args(vals, params, p);

	params.sub.K        = params.K;
	params.sub.n_frames = params.n_frames;
	params.sub.seed     = params.seed;

	Encoder_RSC_DB::store_args(vals, params.sub, p+"-sub");

	params.N_cw = 2 * params.sub.N_cw - params.K;
	params.R    = (float)params.K / (float)params.N_cw;

	params.itl.size     = params.K >> 1;
	params.itl.n_frames = params.n_frames;
	Interleaver::store_args(vals, params.itl, "itl");

	if (params.sub.standard == "DVB-RCS1" && !exist(vals, {"itl-type"}))
		params.itl.type = "DVB-RCS1";

	if (params.sub.standard == "DVB-RCS2" && !exist(vals, {"itl-type"}))
		params.itl.type = "DVB-RCS2";
}

void Encoder_turbo_DB
::make_header(params_list& head_enc, params_list& head_itl, const parameters& params, const bool full)
{
	Encoder    ::make_header(head_enc, params,     full);
	Interleaver::make_header(head_itl, params.itl, full);

	if (params.tail_length)
		head_enc.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));

	if (!params.json_path.empty())
		head_enc.push_back(std::make_pair("Path to the JSON file", params.json_path));

	params_list head_enc_sub;
	Encoder_RSC_DB::make_header(head_enc_sub, params.sub, full);
	for (auto p : head_enc_sub) { p.first.insert(0, Encoder_RSC_DB::name + ": "); head_enc.push_back(p); }
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_turbo_DB::build<B_8 >(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_RSC_DB<B_8 >&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_turbo_DB::build<B_16>(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_RSC_DB<B_16>&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_turbo_DB::build<B_32>(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_RSC_DB<B_32>&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_turbo_DB::build<B_64>(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_RSC_DB<B_64>&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_turbo_DB::build<B>(const aff3ct::factory::Encoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_RSC_DB<B>&);
#endif
// ==================================================================================== explicit template instantiation
