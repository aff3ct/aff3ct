#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Factory/Module/Puncturer/Puncturer.hpp"

#include "Codec_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_BCH<B,Q>
::Codec_BCH(const factory::Encoder_BCH::parameters &enc_params,
            const factory::Decoder_BCH::parameters &dec_params)
: Codec          <B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  Codec_SIHO_HIHO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  GF_poly(dec_params.K, tools::next_power_of_2(dec_params.N_cw) -1, dec_params.t)
{
	const std::string name = "Codec_BCH";
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

	// ---------------------------------------------------------------------------------------------------- allocations
	factory::Puncturer::parameters pct_params;
	pct_params.type     = "NO";
	pct_params.K        = enc_params.K;
	pct_params.N        = enc_params.N_cw;
	pct_params.N_cw     = enc_params.N_cw;
	pct_params.n_frames = enc_params.n_frames;

	this->set_puncturer(factory::Puncturer::build<B,Q>(pct_params));

	Encoder<B>* encoder;
	try
	{
		encoder = factory::Encoder_BCH::build<B>(enc_params, GF_poly);
	}
	catch (tools::cannot_allocate const&)
	{
		encoder = factory::Encoder::build<B>(enc_params);
	}

	if (dec_params.implem == "GENIUS")
		encoder->set_memorizing(true);

	this->set_encoder(encoder);

	auto decoder_hiho_siho = factory::Decoder_BCH::build_hiho<B,Q>(dec_params, GF_poly, this->get_encoder());
	this->set_decoder_siho(decoder_hiho_siho);
	this->set_decoder_hiho(decoder_hiho_siho);
}

template <typename B, typename Q>
Codec_BCH<B,Q>
::~Codec_BCH()
{
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Codec_BCH<B_8,Q_8>;
template class aff3ct::module::Codec_BCH<B_16,Q_16>;
template class aff3ct::module::Codec_BCH<B_32,Q_32>;
template class aff3ct::module::Codec_BCH<B_64,Q_64>;
#else
template class aff3ct::module::Codec_BCH<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
