#include <functional>
#include <cctype>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker_DB.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check_DB.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Decoder/RSC_DB/Decoder_RSC_DB.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check_DB.hpp"

#include "Codec_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_turbo_DB<B,Q>
::Codec_turbo_DB(const factory::Encoder_turbo_DB  ::parameters &enc_params,
                 const factory::Decoder_turbo_DB  ::parameters &dec_params,
                 const factory::Puncturer_turbo_DB::parameters *pct_params,
                 CRC<B>* crc)
: Codec     <B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  Codec_SIHO<B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  sub_enc  (nullptr),
  sub_dec_n(nullptr),
  sub_dec_i(nullptr)
{
	const std::string name = "Codec_turbo_DB";
	this->set_name(name);

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
	auto encoder_RSC = factory::Encoder_RSC_DB::build<B>(*enc_params.sub);
	trellis = encoder_RSC->get_trellis();
	delete encoder_RSC;

	// ---------------------------------------------------------------------------------------------------- allocations
	this->set_interleaver(factory::Interleaver_core::build<>(*enc_params.itl->core));

	if (pct_params)
	{
		try
		{
			this->set_puncturer(factory::Puncturer_turbo_DB::build<B,Q>(*pct_params));
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
		sub_enc = factory::Encoder_RSC_DB::build<B>(*enc_params.sub);
		this->set_encoder(factory::Encoder_turbo_DB::build<B>(enc_params, this->get_interleaver_bit(), *sub_enc));
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(factory::Encoder::build<B>(enc_params));
	}

	sub_dec_n = factory::Decoder_RSC_DB::build_siso<B,Q>(*dec_params.sub, trellis);
	sub_dec_i = factory::Decoder_RSC_DB::build_siso<B,Q>(*dec_params.sub, trellis);
	auto decoder = factory::Decoder_turbo_DB::build<B,Q>(dec_params, this->get_interleaver_llr(), *sub_dec_n, *sub_dec_i);
	this->set_decoder_siho(decoder);

	// ------------------------------------------------------------------------------------------------ post processing
	// then add post processing modules
	if (dec_params.sf->enable)
		post_pros.push_back(factory::Scaling_factor::build<B,Q>(*dec_params.sf));

	if (dec_params.fnc->enable)
	{
		if (crc == nullptr || crc->get_size() == 0)
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "The Flip aNd Check requires a CRC.");

		post_pros.push_back(factory::Flip_and_check_DB::build<B,Q>(*dec_params.fnc, *crc));
	}
	else if (crc != nullptr && crc->get_size() > 0)
		post_pros.push_back(new tools::CRC_checker_DB<B,Q>(*crc, 2, decoder->get_simd_inter_frame_level()));

	for (auto i = 0; i < (int)post_pros.size(); i++)
	{
		if (post_pros[i] != nullptr)
		{
			using namespace std::placeholders;

			auto pp = post_pros[i];
			decoder->add_handler_siso_n(std::bind(&tools::Post_processing_SISO<B,Q>::siso_n, pp, _1, _2, _3, _4));
			decoder->add_handler_siso_i(std::bind(&tools::Post_processing_SISO<B,Q>::siso_i, pp, _1, _2, _3    ));
			decoder->add_handler_end   (std::bind(&tools::Post_processing_SISO<B,Q>::end,    pp, _1            ));
		}
	}
}

template <typename B, typename Q>
Codec_turbo_DB<B,Q>
::~Codec_turbo_DB()
{
	if (sub_enc   != nullptr) { delete sub_enc;   sub_enc   = nullptr; }
	if (sub_dec_n != nullptr) { delete sub_dec_n; sub_dec_n = nullptr; }
	if (sub_dec_i != nullptr) { delete sub_dec_i; sub_dec_i = nullptr; }

	if (post_pros.size())
		for (auto i = 0; i < (int)post_pros.size(); i++)
			if (post_pros[i] != nullptr)
			{
				delete post_pros[i];
				post_pros[i] = nullptr;
			}
	post_pros.clear();
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Codec_turbo_DB<B_8,Q_8>;
template class aff3ct::module::Codec_turbo_DB<B_16,Q_16>;
template class aff3ct::module::Codec_turbo_DB<B_32,Q_32>;
template class aff3ct::module::Codec_turbo_DB<B_64,Q_64>;
#else
template class aff3ct::module::Codec_turbo_DB<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
