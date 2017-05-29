#ifndef CODEC_RSC_HPP_
#define CODEC_RSC_HPP_

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float, typename QD = Q>
class Codec_RSC : public Codec_SISO<B,Q>
{
protected:
	// the trellis representation
	std::vector<std::vector<int>> trellis;

public:
	Codec_RSC(const parameters& params);
	virtual ~Codec_RSC();

	module::Encoder_RSC_sys<B  >* build_encoder     (const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::Decoder_SISO   <B,Q>* build_decoder_siso(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                          module::CRC        <B  >* crc = nullptr);
	module::SISO           <  Q>* build_siso        (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                          module::CRC        <B  >* crc = nullptr);
	module::Decoder        <B,Q>* build_decoder     (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                          module::CRC        <B  >* crc = nullptr);
};
}
}

#endif /* CODEC_RSC_HPP_ */
