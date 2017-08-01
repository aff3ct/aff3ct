#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"

#include "Decoder_turbo_DB.hpp"

const std::string aff3ct::factory::Decoder_turbo_DB::name   = "Decoder Turbo DB";
const std::string aff3ct::factory::Decoder_turbo_DB::prefix = "dec";

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B, typename Q>
module::Decoder_turbo_DB<B,Q>* Decoder_turbo_DB
::build(const parameters                       &params,
        const module::Interleaver<int>         &itl,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_n,
              module::Decoder_RSC_DB_BCJR<B,Q> &siso_i)
{
	if (params.type == "TURBO_DB")
	{
		if (params.implem == "STD") return new module::Decoder_turbo_DB<B,Q>(params.K, params.N_cw, params.n_ite, itl, siso_n, siso_i);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Decoder_turbo_DB
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Decoder::build_args(req_args, opt_args, p);
	req_args.erase({p+"-cw-size", "N"});

	Interleaver::build_args(req_args, opt_args, "itl");
	req_args.erase({"itl-size"    });
	opt_args.erase({"itl-fra", "F"});

	opt_args[{p+"-type", "D"}].push_back("TURBO_DB");

	opt_args[{p+"-implem"}].push_back("STD");

	opt_args[{p+"-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the turbo."};

	Scaling_factor::build_args(req_args, opt_args, p+"-sf" );
	opt_args.erase({p+"-sf-ite"});

	Flip_and_check::build_args(req_args, opt_args, p+"-fnc");
	req_args.erase({p+"-fnc-size", "K"});
	opt_args.erase({p+"-fnc-fra",  "F"});
	opt_args.erase({p+"-fnc-ite",  "i"});

	Decoder_RSC_DB::build_args(req_args, opt_args, p+"-sub");
	req_args.erase({p+"-sub-info-bits", "K"});
	req_args.erase({p+"-sub-cw-size",   "N"});
	opt_args.erase({p+"-sub-fra",       "F"});
}

void Decoder_turbo_DB
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type   = "TURBO_DB";
	params.implem = "STD";

	Decoder::store_args(vals, params, p);

	if(exist(vals, {p+"-ite", "i"})) params.n_ite = std::stoi(vals.at({p+"-ite", "i"}));

	params.sub.K        = params.K;
	params.sub.n_frames = params.n_frames;

	Decoder_RSC_DB::store_args(vals, params.sub, p+"-sub");

	params.N_cw = 2 * params.sub.N_cw - params.K;
	params.R    = (float)params.K / (float)params.N_cw;

	params.itl.size     = params.K >> 1;
	params.itl.n_frames = params.n_frames;
	Interleaver::store_args(vals, params.itl, "itl");

	if (params.sub.implem == "DVB-RCS1" && !exist(vals, {"itl-type"}))
		params.itl.type = "ARP_DVBS1";

	if (params.sub.implem == "DVB-RCS2" && !exist(vals, {"itl-type"}))
		params.itl.type = "ARP_DVBS2";

	params.sf.n_ite = params.n_ite;
	Scaling_factor::store_args(vals, params.sf, p+"-sf");

	params.fnc.size     = params.K;
	params.fnc.n_frames = params.n_frames;
	params.fnc.n_ite    = params.n_ite;
	Flip_and_check::store_args(vals, params.fnc, p+"-fnc");
}

void Decoder_turbo_DB
::make_header(params_list& head_dec, params_list& head_itl, const parameters& params, const bool full)
{
	Decoder    ::make_header(head_dec, params,     full);
	Interleaver::make_header(head_itl, params.itl, full);

	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));
	if (params.tail_length && full)
		head_dec.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));

	Scaling_factor::make_header(head_dec, params.sf,  full);
	Flip_and_check::make_header(head_dec, params.fnc, full);

	params_list head_dec_sub;
	Decoder_RSC_DB::make_header(head_dec_sub, params.sub, full);
	for (auto p : head_dec_sub) { p.first.insert(0, Decoder_RSC_DB::name + ": "); head_dec.push_back(p); }
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder_turbo_DB<B_8 ,Q_8 >* aff3ct::factory::Decoder_turbo_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&, aff3ct::module::Decoder_RSC_DB_BCJR<B_8 ,Q_8 >&);
template aff3ct::module::Decoder_turbo_DB<B_16,Q_16>* aff3ct::factory::Decoder_turbo_DB::build<B_16,Q_16>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>&);
template aff3ct::module::Decoder_turbo_DB<B_32,Q_32>* aff3ct::factory::Decoder_turbo_DB::build<B_32,Q_32>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>&);
template aff3ct::module::Decoder_turbo_DB<B_64,Q_64>* aff3ct::factory::Decoder_turbo_DB::build<B_64,Q_64>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&, aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>&);
#else
template aff3ct::module::Decoder_turbo_DB<B,Q>* aff3ct::factory::Decoder_turbo_DB::build<B,Q>(const aff3ct::factory::Decoder_turbo_DB::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&, aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>&);
#endif
// ==================================================================================== explicit template instantiation
