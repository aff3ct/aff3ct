#ifndef CODEC_RA_HPP_
#define CODEC_RA_HPP_

#include "Factory/Module/Code/RA/Encoder_RA.hpp"
#include "Factory/Module/Code/RA/Decoder_RA.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_RA : public Codec<B,Q>
{
protected :
	const factory::Encoder_RA::parameters& enc_par;
	const factory::Decoder_RA::parameters& dec_par;

public:
	Codec_RA(const factory::Encoder_RA::parameters &enc_params,
	         const factory::Decoder_RA::parameters &dec_params);
	virtual ~Codec_RA();

	tools::Interleaver_core<   >* build_interleaver(const int tid = 0, const int seed = 0);
	module::Encoder        <B  >* build_encoder    (const int tid = 0, const module::Interleaver<B>* itl = nullptr);
	module::Decoder_SIHO   <B,Q>* build_decoder    (const int tid = 0, const module::Interleaver<Q>* itl = nullptr,
	                                                                         module::CRC        <B>* crc = nullptr);
};
}
}

#endif /* CODEC_RA_HPP_ */
