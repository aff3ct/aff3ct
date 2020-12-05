/*!
 * \file
 * \brief Class tools::Codec_polar.
 */
#ifndef CODEC_POLAR_HPP_
#define CODEC_POLAR_HPP_

#include <vector>
#include <memory>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Puncturer/Polar/Puncturer_polar_shortlast.hpp"
#include "Factory/Module/Encoder/Polar/Encoder_polar.hpp"
#include "Factory/Module/Decoder/Polar/Decoder_polar.hpp"
#include "Factory/Module/Puncturer/Polar/Puncturer_polar.hpp"
#include "Factory/Tools/Code/Polar/Frozenbits_generator.hpp"
#include "Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_polar : public Codec_SISO<B,Q>,
                    public Interface_get_set_frozen_bits
{
protected:
	const bool adaptive_fb;
	std::shared_ptr<std::vector<bool>> frozen_bits; // known bits (alias frozen bits) are set to true
	const bool generated_decoder;

	std::shared_ptr<Frozenbits_generator> fb_generator;

	module::Puncturer_polar_shortlast<B,Q>* puncturer_shortlast;
	Interface_get_set_frozen_bits* fb_decoder;
	Interface_get_set_frozen_bits* fb_encoder;
	Interface_get_set_frozen_bits* fb_extractor;

public:
	Codec_polar(const factory::Frozenbits_generator &fb_par,
	            const factory::Encoder_polar        &enc_par,
	            const factory::Decoder_polar        &dec_par,
	            const factory::Puncturer_polar      *pct_par = nullptr,
	            const module::CRC<B>                *crc     = nullptr);
	virtual ~Codec_polar() = default;

	virtual Codec_polar<B,Q>* clone() const;

	const std::vector<bool>& get_frozen_bits() const;
	bool is_adaptive_frozen_bits() const;
	bool is_generated_decoder() const;
	const Frozenbits_generator& get_frozen_bits_generator() const;

	void set_frozen_bits(const std::vector<bool>& frozen_bits);
	virtual void notify_noise_update();

protected:
	virtual void deep_copy(const Codec_polar<B,Q> &t);
	void check_noise();
};
}
}

#endif /* CODEC_POLAR_HPP_ */
