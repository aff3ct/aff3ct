#include "Codec_turbo.hpp"

#include "Factory/Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Factory/Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Factory/Module/Puncturer/Turbo/Puncturer_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_turbo_name   = "Codec Turbo";
const std::string aff3ct::factory::Codec_turbo_prefix = "cdc";

Codec_turbo::parameters
::parameters(const std::string &prefix)
: Codec     ::parameters(Codec_turbo_name, prefix),
  Codec_SIHO::parameters(Codec_turbo_name, prefix)
{
	auto enc_t = new Encoder_turbo::parameters<>("enc");
	Codec::parameters::set_enc(enc_t);
	Codec::parameters::set_dec(new Decoder_turbo::parameters<>("dec"));
	Codec::parameters::set_itl(std::move(enc_t->itl));
	// delete dec->itl; dec->itl = enc->itl;
}

Codec_turbo::parameters* Codec_turbo::parameters
::clone() const
{
	return new Codec_turbo::parameters(*this);
}

void Codec_turbo::parameters
::enable_puncturer()
{
	set_pct(new Puncturer_turbo::parameters("pct"));
}

void Codec_turbo::parameters
::get_description(tools::Argument_map_info &args) const
{
	Codec_SIHO::parameters::get_description(args);

	auto dec_tur = dynamic_cast<Decoder_turbo::parameters<>*>(dec.get());

	if (pct != nullptr)
	{
		pct->get_description(args);

		auto ppct = pct->get_prefix();

		args.erase({ppct+"-info-bits", "K"});
		args.erase({ppct+"-no-buff"       });
		args.erase({ppct+"-fra",       "F"});
		args.erase({ppct+"-tail-length"   });
	}

	enc->get_description(args);
	dec->get_description(args);

	auto pdec = dec_tur->get_prefix();
	auto pdes = dec_tur->sub1->get_prefix();

	args.erase({pdec+"-cw-size",   "N"});
	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-fra",       "F"});
	args.erase({pdes+"-no-buff"       });
	args.erase({pdes+"-poly"          });
	args.erase({pdes+"-std"           });
	args.erase({pdec+"-json"          });
}

void Codec_turbo::parameters
::store(const tools::Argument_map_value &vals)
{
	Codec_SIHO::parameters::store(vals);

	auto enc_tur = dynamic_cast<Encoder_turbo::parameters<>*>(enc.get());
	auto dec_tur = dynamic_cast<Decoder_turbo::parameters<>*>(dec.get());

	enc->store(vals);

	if (pct != nullptr)
	{
		auto pct_tur = dynamic_cast<Puncturer_turbo::parameters*>(pct.get());

		pct_tur->K           = enc_tur->K;
		pct_tur->N_cw        = enc_tur->N_cw;
		pct_tur->buffered    = enc_tur->sub1->buffered;
		pct_tur->n_frames    = enc_tur->n_frames;
		pct_tur->tail_length = enc_tur->tail_length;

		pct->store(vals);
	}

	dec_tur->K                 = enc_tur->K;
	dec_tur->N_cw              = enc_tur->N_cw;
	dec_tur->sub1->buffered    = enc_tur->sub1->buffered;
	dec_tur->sub2->buffered    = enc_tur->sub2->buffered;
	dec_tur->n_frames          = enc_tur->n_frames;
	dec_tur->sub1->n_frames    = enc_tur->sub1->n_frames;
	dec_tur->sub2->n_frames    = enc_tur->sub2->n_frames;
	dec_tur->tail_length       = enc_tur->tail_length;
	dec_tur->sub1->tail_length = enc_tur->sub1->tail_length;
	dec_tur->sub2->tail_length = enc_tur->sub2->tail_length;
	dec_tur->sub1->poly        = enc_tur->sub1->poly;
	dec_tur->sub2->poly        = enc_tur->sub2->poly;
	dec_tur->sub1->standard    = enc_tur->sub1->standard;
	dec_tur->sub2->standard    = enc_tur->sub2->standard;
	dec_tur->enable_json       =!enc_tur->json_path.empty();

	dec->store(vals);

	K           = enc->K;
	N_cw        = enc->N_cw;
	N           = pct != nullptr ? pct->N : enc->N_cw;
	tail_length = enc->tail_length;
}

void Codec_turbo::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SIHO::parameters::get_headers(headers, full);
	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
	if (pct != nullptr)
		pct->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_turbo<B,Q>* Codec_turbo::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_turbo<B,Q>(dynamic_cast<const Encoder_turbo  ::parameters<>&>(*enc),
	                                    dynamic_cast<const Decoder_turbo  ::parameters<>&>(*dec),
	                                    dynamic_cast<const Puncturer_turbo::parameters*  >(pct.get()),
	                                    crc);
}


template <typename B, typename Q>
module::Codec_turbo<B,Q>* Codec_turbo
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
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

