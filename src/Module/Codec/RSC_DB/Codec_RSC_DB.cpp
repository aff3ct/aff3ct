#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Puncturer/Puncturer.hpp"

#include "Codec_RSC_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_RSC_DB<B,Q>
::Codec_RSC_DB(const factory::Encoder_RSC_DB::parameters &enc_params,
               const factory::Decoder_RSC_DB::parameters &dec_params,
               const std::string name)
: Codec_SISO_SIHO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, enc_params.tail_length, enc_params.n_frames, name),
  buffered_encoding(enc_params.buffered)
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
	auto enc_cpy = enc_params;
	enc_cpy.type = "RSC_DB";

	auto encoder_RSC = factory::Encoder_RSC_DB::build<B>(enc_cpy);
	trellis = encoder_RSC->get_trellis();
	delete encoder_RSC;

	// ---------------------------------------------------------------------------------------------------- allocations
	factory::Puncturer::parameters pct_params;
	pct_params.type     = "NO";
	pct_params.K        = enc_params.K;
	pct_params.N        = enc_params.N_cw;
	pct_params.N_cw     = enc_params.N_cw;
	pct_params.n_frames = enc_params.n_frames;

	this->set_puncturer(factory::Puncturer::build<B,Q>(pct_params));

	try
	{
		this->set_encoder(factory::Encoder_RSC_DB::build<B>(enc_params));
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(factory::Encoder::build<B>(enc_params));
	}

	auto decoder_siso_siho = factory::Decoder_RSC_DB::build<B,Q>(dec_params, trellis);
	this->set_decoder_siso(decoder_siso_siho);
	this->set_decoder_siho(decoder_siso_siho);
}

template <typename B, typename Q>
Codec_RSC_DB<B,Q>
::~Codec_RSC_DB()
{
	this->set_decoder_siho(nullptr);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Codec_RSC_DB<B_8,Q_8>;
template class aff3ct::module::Codec_RSC_DB<B_16,Q_16>;
template class aff3ct::module::Codec_RSC_DB<B_32,Q_32>;
template class aff3ct::module::Codec_RSC_DB<B_64,Q_64>;
#else
template class aff3ct::module::Codec_RSC_DB<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
