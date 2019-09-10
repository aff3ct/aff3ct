#ifndef CODEC_POLAR_MK_HPP_
#define CODEC_POLAR_MK_HPP_

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "Tools/Code/Polar/Frozenbits_notifier.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"

#include "Factory/Module/Encoder/Polar_MK/Encoder_polar_MK.hpp"
#include "Factory/Module/Decoder/Polar_MK/Decoder_polar_MK.hpp"
#include "Factory/Tools/Code/Polar_MK/Frozenbits_generator_MK.hpp"
#include "Factory/Tools/Code/Polar_MK/Polar_code.hpp"

#include "Module/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_polar_MK : public Codec_SIHO<B,Q>, public tools::Frozenbits_notifier
{
protected:
	const bool adaptive_fb;
	std::vector<bool> frozen_bits; // known bits (alias frozen bits) are set to true

	std::unique_ptr<tools::Polar_code>           code;
	std::unique_ptr<tools::Frozenbits_generator> fb_generator;

	tools::Frozenbits_notifier* fb_decoder;
	tools::Frozenbits_notifier* fb_encoder;

public:
	Codec_polar_MK(const factory::Polar_code             ::parameters &pc_par,
	               const factory::Frozenbits_generator_MK::parameters &fb_par,
	               const factory::Encoder_polar_MK       ::parameters &enc_par,
	               const factory::Decoder_polar_MK       ::parameters &dec_par,
	               CRC<B>* crc = nullptr);
	virtual ~Codec_polar_MK() = default;

	virtual void set_noise(const tools::Noise<float>& noise);
	virtual void set_noise(const tools::Noise<double>& noise);

	std::vector<bool>& get_frozen_bits();

	virtual void notify_frozenbits_update();

protected:
	void _extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id);
	void _extract_sys_llr(const Q *Y_N, Q *sys,         const int frame_id);
	void _add_sys_ext    (const Q *ext, Q *Y_N,         const int frame_id);
};
}
}

#endif /* CODEC_POLAR_MK_HPP_ */
