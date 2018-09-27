#include "Codec_RSC_DB.hpp"

#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Decoder/RSC_DB/Decoder_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_RSC_DB_name   = "Codec RSC DB";
const std::string aff3ct::factory::Codec_RSC_DB_prefix = "cdc";

Codec_RSC_DB::parameters
::parameters(const std::string &prefix)
: Codec          ::parameters(Codec_RSC_DB_name, prefix),
  Codec_SISO_SIHO::parameters(Codec_RSC_DB_name, prefix)
{
	Codec::parameters::set_enc(new Encoder_RSC_DB::parameters("enc"));
	Codec::parameters::set_dec(new Decoder_RSC_DB::parameters("dec"));
}

Codec_RSC_DB::parameters* Codec_RSC_DB::parameters
::clone() const
{
	return new Codec_RSC_DB::parameters(*this);
}

void Codec_RSC_DB::parameters
::get_description(tools::Argument_map_info &args) const
{
	Codec_SISO_SIHO::parameters::get_description(args);

	enc->get_description(args);
	dec->get_description(args);

	auto pdec = dec->get_prefix();

	args.erase({pdec+"-cw-size",   "N"});
	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-fra",       "F"});
	args.erase({pdec+"-no-buff"       });
}

void Codec_RSC_DB::parameters
::store(const tools::Argument_map_value &vals)
{
	Codec_SISO_SIHO::parameters::store(vals);

	auto enc_rsc = dynamic_cast<Encoder_RSC_DB::parameters*>(enc.get());
	auto dec_rsc = dynamic_cast<Decoder_RSC_DB::parameters*>(dec.get());

	enc->store(vals);

	dec_rsc->K        = enc_rsc->K;
	dec_rsc->N_cw     = enc_rsc->N_cw;
	dec_rsc->n_frames = enc_rsc->n_frames;
	dec_rsc->buffered = enc_rsc->buffered;

	dec->store(vals);

	auto pdec = dec->get_prefix();

	if (!enc_rsc->standard.empty() && !vals.exist({pdec+"-implem"}))
		dec->implem = enc_rsc->standard;

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = enc->N_cw;
}

void Codec_RSC_DB::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SISO_SIHO::parameters::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_RSC_DB<B,Q>* Codec_RSC_DB::parameters
::build(module::CRC<B>* crc) const
{
	return new module::Codec_RSC_DB<B,Q>(dynamic_cast<const Encoder_RSC_DB::parameters&>(*enc),
	                                      dynamic_cast<const Decoder_RSC_DB::parameters&>(*dec));
}

template <typename B, typename Q>
module::Codec_RSC_DB<B,Q>* Codec_RSC_DB
::build(const parameters &params, module::CRC<B>* crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_RSC_DB<B_8 ,Q_8 >* aff3ct::factory::Codec_RSC_DB::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_RSC_DB<B_16,Q_16>* aff3ct::factory::Codec_RSC_DB::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_RSC_DB<B_32,Q_32>* aff3ct::factory::Codec_RSC_DB::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_RSC_DB<B_64,Q_64>* aff3ct::factory::Codec_RSC_DB::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_RSC_DB<B_8 ,Q_8 >* aff3ct::factory::Codec_RSC_DB::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_RSC_DB::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_RSC_DB<B_16,Q_16>* aff3ct::factory::Codec_RSC_DB::build<B_16,Q_16>(const aff3ct::factory::Codec_RSC_DB::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_RSC_DB<B_32,Q_32>* aff3ct::factory::Codec_RSC_DB::build<B_32,Q_32>(const aff3ct::factory::Codec_RSC_DB::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_RSC_DB<B_64,Q_64>* aff3ct::factory::Codec_RSC_DB::build<B_64,Q_64>(const aff3ct::factory::Codec_RSC_DB::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_RSC_DB<B,Q>* aff3ct::factory::Codec_RSC_DB::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_RSC_DB<B,Q>* aff3ct::factory::Codec_RSC_DB::build<B,Q>(const aff3ct::factory::Codec_RSC_DB::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation
