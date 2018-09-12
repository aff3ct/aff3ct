#include <sstream>
#include "Codec_LDPC.hpp"

#include "Factory/Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Factory/Module/Puncturer/LDPC/Puncturer_LDPC.hpp"
#include "Factory/Module/Decoder/LDPC/Decoder_LDPC.hpp"

#include "Tools/Exception/exception.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Codec_LDPC_name   = "Codec LDPC";
const std::string aff3ct::factory::Codec_LDPC_prefix = "cdc";

Codec_LDPC::parameters
::parameters(const std::string &prefix)
: Codec          ::parameters(Codec_LDPC_name, prefix),
  Codec_SISO_SIHO::parameters(Codec_LDPC_name, prefix)
{
	Codec::parameters::set_enc(new Encoder_LDPC::parameters("enc"));
	Codec::parameters::set_dec(new Decoder_LDPC::parameters("dec"));
}

Codec_LDPC::parameters* Codec_LDPC::parameters
::clone() const
{
	return new Codec_LDPC::parameters(*this);
}

void Codec_LDPC::parameters
::enable_puncturer()
{
	set_pct(new Puncturer_LDPC::parameters("pct"));
}

void Codec_LDPC::parameters
::get_description(tools::Argument_map_info &args) const
{
	Codec_SISO_SIHO::parameters::get_description(args);

	enc->get_description(args);
	dec->get_description(args);

	auto penc = enc->get_prefix();
	auto pdec = dec->get_prefix();

	args.erase({penc+"-h-path"           });
	args.erase({penc+"-h-reorder"        });
	args.erase({pdec+"-cw-size",   "N"   });
	args.erase({pdec+"-info-bits", "K"   });
	args.erase({pdec+"-fra",       "F"   });

	args.add_link({pdec+"-h-path"}, {penc+"-info-bits", "K"});
	args.add_link({pdec+"-h-path"}, {penc+"-cw-size",   "N"});

	if (pct != nullptr)
	{
		pct->get_description(args);

		auto ppct = pct->get_prefix();

		args.erase({ppct+"-info-bits", "K"   });
		args.erase({ppct+"-fra",       "F"   });
		args.erase({ppct+"-cw-size",   "N_cw"});

		args.add_link({pdec+"-h-path"}, {ppct+"-fra-size", "N"});
	}
}

void Codec_LDPC::parameters
::store(const tools::Argument_map_value &vals)
{
	Codec_SISO_SIHO::parameters::store(vals);

	auto enc_ldpc = dynamic_cast<Encoder_LDPC::parameters*>(dec.get());
	auto dec_ldpc = dynamic_cast<Decoder_LDPC::parameters*>(dec.get());

	enc->store(vals);
	dec->store(vals);

	if (enc->type == "LDPC_DVBS2" || enc->type == "LDPC")
		dec->N_cw = enc->N_cw; // then the encoder knows the N_cw
	else
		enc->N_cw = dec->N_cw; // then the decoder knows the N_cw

	if (enc->K != 0)
		dec->K = enc->K; // then the encoder knows the K
	else
		enc->K = dec->K; // then the decoder knows the K

	if (enc->type == "LDPC_H")
		enc_ldpc->H_path = dec_ldpc->H_path;

	// if (dec->K == 0 || dec->N_cw == 0 || enc->K == 0 || enc->N_cw == 0)
	// {
	// 	std::stringstream message;
	// 	message << "Error while initializing decoder and encoder dimensions ('dec->K' = " << dec->K
	// 	        << ", 'dec->N_cw' = " << dec->N_cw << ", 'enc->K' = " << enc->K
	// 	        << ", 'enc->N_cw' = " << enc->N_cw << ").";
	// 	throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	// }

	dec->n_frames = enc->n_frames;

	enc->R = (float)enc->K / (float)enc->N_cw;
	dec->R = (float)dec->K / (float)dec->N_cw;

	if (pct != nullptr)
	{
		pct->K        = enc->K;
		pct->N        = enc->N_cw;
		pct->N_cw     = enc->N_cw;
		pct->n_frames = enc->n_frames;

		pct->store(vals);
	}

	K    = enc->K;
	N_cw = enc->N_cw;
	N    = pct != nullptr ? pct->N : N_cw;
}

void Codec_LDPC::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Codec_SISO_SIHO::parameters::get_headers(headers, full);

	enc->get_headers(headers, full);
	dec->get_headers(headers, full);
	if (pct != nullptr)
		pct->get_headers(headers, full);
}

template <typename B, typename Q>
module::Codec_LDPC<B,Q>* Codec_LDPC::parameters
::build(module::CRC<B>* crc) const
{
	return new module::Codec_LDPC<B,Q>(dynamic_cast<const Encoder_LDPC  ::parameters&>(*enc),
	                                   dynamic_cast<const Decoder_LDPC  ::parameters&>(*dec),
	                                   dynamic_cast<Puncturer_LDPC::parameters*>(pct.get()));
}

template <typename B, typename Q>
module::Codec_LDPC<B,Q>* Codec_LDPC
::build(const parameters &params, module::CRC<B>* crc)
{
	return params.template build<B,Q>(crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Codec_LDPC<B_8 ,Q_8 >* aff3ct::factory::Codec_LDPC::parameters::build<B_8 ,Q_8 >(aff3ct::module::CRC<B_8 >*) const;
template aff3ct::module::Codec_LDPC<B_16,Q_16>* aff3ct::factory::Codec_LDPC::parameters::build<B_16,Q_16>(aff3ct::module::CRC<B_16>*) const;
template aff3ct::module::Codec_LDPC<B_32,Q_32>* aff3ct::factory::Codec_LDPC::parameters::build<B_32,Q_32>(aff3ct::module::CRC<B_32>*) const;
template aff3ct::module::Codec_LDPC<B_64,Q_64>* aff3ct::factory::Codec_LDPC::parameters::build<B_64,Q_64>(aff3ct::module::CRC<B_64>*) const;
template aff3ct::module::Codec_LDPC<B_8 ,Q_8 >* aff3ct::factory::Codec_LDPC::build<B_8 ,Q_8 >(const aff3ct::factory::Codec_LDPC::parameters&, aff3ct::module::CRC<B_8 >*);
template aff3ct::module::Codec_LDPC<B_16,Q_16>* aff3ct::factory::Codec_LDPC::build<B_16,Q_16>(const aff3ct::factory::Codec_LDPC::parameters&, aff3ct::module::CRC<B_16>*);
template aff3ct::module::Codec_LDPC<B_32,Q_32>* aff3ct::factory::Codec_LDPC::build<B_32,Q_32>(const aff3ct::factory::Codec_LDPC::parameters&, aff3ct::module::CRC<B_32>*);
template aff3ct::module::Codec_LDPC<B_64,Q_64>* aff3ct::factory::Codec_LDPC::build<B_64,Q_64>(const aff3ct::factory::Codec_LDPC::parameters&, aff3ct::module::CRC<B_64>*);
#else
template aff3ct::module::Codec_LDPC<B,Q>* aff3ct::factory::Codec_LDPC::parameters::build<B,Q>(aff3ct::module::CRC<B>*) const;
template aff3ct::module::Codec_LDPC<B,Q>* aff3ct::factory::Codec_LDPC::build<B,Q>(const aff3ct::factory::Codec_LDPC::parameters&, aff3ct::module::CRC<B>*);
#endif
// ==================================================================================== explicit template instantiation

