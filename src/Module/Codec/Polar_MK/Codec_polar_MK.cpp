#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Puncturer/Puncturer.hpp"

#include "Codec_polar_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_polar_MK<B,Q>
::Codec_polar_MK(const factory::Polar_code             ::parameters &pc_params,
                 const factory::Frozenbits_generator_MK::parameters &fb_params,
                 const factory::Encoder_polar_MK       ::parameters &enc_params,
                 const factory::Decoder_polar_MK       ::parameters &dec_params,
                 CRC<B>* crc)
: Codec     <B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  Codec_SIHO<B,Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  adaptive_fb(fb_params.noise == -1.f),
  frozen_bits(fb_params.N_cw, true),
  fb_decoder(nullptr),
  fb_encoder(nullptr)
{
	const std::string name = "Codec_polar_MK";
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
	// build the polar code
	code.reset(factory::Polar_code::build(pc_params));

	// build the frozen bits generator
	fb_generator.reset(factory::Frozenbits_generator_MK::build(fb_params, *code.get()));

	// ---------------------------------------------------------------------------------------------------- allocations
	factory::Puncturer::parameters pct_params;
	pct_params.type     = "NO";
	pct_params.K        = enc_params.K;
	pct_params.N        = enc_params.N_cw;
	pct_params.N_cw     = enc_params.N_cw;
	pct_params.n_frames = enc_params.n_frames;

	this->set_puncturer(factory::Puncturer::build<B,Q>(pct_params));

	std::fill(frozen_bits.begin(), frozen_bits.begin() + this->K, false);

	try
	{
		this->set_encoder(factory::Encoder_polar_MK::build<B>(enc_params, *code.get(), frozen_bits));
		fb_encoder = dynamic_cast<tools::Frozenbits_notifier*>(this->get_encoder().get());
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(factory::Encoder::build<B>(enc_params));
	}

	this->set_decoder_siho(factory::Decoder_polar_MK::build<B,Q>(dec_params, *code.get(), frozen_bits, crc,
	                                                             this->get_encoder()));
	this->fb_decoder = dynamic_cast<tools::Frozenbits_notifier*>(this->get_decoder_siho().get());

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

		fb_generator->generate(frozen_bits);
		this->notify_frozenbits_update();
	}
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::notify_frozenbits_update()
{
	if (this->fb_decoder) this->fb_decoder->notify_frozenbits_update();
	if (this->fb_encoder) this->fb_encoder->notify_frozenbits_update();
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::set_noise(const tools::Noise<float>& noise)
{
	Codec_SIHO<B,Q>::set_noise(noise);

	// adaptive frozen bits generation
	if (adaptive_fb)
	{
		fb_generator->set_noise(noise);
		fb_generator->generate(frozen_bits);

		this->notify_frozenbits_update();
	}
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::set_noise(const tools::Noise<double>& noise)
{
	Codec_SIHO<B,Q>::set_noise(noise);

	// adaptive frozen bits generation
	if (adaptive_fb)
	{
		fb_generator->set_noise(noise);
		fb_generator->generate(frozen_bits);

		this->notify_frozenbits_update();
	}
}

template <typename B, typename Q>
std::vector<bool>& Codec_polar_MK<B,Q>
::get_frozen_bits()
{
	return this->frozen_bits;
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::_extract_sys_par(const Q* Y_N, Q* sys, Q* par, const int frame_id)
{
	auto par_idx = 0, sys_idx = 0;
	auto N_cw = this->N_cw;

	for (auto i = 0; i < N_cw; i++)
	{
		if (!frozen_bits[i])
		{
			sys[sys_idx] = Y_N[i];
			sys_idx++;
		}
		else // parity bit
		{
			par[par_idx] = Y_N[i];
			par_idx++;
		}
	}
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::_extract_sys_llr(const Q* Y_N, Q* sys, const int frame_id)
{
	auto sys_idx = 0;
	auto N_cw = this->N_cw;

	for (auto i = 0; i < N_cw; i++)
	{
		if (!frozen_bits[i])
		{
			sys[sys_idx] = Y_N[i];
			sys_idx++;
		}
	}
}

template <typename B, typename Q>
void Codec_polar_MK<B,Q>
::_add_sys_ext(const Q* ext, Q* Y_N, const int frame_id)
{
	auto sys_idx = 0;
	for (auto i = 0; i < this->N_cw; i++)
		if (!frozen_bits[i])
		{
			Y_N[i] += ext[sys_idx];
			sys_idx++;
		}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Codec_polar_MK<B_8,Q_8>;
template class aff3ct::module::Codec_polar_MK<B_16,Q_16>;
template class aff3ct::module::Codec_polar_MK<B_32,Q_32>;
template class aff3ct::module::Codec_polar_MK<B_64,Q_64>;
#else
template class aff3ct::module::Codec_polar_MK<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
