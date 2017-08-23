#include "Codec_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_turbo::name   = "Codec Turbo";
const std::string aff3ct::factory::Codec_turbo::prefix = "cdc";

template <typename B, typename Q>
module::Codec_turbo<B,Q>* Codec_turbo::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_turbo<B,Q>(enc, dec, pct, crc);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}


template <typename B, typename Q>
module::Codec_turbo<B,Q>* Codec_turbo
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
}

void Codec_turbo
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Codec          ::build_args(req_args, opt_args, p);
	Encoder_turbo  ::build_args(req_args, opt_args   );
	Puncturer_turbo::build_args(req_args, opt_args   );
	Decoder_turbo  ::build_args(req_args, opt_args   );

	req_args.erase({"pct-info-bits", "K"});
	opt_args.erase({"pct-no-buff"       });
	opt_args.erase({"pct-fra",       "F"});
	opt_args.erase({"pct-tail-length"   });
	req_args.erase({"dec-cw-size",   "N"});
	req_args.erase({"dec-info-bits", "K"});
	opt_args.erase({"dec-fra",       "F"});
	opt_args.erase({"dec-sub-no-buff"   });
	opt_args.erase({"dec-sub-poly"      });
	opt_args.erase({"dec-sub-std"       });
	opt_args.erase({"dec-json"          });
}

void Codec_turbo
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Codec::store_args(vals, params, p);

	Encoder_turbo::store_args(vals, params.enc);

	params.pct.K           = params.enc.K;
	params.pct.N_cw        = params.enc.N_cw;
	params.pct.buffered    = params.enc.sub1.buffered;
	params.pct.n_frames    = params.enc.n_frames;
	params.pct.tail_length = params.enc.tail_length;

	Puncturer_turbo::store_args(vals, params.pct);

	params.dec.K                = params.enc.K;
	params.dec.N_cw             = params.enc.N_cw;
	params.dec.sub1.buffered    = params.enc.sub1.buffered;
	params.dec.sub2.buffered    = params.enc.sub2.buffered;
	params.dec.n_frames         = params.enc.n_frames;
	params.dec.sub1.n_frames    = params.enc.sub1.n_frames;
	params.dec.sub2.n_frames    = params.enc.sub2.n_frames;
	params.dec.tail_length      = params.enc.tail_length;
	params.dec.sub1.tail_length = params.enc.sub1.tail_length;
	params.dec.sub2.tail_length = params.enc.sub2.tail_length;
	params.dec.sub1.poly        = params.enc.sub1.poly;
	params.dec.sub2.poly        = params.enc.sub2.poly;
	params.dec.sub1.standard    = params.enc.sub1.standard;
	params.dec.sub2.standard    = params.enc.sub2.standard;
	params.dec.enable_json      = !params.enc.json_path.empty();

	Decoder_turbo::store_args(vals, params.dec);

	params.K           = params.enc.K;
	params.N_cw        = params.enc.N_cw;
	params.N           = params.pct.N;
	params.tail_length = params.enc.tail_length;
}

void Codec_turbo
::make_header(params_list& head_enc, params_list& head_dec, params_list& head_pct, params_list& head_itl,
              const parameters& params, const bool full)
{
	params_list trash;
	Codec          ::make_header(trash,              params,     full);
	Encoder_turbo  ::make_header(head_enc, head_itl, params.enc, full);
	Puncturer_turbo::make_header(head_pct,           params.pct, full);
	Decoder_turbo  ::make_header(head_dec, trash   , params.dec, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_turbo<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_turbo<B_16,Q_16>* aff3ct::factory::Codec_turbo::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_turbo<B_32,Q_32>* aff3ct::factory::Codec_turbo::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_turbo<B_64,Q_64>* aff3ct::factory::Codec_turbo::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_turbo<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_turbo<B_16,Q_16>* aff3ct::factory::Codec_turbo::build<B_16,Q_16>(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_turbo<B_32,Q_32>* aff3ct::factory::Codec_turbo::build<B_32,Q_32>(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_turbo<B_64,Q_64>* aff3ct::factory::Codec_turbo::build<B_64,Q_64>(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_turbo<B,Q>* aff3ct::factory::Codec_turbo::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_turbo<B,Q>* aff3ct::factory::Codec_turbo::build<B,Q>(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation

