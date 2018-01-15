#include "Codec_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_polar_name   = "Codec Polar";
const std::string aff3ct::factory::Codec_polar_prefix = "cdc";

Codec_polar::parameters
::parameters(const std::string &prefix)
: Codec          ::parameters(Codec_polar_name, prefix),
  Codec_SISO_SIHO::parameters(Codec_polar_name, prefix),
  enc(new Encoder_polar::parameters("enc")),
  fbg(new Frozenbits_generator::parameters(enc->get_prefix()+"-fb")),
  dec(new Decoder_polar::parameters("dec")),
  pct(nullptr)
{
	Codec::parameters::enc = enc;
	Codec::parameters::dec = dec;
}

Codec_polar::parameters
::~parameters()
{
	if (enc != nullptr) { delete enc; enc = nullptr; }
	if (fbg != nullptr) { delete fbg; fbg = nullptr; }
	if (dec != nullptr) { delete dec; dec = nullptr; }
	if (pct != nullptr) { delete pct; pct = nullptr; }

	Codec::parameters::enc = nullptr;
	Codec::parameters::dec = nullptr;
}

Codec_polar::parameters* Codec_polar::parameters
::clone() const
{
	auto clone = new Codec_polar::parameters(*this);

	if (enc != nullptr) { clone->enc = enc->clone(); }
	if (fbg != nullptr) { clone->fbg = fbg->clone(); }
	if (dec != nullptr) { clone->dec = dec->clone(); }
	if (pct != nullptr) { clone->pct = pct->clone(); }

	clone->set_enc(clone->enc);
	clone->set_dec(clone->dec);

	return clone;
}

void Codec_polar::parameters
::enable_puncturer()
{
	this->pct = new Puncturer_polar::parameters("pct");
}

std::vector<std::string> Codec_polar::parameters
::get_names() const
{
	auto n = Codec::parameters::get_names();
	std::vector<std::string> n2;

	for (size_t i = 0; i < n.size(); i++)
	{
		n2.push_back(n[i]);
		if (enc != nullptr && n[i] == enc->get_name() && fbg != nullptr)
		{
			auto nn = fbg->get_names();
			for (auto &x : nn)
				n2.push_back(x);
		}
	}

//	if (fbg != nullptr) { auto nn = fbg->get_names(); for (auto &x : nn) n.push_back(x); }
	if (pct != nullptr) { auto nn = pct->get_names(); for (auto &x : nn) n2.push_back(x); }
	return n2;
}

std::vector<std::string> Codec_polar::parameters
::get_short_names() const
{
	auto sn = Codec::parameters::get_short_names();
	std::vector<std::string> sn2;

	for (size_t i = 0; i < sn.size(); i++)
	{
		sn2.push_back(sn[i]);
		if (enc != nullptr && sn[i] == enc->get_short_name() && fbg != nullptr)
		{
			auto nn = fbg->get_short_names();
			for (auto &x : nn)
				sn2.push_back(x);
		}
	}

//	if (fbg != nullptr) { auto nn = fbg->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	if (pct != nullptr) { auto nn = pct->get_short_names(); for (auto &x : nn) sn2.push_back(x); }
	return sn2;
}

std::vector<std::string> Codec_polar::parameters
::get_prefixes() const
{
	auto p = Codec::parameters::get_prefixes();
	std::vector<std::string> p2;

	for (size_t i = 0; i < p.size(); i++)
	{
		p2.push_back(p[i]);
		if (enc != nullptr && p[i] == enc->get_prefix() && fbg != nullptr)
		{
			auto nn = fbg->get_prefixes();
			for (auto &x : nn)
				p2.push_back(x);
		}
	}

//	if (fbg != nullptr) { auto nn = fbg->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	if (pct != nullptr) { auto nn = pct->get_prefixes(); for (auto &x : nn) p2.push_back(x); }
	return p2;
}

void Codec_polar::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Codec_SISO_SIHO::parameters::get_description(req_args, opt_args);

	enc->get_description(req_args, opt_args);
	fbg->get_description(req_args, opt_args);
	dec->get_description(req_args, opt_args);

	auto pdec = dec->get_prefix();
	auto pfbg = fbg->get_prefix();

	req_args.erase({pdec+"-info-bits", "K"});
	opt_args.erase({pdec+"-fra",       "F"});
	opt_args.erase({pdec+"-no-sys"        });
	req_args.erase({pdec+"-cw-size",   "N"});
	req_args.erase({pfbg+"-cw-size",   "N"});
	req_args.erase({pfbg+"-info-bits", "K"});

	if (this->pct)
	{
		pct->get_description(req_args, opt_args);

		auto penc = enc->get_prefix();

		req_args.erase({penc+"-cw-size",   "N"});
		req_args.erase({penc+"-info-bits", "K"});
		opt_args.erase({penc+"-fra",       "F"});
	}
}

void Codec_polar::parameters
::store(const tools::Argument_map_value &vals)
{
	Codec_SISO_SIHO::parameters::store(vals);

	if (this->pct)
	{
		pct->store(vals);

		this->enc->K        = this->fbg->K    = this->dec->K        = this->pct->K;
		this->enc->N_cw     = this->fbg->N_cw = this->dec->N_cw     = this->pct->N_cw;
		this->enc->n_frames                   = this->dec->n_frames = this->pct->n_frames;
	}

	enc->store(vals);

	if (!this->pct)
	{
		this->fbg->K    = this->dec->K        = this->enc->K;
		this->fbg->N_cw = this->dec->N_cw     = this->enc->N_cw;
		                  this->dec->n_frames = this->enc->n_frames;
	}

	fbg->store(vals);

	this->dec->systematic = this->enc->systematic;

	dec->store(vals);

	this->K    = this->pct ? this->pct->K    : this->enc->K;
	this->N_cw = this->pct ? this->pct->N_cw : this->enc->N_cw;
	this->N    = this->pct ? this->pct->N    : this->enc->N_cw;
}

void Codec_polar::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SISO_SIHO::parameters::get_headers(headers, full);

	enc->get_headers(headers, full);
	fbg->get_headers(headers, full);
	dec->get_headers(headers, full);
	if (this->pct)
		pct->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_polar<B,Q>* Codec_polar::parameters
::build(module::CRC<B> *crc) const
{
	return new module::Codec_polar<B,Q>(*fbg, *enc, *dec, pct, crc);

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
