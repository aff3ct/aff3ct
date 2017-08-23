#include "Codec_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_RA::name   = "Codec RA";
const std::string aff3ct::factory::Codec_RA::prefix = "cdc";

template <typename B, typename Q>
module::Codec_RA<B,Q>* Codec_RA::parameters
::build() const
{
	return new module::Codec_RA<B,Q>(enc, dec);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Codec_RA<B,Q>* Codec_RA
::build(const parameters &params)
{
	return params.template build<B,Q>();
}

void Codec_RA
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Codec     ::build_args(req_args, opt_args, p);
	Encoder_RA::build_args(req_args, opt_args   );
	Decoder_RA::build_args(req_args, opt_args   );

	req_args.erase({"dec-cw-size",   "N"});
	req_args.erase({"dec-info-bits", "K"});
	opt_args.erase({"dec-fra",       "F"});
}

void Codec_RA
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Codec::store_args(vals, params, p);

	Encoder_RA::store_args(vals, params.enc);

	params.dec.K                 = params.enc.K;
	params.dec.N_cw              = params.enc.N_cw;
	params.dec.n_frames          = params.enc.n_frames;
	params.dec.itl.core.n_frames = params.enc.n_frames;

	Decoder_RA::store_args(vals, params.dec);

	params.K    = params.enc.K;
	params.N_cw = params.enc.N_cw;
	params.N    = params.enc.N_cw;
}

void Codec_RA
::make_header(params_list& head_enc, params_list& head_dec, params_list& head_itl, const parameters& params,
              const bool full)
{
	params_list trash;
	Codec     ::make_header(trash,              params,     full);
	Encoder_RA::make_header(head_enc, head_itl, params.enc, full);
	Decoder_RA::make_header(head_dec, trash   , params.dec, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_RA<B_8 ,Q_8 >* aff3ct::factory::Codec_RA::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Codec_RA<B_16,Q_16>* aff3ct::factory::Codec_RA::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Codec_RA<B_32,Q_32>* aff3ct::factory::Codec_RA::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Codec_RA<B_64,Q_64>* aff3ct::factory::Codec_RA::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Codec_RA<B_8 ,Q_8 >* aff3ct::factory::Codec_RA::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_RA::parameters&);
template aff3ct::module::Codec_RA<B_16,Q_16>* aff3ct::factory::Codec_RA::build<B_16,Q_16>(const aff3ct::factory::Codec_RA::parameters&);
template aff3ct::module::Codec_RA<B_32,Q_32>* aff3ct::factory::Codec_RA::build<B_32,Q_32>(const aff3ct::factory::Codec_RA::parameters&);
template aff3ct::module::Codec_RA<B_64,Q_64>* aff3ct::factory::Codec_RA::build<B_64,Q_64>(const aff3ct::factory::Codec_RA::parameters&);
#else
template aff3ct::module::Codec_RA<B,Q>* aff3ct::factory::Codec_RA::parameters::build<B,Q>() const;
template aff3ct::module::Codec_RA<B,Q>* aff3ct::factory::Codec_RA::build<B,Q>(const aff3ct::factory::Codec_RA::parameters&);
#endif
// ==================================================================================== explicit template instantiation

