#include "Codec_uncoded.hpp"

#include "Factory/Module/Decoder/NO/Decoder_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_uncoded_name   = "Codec Uncoded";
const std::string aff3ct::factory::Codec_uncoded_prefix = "cdc";

Codec_uncoded::parameters
::parameters(const std::string &prefix)
: Codec          ::parameters(Codec_uncoded_name, prefix),
  Codec_SISO_SIHO::parameters(Codec_uncoded_name, prefix)
{
	Codec::parameters::set_enc(new Encoder   ::parameters("enc"));
	Codec::parameters::set_dec(new Decoder_NO::parameters("dec"));
}

Codec_uncoded::parameters* Codec_uncoded::parameters
::clone() const
{
	return new Codec_uncoded::parameters(*this);
}

void Codec_uncoded::parameters
::get_description(tools::Argument_map_info &args) const
{
	Codec_SISO_SIHO::parameters::get_description(args);

	enc->get_description(args);
	dec->get_description(args);

	auto penc = enc->get_prefix();
	auto pdec = dec->get_prefix();

	args.erase({penc+"-type"          });
	args.erase({penc+"-cw-size",   "N"});
	args.erase({penc+"-path"          });
	args.erase({penc+"-seed",      "S"});
	args.erase({pdec+"-cw-size",   "N"});
	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-fra",       "F"});
}

void Codec_uncoded::parameters
::store(const tools::Argument_map_value &vals)
{
	Codec_SISO_SIHO::parameters::store(vals);

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

void Codec_uncoded::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SISO_SIHO::parameters::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_uncoded<B,Q>* Codec_uncoded::parameters
::build(module::CRC<B>* crc) const
{
	return new module::Codec_uncoded<B,Q>(*enc, dynamic_cast<const Decoder_NO::parameters&>(*dec));
}

template <typename B, typename Q>
module::Codec_uncoded<B,Q>* Codec_uncoded
::build(const parameters &params, module::CRC<B>* crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Codec_uncoded<B_8 ,Q_8 >* aff3ct::factory::Codec_uncoded::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_uncoded<B_16,Q_16>* aff3ct::factory::Codec_uncoded::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_uncoded<B_32,Q_32>* aff3ct::factory::Codec_uncoded::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_uncoded<B_64,Q_64>* aff3ct::factory::Codec_uncoded::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_uncoded<B_8 ,Q_8 >* aff3ct::factory::Codec_uncoded::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_uncoded::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_uncoded<B_16,Q_16>* aff3ct::factory::Codec_uncoded::build<B_16,Q_16>(const aff3ct::factory::Codec_uncoded::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_uncoded<B_32,Q_32>* aff3ct::factory::Codec_uncoded::build<B_32,Q_32>(const aff3ct::factory::Codec_uncoded::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_uncoded<B_64,Q_64>* aff3ct::factory::Codec_uncoded::build<B_64,Q_64>(const aff3ct::factory::Codec_uncoded::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_uncoded<B,Q>* aff3ct::factory::Codec_uncoded::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_uncoded<B,Q>* aff3ct::factory::Codec_uncoded::build<B,Q>(const aff3ct::factory::Codec_uncoded::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation
