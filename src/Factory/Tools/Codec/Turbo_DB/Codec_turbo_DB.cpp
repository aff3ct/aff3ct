#include "Factory/Tools/Codec/Turbo_DB/Codec_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_turbo_DB_name   = "Codec Turbo DB";
const std::string aff3ct::factory::Codec_turbo_DB_prefix = "cdc";

Codec_turbo_DB
::Codec_turbo_DB(const std::string &prefix)
: Codec_SIHO(Codec_turbo_DB_name, prefix)
{
	auto enc_t = new Encoder_turbo_DB("enc");
	auto dec_t = new Decoder_turbo_DB("dec");
	Codec::set_enc(enc_t);
	Codec::set_dec(dec_t);
	Codec::set_itl(std::move(enc_t->itl));
	dec_t->itl = nullptr;
}

Codec_turbo_DB* Codec_turbo_DB
::clone() const
{
	return new Codec_turbo_DB(*this);
}

void Codec_turbo_DB
::enable_puncturer()
{
	set_pct(new Puncturer_turbo_DB("pct"));
}

void Codec_turbo_DB
::get_description(cli::Argument_map_info &args) const
{
	Codec_SIHO::get_description(args);

	auto dec_tur = dynamic_cast<Decoder_turbo_DB*>(dec.get());

	if (pct != nullptr)
	{
		pct->get_description(args);

		auto ppct = pct->get_prefix();

		args.erase({ppct+"-info-bits", "K"});
		args.erase({ppct+"-no-buff"       });
		args.erase({ppct+"-fra",       "F"});
		args.erase({ppct+"-tail-length"   });

		args[{ppct+"-fra-size", "N"}]->rank = cli::arg_rank::OPT;
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


	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}
}

void Codec_turbo_DB
::store(const cli::Argument_map_value &vals)
{
	Codec_SIHO::store(vals);

	auto enc_tur = dynamic_cast<Encoder_turbo_DB*>(enc.get());
	auto dec_tur = dynamic_cast<Decoder_turbo_DB*>(dec.get());

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
	enc_tur->sub->standard = dec_tur->sub->implem;

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = pct != nullptr ? pct->N : enc->N_cw;


	if (itl != nullptr)
	{
		itl->core->size     = enc->K >> 1;
		itl->core->n_frames = enc->n_frames;

		itl->store(vals);

		if (dec_tur->sub->implem == "DVB-RCS1" && !vals.exist({"itl-type"}))
			itl->core->type = "DVB-RCS1";

		if (dec_tur->sub->implem == "DVB-RCS2" && !vals.exist({"itl-type"}))
			itl->core->type = "DVB-RCS2";
	}
}

void Codec_turbo_DB
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Codec_SIHO::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
	if (pct != nullptr)
		pct->get_headers(headers, full);
	if (itl != nullptr)
		itl->get_headers(headers, full);
}

template <typename B, typename Q>
tools::Codec_turbo_DB<B,Q>* Codec_turbo_DB
::build(module::CRC<B> *crc) const
{
	return new tools::Codec_turbo_DB<B,Q>(dynamic_cast<const Encoder_turbo_DB  &>(*enc),
	                                      dynamic_cast<const Decoder_turbo_DB  &>(*dec),
	                                      *itl,
	                                      dynamic_cast<const Puncturer_turbo_DB*>(pct.get()),
	                                      crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Codec_turbo_DB<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo_DB::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::tools::Codec_turbo_DB<B_16,Q_16>* aff3ct::factory::Codec_turbo_DB::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::tools::Codec_turbo_DB<B_32,Q_32>* aff3ct::factory::Codec_turbo_DB::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::tools::Codec_turbo_DB<B_64,Q_64>* aff3ct::factory::Codec_turbo_DB::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::tools::Codec_turbo_DB<B,Q>* aff3ct::factory::Codec_turbo_DB::build<B,Q>(aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
