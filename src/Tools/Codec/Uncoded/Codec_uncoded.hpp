#ifndef CODEC_UNCODED_HPP_
#define CODEC_UNCODED_HPP_

#include "../Codec_SISO.hpp"

#include "Tools/Factory/Module/Code/Factory_encoder.hpp"
#include "Tools/Factory/Module/Code/NO/Factory_decoder_NO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_uncoded : public Codec_SISO<B,Q>
{
public:
	Codec_uncoded(const typename Factory_encoder   ::parameters &enc_params,
	              const typename Factory_decoder_NO::parameters &dec_params);
	virtual ~Codec_uncoded();

	module::Encoder<B  >* build_encoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::SISO   <  Q>* build_siso   (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                             module::CRC        <B  >* crc = nullptr);
	module::Decoder<B,Q>* build_decoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                             module::CRC        <B  >* crc = nullptr);

	void extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par);
};
}
}

#endif /* CODEC_UNCODED_HPP_ */
