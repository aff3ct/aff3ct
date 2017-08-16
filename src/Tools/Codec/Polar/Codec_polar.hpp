#ifndef CODEC_POLAR_HPP_
#define CODEC_POLAR_HPP_

#include <mipp.h>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

#include "Factory/Module/Code/Polar/Encoder_polar.hpp"
#include "Factory/Module/Code/Polar/Decoder_polar.hpp"
#include "Factory/Module/Code/Polar/Puncturer_polar.hpp"
#include "Factory/Tools/Code/Polar/Frozenbits_generator.hpp"

#include "../Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_polar : public Codec_SISO<B,Q>
{
protected:
	const factory::Frozenbits_generator::parameters &fb_par;
	const factory::Puncturer_polar     ::parameters &pct_par;
	const factory::Encoder_polar       ::parameters &enc_par;
	const factory::Decoder_polar       ::parameters &dec_par;

	std::vector<bool> frozen_bits; // known bits (alias frozen bits) are set to true
	const bool is_generated_decoder;
	Frozenbits_generator *fb_generator;

	std::vector<module::Decoder_SISO_SIHO<B,Q>*> decoder_siso;

public:
	Codec_polar(const factory::Frozenbits_generator::parameters &fb_par,
	            const factory::Puncturer_polar     ::parameters &pct_par,
	            const factory::Encoder_polar       ::parameters &enc_par,
	            const factory::Decoder_polar       ::parameters &dec_par,
	            const int n_threads);
	virtual ~Codec_polar();

	void                       launch_precompute(                                                              );
	void                       snr_precompute   (const float sigma                                             );
	module::Puncturer   <B,Q>* build_puncturer  (const int tid = 0                                             );
	module::Encoder     <B  >* build_encoder    (const int tid = 0, const module::Interleaver<B>* itl = nullptr);
	module::Decoder_SISO<  Q>* build_siso       (const int tid = 0, const module::Interleaver<Q>* itl = nullptr,
	                                                                      module::CRC        <B>* crc = nullptr);
	module::Decoder_SIHO<B,Q>* build_decoder    (const int tid = 0, const module::Interleaver<Q>* itl = nullptr,
	                                                                      module::CRC        <B>* crc = nullptr);

	void extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par);
};
}
}

#endif /* CODEC_POLAR_HPP_ */
