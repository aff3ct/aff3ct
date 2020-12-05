#include <algorithm>
#include <sstream>
#include <string>

#include "Module/Extractor/Polar/Extractor_polar.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/Sigma.hpp"
#include "Tools/Noise/Event_probability.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Tools/Codec/Polar_MK/Codec_polar_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_polar_MK<B,Q>
::Codec_polar_MK(const factory::Polar_code              &pc_params,
                 const factory::Frozenbits_generator_MK &fb_params,
                 const factory::Encoder_polar_MK        &enc_params,
                 const factory::Decoder_polar_MK        &dec_params,
                 const module::CRC<B>                   *crc)
: Codec_SIHO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw),
  adaptive_fb(fb_params.noise == -1.f),
  frozen_bits(new std::vector<bool>(fb_params.N_cw, true)),
  fb_decoder(nullptr),
  fb_encoder(nullptr),
  fb_extractor(nullptr)
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

	// ---------------------------------------------------------------------------------------------------------- tools
	// build the polar code
	code.reset(pc_params.build());

	// build the frozen bits generator
	fb_generator.reset(fb_params.build(*code.get()));

	// ---------------------------------------------------------------------------------------------------- allocations
	factory::Puncturer pct_params;
	pct_params.type = "NO";
	pct_params.K    = enc_params.K;
	pct_params.N    = enc_params.N_cw;
	pct_params.N_cw = enc_params.N_cw;

	this->set_puncturer(pct_params.build<B,Q>());

	std::fill(frozen_bits->begin(), frozen_bits->begin() + this->K, false);

	try
	{
		this->set_encoder(enc_params.build<B>(*code.get(), *frozen_bits));
		fb_encoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_encoder());
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(static_cast<const factory::Encoder*>(&enc_params)->build<B>());
	}

	this->set_decoder_siho(dec_params.build<B,Q>(*code.get(), *frozen_bits, crc, &this->get_encoder()));
	this->fb_decoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_decoder_siho());

	this->set_extractor(new module::Extractor_polar<B,Q>(enc_params.K,
	                                                     enc_params.N_cw,
	                                                     *frozen_bits));
	fb_extractor = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_extractor());

	// ------------------------------------------------------------------------------------------------- frozen bit gen
	if (!adaptive_fb)
	{
		if (fb_params.type == "BEC")
		{
			auto ep = tools::Event_probability<float>(fb_params.noise);
			fb_generator->set_noise(ep);
		}
		else /* type = GA, TV or FILE */
		{
			auto sigma = tools::Sigma<float>(fb_params.noise);
			fb_generator->set_noise(sigma);
		}

		fb_generator->generate(*frozen_bits);
		this->set_frozen_bits(*frozen_bits);
	}
}

template <typename B, typename Q>
Codec_polar_MK<B,Q>* Codec_polar_MK<B,Q>
::clone() const
{
	auto t = new Codec_polar_MK(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::deep_copy(const Codec_polar_MK<B,Q> &t)
{
	Codec_SIHO<B,Q>::deep_copy(t);
	if (t.fb_encoder != nullptr)
		this->fb_encoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_encoder());
	if (t.fb_decoder != nullptr)
		this->fb_decoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_decoder_siho());
	if (t.fb_extractor != nullptr)
		this->fb_extractor = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_extractor());
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::set_frozen_bits(const std::vector<bool>& fb)
{
	if (this->fb_decoder)
		this->fb_decoder->set_frozen_bits(fb);
	if (this->fb_encoder)
		this->fb_encoder->set_frozen_bits(fb);
	if (this->fb_extractor)
		this->fb_extractor->set_frozen_bits(fb);
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::notify_noise_update()
{
	Codec<B,Q>::notify_noise_update();
	if (this->adaptive_fb)
	{
		this->fb_generator->set_noise(*this->noise);
		this->fb_generator->generate(*this->frozen_bits);
		this->set_frozen_bits(*this->frozen_bits);
	}
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::check_noise()
{
	Codec<B,Q>::check_noise();
	if (!this->noise->is_of_type(Noise_type::SIGMA) && !this->noise->is_of_type(Noise_type::EP))
	{
		std::stringstream message;
		message << "Incompatible noise type, expected noise types are SIGMA or EP ('noise->get_type()' = "
		        << Noise<>::type_to_str(this->noise->get_type()) << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename Q>
const std::vector<bool>& Codec_polar_MK<B,Q>
::get_frozen_bits() const
{
	return *this->frozen_bits;
}

template <typename B, typename Q>
bool Codec_polar_MK<B,Q>
::is_adaptive_frozen_bits() const
{
	return this->adaptive_fb;
}

template <typename B, typename Q>
const Polar_code& Codec_polar_MK<B,Q>
::get_code() const
{
	return *this->code;
}

template <typename B, typename Q>
const Frozenbits_generator& Codec_polar_MK<B,Q>
::get_frozen_bits_generator() const
{
	if (this->fb_generator == nullptr)
	{
		std::stringstream message;
		message << "'fb_generator' can't be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->fb_generator.get();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Codec_polar_MK<B_8,Q_8>;
template class aff3ct::tools::Codec_polar_MK<B_16,Q_16>;
template class aff3ct::tools::Codec_polar_MK<B_32,Q_32>;
template class aff3ct::tools::Codec_polar_MK<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_polar_MK<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
