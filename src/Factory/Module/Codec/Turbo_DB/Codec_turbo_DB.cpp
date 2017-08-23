#include "Codec_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_turbo_DB::name   = "Codec Turbo DB";
const std::string aff3ct::factory::Codec_turbo_DB::prefix = "cdc";

template <typename B, typename Q>
module::Codec_turbo_DB<B,Q>* Codec_turbo_DB::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_turbo_DB<B,Q>(enc, dec, pct, crc);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Codec_turbo_DB<B,Q>* Codec_turbo_DB
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
}

void Codec_turbo_DB
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Codec             ::build_args(req_args, opt_args, p);
	Encoder_turbo_DB  ::build_args(req_args, opt_args   );
	Puncturer_turbo_DB::build_args(req_args, opt_args   );
	Decoder_turbo_DB  ::build_args(req_args, opt_args   );

	req_args.erase({"pct-info-bits", "K"});
	opt_args.erase({"pct-no-buff"       });
	opt_args.erase({"pct-fra",       "F"});
	opt_args.erase({"pct-tail-length"   });
	req_args.erase({"dec-cw-size",   "N"});
	req_args.erase({"dec-info-bits", "K"});
	opt_args.erase({"dec-fra",       "F"});
	opt_args.erase({"dec-sub-no-buff"   });
	opt_args.erase({"dec-sub-std"       });
	opt_args.erase({"dec-json"          });

	opt_args[{"pct-fra-size", "N"}] = req_args[{"pct-fra-size", "N"}];
	req_args.erase({"pct-fra-size", "N"});
}

void Codec_turbo_DB
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Codec::store_args(vals, params, p);

	Encoder_turbo_DB::store_args(vals, params.enc);

	params.pct.K        = params.enc.K;
	params.pct.N_cw     = params.enc.N_cw;
	params.pct.n_frames = params.enc.n_frames;

	Puncturer_turbo_DB::store_args(vals, params.pct);

	params.dec.K            = params.enc.K;
	params.dec.N_cw         = params.enc.N_cw;
	params.dec.sub.buffered = params.enc.sub.buffered;
	params.dec.n_frames     = params.enc.n_frames;
	params.dec.sub.n_frames = params.enc.sub.n_frames;

	Decoder_turbo_DB::store_args(vals, params.dec);

	if (!params.enc.sub.standard.empty() && !exist(vals, {"dec-sub-implem"}))
		params.dec.sub.implem = params.enc.sub.standard;

	params.K    = params.enc.K;
	params.N_cw = params.enc.N_cw;
	params.N    = params.pct.N;
}

void Codec_turbo_DB
::make_header(params_list& head_enc, params_list& head_dec, params_list& head_pct, params_list& head_itl,
              const parameters& params, const bool full)
{
	params_list trash;
	Codec             ::make_header(trash,              params,     full);
	Encoder_turbo_DB  ::make_header(head_enc, head_itl, params.enc, full);
	Puncturer_turbo_DB::make_header(head_pct,           params.pct, full);
	Decoder_turbo_DB  ::make_header(head_dec, trash   , params.dec, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_turbo_DB<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo_DB::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_turbo_DB<B_16,Q_16>* aff3ct::factory::Codec_turbo_DB::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_turbo_DB<B_32,Q_32>* aff3ct::factory::Codec_turbo_DB::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_turbo_DB<B_64,Q_64>* aff3ct::factory::Codec_turbo_DB::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_turbo_DB<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_turbo_DB::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_turbo_DB<B_16,Q_16>* aff3ct::factory::Codec_turbo_DB::build<B_16,Q_16>(const aff3ct::factory::Codec_turbo_DB::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_turbo_DB<B_32,Q_32>* aff3ct::factory::Codec_turbo_DB::build<B_32,Q_32>(const aff3ct::factory::Codec_turbo_DB::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_turbo_DB<B_64,Q_64>* aff3ct::factory::Codec_turbo_DB::build<B_64,Q_64>(const aff3ct::factory::Codec_turbo_DB::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_turbo_DB<B,Q>* aff3ct::factory::Codec_turbo_DB::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_turbo_DB<B,Q>* aff3ct::factory::Codec_turbo_DB::build<B,Q>(const aff3ct::factory::Codec_turbo_DB::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation
