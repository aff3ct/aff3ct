#include "Codec_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_repetition::name   = "Codec Repetition";
const std::string aff3ct::factory::Codec_repetition::prefix = "cdc";

template <typename B, typename Q>
module::Codec_repetition<B,Q>* Codec_repetition::parameters
::build() const
{
	return new module::Codec_repetition<B,Q>(enc, dec);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Codec_repetition<B,Q>* Codec_repetition
::build(const parameters &params)
{
	return params.template build<B,Q>();
}

void Codec_repetition
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Codec             ::build_args(req_args, opt_args, p);
	Encoder_repetition::build_args(req_args, opt_args   );
	Decoder_repetition::build_args(req_args, opt_args   );

	req_args.erase({"dec-cw-size",   "N"});
	req_args.erase({"dec-info-bits", "K"});
	opt_args.erase({"dec-fra",       "F"});
}

void Codec_repetition
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Codec::store_args(vals, params, p);

	Encoder_repetition::store_args(vals, params.enc);

	params.dec.K        = params.enc.K;
	params.dec.N_cw     = params.enc.N_cw;
	params.dec.n_frames = params.enc.n_frames;

	Decoder_repetition::store_args(vals, params.dec);

	params.K    = params.enc.K;
	params.N_cw = params.enc.N_cw;
	params.N    = params.enc.N_cw;
}

void Codec_repetition
::make_header(params_list& head_enc, params_list& head_dec, const parameters& params, const bool full)
{
	params_list trash;
	Codec             ::make_header(trash,    params,     full);
	Encoder_repetition::make_header(head_enc, params.enc, full);
	Decoder_repetition::make_header(head_dec, params.dec, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_repetition<B_8 ,Q_8 >* aff3ct::factory::Codec_repetition::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Codec_repetition<B_16,Q_16>* aff3ct::factory::Codec_repetition::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Codec_repetition<B_32,Q_32>* aff3ct::factory::Codec_repetition::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Codec_repetition<B_64,Q_64>* aff3ct::factory::Codec_repetition::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Codec_repetition<B_8 ,Q_8 >* aff3ct::factory::Codec_repetition::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_repetition::parameters&);
template aff3ct::module::Codec_repetition<B_16,Q_16>* aff3ct::factory::Codec_repetition::build<B_16,Q_16>(const aff3ct::factory::Codec_repetition::parameters&);
template aff3ct::module::Codec_repetition<B_32,Q_32>* aff3ct::factory::Codec_repetition::build<B_32,Q_32>(const aff3ct::factory::Codec_repetition::parameters&);
template aff3ct::module::Codec_repetition<B_64,Q_64>* aff3ct::factory::Codec_repetition::build<B_64,Q_64>(const aff3ct::factory::Codec_repetition::parameters&);
#else
template aff3ct::module::Codec_repetition<B,Q>* aff3ct::factory::Codec_repetition::parameters::build<B,Q>() const;
template aff3ct::module::Codec_repetition<B,Q>* aff3ct::factory::Codec_repetition::build<B,Q>(const aff3ct::factory::Codec_repetition::parameters&);
#endif
// ==================================================================================== explicit template instantiation
