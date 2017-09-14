#include "Codec_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_polar::name   = "Codec Polar";
const std::string aff3ct::factory::Codec_polar::prefix = "cdc";

Codec_polar::parameters
::parameters(const std::string prefix)
: Codec          ::parameters(Codec_polar::name, prefix),
  Codec_SISO_SIHO::parameters(Codec_polar::name, prefix),
  enc(new Encoder_polar::parameters("enc")),
  fbg(new Frozenbits_generator::parameters(enc->get_prefix()+"-fb")),
  dec(new Decoder_polar::parameters("dec")),
  pct(new Puncturer_polar::parameters("pct"))
{
	Codec::parameters::enc = enc;
	Codec::parameters::dec = dec;
}

Codec_polar::parameters
::~parameters()
{
	if (enc != nullptr) { delete enc; enc = nullptr; }
	if (dec != nullptr) { delete dec; dec = nullptr; }
	if (fbg != nullptr) { delete fbg; fbg = nullptr; }
	if (pct != nullptr) { delete pct; pct = nullptr; }

	Codec::parameters::enc = nullptr;
	Codec::parameters::dec = nullptr;
}

Codec_polar::parameters* Codec_polar::parameters
::clone() const
{
	auto clone = new Codec_polar::parameters(*this);

	if (enc != nullptr) { clone->enc = enc->clone(); }
	if (dec != nullptr) { clone->dec = dec->clone(); }
	if (fbg != nullptr) { clone->fbg = fbg->clone(); }
	if (pct != nullptr) { clone->pct = pct->clone(); }

	clone->set_enc(clone->enc);
	clone->set_dec(clone->dec);

	return clone;
}

void Codec_polar::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Codec_SISO_SIHO::parameters::get_description(req_args, opt_args);

	pct->get_description(req_args, opt_args);
	enc->get_description(req_args, opt_args);
	fbg->get_description(req_args, opt_args);
	dec->get_description(req_args, opt_args);

	auto penc = enc->get_prefix();
	auto pdec = dec->get_prefix();
	auto pfbg = fbg->get_prefix();

	req_args.erase({penc+"-cw-size",   "N"});
	req_args.erase({penc+"-info-bits", "K"});
	opt_args.erase({penc+"-fra",       "F"});
	req_args.erase({penc+"-cw-size",   "N"});
	req_args.erase({pdec+"-info-bits", "K"});
	opt_args.erase({pdec+"-fra",       "F"});
	opt_args.erase({pdec+"-no-sys"        });
	req_args.erase({pfbg+"-cw-size",   "N"});
	req_args.erase({pfbg+"-info-bits", "K"});
}

void Codec_polar::parameters
::store(const arg_val_map &vals)
{
	Codec_SISO_SIHO::parameters::store(vals);

	pct->store(vals);

	this->enc->K        = this->fbg->K    = this->dec->K        = this->pct->K;
	this->enc->N_cw     = this->fbg->N_cw = this->dec->N_cw     = this->pct->N_cw;
	this->enc->n_frames                   = this->dec->n_frames = this->pct->n_frames;

	enc->store(vals);
	fbg->store(vals);

	this->dec->systematic = this->enc->systematic;

	dec->store(vals);

	this->K    = this->pct->K;
	this->N_cw = this->pct->N_cw;
	this->N    = this->pct->N;
}

void Codec_polar::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SISO_SIHO::parameters::get_headers(headers, full);

	pct->get_headers(headers, full);
	enc->get_headers(headers, full);
	fbg->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_polar<B,Q>* Codec_polar::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_polar<B,Q>(*fbg, *enc, *dec, *pct, crc);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Codec_polar<B,Q>* Codec_polar
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_polar<B_8 ,Q_8 >* aff3ct::factory::Codec_polar::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_polar<B_16,Q_16>* aff3ct::factory::Codec_polar::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_polar<B_32,Q_32>* aff3ct::factory::Codec_polar::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_polar<B_64,Q_64>* aff3ct::factory::Codec_polar::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_polar<B_8 ,Q_8 >* aff3ct::factory::Codec_polar::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_polar<B_16,Q_16>* aff3ct::factory::Codec_polar::build<B_16,Q_16>(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_polar<B_32,Q_32>* aff3ct::factory::Codec_polar::build<B_32,Q_32>(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_polar<B_64,Q_64>* aff3ct::factory::Codec_polar::build<B_64,Q_64>(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_polar<B,Q>* aff3ct::factory::Codec_polar::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_polar<B,Q>* aff3ct::factory::Codec_polar::build<B,Q>(const aff3ct::factory::Codec_polar::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation
