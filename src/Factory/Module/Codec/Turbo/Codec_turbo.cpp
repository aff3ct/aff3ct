#include <algorithm>

#include "Factory/Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Factory/Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Factory/Module/Puncturer/Turbo/Puncturer_turbo.hpp"
#include "Factory/Module/Codec/Turbo/Codec_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_turbo_name   = "Codec Turbo";
const std::string aff3ct::factory::Codec_turbo_prefix = "cdc";

Codec_turbo
::Codec_turbo(const std::string &prefix)
: Codec     (Codec_turbo_name, prefix),
  Codec_SIHO(Codec_turbo_name, prefix)
{
	auto enc_t = new Encoder_turbo<>("enc");
	auto dec_t = new Decoder_turbo<>("dec");
	Codec::set_enc(enc_t);
	Codec::set_dec(dec_t);
	Codec::set_itl(std::move(enc_t->itl));
	dec_t->itl = nullptr;
}

Codec_turbo* Codec_turbo
::clone() const
{
	return new Codec_turbo(*this);
}

void Codec_turbo
::enable_puncturer()
{
	set_pct(new Puncturer_turbo("pct"));
}

void Codec_turbo
::get_description(cli::Argument_map_info &args) const
{
	Codec_SIHO::get_description(args);

	auto dec_tur = dynamic_cast<Decoder_turbo<>*>(dec.get());

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


	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}
}

void Codec_turbo
::store(const cli::Argument_map_value &vals)
{
	Codec_SIHO::store(vals);

	auto enc_tur = dynamic_cast<Encoder_turbo<>*>(enc.get());
	auto dec_tur = dynamic_cast<Decoder_turbo<>*>(dec.get());

	enc->store(vals);

	if (pct != nullptr)
	{
		auto pct_tur = dynamic_cast<Puncturer_turbo*>(pct.get());

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

	if (itl != nullptr)
	{
		itl->core->size     = enc->K;
		itl->core->n_frames = enc->n_frames;

		itl->store(vals);

		if (enc_tur->sub1->standard == "LTE" && !vals.exist({"itl-type"}))
			itl->core->type = "LTE";

		if (enc_tur->sub1->standard == "CCSDS" && !vals.exist({"itl-type"}))
			itl->core->type = "CCSDS";
	}
}

void Codec_turbo
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
module::Codec_turbo<B,Q>* Codec_turbo
::build(module::CRC<B> *crc) const
{
	return new module::Codec_turbo<B,Q>(dynamic_cast<const Encoder_turbo  <>&>(*enc),
	                                    dynamic_cast<const Decoder_turbo  <>&>(*dec),
	                                    *itl,
	                                    dynamic_cast<const Puncturer_turbo*  >(pct.get()),
	                                    crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Codec_turbo<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_turbo<B_16,Q_16>* aff3ct::factory::Codec_turbo::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_turbo<B_32,Q_32>* aff3ct::factory::Codec_turbo::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_turbo<B_64,Q_64>* aff3ct::factory::Codec_turbo::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::module::Codec_turbo<B,Q>* aff3ct::factory::Codec_turbo::build<B,Q>(aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation

