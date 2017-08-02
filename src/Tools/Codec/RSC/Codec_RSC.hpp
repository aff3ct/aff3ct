#ifndef CODEC_RSC_HPP_
#define CODEC_RSC_HPP_

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

#include "Factory/Module/Code/RSC/Encoder_RSC.hpp"
#include "Factory/Module/Code/RSC/Decoder_RSC.hpp"

#include "../Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float, typename QD = Q>
class Codec_RSC : public Codec_SISO<B,Q>
{
protected:
	const factory::Encoder_RSC::parameters& enc_par;
	const factory::Decoder_RSC::parameters& dec_par;

	// the trellis representation
	std::vector<std::vector<int>> trellis;

public:
	Codec_RSC(const factory::Encoder_RSC::parameters &enc_params,
	          const factory::Decoder_RSC::parameters &dec_params);
	virtual ~Codec_RSC();

	module::Encoder_RSC_sys  <B  >* build_encoder     (const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::Decoder_SISO_SIHO<B,Q>* build_decoder_siso(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                            module::CRC        <B  >* crc = nullptr);
	module::Decoder_SISO     <  Q>* build_siso        (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                            module::CRC        <B  >* crc = nullptr);
	module::Decoder_SIHO     <B,Q>* build_decoder     (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                            module::CRC        <B  >* crc = nullptr);

	void extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par);
};
}
}

#endif /* CODEC_RSC_HPP_ */
