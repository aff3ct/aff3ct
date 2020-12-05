/*!
 * \file
 * \brief Class tools::Codec_polar_MK.
 */
#ifndef CODEC_POLAR_MK_HPP_
#define CODEC_POLAR_MK_HPP_

#include <vector>
#include <memory>

#include "Module/CRC/CRC.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"
#include "Factory/Module/Encoder/Polar_MK/Encoder_polar_MK.hpp"
#include "Factory/Module/Decoder/Polar_MK/Decoder_polar_MK.hpp"
#include "Factory/Tools/Code/Polar_MK/Frozenbits_generator_MK.hpp"
#include "Factory/Tools/Code/Polar_MK/Polar_code.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_polar_MK : public Codec_SIHO<B,Q>,
                       public Interface_get_set_frozen_bits
{
protected:
	const bool adaptive_fb;
	std::shared_ptr<std::vector<bool>> frozen_bits; // known bits (alias frozen bits) are set to true

	std::shared_ptr<Polar_code>           code;
	std::shared_ptr<Frozenbits_generator> fb_generator;

	Interface_get_set_frozen_bits* fb_decoder;
	Interface_get_set_frozen_bits* fb_encoder;
	Interface_get_set_frozen_bits* fb_extractor;

public:
	Codec_polar_MK(const factory::Polar_code              &pc_par,
	               const factory::Frozenbits_generator_MK &fb_par,
	               const factory::Encoder_polar_MK        &enc_par,
	               const factory::Decoder_polar_MK        &dec_par,
	               const module::CRC<B>                   *crc = nullptr);
	virtual ~Codec_polar_MK() = default;

	virtual Codec_polar_MK<B,Q>* clone() const;

	const std::vector<bool>& get_frozen_bits() const;
	bool is_adaptive_frozen_bits() const;
	const Frozenbits_generator& get_frozen_bits_generator() const;
	const Polar_code& get_code() const;

	void set_frozen_bits(const std::vector<bool>& fb);
	virtual void notify_noise_update();

protected:
	virtual void deep_copy(const Codec_polar_MK<B,Q> &t);
	void check_noise();
};
}
}

#endif /* CODEC_POLAR_MK_HPP_ */
