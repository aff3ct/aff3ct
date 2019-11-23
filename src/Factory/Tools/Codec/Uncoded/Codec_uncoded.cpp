#include "Factory/Module/Encoder/NO/Encoder_NO.hpp"
#include "Factory/Module/Decoder/NO/Decoder_NO.hpp"
#include "Factory/Tools/Codec/Uncoded/Codec_uncoded.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_uncoded_name   = "Codec Uncoded";
const std::string aff3ct::factory::Codec_uncoded_prefix = "cdc";

Codec_uncoded
::Codec_uncoded(const std::string &prefix)
: Codec_SISO(Codec_uncoded_name, prefix)
{
	Codec::set_enc(new Encoder_NO("enc"));
	Codec::set_dec(new Decoder_NO("dec"));
}

Codec_uncoded* Codec_uncoded
::clone() const
{
	return new Codec_uncoded(*this);
}

void Codec_uncoded
::get_description(cli::Argument_map_info &args) const
{
	Codec_SISO::get_description(args);

	enc->get_description(args);
	dec->get_description(args);

	auto penc = enc->get_prefix();
	auto pdec = dec->get_prefix();

	args.erase({penc+"-type"          });
	args.erase({pdec+"-cw-size",   "N"});
	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-fra",       "F"});
}

void Codec_uncoded
::store(const cli::Argument_map_value &vals)
{
	Codec_SISO::store(vals);

	enc->type = "NO";

	enc->store(vals);

	enc->N_cw     = enc->K;

	dec->type     = "NONE";
	dec->implem   = "HARD_DECISION";
	dec->K        = enc->K;
	dec->N_cw     = enc->N_cw;
	dec->n_frames = enc->n_frames;

	dec->store(vals);

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = enc->N_cw;
}

void Codec_uncoded
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Codec_SISO::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
tools::Codec_uncoded<B,Q>* Codec_uncoded
::build(module::CRC<B>* crc) const
{
	return new tools::Codec_uncoded<B,Q>(dynamic_cast<const Encoder_NO&>(*enc),
	                                     dynamic_cast<const Decoder_NO&>(*dec));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Codec_uncoded<B_8 ,Q_8 >* aff3ct::factory::Codec_uncoded::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::tools::Codec_uncoded<B_16,Q_16>* aff3ct::factory::Codec_uncoded::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::tools::Codec_uncoded<B_32,Q_32>* aff3ct::factory::Codec_uncoded::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::tools::Codec_uncoded<B_64,Q_64>* aff3ct::factory::Codec_uncoded::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::tools::Codec_uncoded<B,Q>* aff3ct::factory::Codec_uncoded::build<B,Q>(aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
