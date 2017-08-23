#include "Codec_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_polar::name   = "Codec Polar";
const std::string aff3ct::factory::Codec_polar::prefix = "cdc";

template <typename B, typename Q>
module::Codec_polar<B,Q>* Codec_polar::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_polar<B,Q>(fbg, enc, dec, pct, crc);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Codec_polar<B,Q>* Codec_polar
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
}

void Codec_polar
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Codec               ::build_args(req_args, opt_args, p       );
	Puncturer_polar     ::build_args(req_args, opt_args          );
	Encoder_polar       ::build_args(req_args, opt_args          );
	Frozenbits_generator::build_args(req_args, opt_args, "enc-fb");
	Decoder_polar       ::build_args(req_args, opt_args          );

	req_args.erase({"enc-cw-size",      "N"});
	req_args.erase({"enc-info-bits",    "K"});
	opt_args.erase({"enc-fra",          "F"});
	req_args.erase({"dec-cw-size",      "N"});
	req_args.erase({"dec-info-bits",    "K"});
	opt_args.erase({"dec-fra",          "F"});
	opt_args.erase({"dec-no-sys"           });
	req_args.erase({"enc-fb-cw-size",   "N"});
	req_args.erase({"enc-fb-info-bits", "K"});
}

void Codec_polar
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Codec::store_args(vals, params, p);

	Puncturer_polar::store_args(vals, params.pct);

	params.enc.K        = params.fbg.K    = params.dec.K        = params.pct.K;
	params.enc.N_cw     = params.fbg.N_cw = params.dec.N_cw     = params.pct.N_cw;
	params.enc.n_frames                   = params.dec.n_frames = params.pct.n_frames;

	Encoder_polar::store_args(vals, params.enc);

	params.dec.systematic = params.enc.systematic;

	Frozenbits_generator::store_args(vals, params.fbg, "enc-fb");

	Decoder_polar::store_args(vals, params.dec);

	params.K    = params.pct.K;
	params.N_cw = params.pct.N_cw;
	params.N    = params.pct.N;
}

void Codec_polar
::make_header(params_list& head_enc, params_list& head_fbg, params_list& head_dec, params_list& head_pct,
              const parameters& params, const bool full)
{
	params_list trash;
	Codec               ::make_header(trash,    params,     full);
	Puncturer_polar     ::make_header(head_pct, params.pct, full);
	Encoder_polar       ::make_header(head_enc, params.enc, full);
	Frozenbits_generator::make_header(head_fbg, params.fbg, full);
	Decoder_polar       ::make_header(head_dec, params.dec, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_polar<B_8 ,Q_8 >* aff3ct::factory::Codec_polar::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_polar<B_16,Q_16>* aff3ct::factory::Codec_polar::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_polar<B_32,Q_32>* aff3ct::factory::Codec_polar::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_polar<B_64,Q_64>* aff3ct::factory::Codec_polar::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_polar<B_8 ,Q_8 >* aff3ct::factory::Codec_polar::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_polar<B_16,Q_16>* aff3ct::factory::Codec_polar::build<B_16,Q_16>(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_polar<B_32,Q_32>* aff3ct::factory::Codec_polar::build<B_32,Q_32>(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_polar<B_64,Q_64>* aff3ct::factory::Codec_polar::build<B_64,Q_64>(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_polar<B,Q>* aff3ct::factory::Codec_polar::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_polar<B,Q>* aff3ct::factory::Codec_polar::build<B,Q>(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation

