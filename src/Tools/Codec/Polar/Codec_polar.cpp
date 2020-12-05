#include <exception>
#include <algorithm>
#include <sstream>

#include "Module/Extractor/Polar/Extractor_polar.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Tools/Noise/Sigma.hpp"
#include "Tools/Noise/Event_probability.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Tools/Codec/Polar/Codec_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_polar<B,Q>
::Codec_polar(const factory::Frozenbits_generator &fb_params,
              const factory::Encoder_polar        &enc_params,
              const factory::Decoder_polar        &dec_params,
              const factory::Puncturer_polar      *pct_params,
              const module::CRC<B>                *crc)
: Codec_SISO<B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw),
  adaptive_fb(fb_params.noise == -1.f),
  frozen_bits(new std::vector<bool>(fb_params.N_cw, true)),
  generated_decoder((dec_params.implem.find("_SNR") != std::string::npos)),
  puncturer_shortlast(nullptr),
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
	if (!generated_decoder)
		// build the frozen bits generator
		fb_generator.reset(fb_params.build());
	else if (this->N_cw != this->N)
	{
		std::stringstream message;
		message << "'N_cw' has to be equal to 'N' ('N_cw' = " << this->N_cw << ", 'N' = "
		        << this->N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------- allocations
	std::fill(frozen_bits->begin(), frozen_bits->begin() + this->K, false);

	if (generated_decoder || !pct_params)
	{
		factory::Puncturer pctno_params;
		pctno_params.type = "NO";
		pctno_params.K    = enc_params.K;
		pctno_params.N    = enc_params.N_cw;
		pctno_params.N_cw = enc_params.N_cw;

		this->set_puncturer(pctno_params.build<B,Q>());
	}
	else
	{
		try
		{
			this->set_puncturer(pct_params->build<B,Q>(*fb_generator));
			puncturer_shortlast = dynamic_cast<module::Puncturer_polar_shortlast<B,Q>*>(&this->get_puncturer());
		}
		catch(cannot_allocate const&)
		{
			this->set_puncturer(static_cast<const factory::Puncturer*>(pct_params)->build<B,Q>());
		}
	}

	try
	{
		this->set_encoder(enc_params.build<B>(*frozen_bits));
		fb_encoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_encoder());
	}
	catch (cannot_allocate const&)
	{
		this->set_encoder(static_cast<const factory::Encoder*>(&enc_params)->build<B>());
	}

	try
	{
		this->set_decoder_siso(dec_params.build_siso<B,Q>(*frozen_bits, &this->get_encoder()));
	}
	catch (const std::exception&)
	{
		if (generated_decoder)
			this->set_decoder_siho(dec_params.build_gen<B,Q>(crc, &this->get_encoder()));
		else
			this->set_decoder_siho(dec_params.build<B,Q>(*frozen_bits, crc, &this->get_encoder()));
	}

	try
	{
		this->fb_decoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_decoder_siho());
	}
	catch(std::exception&) {}

	this->set_extractor(new module::Extractor_polar<B,Q>(enc_params.K,
	                                                     enc_params.N_cw,
	                                                     *frozen_bits));
	fb_extractor = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_extractor());

	// ------------------------------------------------------------------------------------------------- frozen bit gen
	if (!generated_decoder)
	{
		if (!adaptive_fb)
		{
			if (fb_params.type == "BEC")
			{
				Event_probability<> ep(fb_params.noise);
				fb_generator->set_noise(ep);
				fb_generator->generate(*frozen_bits);
			}
			else /* type = GA, TV or FILE */
			{
				Sigma<> sigma(fb_params.noise);
				fb_generator->set_noise(sigma);
				fb_generator->generate(*frozen_bits);
			}
			if (this->N_cw != this->N && puncturer_shortlast)
				puncturer_shortlast->gen_frozen_bits(*frozen_bits);

			this->set_frozen_bits(*frozen_bits);
		}
	}
	else
	{
		const auto fb = factory::Decoder_polar::get_frozen_bits(dec_params.implem);
		if (fb.size() != frozen_bits->size())
		{
			std::stringstream message;
			message << "'fb.size()' has to be equal to 'frozen_bits->size()' ('fb.size()' = " << fb.size()
			        << ", 'frozen_bits->size()' = " << frozen_bits->size() << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
		std::copy(fb.begin(), fb.end(), frozen_bits->begin());
		if (this->N_cw != this->N && puncturer_shortlast)
			puncturer_shortlast->gen_frozen_bits(*frozen_bits);
		this->set_frozen_bits(*frozen_bits);
	}
}

template <typename B, typename Q>
Codec_polar<B,Q>* Codec_polar<B,Q>
::clone() const
{
	auto t = new Codec_polar(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::deep_copy(const Codec_polar<B,Q> &t)
{
	Codec_SISO<B,Q>::deep_copy(t);
	if (t.puncturer_shortlast != nullptr)
		this->puncturer_shortlast = dynamic_cast<module::Puncturer_polar_shortlast<B,Q>*>(&this->get_puncturer());
	if (t.fb_encoder != nullptr)
		this->fb_encoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_encoder());
	if (t.fb_decoder != nullptr)
		this->fb_decoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_decoder_siho());
	if (t.fb_extractor != nullptr)
		this->fb_extractor = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_extractor());
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::set_frozen_bits(const std::vector<bool>& frozen_bits)
{
	if (this->fb_decoder)
		this->fb_decoder->set_frozen_bits(frozen_bits);
	if (this->fb_encoder)
		this->fb_encoder->set_frozen_bits(frozen_bits);
	if (this->fb_extractor)
		this->fb_extractor->set_frozen_bits(frozen_bits);
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::notify_noise_update()
{
	Codec<B,Q>::notify_noise_update();
	if (this->adaptive_fb && !this->generated_decoder)
	{
		this->fb_generator->set_noise(*this->noise);
		this->fb_generator->generate(*this->frozen_bits);
		if (this->N_cw != this->N && puncturer_shortlast)
		{
			puncturer_shortlast->gen_frozen_bits(*frozen_bits);
		}
		this->set_frozen_bits(*this->frozen_bits);
	}
}

template <typename B, typename Q>
void Codec_polar<B,Q>
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
const std::vector<bool>& Codec_polar<B,Q>
::get_frozen_bits() const
{
	return *this->frozen_bits;
}

template <typename B, typename Q>
bool Codec_polar<B,Q>
::is_adaptive_frozen_bits() const
{
	return this->adaptive_fb;
}

template <typename B, typename Q>
bool Codec_polar<B,Q>
::is_generated_decoder() const
{
	return this->generated_decoder;
}

template <typename B, typename Q>
const Frozenbits_generator& Codec_polar<B,Q>
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
template class aff3ct::tools::Codec_polar<B_8,Q_8>;
template class aff3ct::tools::Codec_polar<B_16,Q_16>;
template class aff3ct::tools::Codec_polar<B_32,Q_32>;
template class aff3ct::tools::Codec_polar<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
