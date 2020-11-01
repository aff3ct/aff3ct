#include <functional>
#include <sstream>
#include <memory>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker_DB.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"
#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Tools/Interleaver/Interleaver_core.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check_DB.hpp"
#include "Tools/Codec/Turbo_DB/Codec_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_turbo_DB<B,Q>
::Codec_turbo_DB(const factory::Encoder_turbo_DB   &enc_params,
                 const factory::Decoder_turbo_DB   &dec_params,
                 const factory::Interleaver        &itl_params,
                 const factory::Puncturer_turbo_DB *pct_params,
                 const module::CRC<B>              *crc)
: Codec_SIHO<B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw),
  trellis(new std::vector<std::vector<int>>())
{
	// ----------------------------------------------------------------------------------------------------- exceptions
	if (enc_params.K != dec_params.K)
	{
		std::stringstream message;
		message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
		        << ", 'dec_params.K' = " << dec_params.K << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.N_cw != dec_params.N_cw)
	{
		std::stringstream message;
		message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
		        << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------------- tools
	std::unique_ptr<module::Encoder_RSC_DB<B>> encoder_RSC(enc_params.sub->build<B>());
	*trellis = encoder_RSC->get_trellis();

	// ---------------------------------------------------------------------------------------------------- allocations
	this->set_interleaver(itl_params.core->build<>());
	if (pct_params)
	{
		try
		{
			this->set_puncturer(pct_params->build<B,Q>());
		}
		catch (cannot_allocate const&)
		{
			this->set_puncturer(static_cast<const factory::Puncturer*>(pct_params)->build<B,Q>());
		}
	}
	else
	{
		factory::Puncturer pctno_params;
		pctno_params.type = "NO";
		pctno_params.K    = enc_params.K;
		pctno_params.N    = enc_params.N_cw;
		pctno_params.N_cw = enc_params.N_cw;

		this->set_puncturer(pctno_params.build<B,Q>());
	}

	try
	{
		std::unique_ptr<module::Encoder_RSC_DB<B>> sub_enc(enc_params.sub->build<B>());
		this->set_encoder(enc_params.build<B>(*sub_enc, this->get_interleaver_bit()));
	}
	catch (cannot_allocate const&)
	{
		this->set_encoder(static_cast<const factory::Encoder*>(&enc_params)->build<B>());
	}


	std::shared_ptr<module::Decoder_turbo_DB<B,Q>> decoder_turbo;
	try
	{
		this->set_decoder_siho(dec_params.build<B,Q>(&this->get_encoder()));
	}
	catch (cannot_allocate const&)
	{
		std::unique_ptr<module::Decoder_RSC_DB_BCJR<B,Q>> sub_dec(dec_params.sub->build_siso<B,Q>(*trellis));

		decoder_turbo.reset(dec_params.build<B,Q>(*sub_dec, *sub_dec, this->get_interleaver_llr(),
		                                          &this->get_encoder()));
		this->set_decoder_siho(std::static_pointer_cast<module::Decoder_SIHO<B,Q>>(decoder_turbo));
	}

	// ------------------------------------------------------------------------------------------------ post processing
	if (decoder_turbo)
	{
		std::vector<std::unique_ptr<Post_processing_SISO<B,Q>>> post_pros;

		// then add post processing modules
		if (dec_params.sf->enable)
			post_pros.push_back(std::unique_ptr<Post_processing_SISO<B,Q>>(dec_params.sf->build<B,Q>()));

		if (dec_params.fnc->enable)
		{
			if (crc == nullptr || std::unique_ptr<module::CRC<B>>(crc->clone())->get_size() == 0)
				throw runtime_error(__FILE__, __LINE__, __func__, "The Flip aNd Check requires a CRC.");

			post_pros.push_back(std::unique_ptr<Post_processing_SISO<B,Q>>(dec_params.fnc->build<B,Q>(*crc)));
		}
		else if (crc != nullptr && std::unique_ptr<module::CRC<B>>(crc->clone())->get_size() > 0)
			post_pros.push_back(std::unique_ptr<Post_processing_SISO<B,Q>>(new CRC_checker_DB<B,Q>(
				*crc,
				dec_params.crc_start_ite)));

		for (auto i = 0; i < (int)post_pros.size(); i++)
			if (post_pros[i] != nullptr)
				decoder_turbo->add_post_processing(*post_pros[i]);
	}
}

template <typename B, typename Q>
Codec_turbo_DB<B,Q>* Codec_turbo_DB<B,Q>
::clone() const
{
	auto t = new Codec_turbo_DB(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename Q>
const std::vector<std::vector<int>>& Codec_turbo_DB<B,Q>
::get_trellis() const
{
	return *this->trellis;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Codec_turbo_DB<B_8,Q_8>;
template class aff3ct::tools::Codec_turbo_DB<B_16,Q_16>;
template class aff3ct::tools::Codec_turbo_DB<B_32,Q_32>;
template class aff3ct::tools::Codec_turbo_DB<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_turbo_DB<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
