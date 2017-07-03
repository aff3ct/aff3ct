#ifndef CODEC_POLAR_HPP_
#define CODEC_POLAR_HPP_

#include <mipp.h>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Module/Decoder/Decoder_SISO.hpp"
#include "Tools/Factory/Polar/Factory_decoder_polar.hpp"
#include "Tools/Factory/Polar/Factory_puncturer_polar.hpp"

#include "../Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_polar : public Codec_SISO<B,Q>
{
protected:
	const typename Factory_decoder_polar  <B,Q>::decoder_parameters_polar &dec_par;
	const typename Factory_puncturer_polar<B,Q>::puncturer_parameters     &pct_par;

	mipp::vector<B> frozen_bits; // known bits (alias frozen bits) are set to true
	const bool is_generated_decoder;
	Frozenbits_generator<B> *fb_generator;

	std::vector<module::Decoder_SISO<B,Q>*> decoder_siso;

public:
	Codec_polar(const typename Factory_encoder_common <B  >::encoder_parameters       &enc_params,
	            const typename Factory_decoder_polar  <B,Q>::decoder_parameters_polar &dec_params,
	            const typename Factory_puncturer_polar<B,Q>::puncturer_parameters     &pct_params,
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
