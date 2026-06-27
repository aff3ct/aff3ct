/*!
 * \file
 * \brief Class tools::Codec_polar_PAC.
 */
#ifndef CODEC_POLAR_PAC_HPP_
#define CODEC_POLAR_PAC_HPP_

#include <memory>
#include <vector>

#include "Factory/Module/Decoder/Polar_PAC/Decoder_polar_PAC.hpp"
#include "Factory/Module/Encoder/Polar_PAC/Encoder_polar_PAC.hpp"
#include "Factory/Tools/Code/Polar/Frozenbits_generator.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "Tools/Codec/Codec_SISO.hpp"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"

namespace aff3ct
{
namespace tools
{
template<typename B = int, typename Q = float>
class Codec_polar_PAC
  : public Codec_SISO<B, Q>
  , public Interface_get_set_frozen_bits
{
  protected:
    const bool adaptive_fb;
    std::shared_ptr<std::vector<bool>> frozen_bits; // known bits (alias frozen bits) are set to true
    std::shared_ptr<std::vector<double>> Perr;

    std::shared_ptr<Frozenbits_generator> fb_generator;

    Interface_get_set_frozen_bits* fb_decoder;
    Interface_get_set_frozen_bits* fb_encoder;

  public:
    Codec_polar_PAC(const factory::Frozenbits_generator& fb_par,
                    const factory::Encoder_polar_PAC& enc_par,
                    const factory::Decoder_polar_PAC& dec_par,
                    const module::CRC<B>* crc = nullptr);
    virtual ~Codec_polar_PAC() = default;

    virtual Codec_polar_PAC<B, Q>* clone() const;

    const std::vector<bool>& get_frozen_bits() const;
    bool is_adaptive_frozen_bits() const;
    const Frozenbits_generator& get_frozen_bits_generator() const;

    void set_frozen_bits(const std::vector<bool>& frozen_bits);
    virtual void notify_noise_update();

  protected:
    virtual void deep_copy(const Codec_polar_PAC<B, Q>& t);
    void check_noise();
};
}
}

#endif /* CODEC_POLAR_PAC_HPP_ */
