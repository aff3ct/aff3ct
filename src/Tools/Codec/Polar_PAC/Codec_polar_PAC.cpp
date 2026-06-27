#include <algorithm>
#include <exception>
#include <sstream>
#include <streampu.hpp>

#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Module/Extractor/Polar/Extractor_polar.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA_Arikan.hpp"
#include "Tools/Codec/Polar/Codec_polar.hpp"
#include "Tools/Codec/Polar_PAC/Codec_polar_PAC.hpp"
#include "Tools/Noise/Event_probability.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Tools/Noise/Sigma.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename B, typename Q>
Codec_polar_PAC<B, Q>::Codec_polar_PAC(const factory::Frozenbits_generator& fb_params,
                                       const factory::Encoder_polar_PAC& enc_params,
                                       const factory::Decoder_polar_PAC& dec_params,
                                       const module::CRC<B>* crc)
  : Codec_SISO<B, Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw)
  , adaptive_fb(fb_params.noise == -1.f)
  , frozen_bits(new std::vector<bool>(fb_params.N_cw, true))
  , fb_decoder(nullptr)
  , fb_encoder(nullptr)
  , Perr(new std::vector<double>(fb_params.N_cw, 0.0))
{
    // ----------------------------------------------------------------------------------------------------- exceptions
    if (enc_params.K != dec_params.K)
    {
        std::stringstream message;
        message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
                << ", 'dec_params.K' = " << dec_params.K << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

    if (enc_params.N_cw != dec_params.N_cw)
    {
        std::stringstream message;
        message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
                << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

    // ---------------------------------------------------------------------------------------------------------- tools
    // build the frozen bits generator
    fb_generator.reset(fb_params.build());

    if (dec_params.type == "FANO")
    {
        Frozenbits_generator_GA_Arikan* p_err_gen =
          new Frozenbits_generator_GA_Arikan(enc_params.K, enc_params.N_cw, fb_params.dump_channels_path);
        p_err_gen->generate_error_probability(*this->Perr, (double)dec_params.p_err_snr);
    }
    // ---------------------------------------------------------------------------------------------------- allocations
    std::fill(frozen_bits->begin(), frozen_bits->begin() + this->K, false);

    factory::Puncturer pctno_params;
    pctno_params.type = "NO";
    pctno_params.K = enc_params.K;
    pctno_params.N = enc_params.N_cw;
    pctno_params.N_cw = enc_params.N_cw;

    this->set_puncturer(pctno_params.build<B, Q>());
    try
    {
        this->set_encoder(enc_params.build<B>(*frozen_bits));
        fb_encoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_encoder());
    }
    catch (spu::tools::cannot_allocate const&)
    {
        this->set_encoder(static_cast<const factory::Encoder*>(&enc_params)->build<B>());
    }

    this->set_decoder_siho(dec_params.build<B, Q>(*frozen_bits, *Perr, crc, &this->get_encoder()));
    try
    {
        this->fb_decoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_decoder_siho());
    }
    catch (std::exception&)
    {
    }

    // ------------------------------------------------------------------------------------------------- frozen bit gen
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

        this->set_frozen_bits(*frozen_bits);
    }
}

template<typename B, typename Q>
Codec_polar_PAC<B, Q>*
Codec_polar_PAC<B, Q>::clone() const
{
    auto t = new Codec_polar_PAC(*this);
    t->deep_copy(*this);
    return t;
}

template<typename B, typename Q>
void
Codec_polar_PAC<B, Q>::deep_copy(const Codec_polar_PAC<B, Q>& t)
{
    Codec_SISO<B, Q>::deep_copy(t);
    if (t.fb_encoder != nullptr) this->fb_encoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_encoder());
    if (t.fb_decoder != nullptr)
        this->fb_decoder = dynamic_cast<Interface_get_set_frozen_bits*>(&this->get_decoder_siho());
}

template<typename B, typename Q>
void
Codec_polar_PAC<B, Q>::set_frozen_bits(const std::vector<bool>& frozen_bits)
{
    if (this->fb_decoder) this->fb_decoder->set_frozen_bits(frozen_bits);
    if (this->fb_encoder) this->fb_encoder->set_frozen_bits(frozen_bits);
}

template<typename B, typename Q>
void
Codec_polar_PAC<B, Q>::notify_noise_update()
{
    Codec<B, Q>::notify_noise_update();
    if (this->adaptive_fb)
    {
        this->fb_generator->set_noise(*this->noise);
        this->fb_generator->generate(*this->frozen_bits);
        this->set_frozen_bits(*this->frozen_bits);
    }
}

template<typename B, typename Q>
void
Codec_polar_PAC<B, Q>::check_noise()
{
    Codec<B, Q>::check_noise();
    if (!this->noise->is_of_type(Noise_type::SIGMA) && !this->noise->is_of_type(Noise_type::EP))
    {
        std::stringstream message;
        message << "Incompatible noise type, expected noise types are SIGMA or EP ('noise->get_type()' = "
                << Noise<>::type_to_str(this->noise->get_type()) << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }
}

template<typename B, typename Q>
const std::vector<bool>&
Codec_polar_PAC<B, Q>::get_frozen_bits() const
{
    return *this->frozen_bits;
}

template<typename B, typename Q>
bool
Codec_polar_PAC<B, Q>::is_adaptive_frozen_bits() const
{
    return this->adaptive_fb;
}

template<typename B, typename Q>
const Frozenbits_generator&
Codec_polar_PAC<B, Q>::get_frozen_bits_generator() const
{
    if (this->fb_generator == nullptr)
    {
        std::stringstream message;
        message << "'fb_generator' can't be nullptr.";
        throw spu::tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
    }

    return *this->fb_generator.get();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Codec_polar_PAC<B_8, Q_8>;
template class aff3ct::tools::Codec_polar_PAC<B_16, Q_16>;
template class aff3ct::tools::Codec_polar_PAC<B_32, Q_32>;
template class aff3ct::tools::Codec_polar_PAC<B_64, Q_64>;
#else
template class aff3ct::tools::Codec_polar_PAC<B, Q>;
#endif
// ==================================================================================== explicit template instantiation
