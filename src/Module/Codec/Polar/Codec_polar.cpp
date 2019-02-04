#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Codec_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_polar<B,Q>
::Codec_polar(const factory::Frozenbits_generator::parameters &fb_params,
              const factory::Encoder_polar       ::parameters &enc_params,
              const factory::Decoder_polar       ::parameters &dec_params,
              const factory::Puncturer_polar     ::parameters *pct_params,
              CRC<B>* crc)
: Codec          <B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  Codec_SISO_SIHO<B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  adaptive_fb(fb_params.noise == -1.f),
  frozen_bits(fb_params.N_cw, true),
  generated_decoder((dec_params.implem.find("_SNR") != std::string::npos)),
  puncturer_shortlast(nullptr),
  fb_decoder(nullptr),
  fb_encoder(nullptr)
{
	const std::string name = "Codec_polar";
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
	if (!generated_decoder)
		// build the frozen bits generator
		fb_generator.reset(factory::Frozenbits_generator::build(fb_params));
	else if (this->N_cw != this->N)
	{
		std::stringstream message;
		message << "'N_cw' has to be equal to 'N' ('N_cw' = " << this->N_cw << ", 'N' = "
		        << this->N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------- allocations
	std::fill(frozen_bits.begin(), frozen_bits.begin() + this->K, false);

	if (generated_decoder || !pct_params)
	{
		factory::Puncturer::parameters pctno_params;
		pctno_params.type     = "NO";
		pctno_params.K        = enc_params.K;
		pctno_params.N        = enc_params.N_cw;
		pctno_params.N_cw     = enc_params.N_cw;
		pctno_params.n_frames = enc_params.n_frames;

		this->set_puncturer(factory::Puncturer::build<B,Q>(pctno_params));
	}
	else
	{
		try
		{
			this->set_puncturer(factory::Puncturer_polar::build<B,Q>(*pct_params, *fb_generator));
			puncturer_shortlast = dynamic_cast<Puncturer_polar_shortlast<B,Q>*>(this->get_puncturer().get());
		}
		catch(tools::cannot_allocate const&)
		{
			this->set_puncturer(factory::Puncturer::build<B,Q>(*pct_params));
		}
	}

	try
	{
		this->set_encoder(factory::Encoder_polar::build<B>(enc_params, frozen_bits));
		fb_encoder = dynamic_cast<tools::Frozenbits_notifier*>(this->get_encoder().get());
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(factory::Encoder::build<B>(enc_params));
	}

	try
	{
		this->set_decoder_siso_siho(factory::Decoder_polar::build_siso<B,Q>(dec_params, frozen_bits, this->get_encoder()));
	}
	catch (const std::exception&)
	{
		if (generated_decoder)
			this->set_decoder_siho(factory::Decoder_polar::build_gen<B,Q>(dec_params,              crc, this->get_encoder()));
		else
			this->set_decoder_siho(factory::Decoder_polar::build    <B,Q>(dec_params, frozen_bits, crc, this->get_encoder()));
	}

	try
	{
		this->fb_decoder = dynamic_cast<tools::Frozenbits_notifier*>(this->get_decoder_siho().get());
	} catch(std::exception&) { }

	// ------------------------------------------------------------------------------------------------- frozen bit gen
	if (!generated_decoder)
	{
		if (!adaptive_fb)
		{
			if(fb_params.type == "BEC")
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
	else
	{
		const auto fb = factory::Decoder_polar::get_frozen_bits(dec_params.implem);
		if (fb.size() != frozen_bits.size())
		{
			std::stringstream message;
			message << "'fb.size()' has to be equal to 'frozen_bits.size()' ('fb.size()' = " << fb.size()
			        << ", 'frozen_bits.size()' = " << frozen_bits.size() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
		std::copy(fb.begin(), fb.end(), frozen_bits.begin());
		this->notify_frozenbits_update();
	}
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::notify_frozenbits_update()
{
	if (this->N_cw != this->N && puncturer_shortlast)
		puncturer_shortlast->gen_frozen_bits(frozen_bits);
	if (this->fb_decoder)
		this->fb_decoder->notify_frozenbits_update();
	if (this->fb_encoder)
		this->fb_encoder->notify_frozenbits_update();
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::set_noise(const tools::Noise<float>& noise)
{
	Codec_SISO_SIHO<B,Q>::set_noise(noise);

	// adaptive frozen bits generation
	if (adaptive_fb && !generated_decoder)
	{
		fb_generator->set_noise(noise);
		fb_generator->generate(frozen_bits);

		this->notify_frozenbits_update();
	}
}

template <typename B, typename Q>
std::vector<bool>& Codec_polar<B,Q>
::get_frozen_bits()
{
	return this->frozen_bits;
}

template <typename B, typename Q>
void Codec_polar<B,Q>
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
void Codec_polar<B,Q>
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
void Codec_polar<B,Q>
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
template class aff3ct::module::Codec_polar<B_8,Q_8>;
template class aff3ct::module::Codec_polar<B_16,Q_16>;
template class aff3ct::module::Codec_polar<B_32,Q_32>;
template class aff3ct::module::Codec_polar<B_64,Q_64>;
#else
template class aff3ct::module::Codec_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
