#include <functional>
#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker_DB.hpp"
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
                 module::CRC<B>* crc)
: Codec     <B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.n_frames),
  Codec_SIHO<B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.n_frames)
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

	if (enc_params.n_frames != dec_params.n_frames)
	{
		std::stringstream message;
		message << "'enc_params.n_frames' has to be equal to 'dec_params.n_frames' ('enc_params.n_frames' = "
		        << enc_params.n_frames << ", 'dec_params.n_frames' = " << dec_params.n_frames << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------------- tools
	std::unique_ptr<module::Encoder_RSC_DB<B>> encoder_RSC(enc_params.sub->build<B>());
	trellis = encoder_RSC->get_trellis();

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
		pctno_params.type     = "NO";
		pctno_params.K        = enc_params.K;
		pctno_params.N        = enc_params.N_cw;
		pctno_params.N_cw     = enc_params.N_cw;
		pctno_params.n_frames = enc_params.n_frames;

		this->set_puncturer(pctno_params.build<B,Q>());
	}

	try
	{
		sub_enc.reset(enc_params.sub->build<B>());
		this->set_encoder(enc_params.build<B>(this->get_interleaver_bit(), *sub_enc));
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
		sub_dec_n.reset(dec_params.sub->build_siso<B,Q>(trellis));
		sub_dec_i.reset(dec_params.sub->build_siso<B,Q>(trellis));
		decoder_turbo.reset(dec_params.build<B,Q>(this->get_interleaver_llr(), *sub_dec_n, *sub_dec_i,
		                                          &this->get_encoder()));
		this->set_decoder_siho(std::static_pointer_cast<module::Decoder_SIHO<B,Q>>(decoder_turbo));
	}

	// ------------------------------------------------------------------------------------------------ post processing
	if (decoder_turbo)
	{
		// then add post processing modules
		if (dec_params.sf->enable)
			add_post_processings(dec_params.sf->build<B,Q>());

		if (dec_params.fnc->enable)
		{
			if (crc == nullptr || crc->get_size() == 0)
				throw runtime_error(__FILE__, __LINE__, __func__, "The Flip aNd Check requires a CRC.");

			add_post_processings(dec_params.fnc->build<B,Q>(*crc));
		}
		else if (crc != nullptr && crc->get_size() > 0)
			add_post_processings(new CRC_checker_DB<B,Q>(*crc,
			                                             dec_params.crc_start_ite,
			                                             decoder_turbo->get_simd_inter_frame_level()));

		for (auto i = 0; i < (int)post_pros.size(); i++)
		{
			if (post_pros[i] != nullptr)
			{
				using namespace std::placeholders;

				auto pp = post_pros[i];
				decoder_turbo->add_handler_siso_n(std::bind(&Post_processing_SISO<B,Q>::siso_n, pp, _1, _2, _3, _4));
				decoder_turbo->add_handler_siso_i(std::bind(&Post_processing_SISO<B,Q>::siso_i, pp, _1, _2, _3    ));
				decoder_turbo->add_handler_end   (std::bind(&Post_processing_SISO<B,Q>::end,    pp, _1            ));
			}
		}
	}
}

template <typename B, typename Q>
const std::vector<std::vector<int>>& Codec_turbo_DB<B,Q>
::get_trellis() const
{
	return this->trellis;
}

template <typename B, typename Q>
const module::Encoder_RSC_DB<B>& Codec_turbo_DB<B,Q>
::get_sub_encoder() const
{
	if (this->sub_enc == nullptr)
	{
		std::stringstream message;
		message << "'sub_enc' can't be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->sub_enc.get();
}

template <typename B, typename Q>
const module::Decoder_RSC_DB_BCJR<B,Q>& Codec_turbo_DB<B,Q>
::get_sub_decoder_n() const
{
	if (this->sub_dec_n == nullptr)
	{
		std::stringstream message;
		message << "'sub_dec_n' can't be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->sub_dec_n.get();
}

template <typename B, typename Q>
const module::Decoder_RSC_DB_BCJR<B,Q>& Codec_turbo_DB<B,Q>
::get_sub_decoder_i() const
{
	if (this->sub_dec_i == nullptr)
	{
		std::stringstream message;
		message << "'sub_dec_i' can't be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->sub_dec_i.get();
}

template <typename B, typename Q>
const std::vector<std::shared_ptr<Post_processing_SISO<B,Q>>>& Codec_turbo_DB<B,Q>
::get_post_processings() const
{
	return this->post_pros;
}

template <typename B, typename Q>
void Codec_turbo_DB<B,Q>
::add_post_processings(Post_processing_SISO<B,Q>* p)
{
	post_pros.push_back(std::shared_ptr<Post_processing_SISO<B,Q>>(p));
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
