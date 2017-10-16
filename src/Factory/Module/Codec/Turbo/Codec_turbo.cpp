#include "Codec_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_turbo::name   = "Codec Turbo";
const std::string aff3ct::factory::Codec_turbo::prefix = "cdc";

Codec_turbo::parameters
::parameters(const std::string prefix)
: Codec     ::parameters(Codec_turbo::name, prefix),
  Codec_SIHO::parameters(Codec_turbo::name, prefix),
  enc(new Encoder_turbo::parameters<>("enc")),
  dec(new Decoder_turbo ::parameters<>("dec")),
  pct(nullptr)
{
	Codec::parameters::enc = enc;
	Codec::parameters::dec = dec;
	Codec::parameters::itl = enc->itl;
	delete dec->itl; dec->itl = enc->itl;
}

Codec_turbo::parameters
::~parameters()
{
	if (enc != nullptr) { enc->itl = nullptr; delete enc; enc = nullptr; }
	if (dec != nullptr) { dec->itl = nullptr; delete dec; dec = nullptr; }
	if (pct != nullptr) {                     delete pct; pct = nullptr; }

	Codec::parameters::enc = nullptr;
	Codec::parameters::dec = nullptr;
	if (Codec::parameters::itl != nullptr)
	{
		delete Codec::parameters::itl;
		Codec::parameters::itl = nullptr;
	}
}

Codec_turbo::parameters* Codec_turbo::parameters
::clone() const
{
	auto clone = new Codec_turbo::parameters(*this);

	if (enc != nullptr) { clone->enc = enc->clone(); }
	if (dec != nullptr) { clone->dec = dec->clone(); }
	if (pct != nullptr) { clone->pct = pct->clone(); }

	clone->set_enc(clone->enc);
	clone->set_dec(clone->dec);
	clone->set_itl(clone->enc->itl);
	delete clone->dec->itl; clone->dec->itl = clone->enc->itl;

	return clone;
}

void Codec_turbo::parameters
::enable_puncturer()
{
	this->pct = new Puncturer_turbo::parameters("pct");
}

std::vector<std::string> Codec_turbo::parameters
::get_names() const
{
	auto n = Codec::parameters::get_names();
	if (pct != nullptr) { auto nn = pct->get_names(); for (auto &x : nn) n.push_back(x); }
	return n;
}

std::vector<std::string> Codec_turbo::parameters
::get_short_names() const
{
	auto sn = Codec::parameters::get_short_names();
	if (pct != nullptr) { auto nn = pct->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn;
}

std::vector<std::string> Codec_turbo::parameters
::get_prefixes() const
{
	auto p = Codec::parameters::get_prefixes();
	if (pct != nullptr) { auto nn = pct->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p;
}

void Codec_turbo::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Codec_SIHO::parameters::get_description(req_args, opt_args);

	if (this->pct)
	{
		pct->get_description(req_args, opt_args);

		auto ppct = pct->get_prefix();

		req_args.erase({ppct+"-info-bits", "K"});
		opt_args.erase({ppct+"-no-buff"       });
		opt_args.erase({ppct+"-fra",       "F"});
		opt_args.erase({ppct+"-tail-length"   });
	}

	enc->get_description(req_args, opt_args);
	dec->get_description(req_args, opt_args);

	auto pdec = dec->get_prefix();
	auto pdes = dec->sub1->get_prefix();

	req_args.erase({pdec+"-cw-size",   "N"});
	req_args.erase({pdec+"-info-bits", "K"});
	opt_args.erase({pdec+"-fra",       "F"});
	opt_args.erase({pdes+"-no-buff"       });
	opt_args.erase({pdes+"-poly"          });
	opt_args.erase({pdes+"-std"           });
	opt_args.erase({pdec+"-json"          });
}

void Codec_turbo::parameters
::store(const arg_val_map &vals)
{
	Codec_SIHO::parameters::store(vals);

	enc->store(vals);

	if (this->pct)
	{
		this->pct->K           = this->enc->K;
		this->pct->N_cw        = this->enc->N_cw;
		this->pct->buffered    = this->enc->sub1->buffered;
		this->pct->n_frames    = this->enc->n_frames;
		this->pct->tail_length = this->enc->tail_length;

		pct->store(vals);
	}

	this->dec->K                 = this->enc->K;
	this->dec->N_cw              = this->enc->N_cw;
	this->dec->sub1->buffered    = this->enc->sub1->buffered;
	this->dec->sub2->buffered    = this->enc->sub2->buffered;
	this->dec->n_frames          = this->enc->n_frames;
	this->dec->sub1->n_frames    = this->enc->sub1->n_frames;
	this->dec->sub2->n_frames    = this->enc->sub2->n_frames;
	this->dec->tail_length       = this->enc->tail_length;
	this->dec->sub1->tail_length = this->enc->sub1->tail_length;
	this->dec->sub2->tail_length = this->enc->sub2->tail_length;
	this->dec->sub1->poly        = this->enc->sub1->poly;
	this->dec->sub2->poly        = this->enc->sub2->poly;
	this->dec->sub1->standard    = this->enc->sub1->standard;
	this->dec->sub2->standard    = this->enc->sub2->standard;
	this->dec->enable_json       = !this->enc->json_path.empty();

	dec->store(vals);

	this->K           = this->enc->K;
	this->N_cw        = this->enc->N_cw;
	this->N           = this->pct ? this->pct->N : this->enc->N_cw;
	this->tail_length = this->enc->tail_length;
}

void Codec_turbo::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SIHO::parameters::get_headers(headers, full);
	enc->get_headers(headers, full);
	if (this->pct)
	pct->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_turbo<B,Q>* Codec_turbo::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_turbo<B,Q>(*enc, *dec, pct, crc);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}


template <typename B, typename Q>
module::Codec_turbo<B,Q>* Codec_turbo
::build(const parameters &params, module::CRC<B> *crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_turbo<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_turbo<B_16,Q_16>* aff3ct::factory::Codec_turbo::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_turbo<B_32,Q_32>* aff3ct::factory::Codec_turbo::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_turbo<B_64,Q_64>* aff3ct::factory::Codec_turbo::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_turbo<B_8 ,Q_8 >* aff3ct::factory::Codec_turbo::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_turbo<B_16,Q_16>* aff3ct::factory::Codec_turbo::build<B_16,Q_16>(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_turbo<B_32,Q_32>* aff3ct::factory::Codec_turbo::build<B_32,Q_32>(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_turbo<B_64,Q_64>* aff3ct::factory::Codec_turbo::build<B_64,Q_64>(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_turbo<B,Q>* aff3ct::factory::Codec_turbo::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_turbo<B,Q>* aff3ct::factory::Codec_turbo::build<B,Q>(const aff3ct::factory::Codec_turbo::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation

