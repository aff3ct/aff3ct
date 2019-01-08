#include "Codec_BCH.hpp"

#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_BCH_name   = "Codec BCH";
const std::string aff3ct::factory::Codec_BCH_prefix = "cdc";

Codec_BCH::parameters
::parameters(const std::string &prefix)
: Codec          ::parameters(Codec_BCH_name, prefix),
  Codec_SIHO_HIHO::parameters(Codec_BCH_name, prefix)
{
	Codec::parameters::set_enc(new Encoder_BCH::parameters("enc"));
	Codec::parameters::set_dec(new Decoder_BCH::parameters("dec"));
}

Codec_BCH::parameters* Codec_BCH::parameters
::clone() const
{
	return new Codec_BCH::parameters(*this);
}

void Codec_BCH::parameters
::get_description(tools::Argument_map_info &args) const
{
	Codec_SIHO_HIHO::parameters::get_description(args);

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

void Codec_BCH::parameters
::store(const tools::Argument_map_value &vals)
{
	Codec_SIHO_HIHO::parameters::store(vals);

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

void Codec_BCH::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SIHO_HIHO::parameters::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_BCH<B,Q>* Codec_BCH::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_BCH<B,Q>(dynamic_cast<const Encoder_BCH::parameters&>(*enc),
	                                  dynamic_cast<const Decoder_BCH::parameters&>(*dec));
}

template <typename B, typename Q>
module::Codec_BCH<B,Q>* Codec_BCH
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Codec_BCH<B_8 ,Q_8 >* aff3ct::factory::Codec_BCH::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_BCH<B_16,Q_16>* aff3ct::factory::Codec_BCH::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_BCH<B_32,Q_32>* aff3ct::factory::Codec_BCH::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_BCH<B_64,Q_64>* aff3ct::factory::Codec_BCH::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_BCH<B_8 ,Q_8 >* aff3ct::factory::Codec_BCH::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_BCH::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_BCH<B_16,Q_16>* aff3ct::factory::Codec_BCH::build<B_16,Q_16>(const aff3ct::factory::Codec_BCH::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_BCH<B_32,Q_32>* aff3ct::factory::Codec_BCH::build<B_32,Q_32>(const aff3ct::factory::Codec_BCH::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_BCH<B_64,Q_64>* aff3ct::factory::Codec_BCH::build<B_64,Q_64>(const aff3ct::factory::Codec_BCH::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_BCH<B,Q>* aff3ct::factory::Codec_BCH::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_BCH<B,Q>* aff3ct::factory::Codec_BCH::build<B,Q>(const aff3ct::factory::Codec_BCH::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation

