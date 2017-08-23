#include "Codec_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_RSC_DB::name   = "Codec RSC DB";
const std::string aff3ct::factory::Codec_RSC_DB::prefix = "cdc";

template <typename B, typename Q>
module::Codec_RSC_DB<B,Q>* Codec_RSC_DB::parameters
::build() const
{
	return new module::Codec_RSC_DB<B,Q>(enc, dec);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Codec_RSC_DB<B,Q>* Codec_RSC_DB
::build(const parameters &params)
{
	return params.template build<B,Q>();
}

void Codec_RSC_DB
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Codec         ::build_args(req_args, opt_args, p);
	Encoder_RSC_DB::build_args(req_args, opt_args   );
	Decoder_RSC_DB::build_args(req_args, opt_args   );

	req_args.erase({"dec-cw-size",   "N"});
	req_args.erase({"dec-info-bits", "K"});
	opt_args.erase({"dec-fra",       "F"});
	opt_args.erase({"dec-no-buff"       });

}

void Codec_RSC_DB
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Codec::store_args(vals, params, p);

	Encoder_RSC_DB::store_args(vals, params.enc);

	params.dec.K        = params.enc.K;
	params.dec.N_cw     = params.enc.N_cw;
	params.dec.n_frames = params.enc.n_frames;
	params.dec.buffered = params.enc.buffered;

	Decoder_RSC_DB::store_args(vals, params.dec);

	if (!params.enc.standard.empty() && !exist(vals, {"dec-implem"}))
		params.dec.implem = params.enc.standard;

	params.K    = params.enc.K;
	params.N_cw = params.enc.N_cw;
	params.N    = params.enc.N_cw;
}

void Codec_RSC_DB
::make_header(params_list& head_enc, params_list& head_dec, const parameters& params, const bool full)
{
	params_list trash;
	Codec         ::make_header(trash,    params,     full);
	Encoder_RSC_DB::make_header(head_enc, params.enc, full);
	Decoder_RSC_DB::make_header(head_dec, params.dec, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_RSC_DB<B_8 ,Q_8 >* aff3ct::factory::Codec_RSC_DB::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Codec_RSC_DB<B_16,Q_16>* aff3ct::factory::Codec_RSC_DB::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Codec_RSC_DB<B_32,Q_32>* aff3ct::factory::Codec_RSC_DB::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Codec_RSC_DB<B_64,Q_64>* aff3ct::factory::Codec_RSC_DB::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Codec_RSC_DB<B_8 ,Q_8 >* aff3ct::factory::Codec_RSC_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_RSC_DB::parameters&);
template aff3ct::module::Codec_RSC_DB<B_16,Q_16>* aff3ct::factory::Codec_RSC_DB::build<B_16,Q_16>(const aff3ct::factory::Codec_RSC_DB::parameters&);
template aff3ct::module::Codec_RSC_DB<B_32,Q_32>* aff3ct::factory::Codec_RSC_DB::build<B_32,Q_32>(const aff3ct::factory::Codec_RSC_DB::parameters&);
template aff3ct::module::Codec_RSC_DB<B_64,Q_64>* aff3ct::factory::Codec_RSC_DB::build<B_64,Q_64>(const aff3ct::factory::Codec_RSC_DB::parameters&);
#else
template aff3ct::module::Codec_RSC_DB<B,Q>* aff3ct::factory::Codec_RSC_DB::parameters::build<B,Q>() const;
template aff3ct::module::Codec_RSC_DB<B,Q>* aff3ct::factory::Codec_RSC_DB::build<B,Q>(const aff3ct::factory::Codec_RSC_DB::parameters&);
#endif
// ==================================================================================== explicit template instantiation
