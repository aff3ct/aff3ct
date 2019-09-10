#include "Factory/Module/Encoder/RA/Encoder_RA.hpp"
#include "Factory/Module/Decoder/RA/Decoder_RA.hpp"

#include "Factory/Module/Codec/RA/Codec_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_RA_name   = "Codec RA";
const std::string aff3ct::factory::Codec_RA_prefix = "cdc";

Codec_RA::parameters
::parameters(const std::string &prefix)
: Codec     ::parameters(Codec_RA_name, prefix),
  Codec_SIHO::parameters(Codec_RA_name, prefix)
{
	auto enc_t = new Encoder_RA::parameters("enc");
	auto dec_t = new Decoder_RA::parameters("dec");
	Codec::parameters::set_enc(enc_t);
	Codec::parameters::set_dec(dec_t);
	Codec::parameters::set_itl(std::move(enc_t->itl));
	dec_t->itl = nullptr;
}

Codec_RA::parameters* Codec_RA::parameters
::clone() const
{
	return new Codec_RA::parameters(*this);
}

void Codec_RA::parameters
::get_description(tools::Argument_map_info &args) const
{
	Codec_SIHO::parameters::get_description(args);

	enc->get_description(args);
	dec->get_description(args);

	auto pdec = dec->get_prefix();

	args.erase({pdec+"-cw-size",   "N"});
	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-fra",       "F"});

	if (itl != nullptr)
	{
		itl->get_description(args);

		auto pi = itl->get_prefix();

		args.erase({pi+"-size"    });
		args.erase({pi+"-fra", "F"});
	}
}

void Codec_RA::parameters
::store(const tools::Argument_map_value &vals)
{
	Codec_SIHO::parameters::store(vals);

	auto dec_ra = dynamic_cast<Decoder_RA::parameters*>(dec.get());

	enc->store(vals);

	dec_ra->K                   = enc->K;
	dec_ra->N_cw                = enc->N_cw;
	dec_ra->n_frames            = enc->n_frames;

	if (dec_ra->itl != nullptr)
		dec_ra->itl->core->n_frames = enc->n_frames;

	dec->store(vals);

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = enc->N_cw;

	if (itl != nullptr)
	{
		itl->core->size     = dec_ra->N_cw;
		itl->core->n_frames = dec_ra->n_frames;

		itl->store(vals);
	}
}

void Codec_RA::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SIHO::parameters::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
	if (itl != nullptr)
		itl->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_RA<B,Q>* Codec_RA::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_RA<B,Q>(dynamic_cast<const Encoder_RA::parameters&>(*enc),
	                                 dynamic_cast<const Decoder_RA::parameters&>(*dec),
	                                 *itl);
}

template <typename B, typename Q>
module::Codec_RA<B,Q>* Codec_RA
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Codec_RA<B_8 ,Q_8 >* aff3ct::factory::Codec_RA::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_RA<B_16,Q_16>* aff3ct::factory::Codec_RA::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_RA<B_32,Q_32>* aff3ct::factory::Codec_RA::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_RA<B_64,Q_64>* aff3ct::factory::Codec_RA::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_RA<B_8 ,Q_8 >* aff3ct::factory::Codec_RA::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_RA::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_RA<B_16,Q_16>* aff3ct::factory::Codec_RA::build<B_16,Q_16>(const aff3ct::factory::Codec_RA::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_RA<B_32,Q_32>* aff3ct::factory::Codec_RA::build<B_32,Q_32>(const aff3ct::factory::Codec_RA::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_RA<B_64,Q_64>* aff3ct::factory::Codec_RA::build<B_64,Q_64>(const aff3ct::factory::Codec_RA::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_RA<B,Q>* aff3ct::factory::Codec_RA::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_RA<B,Q>* aff3ct::factory::Codec_RA::build<B,Q>(const aff3ct::factory::Codec_RA::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation

