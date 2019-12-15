#include "Factory/Module/Encoder/Polar_MK/Encoder_polar_MK.hpp"
#include "Factory/Module/Decoder/Polar_MK/Decoder_polar_MK.hpp"
#include "Factory/Tools/Codec/Polar_MK/Codec_polar_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_polar_MK_name   = "Codec Polar MK";
const std::string aff3ct::factory::Codec_polar_MK_prefix = "cdc";

Codec_polar_MK
::Codec_polar_MK(const std::string &prefix)
: Codec_SIHO(Codec_polar_MK_name, prefix)
{
	Codec::set_enc(new Encoder_polar_MK("enc"));
	Codec::set_dec(new Decoder_polar_MK("dec"));

	pc  = new Polar_code(enc->get_prefix()+"-pc");
	fbg = new Frozenbits_generator_MK(enc->get_prefix()+"-fb");
}

Codec_polar_MK* Codec_polar_MK
::clone() const
{
	return new Codec_polar_MK(*this);
}

std::vector<std::string> Codec_polar_MK
::get_names() const
{
	auto n = Codec::get_names();
	std::vector<std::string> n2;

	for (size_t i = 0; i < n.size(); i++)
	{
		n2.push_back(n[i]);
		if (enc != nullptr && n[i] == enc->get_name() && pc != nullptr && fbg != nullptr)
		{
			auto nn = pc->get_names();
			for (auto &x : nn)
				n2.push_back(x);

			nn = fbg->get_names();
			for (auto &x : nn)
				n2.push_back(x);
		}
	}

//	if (fbg != nullptr) { auto nn = fbg->get_names(); for (auto &x : nn) n.push_back(x); }
	return n2;
}

std::vector<std::string> Codec_polar_MK
::get_short_names() const
{
	auto sn = Codec::get_short_names();
	std::vector<std::string> sn2;

	for (size_t i = 0; i < sn.size(); i++)
	{
		sn2.push_back(sn[i]);
		if (enc != nullptr && sn[i] == enc->get_short_name() && pc != nullptr && fbg != nullptr)
		{
			auto nn = pc->get_short_names();
			for (auto &x : nn)
				sn2.push_back(x);

			nn = fbg->get_short_names();
			for (auto &x : nn)
				sn2.push_back(x);
		}
	}

//	if (fbg != nullptr) { auto nn = fbg->get_short_names(); for (auto &x : nn) sn.push_back(x); }
	return sn2;
}

std::vector<std::string> Codec_polar_MK
::get_prefixes() const
{
	auto p = Codec::get_prefixes();
	std::vector<std::string> p2;

	for (size_t i = 0; i < p.size(); i++)
	{
		p2.push_back(p[i]);
		if (enc != nullptr && p[i] == enc->get_prefix() && pc != nullptr && fbg != nullptr)
		{
			auto nn = pc->get_prefixes();
			for (auto &x : nn)
				p2.push_back(x);

			nn = fbg->get_prefixes();
			for (auto &x : nn)
				p2.push_back(x);
		}
	}

//	if (fbg != nullptr) { auto nn = fbg->get_prefixes(); for (auto &x : nn) p.push_back(x); }
	return p2;
}

void Codec_polar_MK
::get_description(cli::Argument_map_info &args) const
{
	Codec_SIHO::get_description(args);

	enc->get_description(args);
	pc ->get_description(args);
	fbg->get_description(args);
	dec->get_description(args);

	auto ppc  = pc->get_prefix();
	auto pdec = dec->get_prefix();
	auto pfbg = fbg->get_prefix();

	args.erase({pdec+"-info-bits", "K"});
	args.erase({pdec+"-fra",       "F"});
	args.erase({pdec+"-no-sys"        });
	args.erase({pdec+"-cw-size",   "N"});
	args.erase({ppc +"-cw-size",   "N"});
	args.erase({pfbg+"-cw-size",   "N"});
	args.erase({pfbg+"-info-bits", "K"});
}

void Codec_polar_MK
::store(const cli::Argument_map_value &vals)
{
	Codec_SIHO::store(vals);

	enc->store(vals);

	           fbg->K    = dec->K        = enc->K;
	pc->N_cw = fbg->N_cw = dec->N_cw     = enc->N_cw;
	                       dec->n_frames = enc->n_frames;

	pc->store(vals);

	// if (enc->code_path.empty())
	// 	fbg->base = enc->kernel_matrix.size();

	fbg->store(vals);

	dec->systematic = enc->systematic;

	dec->store(vals);

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = enc->N_cw;
}

void Codec_polar_MK
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Codec_SIHO::get_headers(headers, full);

	enc->get_headers(headers, full);
	pc ->get_headers(headers, full);
	fbg->get_headers(headers, full);
	dec->get_headers(headers, full);
}

template <typename B, typename Q>
tools::Codec_polar_MK<B,Q>* Codec_polar_MK
::build(const module::CRC<B> *crc) const
{
	return new tools::Codec_polar_MK<B,Q>(*pc, *fbg,
	                                      dynamic_cast<const Encoder_polar_MK&>(*enc),
	                                      dynamic_cast<const Decoder_polar_MK&>(*dec),
	                                      crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::tools::Codec_polar_MK<B_8 ,Q_8 >* aff3ct::factory::Codec_polar_MK::build<B_8 ,Q_8 >(const aff3ct::module::CRC<B_8 >*) const;
template aff3ct::tools::Codec_polar_MK<B_16,Q_16>* aff3ct::factory::Codec_polar_MK::build<B_16,Q_16>(const aff3ct::module::CRC<B_16>*) const;
template aff3ct::tools::Codec_polar_MK<B_32,Q_32>* aff3ct::factory::Codec_polar_MK::build<B_32,Q_32>(const aff3ct::module::CRC<B_32>*) const;
template aff3ct::tools::Codec_polar_MK<B_64,Q_64>* aff3ct::factory::Codec_polar_MK::build<B_64,Q_64>(const aff3ct::module::CRC<B_64>*) const;
#else
template aff3ct::tools::Codec_polar_MK<B,Q>* aff3ct::factory::Codec_polar_MK::build<B,Q>(const aff3ct::module::CRC<B>*) const;
#endif
// ==================================================================================== explicit template instantiation
