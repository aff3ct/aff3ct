#include "Codec_turbo_product_code.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_turbo_product_code_name   = "Codec TPC";
const std::string aff3ct::factory::Codec_turbo_product_code_prefix = "cdc";

Codec_turbo_product_code::parameters
::parameters(const std::string &prefix)
: Codec          ::parameters(Codec_turbo_product_code_name, prefix),
  Codec_SISO_SIHO::parameters(Codec_turbo_product_code_name, prefix),
  enc(new Encoder_turbo_product_code::parameters("enc")),
  dec(new Decoder_turbo_product_code::parameters("dec"))
{
	Codec::parameters::enc = enc;
	Codec::parameters::dec = dec;
	Codec::parameters::itl = enc->itl;
	delete dec->itl; dec->itl = enc->itl;
}

Codec_turbo_product_code::parameters
::~parameters()
{
	if (enc != nullptr) { enc->itl = nullptr; delete enc; enc = nullptr; }
	if (dec != nullptr) { dec->itl = nullptr; delete dec; dec = nullptr; }

	Codec::parameters::enc = nullptr;
	Codec::parameters::dec = nullptr;
	if (Codec::parameters::itl != nullptr)
	{
		delete Codec::parameters::itl;
		Codec::parameters::itl = nullptr;
	}
}

Codec_turbo_product_code::parameters* Codec_turbo_product_code::parameters
::clone() const
{
	auto clone = new Codec_turbo_product_code::parameters(*this);

	if (enc != nullptr) { clone->enc = enc->clone(); }
	if (dec != nullptr) { clone->dec = dec->clone(); }

	clone->set_enc(clone->enc);
	clone->set_dec(clone->dec);
	clone->set_itl(clone->enc->itl);
	delete clone->dec->itl; clone->dec->itl = clone->enc->itl;

	return clone;
}

void Codec_turbo_product_code::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Codec_SIHO::parameters::get_description(req_args, opt_args);

	enc->get_description(req_args, opt_args);
	dec->get_description(req_args, opt_args);

	auto pdec = dec->get_prefix();
	auto pdes = dec->sub->get_prefix();

	req_args.erase({pdes+"-cw-size",   "N"});
	req_args.erase({pdes+"-info-bits", "K"});
	opt_args.erase({pdec+"-fra",       "F"});
	opt_args.erase({pdec+"-ext",          });
}

void Codec_turbo_product_code::parameters
::store(const arg_val_map &vals)
{
	Codec_SIHO::parameters::store(vals);

	enc->store(vals);

	this->dec->sub->K           = this->enc->sub->K;
	this->dec->sub->N_cw        = this->enc->sub->N_cw;
	this->dec->n_frames         = this->enc->n_frames;
	this->dec->parity_extended  = this->enc->parity_extended;

	dec->store(vals);

	this->K    = this->enc->K;
	this->N_cw = this->enc->N_cw;
	this->N    = this->enc->N_cw;
}

void Codec_turbo_product_code::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SIHO::parameters::get_headers(headers, full);
	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_turbo_product_code<B,Q>* Codec_turbo_product_code::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_turbo_product_code<B,Q>(*enc, *dec);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}


template <typename B, typename Q>
module::Codec_turbo_product_code<B,Q>* Codec_turbo_product_code
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_turbo_product_code<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo_product_code::parameters::build<B_8 ,Q_8 >(module::CRC<B_8 > *) const;
template aff3ct::module::Codec_turbo_product_code<B_16,Q_16>* aff3ct::factory::Codec_turbo_product_code::parameters::build<B_16,Q_16>(module::CRC<B_16> *) const;
template aff3ct::module::Codec_turbo_product_code<B_32,Q_32>* aff3ct::factory::Codec_turbo_product_code::parameters::build<B_32,Q_32>(module::CRC<B_32> *) const;
template aff3ct::module::Codec_turbo_product_code<B_64,Q_64>* aff3ct::factory::Codec_turbo_product_code::parameters::build<B_64,Q_64>(module::CRC<B_64> *) const;
template aff3ct::module::Codec_turbo_product_code<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo_product_code::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_turbo_product_code::parameters&, module::CRC<B_8 > *);
template aff3ct::module::Codec_turbo_product_code<B_16,Q_16>* aff3ct::factory::Codec_turbo_product_code::build<B_16,Q_16>(const aff3ct::factory::Codec_turbo_product_code::parameters&, module::CRC<B_16> *);
template aff3ct::module::Codec_turbo_product_code<B_32,Q_32>* aff3ct::factory::Codec_turbo_product_code::build<B_32,Q_32>(const aff3ct::factory::Codec_turbo_product_code::parameters&, module::CRC<B_32> *);
template aff3ct::module::Codec_turbo_product_code<B_64,Q_64>* aff3ct::factory::Codec_turbo_product_code::build<B_64,Q_64>(const aff3ct::factory::Codec_turbo_product_code::parameters&, module::CRC<B_64> *);
#else
template aff3ct::module::Codec_turbo_product_code<B,Q>* aff3ct::factory::Codec_turbo_product_code::parameters::build<B,Q>(module::CRC<B> *) const;
template aff3ct::module::Codec_turbo_product_code<B,Q>* aff3ct::factory::Codec_turbo_product_code::build<B,Q>(const aff3ct::factory::Codec_turbo_product_code::parameters&, module::CRC<B> *);
#endif
// ==================================================================================== explicit template instantiation

