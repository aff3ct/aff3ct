#include <functional>
#include <sstream>
#include <string>
#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Self_corrected/Self_corrected.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Tools/Interleaver/Interleaver_core.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check.hpp"
#include "Tools/Codec/Turbo/Codec_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_turbo<B,Q>
::Codec_turbo(const factory::Encoder_turbo  <> &enc_params,
              const factory::Decoder_turbo  <> &dec_params,
              const factory::Interleaver       &itl_params,
              const factory::Puncturer_turbo   *pct_params,
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
	if (!enc_params.json_path.empty())
	{
		json_stream.open(enc_params.json_path.c_str(), std::ios::out | std::ios::trunc);
		json_stream << "[" << std::endl;
	}

	std::unique_ptr<module::Encoder_RSC_sys<B>> encoder_RSC(enc_params.sub1->build<B>());
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
		sub_enc.reset(enc_params.sub1->build<B>(json_stream));
		this->set_encoder(enc_params.build<B>(this->get_interleaver_bit(), sub_enc, sub_enc));
	}
	catch (cannot_allocate const&)
	{
		this->set_encoder(static_cast<const factory::Encoder*>(&enc_params)->build<B>());
	}

	std::shared_ptr<module::Decoder_turbo<B,Q>> decoder_turbo;
	try
	{
		this->set_decoder_siho(dec_params.build<B,Q>(&this->get_encoder()));
	}
	catch (cannot_allocate const&)
	{
		sub_dec.reset(dec_params.sub1->build_siso<B,Q>(trellis, json_stream, dec_params.n_ite));
		decoder_turbo.reset(dec_params.build<B,Q>(this->get_interleaver_llr(), *sub_dec, *sub_dec,
		                                          &this->get_encoder()));
		this->set_decoder_siho(std::static_pointer_cast<module::Decoder_SIHO<B,Q>>(decoder_turbo));
	}

	// ------------------------------------------------------------------------------------------------ post processing
	if (decoder_turbo)
	{
		if (dec_params.sf->enable)
			add_post_processings(dec_params.sf->build<B,Q>());

		if (dec_params.fnc->enable)
		{
			if (crc == nullptr || crc->get_size() == 0)
				throw runtime_error(__FILE__, __LINE__, __func__, "The Flip aNd Check requires a CRC.");

			add_post_processings(dec_params.fnc->build<B,Q>(*crc));
		}
		else if (crc != nullptr && crc->get_size() > 0)
			add_post_processings(new CRC_checker<B,Q>(*crc,
			                                          dec_params.crc_start_ite,
			                                          decoder_turbo->get_simd_inter_frame_level()));

		if (dec_params.self_corrected)
			add_post_processings(new Self_corrected<B,Q>(dec_params.K,
			                                             dec_params.n_ite,
			                                             4,
			                                             dec_params.n_ite,
			                                             decoder_turbo->get_simd_inter_frame_level()));

		for (auto i = 0; i < (int)post_pros.size(); i++)
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

template <typename B, typename Q>
Codec_turbo<B,Q>
::~Codec_turbo()
{
	if (json_stream.is_open())
	{
		json_stream << "[{\"stage\": \"end\"}]]" << std::endl;
		json_stream.close();
	}
}

template <typename B, typename Q>
const std::vector<std::vector<int>>& Codec_turbo<B,Q>
::get_trellis() const
{
	return this->trellis;
}

template <typename B, typename Q>
const module::Encoder_RSC_sys<B>& Codec_turbo<B,Q>
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
const module::Decoder_SISO<Q>& Codec_turbo<B,Q>
::get_sub_decoder() const
{
	if (this->sub_dec == nullptr)
	{
		std::stringstream message;
		message << "'sub_dec' can't be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->sub_dec.get();
}

template <typename B, typename Q>
const std::vector<std::shared_ptr<Post_processing_SISO<B,Q>>>& Codec_turbo<B,Q>
::get_post_processings() const
{
	return this->post_pros;
}

template <typename B, typename Q>
void Codec_turbo<B,Q>
::add_post_processings(Post_processing_SISO<B,Q>* p)
{
	post_pros.push_back(std::shared_ptr<Post_processing_SISO<B,Q>>(p));
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Codec_turbo<B_8,Q_8>;
template class aff3ct::tools::Codec_turbo<B_16,Q_16>;
template class aff3ct::tools::Codec_turbo<B_32,Q_32>;
template class aff3ct::tools::Codec_turbo<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
