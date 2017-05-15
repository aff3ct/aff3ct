#ifndef CODEC_POLAR_HPP_
#define CODEC_POLAR_HPP_

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_polar : public Codec_SISO<B,Q>
{
protected:
	mipp::vector<B> frozen_bits; // known bits (alias frozen bits) are set to true
	const bool is_generated_decoder;
	Frozenbits_generator<B> *fb_generator;

	std::vector<module::Decoder_SISO<B,Q>*> decoder_siso;

public:
	Codec_polar(const parameters& params);
	virtual ~Codec_polar();

	void                    launch_precompute(                                                                );
	void                    snr_precompute   (const float sigma                                               );
	module::Puncturer<B,Q>* build_puncturer  (const int tid = 0                                               );
	module::Encoder  <B  >* build_encoder    (const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::SISO     <  Q>* build_siso       (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                   module::CRC        <B  >* crc = nullptr);
	module::Decoder  <B,Q>* build_decoder    (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                   module::CRC        <B  >* crc = nullptr);
};
}
}

#endif /* CODEC_POLAR_HPP_ */
