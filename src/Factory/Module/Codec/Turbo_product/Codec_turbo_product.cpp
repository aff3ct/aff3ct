#include <algorithm>

#include "Factory/Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"
#include "Factory/Module/Decoder/Turbo_product/Decoder_turbo_product.hpp"
#include "Factory/Module/Codec/Turbo_product/Codec_turbo_product.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_turbo_product_name   = "Codec Turbo Prod";
const std::string aff3ct::factory::Codec_turbo_product_prefix = "cdc";

Codec_turbo_product::parameters
::parameters(const std::string &prefix)
: Codec          ::parameters(Codec_turbo_product_name, prefix),
  Codec_SISO_SIHO::parameters(Codec_turbo_product_name, prefix)
{
	auto enc_t = new Encoder_turbo_product::parameters("enc");
	auto dec_t = new Decoder_turbo_product::parameters("dec");
	Codec::parameters::set_enc(enc_t);
	Codec::parameters::set_dec(dec_t);
	Codec::parameters::set_itl(std::move(enc_t->itl));
	dec_t->itl = nullptr;
}

Codec_turbo_product::parameters* Codec_turbo_product::parameters
::clone() const
{
	return new Codec_turbo_product::parameters(*this);
}

void Codec_turbo_product::parameters
::get_description(cli::Argument_map_info &args) const
{
	Codec_SIHO::parameters::get_description(args);

	auto dec_tur = dynamic_cast<Decoder_turbo_product::parameters*>(dec.get());

	enc->get_description(args);
	dec->get_description(args);

	auto pdec = dec->get_prefix();
	auto pdes = dec_tur->get_prefix();

	args.erase({pdes+"-cw-size",   "N"});
	args.erase({pdes+"-info-bits", "K"});
	args.erase({pdec+"-fra",       "F"});
	args.erase({pdec+"-ext",          });


	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}
}

void Codec_turbo_product::parameters
::store(const cli::Argument_map_value &vals)
{
	Codec_SIHO::parameters::store(vals);

	auto enc_tur = dynamic_cast<Encoder_turbo_product::parameters*>(enc.get());
	auto dec_tur = dynamic_cast<Decoder_turbo_product::parameters*>(dec.get());

	enc->store(vals);

	dec_tur->sub->K           = enc_tur->sub->K;
	dec_tur->sub->N_cw        = enc_tur->sub->N_cw;
	dec_tur->n_frames         = enc_tur->n_frames;
	dec_tur->parity_extended  = enc_tur->parity_extended;

	dec->store(vals);

	if (itl != nullptr)
	{
		itl->core->n_frames = enc_tur->n_frames;
		itl->core->type     = "ROW_COL";

		if (enc_tur->parity_extended)
			itl->core->n_cols = enc_tur->sub->N_cw +1;
		else
			itl->core->n_cols = enc_tur->sub->N_cw;

		itl->core->size = itl->core->n_cols * itl->core->n_cols;
		enc->N_cw = itl->core->size;
		dec->N_cw = itl->core->size;

		itl->store(vals);
	}

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = enc->N_cw;
}

void Codec_turbo_product::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SIHO::parameters::get_headers(headers, full);
	enc->get_headers(headers, full);
	dec->get_headers(headers, full);

	if (itl != nullptr)
		itl->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_turbo_product<B,Q>* Codec_turbo_product::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_turbo_product<B,Q>(dynamic_cast<const Encoder_turbo_product::parameters&>(*enc),
	                                            dynamic_cast<const Decoder_turbo_product::parameters&>(*dec),
	                                            *itl);
}


template <typename B, typename Q>
module::Codec_turbo_product<B,Q>* Codec_turbo_product
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Codec_turbo_product<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo_product::parameters::build<B_8 ,Q_8 >(module::CRC<B_8 > *) const;
template aff3ct::module::Codec_turbo_product<B_16,Q_16>* aff3ct::factory::Codec_turbo_product::parameters::build<B_16,Q_16>(module::CRC<B_16> *) const;
template aff3ct::module::Codec_turbo_product<B_32,Q_32>* aff3ct::factory::Codec_turbo_product::parameters::build<B_32,Q_32>(module::CRC<B_32> *) const;
template aff3ct::module::Codec_turbo_product<B_64,Q_64>* aff3ct::factory::Codec_turbo_product::parameters::build<B_64,Q_64>(module::CRC<B_64> *) const;
template aff3ct::module::Codec_turbo_product<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo_product::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_turbo_product::parameters&, module::CRC<B_8 > *);
template aff3ct::module::Codec_turbo_product<B_16,Q_16>* aff3ct::factory::Codec_turbo_product::build<B_16,Q_16>(const aff3ct::factory::Codec_turbo_product::parameters&, module::CRC<B_16> *);
template aff3ct::module::Codec_turbo_product<B_32,Q_32>* aff3ct::factory::Codec_turbo_product::build<B_32,Q_32>(const aff3ct::factory::Codec_turbo_product::parameters&, module::CRC<B_32> *);
template aff3ct::module::Codec_turbo_product<B_64,Q_64>* aff3ct::factory::Codec_turbo_product::build<B_64,Q_64>(const aff3ct::factory::Codec_turbo_product::parameters&, module::CRC<B_64> *);
#else
template aff3ct::module::Codec_turbo_product<B,Q>* aff3ct::factory::Codec_turbo_product::parameters::build<B,Q>(module::CRC<B> *) const;
template aff3ct::module::Codec_turbo_product<B,Q>* aff3ct::factory::Codec_turbo_product::build<B,Q>(const aff3ct::factory::Codec_turbo_product::parameters&, module::CRC<B> *);
#endif
// ==================================================================================== explicit template instantiation

