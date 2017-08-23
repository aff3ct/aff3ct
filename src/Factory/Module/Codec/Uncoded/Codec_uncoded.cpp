#include "Codec_uncoded.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_uncoded::name   = "Codec Uncoded";
const std::string aff3ct::factory::Codec_uncoded::prefix = "cdc";

template <typename B, typename Q>
module::Codec_uncoded<B,Q>* Codec_uncoded::parameters
::build() const
{
	return new module::Codec_uncoded<B,Q>(enc, dec);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Codec_uncoded<B,Q>* Codec_uncoded
::build(const parameters &params)
{
	return params.template build<B,Q>();
}

void Codec_uncoded
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Codec     ::build_args(req_args, opt_args, p);
	Encoder   ::build_args(req_args, opt_args   );
	Decoder_NO::build_args(req_args, opt_args   );

	opt_args.erase({"enc-type"          });
	req_args.erase({"enc-cw-size",   "N"});
	opt_args.erase({"enc-path"          });
	opt_args.erase({"enc-seed",      "S"});
	req_args.erase({"dec-cw-size",   "N"});
	req_args.erase({"dec-info-bits", "K"});
	opt_args.erase({"dec-fra",       "F"});
	opt_args.erase({"dec-implem"        });
	opt_args.erase({"dec-type",      "D"});
}

void Codec_uncoded
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Codec::store_args(vals, params, p);

	params.enc.type = "NO";

	Encoder::store_args(vals, params.enc);

	params.enc.N_cw     = params.enc.K;

	params.dec.type     = "NONE";
	params.dec.implem   = "HARD_DECISION";
	params.dec.K        = params.enc.K;
	params.dec.N_cw     = params.enc.N_cw;
	params.dec.n_frames = params.enc.n_frames;

	Decoder_NO::store_args(vals, params.dec);

	params.K    = params.enc.K;
	params.N_cw = params.enc.N_cw;
	params.N    = params.enc.N_cw;
}

void Codec_uncoded
::make_header(params_list& head_enc, params_list& head_dec, const parameters& params, const bool full)
{
	params_list trash;
	Codec     ::make_header(trash,    params,     full);
	Encoder   ::make_header(head_enc, params.enc, full);
	Decoder_NO::make_header(head_dec, params.dec, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_uncoded<B_8 ,Q_8 >* aff3ct::factory::Codec_uncoded::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Codec_uncoded<B_16,Q_16>* aff3ct::factory::Codec_uncoded::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Codec_uncoded<B_32,Q_32>* aff3ct::factory::Codec_uncoded::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Codec_uncoded<B_64,Q_64>* aff3ct::factory::Codec_uncoded::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Codec_uncoded<B_8 ,Q_8 >* aff3ct::factory::Codec_uncoded::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_uncoded::parameters&);
template aff3ct::module::Codec_uncoded<B_16,Q_16>* aff3ct::factory::Codec_uncoded::build<B_16,Q_16>(const aff3ct::factory::Codec_uncoded::parameters&);
template aff3ct::module::Codec_uncoded<B_32,Q_32>* aff3ct::factory::Codec_uncoded::build<B_32,Q_32>(const aff3ct::factory::Codec_uncoded::parameters&);
template aff3ct::module::Codec_uncoded<B_64,Q_64>* aff3ct::factory::Codec_uncoded::build<B_64,Q_64>(const aff3ct::factory::Codec_uncoded::parameters&);
#else
template aff3ct::module::Codec_uncoded<B,Q>* aff3ct::factory::Codec_uncoded::parameters::build<B,Q>() const;
template aff3ct::module::Codec_uncoded<B,Q>* aff3ct::factory::Codec_uncoded::build<B,Q>(const aff3ct::factory::Codec_uncoded::parameters&);
#endif
// ==================================================================================== explicit template instantiation
