#ifndef CODEC_POLAR_HPP_
#define CODEC_POLAR_HPP_

#include <mipp.h>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Module/Decoder/Decoder_SISO.hpp"
#include "Tools/Factory/Module/Code/Polar/Factory_encoder_polar.hpp"
#include "Tools/Factory/Module/Code/Polar/Factory_decoder_polar.hpp"
#include "Tools/Factory/Module/Code/Polar/Factory_puncturer_polar.hpp"
#include "Tools/Factory/Module/Code/Polar/Factory_frozenbits_generator.hpp"

#include "../Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_polar : public Codec_SISO<B,Q>
{
protected:
	const Factory_frozenbits_generator::parameters &fb_par;
	const Factory_puncturer_polar     ::parameters &pct_par;
	const Factory_encoder_polar       ::parameters &enc_par;
	const Factory_decoder_polar       ::parameters &dec_par;

	mipp::vector<B> frozen_bits; // known bits (alias frozen bits) are set to true
	const bool is_generated_decoder;
	Frozenbits_generator<B> *fb_generator;

	std::vector<module::Decoder_SISO<B,Q>*> decoder_siso;

public:
	Codec_polar(const Factory_frozenbits_generator::parameters &fb_par,
	            const Factory_puncturer_polar     ::parameters &pct_par,
	            const Factory_encoder_polar       ::parameters &enc_par,
	            const Factory_decoder_polar       ::parameters &dec_par,
	            const int n_threads);
	virtual ~Codec_polar();

	void                    launch_precompute(                                                                );
	void                    snr_precompute   (const float sigma                                               );
	module::Puncturer<B,Q>* build_puncturer  (const int tid = 0                                               );
	module::Encoder  <B  >* build_encoder    (const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::SISO     <  Q>* build_siso       (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                   module::CRC        <B  >* crc = nullptr);
	module::Decoder  <B,Q>* build_decoder    (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                   module::CRC        <B  >* crc = nullptr);

	void extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par);
};
}
}

#endif /* CODEC_POLAR_HPP_ */
