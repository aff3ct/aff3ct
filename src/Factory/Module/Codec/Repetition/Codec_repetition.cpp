#include "Codec_repetition.hpp"

#include "Factory/Module/Encoder/Repetition/Encoder_repetition.hpp"
#include "Factory/Module/Decoder/Repetition/Decoder_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_repetition_name   = "Codec Repetition";
const std::string aff3ct::factory::Codec_repetition_prefix = "cdc";

Codec_repetition::parameters
::parameters(const std::string &prefix)
: Codec     ::parameters(Codec_repetition_name, prefix),
  Codec_SIHO::parameters(Codec_repetition_name, prefix)
{
	Codec::parameters::set_enc(new Encoder_repetition::parameters("enc"));
	Codec::parameters::set_dec(new Decoder_repetition::parameters("dec"));
}

Codec_repetition::parameters* Codec_repetition::parameters
::clone() const
{
	return new Codec_repetition::parameters(*this);
}

void Codec_repetition::parameters
::get_description(tools::Argument_map_info &args) const
{
	Codec_SIHO::parameters::get_description(args);

	enc->get_description(args);
	dec->get_description(args);

	auto pdec = dec->get_prefix();

	args.erase({pdec+"-cw-size",   "N"});
	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-no-buff"       });
	args.erase({pdec+"-fra",       "F"});
}

void Codec_repetition::parameters
::store(const tools::Argument_map_value &vals)
{
	Codec_SIHO::parameters::store(vals);

	auto enc_r = dynamic_cast<Encoder_repetition::parameters*>(enc.get());
	auto dec_r = dynamic_cast<Decoder_repetition::parameters*>(dec.get());

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

void Codec_repetition::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SIHO::parameters::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_repetition<B,Q>* Codec_repetition::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_repetition<B,Q>(dynamic_cast<const Encoder_repetition::parameters&>(*enc),
	                                         dynamic_cast<const Decoder_repetition::parameters&>(*dec));
}

template <typename B, typename Q>
module::Codec_repetition<B,Q>* Codec_repetition
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_repetition<B_8 ,Q_8 >* aff3ct::factory::Codec_repetition::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_repetition<B_16,Q_16>* aff3ct::factory::Codec_repetition::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_repetition<B_32,Q_32>* aff3ct::factory::Codec_repetition::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_repetition<B_64,Q_64>* aff3ct::factory::Codec_repetition::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_repetition<B_8 ,Q_8 >* aff3ct::factory::Codec_repetition::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_repetition::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_repetition<B_16,Q_16>* aff3ct::factory::Codec_repetition::build<B_16,Q_16>(const aff3ct::factory::Codec_repetition::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_repetition<B_32,Q_32>* aff3ct::factory::Codec_repetition::build<B_32,Q_32>(const aff3ct::factory::Codec_repetition::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_repetition<B_64,Q_64>* aff3ct::factory::Codec_repetition::build<B_64,Q_64>(const aff3ct::factory::Codec_repetition::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_repetition<B,Q>* aff3ct::factory::Codec_repetition::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_repetition<B,Q>* aff3ct::factory::Codec_repetition::build<B,Q>(const aff3ct::factory::Codec_repetition::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation
