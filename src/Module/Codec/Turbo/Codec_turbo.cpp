#include <functional>
#include <cctype>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Self_corrected/Self_corrected.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/RSC/Encoder_RSC.hpp"
#include "Factory/Module/Decoder/RSC/Decoder_RSC.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check.hpp"

#include "Codec_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_turbo<B,Q>
::Codec_turbo(const factory::Encoder_turbo  ::parameters<> &enc_params,
              const factory::Decoder_turbo  ::parameters<> &dec_params,
              const factory::Puncturer_turbo::parameters   *pct_params,
              CRC<B>* crc, const std::string name)
: Codec     <B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.tail_length, enc_params.n_frames, name),
  Codec_SIHO<B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.tail_length, enc_params.n_frames, name),
  sub_enc(nullptr),
  sub_dec(nullptr)
{
	// ----------------------------------------------------------------------------------------------------- exceptions
	if (enc_params.K != dec_params.K)
	{
		std::stringstream message;
		message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
		        << ", 'dec_params.K' = " << dec_params.K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.N_cw != dec_params.N_cw)
	{
		std::stringstream message;
		message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
		        << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.n_frames != dec_params.n_frames)
	{
		std::stringstream message;
		message << "'enc_params.n_frames' has to be equal to 'dec_params.n_frames' ('enc_params.n_frames' = "
		        << enc_params.n_frames << ", 'dec_params.n_frames' = " << dec_params.n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------------- tools
	if (!enc_params.json_path.empty())
	{
		json_stream.open(enc_params.json_path.c_str(), std::ios::out | std::ios::trunc);
		json_stream << "[" << std::endl;
	}

	auto encoder_RSC = factory::Encoder_RSC::build<B>(*enc_params.sub1);
	trellis = encoder_RSC->get_trellis();
	delete encoder_RSC;

	// ---------------------------------------------------------------------------------------------------- allocations
	this->set_interleaver(factory::Interleaver_core::build<>(*dec_params.itl->core));

	if (pct_params)
	{
		try
		{
			this->set_puncturer(factory::Puncturer_turbo::build<B,Q>(*pct_params));
		}
		catch (tools::cannot_allocate const&)
		{
			this->set_puncturer(factory::Puncturer::build<B,Q>(*pct_params));
		}
	}
	else
	{
		factory::Puncturer::parameters pctno_params;
		pctno_params.type     = "NO";
		pctno_params.K        = enc_params.K;
		pctno_params.N        = enc_params.N_cw;
		pctno_params.N_cw     = enc_params.N_cw;
		pctno_params.n_frames = enc_params.n_frames;

		this->set_puncturer(factory::Puncturer::build<B,Q>(pctno_params));
	}

	try
	{
		sub_enc = factory::Encoder_RSC::build<B>(*enc_params.sub1, json_stream);
		this->set_encoder(factory::Encoder_turbo::build<B>(enc_params, this->get_interleaver_bit(), sub_enc, sub_enc));
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(factory::Encoder::build<B>(enc_params));
	}

	sub_dec = factory::Decoder_RSC::build<B,Q>(*dec_params.sub1, trellis, json_stream, dec_params.n_ite);
	auto decoder = factory::Decoder_turbo::build<B,Q>(dec_params, this->get_interleaver_llr(), *sub_dec, *sub_dec);
	this->set_decoder_siho(decoder);

	// ------------------------------------------------------------------------------------------------ post processing
	if (dec_params.sf->enable)
		post_pros.push_back(factory::Scaling_factor::build<B,Q>(*dec_params.sf));

	if (dec_params.fnc->enable)
	{
		if (crc == nullptr || crc->get_size() == 0)
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "The Flip aNd Check requires a CRC.");

		post_pros.push_back(factory::Flip_and_check::build<B,Q>(*dec_params.fnc, *crc));
	}
	else if (crc != nullptr && crc->get_size() > 0)
		post_pros.push_back(new tools::CRC_checker<B,Q>(*crc, 2, decoder->get_simd_inter_frame_level()));

	if (dec_params.self_corrected)
		post_pros.push_back(new tools::Self_corrected<B,Q>(dec_params.K,
		                                                   dec_params.n_ite,
		                                                   4,
		                                                   dec_params.n_ite,
		                                                   decoder->get_simd_inter_frame_level()));

	for (auto i = 0; i < (int)post_pros.size(); i++)
		if (post_pros[i] != nullptr)
		{
			using namespace std::placeholders;

			auto pp = post_pros[i];
			decoder->add_handler_siso_n(std::bind(&tools::Post_processing_SISO<B,Q>::siso_n, pp, _1, _2, _3, _4));
			decoder->add_handler_siso_i(std::bind(&tools::Post_processing_SISO<B,Q>::siso_i, pp, _1, _2, _3    ));
			decoder->add_handler_end   (std::bind(&tools::Post_processing_SISO<B,Q>::end,    pp, _1            ));
		}
}

template <typename B, typename Q>
Codec_turbo<B,Q>
::~Codec_turbo()
{
	if (sub_enc != nullptr) { delete sub_enc; sub_enc = nullptr; }
	if (sub_dec != nullptr) { delete sub_dec; sub_dec = nullptr; }

	if (post_pros.size())
		for (auto i = 0; i < (int)post_pros.size(); i++)
			if (post_pros[i] != nullptr)
			{
				delete post_pros[i];
				post_pros[i] = nullptr;
			}
	post_pros.clear();

	if (json_stream.is_open())
	{
		json_stream << "[{\"stage\": \"end\"}]]" << std::endl;
		json_stream.close();
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Codec_turbo<B_8,Q_8>;
template class aff3ct::module::Codec_turbo<B_16,Q_16>;
template class aff3ct::module::Codec_turbo<B_32,Q_32>;
template class aff3ct::module::Codec_turbo<B_64,Q_64>;
#else
template class aff3ct::module::Codec_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
