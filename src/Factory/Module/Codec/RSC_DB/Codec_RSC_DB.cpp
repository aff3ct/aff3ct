#include "Codec_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_RSC_DB::name   = "Codec RSC DB";
const std::string aff3ct::factory::Codec_RSC_DB::prefix = "cdc";

Codec_RSC_DB::parameters
::parameters(const std::string &prefix)
: Codec          ::parameters(Codec_RSC_DB::name, prefix),
  Codec_SISO_SIHO::parameters(Codec_RSC_DB::name, prefix),
  enc(new Encoder_RSC_DB::parameters("enc")),
  dec(new Decoder_RSC_DB::parameters("dec"))
{
	Codec::parameters::enc = enc;
	Codec::parameters::dec = dec;
}

Codec_RSC_DB::parameters* Codec_RSC_DB::parameters
::clone() const
{
	auto clone = new Codec_RSC_DB::parameters(*this);

	if (enc != nullptr) { clone->enc = enc->clone(); }
	if (dec != nullptr) { clone->dec = dec->clone(); }

	clone->set_enc(clone->enc);
	clone->set_dec(clone->dec);

	return clone;
}

Codec_RSC_DB::parameters
::~parameters()
{
	if (enc != nullptr) { delete enc; enc = nullptr; }
	if (dec != nullptr) { delete dec; dec = nullptr; }

	Codec::parameters::enc = nullptr;
	Codec::parameters::dec = nullptr;
}

void Codec_RSC_DB::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Codec_SISO_SIHO::parameters::get_description(req_args, opt_args);

	enc->get_description(req_args, opt_args);
	dec->get_description(req_args, opt_args);

	auto pdec = dec->get_prefix();

	req_args.erase({pdec+"-cw-size",   "N"});
	req_args.erase({pdec+"-info-bits", "K"});
	opt_args.erase({pdec+"-fra",       "F"});
	opt_args.erase({pdec+"-no-buff"       });
}

void Codec_RSC_DB::parameters
::store(const arg_val_map &vals)
{
	Codec_SISO_SIHO::parameters::store(vals);

	enc->store(vals);

	this->dec->K        = this->enc->K;
	this->dec->N_cw     = this->enc->N_cw;
	this->dec->n_frames = this->enc->n_frames;
	this->dec->buffered = this->enc->buffered;

	dec->store(vals);

	auto pdec = dec->get_prefix();

	if (!this->enc->standard.empty() && !exist(vals, {pdec+"-implem"}))
		this->dec->implem = this->enc->standard;

	this->K    = this->enc->K;
	this->N_cw = this->enc->N_cw;
	this->N    = this->enc->N_cw;
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
	return new module::Codec_RSC_DB<B,Q>(*enc, *dec);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
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
