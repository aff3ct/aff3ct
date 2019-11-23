#include "Factory/Module/Encoder/Repetition/Encoder_repetition.hpp"
#include "Factory/Module/Decoder/Repetition/Decoder_repetition.hpp"
#include "Factory/Tools/Codec/Repetition/Codec_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_repetition_name   = "Codec Repetition";
const std::string aff3ct::factory::Codec_repetition_prefix = "cdc";

Codec_repetition
::Codec_repetition(const std::string &prefix)
: Codec_SIHO(Codec_repetition_name, prefix)
{
	Codec::set_enc(new Encoder_repetition("enc"));
	Codec::set_dec(new Decoder_repetition("dec"));
}

Codec_repetition* Codec_repetition
::clone() const
{
	return new Codec_repetition(*this);
}

void Codec_repetition
::get_description(cli::Argument_map_info &args) const
{
	Codec_SIHO::get_description(args);

	enc->get_description(args);
	dec->get_description(args);

	auto pdec = dec->get_prefix();

	args.erase({pdec+"-cw-size",   "N"});
	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-no-buff"       });
	args.erase({pdec+"-fra",       "F"});
}

void Codec_repetition
::store(const cli::Argument_map_value &vals)
{
	Codec_SIHO::store(vals);

	auto enc_r = dynamic_cast<Encoder_repetition*>(enc.get());
	auto dec_r = dynamic_cast<Decoder_repetition*>(dec.get());

	enc->store(vals);

	dec_r->K        = enc_r->K;
	dec_r->N_cw     = enc_r->N_cw;
	dec_r->buffered = enc_r->buffered;
	dec_r->n_frames = enc_r->n_frames;

	dec->store(vals);

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = enc->N_cw;
}

void Codec_repetition
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Codec_SIHO::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
tools::Codec_repetition<B,Q>* Codec_repetition
::build(module::CRC<B> *crc) const
{
	return new tools::Codec_repetition<B,Q>(dynamic_cast<const Encoder_repetition&>(*enc),
	                                        dynamic_cast<const Decoder_repetition&>(*dec));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Codec_repetition<B_8 ,Q_8 >* aff3ct::factory::Codec_repetition::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::tools::Codec_repetition<B_16,Q_16>* aff3ct::factory::Codec_repetition::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::tools::Codec_repetition<B_32,Q_32>* aff3ct::factory::Codec_repetition::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::tools::Codec_repetition<B_64,Q_64>* aff3ct::factory::Codec_repetition::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::tools::Codec_repetition<B,Q>* aff3ct::factory::Codec_repetition::build<B,Q>(aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
