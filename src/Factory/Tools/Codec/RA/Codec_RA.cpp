#include "Factory/Module/Encoder/RA/Encoder_RA.hpp"
#include "Factory/Module/Decoder/RA/Decoder_RA.hpp"
#include "Factory/Tools/Codec/RA/Codec_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_RA_name   = "Codec RA";
const std::string aff3ct::factory::Codec_RA_prefix = "cdc";

Codec_RA
::Codec_RA(const std::string &prefix)
: Codec_SIHO(Codec_RA_name, prefix)
{
	auto enc_t = new Encoder_RA("enc");
	auto dec_t = new Decoder_RA("dec");
	Codec::set_enc(enc_t);
	Codec::set_dec(dec_t);
	Codec::set_itl(std::move(enc_t->itl));
	dec_t->itl = nullptr;
}

Codec_RA* Codec_RA
::clone() const
{
	return new Codec_RA(*this);
}

void Codec_RA
::get_description(cli::Argument_map_info &args) const
{
	Codec_SIHO::get_description(args);

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

void Codec_RA
::store(const cli::Argument_map_value &vals)
{
	Codec_SIHO::store(vals);

	auto dec_ra = dynamic_cast<Decoder_RA*>(dec.get());

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

void Codec_RA
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Codec_SIHO::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
	if (itl != nullptr)
		itl->get_headers(headers, full);
}

template <typename B, typename Q>
tools::Codec_RA<B,Q>* Codec_RA
::build(const module::CRC<B> *crc) const
{
	return new tools::Codec_RA<B,Q>(dynamic_cast<const Encoder_RA&>(*enc),
	                                dynamic_cast<const Decoder_RA&>(*dec),
	                                *itl);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Codec_RA<B_8 ,Q_8 >* aff3ct::factory::Codec_RA::build<B_8 ,Q_8 >(const aff3ct::module::CRC<B_8 >*) const;
template aff3ct::tools::Codec_RA<B_16,Q_16>* aff3ct::factory::Codec_RA::build<B_16,Q_16>(const aff3ct::module::CRC<B_16>*) const;
template aff3ct::tools::Codec_RA<B_32,Q_32>* aff3ct::factory::Codec_RA::build<B_32,Q_32>(const aff3ct::module::CRC<B_32>*) const;
template aff3ct::tools::Codec_RA<B_64,Q_64>* aff3ct::factory::Codec_RA::build<B_64,Q_64>(const aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::tools::Codec_RA<B,Q>* aff3ct::factory::Codec_RA::build<B,Q>(const aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation

