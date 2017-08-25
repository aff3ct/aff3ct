#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Decoder/Polar/Decoder_polar_gen.hpp"

#include "Codec_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_polar<B,Q>
::Codec_polar(const factory::Frozenbits_generator::parameters &fb_params,
              const factory::Encoder_polar       ::parameters &enc_params,
              const factory::Decoder_polar       ::parameters &dec_params,
              const factory::Puncturer_polar     ::parameters &pct_params,
              CRC<B>* crc, const std::string name)
: Codec          <B,Q>(enc_params.K, enc_params.N_cw, pct_params.N, enc_params.tail_length, enc_params.n_frames, name),
  Codec_SISO_SIHO<B,Q>(enc_params.K, enc_params.N_cw, pct_params.N, enc_params.tail_length, enc_params.n_frames, name),
  adaptive_fb(fb_params.sigma == -1.f),
  frozen_bits(fb_params.N_cw, true),
  generated_decoder((dec_params.implem.find("_SNR") != std::string::npos)),
  fb_generator     (nullptr),
  puncturer_wangliu(nullptr),
  fb_decoder       (nullptr)
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
	if (!generated_decoder)
		// build the frozen bits generator
		fb_generator = factory::Frozenbits_generator::build(fb_params);
	else
		if (this->N_cw != this->N)
		{
			std::stringstream message;
			message << "'N_cw' has to be equal to 'N' ('N_cw' = " << this->N_cw << ", 'N' = "
			        << this->N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

	// ---------------------------------------------------------------------------------------------------- allocations
	std::fill(frozen_bits.begin(), frozen_bits.begin() + this->K, false);

	if (generated_decoder)
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
			puncturer_wangliu = factory::Puncturer_polar::build<B,Q>(pct_params, *fb_generator);
			this->set_puncturer(puncturer_wangliu);
		}
		catch(tools::cannot_allocate const&)
		{
			this->set_puncturer(factory::Puncturer::build<B,Q>(pct_params));
		}
	}

	try
	{
		this->set_encoder(factory::Encoder_polar::build<B>(enc_params, frozen_bits));
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(factory::Encoder::build<B>(enc_params));
	}

	try
	{
		auto decoder_siso_siho = factory::Decoder_polar::build_siso<B,Q>(dec_params, frozen_bits);
		this->set_decoder_siso(decoder_siso_siho);
		this->set_decoder_siho(decoder_siso_siho);
	}
	catch (const std::exception&)
	{
		if (generated_decoder)
			this->set_decoder_siho(factory::Decoder_polar_gen::build<B,Q>(dec_params,              crc));
		else
			this->set_decoder_siho(factory::Decoder_polar    ::build<B,Q>(dec_params, frozen_bits, crc));
	}
	this->fb_decoder = dynamic_cast<tools::Frozenbits_notifier*>(this->get_decoder_siho());

	// ------------------------------------------------------------------------------------------------- frozen bit gen
	if (!generated_decoder)
	{
		if (!adaptive_fb)
		{
			fb_generator->generate(frozen_bits);
			if (this->N_cw != this->N)
				puncturer_wangliu->gen_frozen_bits(frozen_bits);
		}
	}
	else
	{
		const auto fb = factory::Decoder_polar_gen::get_frozen_bits(dec_params.implem);
		if (fb.size() != frozen_bits.size())
		{
			std::stringstream message;
			message << "'fb.size()' has to be equal to 'frozen_bits.size()' ('fb.size()' = " << fb.size()
			        << ", 'frozen_bits.size()' = " << frozen_bits.size() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
		std::copy(fb.begin(), fb.end(), frozen_bits.begin());
	}
}

template <typename B, typename Q>
Codec_polar<B,Q>
::~Codec_polar()
{
	this->set_decoder_siho(nullptr);
	if (fb_generator != nullptr) { delete fb_generator; fb_generator = nullptr; }
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::set_sigma(const float sigma)
{
	Codec_SISO_SIHO<B,Q>::set_sigma(sigma);

	// adaptive frozen bits generation
	if (adaptive_fb && !generated_decoder)
	{
		fb_generator->set_sigma(sigma);
		fb_generator->generate(frozen_bits);

		if (this->N_cw != this->N)
			puncturer_wangliu->gen_frozen_bits(frozen_bits);

		this->fb_decoder->notify_frozenbits_update();
	}
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

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Codec_polar<B_8,Q_8>;
template class aff3ct::module::Codec_polar<B_16,Q_16>;
template class aff3ct::module::Codec_polar<B_32,Q_32>;
template class aff3ct::module::Codec_polar<B_64,Q_64>;
#else
template class aff3ct::module::Codec_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
