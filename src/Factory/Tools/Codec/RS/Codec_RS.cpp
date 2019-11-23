#include <utility>

#include "Factory/Module/Encoder/RS/Encoder_RS.hpp"
#include "Factory/Module/Decoder/RS/Decoder_RS.hpp"
#include "Factory/Tools/Codec/RS/Codec_RS.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_RS_name   = "Codec RS";
const std::string aff3ct::factory::Codec_RS_prefix = "cdc";

Codec_RS
::Codec_RS(const std::string &prefix)
: Codec_SIHO(Codec_RS_name, prefix)
{
	Codec::set_enc(new Encoder_RS("enc"));
	Codec::set_dec(new Decoder_RS("dec"));
}

Codec_RS* Codec_RS
::clone() const
{
	return new Codec_RS(*this);
}

void Codec_RS
::get_description(cli::Argument_map_info &args) const
{
	Codec_SIHO::get_description(args);

	enc->get_description(args);
	dec->get_description(args);

	auto pdec = dec->get_prefix();
	auto penc = enc->get_prefix();

	args.erase({pdec+"-cw-size",   "N"});
	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-fra",       "F"});
	args.erase({pdec+"-no-sys"        });

	args.add_link({pdec+"-corr-pow", "T"}, {penc+"-info-bits", "K"});
}

void Codec_RS
::store(const cli::Argument_map_value &vals)
{
	Codec_SIHO::store(vals);

	auto dec_rs = dynamic_cast<Decoder_RS*>(dec.get());

	enc->store(vals);

	dec->K        = enc->K;
	dec->N_cw     = enc->N_cw;
	dec->n_frames = enc->n_frames;

	dec->store(vals);

	if(dec->K != enc->K) // when -T has been given but not -K
		enc->K = dec->K;

	K    = enc->K    * dec_rs->m;
	N_cw = enc->N_cw * dec_rs->m;
	N    = enc->N_cw * dec_rs->m;
}

void Codec_RS
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Codec_SIHO::get_headers(headers, full);

	auto p = get_prefix();

	headers[p].push_back(std::make_pair("Symbols Source size",   std::to_string(enc->K   )));
	headers[p].push_back(std::make_pair("Symbols Codeword size", std::to_string(enc->N_cw)));

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
tools::Codec_RS<B,Q>* Codec_RS
::build(module::CRC<B> *crc) const
{
	return new tools::Codec_RS<B,Q>(dynamic_cast<const Encoder_RS&>(*enc),
	                                dynamic_cast<const Decoder_RS&>(*dec));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Codec_RS<B_8 ,Q_8 >* aff3ct::factory::Codec_RS::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::tools::Codec_RS<B_16,Q_16>* aff3ct::factory::Codec_RS::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::tools::Codec_RS<B_32,Q_32>* aff3ct::factory::Codec_RS::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::tools::Codec_RS<B_64,Q_64>* aff3ct::factory::Codec_RS::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::tools::Codec_RS<B,Q>* aff3ct::factory::Codec_RS::build<B,Q>(aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation

