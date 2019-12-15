#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Factory/Tools/Codec/BCH/Codec_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_BCH_name   = "Codec BCH";
const std::string aff3ct::factory::Codec_BCH_prefix = "cdc";

Codec_BCH
::Codec_BCH(const std::string &prefix)
: Codec_SIHO(Codec_BCH_name, prefix)
{
	Codec::set_enc(new Encoder_BCH("enc"));
	Codec::set_dec(new Decoder_BCH("dec"));
}

Codec_BCH* Codec_BCH
::clone() const
{
	return new Codec_BCH(*this);
}

void Codec_BCH
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

void Codec_BCH
::store(const cli::Argument_map_value &vals)
{
	Codec_SIHO::store(vals);

	enc->store(vals);

	dec->K        = enc->K;
	dec->N_cw     = enc->N_cw;
	dec->n_frames = enc->n_frames;

	dec->store(vals);

	if(dec->K != enc->K) // when -T has been given but not -K
		enc->K = dec->K;

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = enc->N_cw;
}

void Codec_BCH
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Codec_SIHO::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
tools::Codec_BCH<B,Q>* Codec_BCH
::build(const module::CRC<B> *crc) const
{
	return new tools::Codec_BCH<B,Q>(dynamic_cast<const Encoder_BCH&>(*enc),
	                                 dynamic_cast<const Decoder_BCH&>(*dec));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Codec_BCH<B_8 ,Q_8 >* aff3ct::factory::Codec_BCH::build<B_8 ,Q_8 >(const aff3ct::module::CRC<B_8 >*) const;
template aff3ct::tools::Codec_BCH<B_16,Q_16>* aff3ct::factory::Codec_BCH::build<B_16,Q_16>(const aff3ct::module::CRC<B_16>*) const;
template aff3ct::tools::Codec_BCH<B_32,Q_32>* aff3ct::factory::Codec_BCH::build<B_32,Q_32>(const aff3ct::module::CRC<B_32>*) const;
template aff3ct::tools::Codec_BCH<B_64,Q_64>* aff3ct::factory::Codec_BCH::build<B_64,Q_64>(const aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::tools::Codec_BCH<B,Q>* aff3ct::factory::Codec_BCH::build<B,Q>(const aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
