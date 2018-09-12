#include "Codec_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_turbo_DB_name   = "Codec Turbo DB";
const std::string aff3ct::factory::Codec_turbo_DB_prefix = "cdc";

Codec_turbo_DB::parameters
::parameters(const std::string &prefix)
: Codec     ::parameters(Codec_turbo_DB_name, prefix),
  Codec_SIHO::parameters(Codec_turbo_DB_name, prefix)
{
	auto enc_t = new Encoder_turbo_DB::parameters("enc");
	Codec::parameters::set_enc(enc_t);
	Codec::parameters::set_dec(new Decoder_turbo_DB::parameters("dec"));
	Codec::parameters::set_itl(std::move(enc_t->itl));
	// delete dec->itl; dec->itl = enc->itl;
}

Codec_turbo_DB::parameters* Codec_turbo_DB::parameters
::clone() const
{
	return new Codec_turbo_DB::parameters(*this);
}

void Codec_turbo_DB::parameters
::enable_puncturer()
{
	set_pct(new Puncturer_turbo_DB::parameters("pct"));
}

void Codec_turbo_DB::parameters
::get_description(tools::Argument_map_info &args) const
{
	Codec_SIHO::parameters::get_description(args);

	auto dec_tur = dynamic_cast<Decoder_turbo_DB::parameters*>(dec.get());

	if (pct != nullptr)
	{
		pct->get_description(args);

		auto ppct = pct->get_prefix();

		args.erase({ppct+"-info-bits", "K"});
		args.erase({ppct+"-no-buff"       });
		args.erase({ppct+"-fra",       "F"});
		args.erase({ppct+"-tail-length"   });

		args[{ppct+"-fra-size", "N"}]->rank = tools::arg_rank::OPT;
	}

	enc->get_description(args);
	dec->get_description(args);

	auto pdec = dec_tur->get_prefix();
	auto pdes = dec_tur->sub->get_prefix();

	args.erase({pdec+"-cw-size",   "N"});
	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-fra",       "F"});
	args.erase({pdes+"-no-buff"       });
	args.erase({pdes+"-std"           });
	args.erase({pdec+"-json"          });
}

void Codec_turbo_DB::parameters
::store(const tools::Argument_map_value &vals)
{
	Codec_SIHO::parameters::store(vals);

	auto enc_tur = dynamic_cast<Encoder_turbo_DB::parameters*>(enc.get());
	auto dec_tur = dynamic_cast<Decoder_turbo_DB::parameters*>(dec.get());

	enc->store(vals);

	if (pct != nullptr)
	{
		pct->K        = enc->K;
		pct->N        = enc->N_cw;
		pct->N_cw     = enc->N_cw;
		pct->n_frames = enc->n_frames;

		pct->store(vals);
	}

	dec_tur->K             = enc_tur->K;
	dec_tur->N_cw          = enc_tur->N_cw;
	dec_tur->sub->buffered = enc_tur->sub->buffered;
	dec_tur->n_frames      = enc_tur->n_frames;
	dec_tur->sub->n_frames = enc_tur->sub->n_frames;

	dec->store(vals);

	auto pdes = dec_tur->sub->get_prefix();

	if (!enc_tur->sub->standard.empty() && !vals.exist({pdes+"-implem"}))
		dec_tur->sub->implem = enc_tur->sub->standard;

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = pct != nullptr ? pct->N : enc->N_cw;
}

void Codec_turbo_DB::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SIHO::parameters::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
	if (pct != nullptr)
		pct->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_turbo_DB<B,Q>* Codec_turbo_DB::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_turbo_DB<B,Q>(dynamic_cast<const Encoder_turbo_DB  ::parameters&>(*enc),
	                                       dynamic_cast<const Decoder_turbo_DB  ::parameters&>(*dec),
	                                       dynamic_cast<const Puncturer_turbo_DB::parameters*>(pct.get()),
	                                       crc);
}

template <typename B, typename Q>
module::Codec_turbo_DB<B,Q>* Codec_turbo_DB
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
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
